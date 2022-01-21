//Naman Mathur and Rishabh Kumar
//February 16, 2021
//Project UID af1f95f547e44c8ea88730dfb185559d

#include "processing.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]){
    Image* val = new Image;
    char *temp = argv[1];
    ifstream fin(temp);
    if(!fin.is_open()){
        cout << "Error opening file: " << temp << endl;
        delete val;
        return 1;
    }

    Image_init(val, fin);
    
    if(argc > 5 || argc < 4){
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        delete val;
        return 1;
    }

    if(argc == 4){
        char *width = argv[3];
        if(atoi(width) <= 0 || atoi(width) > Image_width(val)){
            cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
            << "WIDTH and HEIGHT must be less than or equal to original" << endl;
            delete val;
            return 1;            
        }
        else{
            seam_carve(val, atoi(width), Image_height(val));
        }
    }
    else{
        char *width = argv[3];
        char *height = argv[4];
        if(atoi(width) <= 0 || atoi(width) > Image_width(val)){
            cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
            << "WIDTH and HEIGHT must be less than or equal to original" << endl;
            delete val;
            return 1; 
        }
        else if(atoi(height) <= 0 || atoi(height) > Image_height(val)){
            cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
            << "WIDTH and HEIGHT must be less than or equal to original" << endl;
            delete val;
            return 1; 
        }
        else{
            seam_carve(val, atoi(width), atoi(height));
        }       
    }
    char *output = argv[2];
    ofstream fout(output);
    Image_print(val, fout);
    delete val;
    return 0;
}