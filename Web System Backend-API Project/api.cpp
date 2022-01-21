/* 
 * Naman Mathur and Rishabh Kumar
 * api.cpp
 * Project UID c1f28c309e55405daf00c565d57ff9ad
 * EECS 280 Project 4
 */

#include "List.h"
#include <list>
#include <string>
#include "json.hpp"
using nlohmann::json;

using namespace std;


class OHQueue {
public:
  
  bool isEmpty(){
    return queue.size() == 0;
  }

  string get_head_name(){
    assert(!isEmpty());
    Student s = queue.front(); 
    return s.uniqname;
  }


  string get_head_location(){
    assert(!isEmpty());
    Student s = queue.front(); 
    return s.location;
  }

  size_t length_tail(){
    Student s = queue.back();
    json j1 = {
      {"location", s.location},
      {"position", s.position},
      {"uniqname", s.uniqname}
    };
    string str = j1.dump(4) + "\n";
    return str.length();
  }

  size_t length_head(){
    Student s = queue.front();
    json j1 = {
      {"location", s.location},
      {"position", s.position},
      {"uniqname", s.uniqname}
    };
    string str = j1.dump(4) + "\n";
    return str.length();
  }


  size_t length_queue(){
    json result;
    for(list<Student>::iterator i = queue.begin(); i != queue.end(); ++i){
      json temp = {
        {"location", i->location},
        {"position", i->position},
        {"uniqname", i->uniqname}
      };
      result.push_back(temp);
    }
    json j1 = {
        {"count", queue.size()},
        {"results", result}
    };
    string str = j1.dump(4) + "\n";
    return str.length();
  }

  void print_tail(){
    Student s = queue.back();
    json j1 = {
      {"location", s.location},
      {"position", s.position},
      {"uniqname", s.uniqname}
    };
    string str = j1.dump(4) + "\n";
    cout << str;
  }

  void print_head(){
    Student s = queue.front();
    json j1 = {
      {"location", s.location},
      {"position", s.position},
      {"uniqname", s.uniqname}
    };
    string str = j1.dump(4) + "\n";
    cout << str;
  }

  void print_queue(){
    json results;
    for(list<Student>::iterator i = queue.begin(); i != queue.end(); ++i){
      json temp = {
        {"location", i->location},
        {"position", i->position},
        {"uniqname", i->uniqname}
      };
      results.push_back(temp);
    }
    json j1 = {
        {"count", queue.size()},
        {"results", results}
    };
    string str = j1.dump(4) + "\n";
    cout << str;
  }

  void delete_head(){
    if(isEmpty()){
      cout << "HTTP/1.1 400 Bad Request" << endl << 
        "Content-Type: application/json; charset=utf-8" 
        << endl << "Content-Length: 0\n" << endl;
    }
    queue.pop_front();
  }

  void add_student(string uniqname, string location, int pos){
    Student s = {uniqname, location, pos};
    queue.push_back(s);
  }

  void get_api(){
    json j1 = {
      {"queue_head_url", "http://localhost/queue/head/"},
      {"queue_list_url", "http://localhost/queue/"},
      {"queue_tail_url", "http://localhost/queue/tail/"}
    };
    string str = j1.dump(4) + "\n";
    cout << str; 
  }

  void get_head_func(string &content, string &content2){
      if(isEmpty()){
        cout << "HTTP/1.1 400 Bad Request" << endl << 
        "Content-Type: application/json; charset=utf-8" 
        << endl << "Content-Length: 0\n" << endl;
      }
      else{
        cout << "HTTP/1.1 200 OK" << endl << content << endl;
        cout << content2 << " " << length_head() << endl << endl;
        print_head();
      } 
  }

private:
  struct Student{
      string uniqname;
      string location;
      int position;
  };
  list<Student> queue;
};



int main(int argc, char* argv[]){
    string first_line, localhost, content, content2, length;
    OHQueue ohqueue;
    int pos = 1;
    while(getline(cin, first_line)){
      if(first_line.empty()){
        return 0;
      }
      getline(cin, localhost);
      getline(cin, content);
      cin >> content2 >> length;
      if(first_line == "GET /api/ HTTP/1.1"){
        cout << "HTTP/1.1 200 OK" << endl << content << endl 
        << content2 << " " << 160 << endl << endl;
        ohqueue.get_api();
      }
      else if(first_line == "GET /api/queue/head/ HTTP/1.1"){
        ohqueue.get_head_func(content, content2);
      }
      else if(first_line == "GET /api/queue/ HTTP/1.1"){
        cout << "HTTP/1.1 200 OK" << endl << content << endl;
        cout << content2 << " " << ohqueue.length_queue() << endl << endl;
        ohqueue.print_queue(); 
      }
      else if(first_line == "DELETE /api/queue/head/ HTTP/1.1"){
        ohqueue.delete_head();
        cout << "HTTP/1.1 204 No Content" << endl << 
          content << endl << "Content-Length: 0\n" << endl;
      }
      else if(first_line == "POST /api/queue/tail/ HTTP/1.1"){
        cout << "HTTP/1.1 201 Created" << endl << content << endl;
        json j1;
        cin >> j1;
        ohqueue.add_student(j1["uniqname"], j1["location"], pos);
        pos++;
        cout << content2 << " " << ohqueue.length_tail() << endl << endl;
        ohqueue.print_tail();
      }
      else{
        cout << "HTTP/1.1 400 Bad Request" << endl << content << 
            endl << content2 << " " << 0 << endl << endl;
      }
      getline(cin, first_line);
    }
    return 0;
}