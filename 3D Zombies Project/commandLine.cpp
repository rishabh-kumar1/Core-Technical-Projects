// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

// EECS 281, Project 2
// rishkum

#include <queue>
#include <getopt.h>
#include <string>
#include <iostream>
using namespace std;

class commandLine {
    public:

    bool medianOutput, verboseOutput, statisticsOutput; 
    uint32_t numEntries; 

    commandLine(){
        medianOutput = false; 
        verboseOutput = false; 
        statisticsOutput = false; 
        numEntries = 0; 
    }

//TODO:
void printHelp(char *argv[]) {
    cout << "Usage: " << argv[0] << " \n";
    cout << "This program is designed to take in a zombie game file and each round of the game inside it\n";
    cout << "and generate random and named zombies as listed in the file and simulate battle with a human \n"; 
    cout << "depending on usage flagged on command line\n";
} 

bool returnVerbose(){
    return verboseOutput; 
}

bool returnMedian(){
    return medianOutput; 
}

bool returnStatistics(){
    return statisticsOutput; 
}

uint32_t returnStatsNumber(){
    return numEntries; 
}

void getMode(int argc, char * argv[]) {
    opterr = false; 
    int choice; 
    int option_index = 0;
    string mode; 
    string outputCalls; 
    
    option long_options[] = {
        {"verbose",      no_argument,   nullptr, 'v'},
        {"statistics",   required_argument, nullptr, 's'},
        {"help",        no_argument, nullptr,  'h'},
        {"median", required_argument, nullptr, 'm'},
        { nullptr, 0,         nullptr, '\0' }
    };

    
    while ((choice = getopt_long(argc, argv, "vs:hm", long_options, &option_index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(argv);
            exit(0);
        case 'v':
            verboseOutput = true; 
            break;
        case 's':
            statisticsOutput = true;
            numEntries = atoi(optarg);
            break;
        case 'm':
            medianOutput = true; 
            break; 
        default:
            cerr << "Error: invalid option" << endl;
            exit(1);
        } 
    } 
} 
    
};