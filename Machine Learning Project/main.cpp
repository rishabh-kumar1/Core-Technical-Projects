/* 
 * Naman Mathur and Rishabh Kumar
 * api.cpp
 * Project UID db1f506d06d84ab787baf250c265e24e
 * EECS 280 Project 5
 */

#include <string>
#include <iostream>
#include <map>
#include <set>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include "csvstream.h"

using namespace std; 

class Classifier{
    private: 
        int total_words; 
        int vocab_size; 
        int total_posts; 
        string label_c; 
        string word_w; 
        map<string, string> input_data; 
        map<string, int> num_posts_with_word;
        map<string, int> posts_with_label; 
        map<string, map<string, int>> num_posts_with_label_and_word;
        set<string> vocab; 
        set<string> current; 

    public:
        Classifier(){
            total_words = 0;
            vocab_size = 0;
            total_posts = 0; 
        } 

        size_t find_total_posts(){
            return total_posts; 
        }

        void find_vocab_size(){
            vocab_size = int(vocab.size()); 
        }

        void print_vocab_size(){
            cout << "vocabulary size = " << vocab_size << endl << endl; 
        }


        void train(string filename, bool cmd_debug){
            csvstream csvin(filename, ',', false);
            string temp_word; 
            //input data map
            if(cmd_debug){
                cout << "training data:" << endl;    
            }
            while (csvin >> input_data){
                //blah blah print 
                if(cmd_debug == true){ 
                    cout << "  " << "label = " << input_data["tag"] 
                        << ", content = " << input_data["content"] << endl; 
                }
                total_posts += 1; 
                istringstream source(input_data["content"]);
                while(source >> temp_word){
                    vocab.insert(temp_word);
                    current.insert(temp_word);
                }
                posts_with_label[input_data["tag"]] += 1;
                
                for(auto iterator = current.begin(); 
                iterator!= current.end(); ++iterator){
                    
                    ++(num_posts_with_word[*iterator]); 
                    ++((num_posts_with_label_and_word[input_data["tag"]])[*iterator]);
                }
                current.clear();
            }

            cout << "trained on " << total_posts << " examples" << endl;
            vocab_size = static_cast<int>(vocab.size());
            if(cmd_debug == true){
                print_training_data_debug();
            }
        }
        

        //label = euchre, content = can the upcard ever be the left bower
        void print_training_data_debug() {
            print_vocab_size();
            cout << "classes:" << endl; 
            for(auto const &pair: posts_with_label){
                cout << "  " << pair.first << ", " << pair.second 
                << " examples, log-prior = " << log_prior(pair.first) << endl; 
            }
            cout << "classifier parameters:" << endl; 
            for(auto const &pair: num_posts_with_label_and_word){
                for(auto const &pair1: num_posts_with_label_and_word[pair.first]){
                    cout << "  " << pair.first << ":" << pair1.first << ", count = " << 
                        pair1.second << ", log-likelihood = " <<
                         log_likelihood(pair.first, pair1.first) << endl; 
                }
            }
        }

        double log_prior(const string &label) {
            double top = double(posts_with_label[label]);
            double bottom = double(total_posts);
            return log(top/bottom);
        }

        double log_likelihood (const string &label, const string &word) {
            if(vocab.find(word) == vocab.end()){
                return log(1/double(total_posts));
            }
            else if(vocab.find(word) != vocab.end()
            && num_posts_with_label_and_word[label].find(word)
             == num_posts_with_label_and_word[label].end()){
                 double top = double(num_posts_with_word[word]);
                 return log(top/double(total_posts));
            }
            else{
                double top = double(num_posts_with_label_and_word[label][word]);
                double bottom = double(posts_with_label[label]);
                return log(top/bottom);
            }
        }

        double log_probability (const string &label, const vector<double> &data) {
            double total = log_prior(label);
            for(int i = 0; i < int(data.size()); ++i){
                total = total + data[i];
            }
            return total;
        }

        void predict_test_results(string filename){
            csvstream csvin(filename, ',', false);
            int predicted_correctly = 0;
            int total = 0; 
            string biggest;
            vector<double> data; 
            vector<pair<string, double>> log_probs;
            map<string, string> test_data;
            set<string> test_vocab, test_current;
            string temp_word;
            cout << endl << "test data:" << endl;
            while (csvin >> test_data){  
                istringstream source(test_data["content"]);
                while(source >> temp_word){
                    test_vocab.insert(temp_word);
                    test_current.insert(temp_word);
                }
                for(auto &pair: posts_with_label){
                    for(auto iterator = test_current.begin(); 
                    iterator != test_current.end(); ++iterator){
                        data.push_back(log_likelihood(pair.first, *iterator));
                    }
                    log_probs.push_back({pair.first, 
                        log_probability(pair.first, data)});
                    data.clear();
                }
                test_current.clear();   
                double small = -100000;
                for(int i = 0; i < int(log_probs.size()); ++i){
                    if(log_probs[i].second > small){
                        small = log_probs[i].second;
                        biggest = log_probs[i].first;
                    }
                }
                if(biggest == test_data["tag"]){
                    predicted_correctly++;
                }
                total++;
                cout << "  correct = " << test_data["tag"] << ", predicted = " 
                << biggest << ", log-probability score = "
                << small << endl << "  content = " << 
                test_data["content"] << endl << endl;      
                log_probs.clear();       
            }
            cout << "performance: " << predicted_correctly << " / " << total 
                << " posts predicted correctly" << endl;
        }

};



int main(int argc, char** argv) {
    cout.precision(3);
    string filename = string(argv[1]); 
    string test_file = string(argv[2]);
    bool debug = false; 
    Classifier c;
    if(argc > 4 || argc < 3){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1; 
    }

    if(argc == 4){
        debug = true; 
        if(string(argv[3]) != "--debug"){
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return 1; 
        }
    }

    c.train(filename, debug);
    c.predict_test_results(test_file);
    return 0;
}