//  PROJECT IDENTIFIER: 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9

#include "OPTTSP.cpp"


void help(){
    cout << "This program is intended to simulate a given zoo" <<
    '\n' << "environment where a zoo keeper must find the shortest" << 
    '\n' << "path and least cost to feed every single cage of animals." << 
    '\n' << "Accepted program arguments are MST, OPTTSP, and FASTTSP"; 
    exit(0);
}

void NumArguments(int num){
    if(num < 2){
        help(); 
    }
}

void programArgument(string argument){ 
    if(argument == "--help" || argument == "-h") {
        help();
    }
    else if(argument == "--mode" || argument == "-m"){
    }
}

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    NumArguments(argc);
    string argument = argv[1];
    string programMode = argv[2];
    programArgument(argument);
    cout << setprecision(2) << fixed;
    vector<vertex> vertices; 
    string tempNumCages; 
    getline(cin, tempNumCages);
    double numCages = stoi(tempNumCages);
    double loopIterator = 0;
    string tempNum; 
    while(loopIterator < numCages){
        getline(cin, tempNum, ' '); 
        vertex v; 
        v.a = stoi(tempNum);
        getline(cin, tempNum);
        v.b = stoi(tempNum);
        vertices.push_back(v);
        loopIterator++;
    }
    if(programMode == "MST"){
        OPT o(vertices);
        o.makeMST();
        o.printMST();
    }
    else if(programMode == "FASTTSP"){
        OPT o(vertices);
        o.makeAFASTTSP();
        o.printFASTTSP();
    }
    else if(programMode == "OPTTSP"){
        OPT o(vertices);
        o.createOTP();
        o.printOTP();
    }

}   

