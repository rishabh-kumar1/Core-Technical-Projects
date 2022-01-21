// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <fstream>

using namespace std; 

struct LogEntry {
    uint32_t entryID; 
    string category, message, stringTimeStamp, sortFriendlyCategory; 
    unsigned long long int timeStamp; 

    bool operator () (const LogEntry& a, const LogEntry& b) const {
        return a.timeStamp < b.timeStamp;
    }

    bool operator > (const LogEntry& logEntry) const {
        return timeStamp >= logEntry.timeStamp;
    }

    bool operator < (const LogEntry& logEntry) const {
        return timeStamp < logEntry.timeStamp;
    }
};

bool compareMasterListEntries(LogEntry& a, LogEntry& b) {
    if (a.timeStamp == b.timeStamp) {
        if (a.sortFriendlyCategory == b.sortFriendlyCategory) {
            return a.entryID < b.entryID;
        }
        return a.sortFriendlyCategory < b.sortFriendlyCategory;
    }
    return a.timeStamp < b.timeStamp;
}

void splitStrings(string sentence, set<string> &set){
    string insert; 
    uint32_t begin = 0; 
    uint32_t last = 0; 
    unsigned long int messageToBeSplitSize = sentence.size(); 
    uint32_t i = 0; 
    while(i < messageToBeSplitSize){
        if(isalnum(sentence[i]) != 0){
            begin = i; 
            while ((isalnum(sentence[i]) != 0) && sentence[i] != '\0') {
                i++;
            }
            last = i; 
            insert = sentence.substr(begin, last - begin);
            if (insert.empty() != true) {
                set.insert(insert);
            }
        }
        else {
            i++;
        }
    }
}

void deleteFrontSpace(string& string){
    uint32_t startSpace = 0;
    for (uint32_t x = 0; x < string.size(); x++) {
        if (string[x] != ' ') {
            startSpace = x;
            break;
        }
    }
    unsigned long int endSpace = string.size() - 1;
    string = string.substr(startSpace, endSpace - startSpace + 1);
}

int main(int argc, char* argv[]){
    //std::ios_base::sync_with_stdio(false);
    if(argc < 2){
        exit(1);
    }
    char command; 
    uint32_t numEntries = 0; 
    bool readFile = true;
    bool previousSearch = false; 
    vector<LogEntry> masterList; 
    vector<uint32_t> searchedEntries; 
    string argument = argv[1];
    if(argument == "-h" || argument == "help"){
        cout << "This program is intended to take log files generated from the usage \n"
        << "of a specific program or application and allow a back-end user to both \n"
        << "access and edit logged information in a fast and efficient manner \n";
        exit(0);
    }
    deque<uint32_t> excerpt; 
    unordered_map<uint32_t, uint32_t> entryIDtoIndex; 
    unordered_map<string, vector<uint32_t>>  categoryToID; 
    unordered_map<string, vector<uint32_t>> keywordToID;
    ifstream masterLogFile(argument);
    while(readFile){
        string timeStamp, category, message; 
        getline(masterLogFile, timeStamp, '|');
        if(timeStamp.empty()) break; 
        LogEntry tempLog; 
        tempLog.stringTimeStamp = timeStamp; 
        timeStamp.erase(std::remove(timeStamp.begin(), timeStamp.end(), ':'), timeStamp.end());
        stringstream convertingTimeStamp(timeStamp);
        convertingTimeStamp >> tempLog.timeStamp; 
        getline(masterLogFile, category, '|');
        tempLog.category = category;
        tempLog.sortFriendlyCategory = category; 
        transform(tempLog.sortFriendlyCategory.begin(), 
            tempLog.sortFriendlyCategory.end(), tempLog.sortFriendlyCategory.begin(), ::tolower);
        getline(masterLogFile, message);
        tempLog.message = message; 
        tempLog.entryID = numEntries++;
        masterList.push_back(tempLog);
    }

    cout << numEntries << " entries read" << '\n';
    sort(masterList.begin(), masterList.end(), compareMasterListEntries); 

    for(uint32_t iterator = 0; iterator < masterList.size(); iterator++){
        categoryToID[masterList[iterator].sortFriendlyCategory].push_back(iterator);

        set<string> insertWordsIntoKeyWordMap;
        string messageToBeSplit = masterList[iterator].message;

        transform(messageToBeSplit.begin(), messageToBeSplit.end(), messageToBeSplit.begin(), ::tolower);
        splitStrings(messageToBeSplit, insertWordsIntoKeyWordMap);

        messageToBeSplit = masterList[iterator].sortFriendlyCategory; 
        splitStrings(messageToBeSplit, insertWordsIntoKeyWordMap);

        for (auto keyWordInsert : insertWordsIntoKeyWordMap) {
            keywordToID[keyWordInsert].push_back(iterator);
        }

        entryIDtoIndex[masterList[iterator].entryID] = iterator;
    }

    do {
        cout << "% ";
        cin >> command;
        if(command == '#'){
            string comment; 
            getline(cin, comment);
            continue;
        }
        else if(command == 'g'){
            for(uint32_t i : searchedEntries){
                cout << masterList[i].entryID << "|" << masterList[i].stringTimeStamp
                << "|" << masterList[i].category << "|" << masterList[i].message << '\n';
            }
        }
        else if(command == 'a'){
            uint32_t entryPosition; 
            cin >> entryPosition; 
            unsigned long int masterListSize = masterList.size(); 
            if(entryPosition > 0 && entryPosition < masterListSize) {
                excerpt.push_back(entryIDtoIndex[entryPosition]);
            }
            else {
                continue;
            }
            cout << "log entry " << entryPosition << " appended" << '\n';
        }
        else if(command == 'p'){
            uint32_t counter = 0;
            for (uint32_t ID : excerpt){
                cout << counter << "|" << masterList[ID].entryID << "|" << masterList[ID].stringTimeStamp 
                << "|" << masterList[ID].category << "|" << masterList[ID].message << "\n";
                counter++;
            }
        }
        else if(command == 't'){
            string timeStamps; 
            cin >> timeStamps; 
            if (timeStamps.length() != 29) {
                break;
            }
            LogEntry timeStamp1; 
            LogEntry timeStamp2; 

            string firstHalfTimeStamp = timeStamps.substr(0, 14);
            firstHalfTimeStamp.erase(std::remove(firstHalfTimeStamp.begin(), firstHalfTimeStamp.end(), ':'), firstHalfTimeStamp.end());
            stringstream convertingTimeStampToInt(firstHalfTimeStamp);
            convertingTimeStampToInt >> timeStamp1.timeStamp;
            
            string secondHalfTimeStamp = timeStamps.substr(15, 14);
            secondHalfTimeStamp.erase(std::remove(secondHalfTimeStamp.begin(), secondHalfTimeStamp.end(), ':'), secondHalfTimeStamp.end());
            stringstream convertingTimeStampToInt2(secondHalfTimeStamp);
            convertingTimeStampToInt2 >> timeStamp2.timeStamp;

            auto lowerBound = lower_bound(masterList.begin(), masterList.end(), timeStamp1, LogEntry());
            if(lowerBound != masterList.end()) {}
            else {
                cout << "Timestamps search: 0 entries found" << '\n';
                continue;
            }

            auto upperBound = upper_bound(lowerBound, masterList.end(), timeStamp2, LogEntry());
            searchedEntries.clear();
            while (upperBound != lowerBound) {
                searchedEntries.push_back((uint32_t(lowerBound - masterList.begin())));
                lowerBound += 1; 
            }
            unsigned long int searchedEntriesSizeT = searchedEntries.size(); 
            cout << "Timestamps search: " << searchedEntriesSizeT << " entries found" << '\n';
            previousSearch = true;
        }
        else if(command == 'r'){
            if(!previousSearch) continue; 
            for (uint32_t item : searchedEntries) {
                excerpt.push_back(item);
            }
            unsigned long int searchedEntriesSizeR = searchedEntries.size(); 
            cout <<  searchedEntriesSizeR << " log entries appended" << '\n';
        }
        else if(command == 'c'){
            string enteredCategory; 
            getline(cin, enteredCategory);
            deleteFrontSpace(enteredCategory);
            transform(enteredCategory.begin(), enteredCategory.end(), enteredCategory.begin(), ::tolower);
            auto iterator = categoryToID.find(enteredCategory);
            searchedEntries.clear(); 
            if(iterator == categoryToID.end()) {}
            else {
                for(uint32_t id : iterator->second){
                    searchedEntries.push_back(id);
                }
            }
            cout << "Category search: " << searchedEntries.size() << " entries found" << '\n';
            previousSearch = true;
        }
        else if(command == 'd'){
            uint32_t position; 
            cin >> position; 
            unsigned long int excerptListSizeD = excerpt.size(); 
            if(position >= excerptListSizeD){
                continue; 
            }
            else{
                auto iterator = excerpt.begin();
                excerpt.erase(iterator + position); 
                cout << "Deleted excerpt list entry " << position << '\n';
            }
        }
        else if(command == 'e'){
            uint32_t position; 
            cin >> position; 
            unsigned long int excerptListSizeE = excerpt.size(); 
            if(position == excerptListSizeE - 1){
                cout << "Moved excerpt list entry " << position << '\n';
                continue;
            }
            else if(position < excerptListSizeE){
                uint32_t movedEntry = excerpt[position];
                auto iterator = excerpt.begin(); 
                excerpt.erase(iterator + position); 
                excerpt.push_back(movedEntry);
                cout << "Moved excerpt list entry " << position << "\n";
            }
            else{
                continue; 
            }
        }
        else if(command == 'l'){
            if(excerpt.empty()){
                cout << "excerpt list cleared" << '\n'; 
                cout << "(previously empty)" << '\n'; 
            }
            else{
                cout << "excerpt list cleared" << '\n'; 
                cout << "previous contents:" << '\n';
                cout << "0|" << masterList[excerpt[0]].entryID << "|" << masterList[excerpt[0]].stringTimeStamp 
                << "|" << masterList[excerpt[0]].category << "|" << masterList[excerpt[0]].message << "\n";
                cout << "..." << '\n'; 
                unsigned long int excerptSizeL = excerpt.size() - 1; 
                cout << excerptSizeL <<"|" << masterList[excerpt[excerptSizeL]].entryID << "|" << masterList[excerpt[excerptSizeL]].stringTimeStamp 
                << "|" << masterList[excerpt[excerptSizeL]].category << "|" << masterList[excerpt[excerptSizeL]].message << "\n";
                excerpt.clear();
            }
        }
        else if(command == 'b'){
            uint32_t position; 
            cin >> position; 
            unsigned long int excerptSizeB = excerpt.size(); 
            if(position > 0 && position < excerptSizeB){
                uint32_t entryToBeMoved = excerpt[position];
                auto iterator = excerpt.begin(); 
                excerpt.erase(iterator + position); 
                excerpt.push_front(entryToBeMoved);
                cout << "Moved excerpt list entry " << position << '\n';
            }
            if(position == 0){
                cout << "Moved excerpt list entry 0" << '\n';
            }
        }
        else if(command == 'm'){
            string givenTimeStamp; 
            cin >> givenTimeStamp; 
            if (givenTimeStamp.length() != 14) {
                break;
            }
            LogEntry searchedEntry; 
            givenTimeStamp.erase(std::remove(givenTimeStamp.begin(), givenTimeStamp.end(), ':'), givenTimeStamp.end());
            stringstream convertingTimeStampToIntM(givenTimeStamp);
            convertingTimeStampToIntM >> searchedEntry.timeStamp; 
            auto lowerBound = lower_bound(masterList.begin(), masterList.end(), searchedEntry, LogEntry());
            if(lowerBound == masterList.end()){
                cout << "Timestamp search: 0 entries found" << '\n';
                continue;
            }
            auto upperBound = upper_bound(masterList.begin(), masterList.end(), searchedEntry, LogEntry());
            searchedEntries.clear();
            bool lowerNotUpper = true; 
            while(lowerNotUpper){
                unsigned long int lowerMinusMasterBegin = lowerBound - masterList.begin();
                searchedEntries.push_back(uint32_t(lowerMinusMasterBegin));
                lowerBound++;
                if(lowerBound == upperBound){
                    break; 
                }
            }
            cout << "Timestamp search: " << searchedEntries.size() << " entries found" << '\n';
            previousSearch = true;
        }
        else if(command == 'k'){
            string keyWordSearch; 
            set<string> keyWordsSearchList; 
            getline(cin, keyWordSearch);
            transform(keyWordSearch.begin(), keyWordSearch.end(), keyWordSearch.begin(), ::tolower);
            splitStrings(keyWordSearch, keyWordsSearchList);
            if(keyWordsSearchList.empty()){
                continue; 
            }
            previousSearch = true;
            searchedEntries.clear();
            if(keyWordsSearchList.size() > 1) {}
            else{
                auto iterator = keywordToID.find(*(keyWordsSearchList.begin()));
                if (iterator != keywordToID.end()) {
                    searchedEntries = iterator->second;
                    cout << "Keyword search: " << searchedEntries.size() << " entries found" << '\n';
                    continue;
                }
                else{
                    cout << "Keyword search: 0 entries found" << '\n';
                    continue;
                }
            }
            vector<vector<uint32_t>*> entryIDPerKeyword; 

            uint32_t oneKeywordNotPresent = 0; 
            for (auto x : keyWordsSearchList) {
                auto iterator = keywordToID.find(x);
                if (iterator == keywordToID.end()) {
                    // If one keyword is not found abort the search
                    oneKeywordNotPresent++;
                    break;
                }
                entryIDPerKeyword.push_back(&iterator->second);
            }
            if(oneKeywordNotPresent > 0){
                cout << "Keyword search: 0 entries found\n";
                continue;
            }
            uint32_t zero = 0; 
            searchedEntries = *entryIDPerKeyword[zero];
            for(uint32_t x = 1; x < entryIDPerKeyword.size(); x++){
                vector<uint32_t>& presentEntryIDs = *entryIDPerKeyword[x];
                vector<uint32_t> presentIntersection;
                set_intersection(searchedEntries.begin(),searchedEntries.end(),presentEntryIDs.begin(),presentEntryIDs.end(),back_inserter(presentIntersection));
                swap(searchedEntries, presentIntersection);
                if (searchedEntries.size() == 0) {
                    break;
                }
            }
            cout << "Keyword search: " << searchedEntries.size() << " entries found" << '\n';
        }
        else if(command == 's'){
            if(excerpt.empty()){
                cout << "excerpt list sorted" << '\n';
                cout << "(previously empty)" << '\n'; 
            }
            else{
                cout << "excerpt list sorted" << '\n'; 
                cout << "previous ordering:" << '\n'; 
                cout << "0|" << masterList[excerpt[0]].entryID << "|" << masterList[excerpt[0]].stringTimeStamp 
                << "|" << masterList[excerpt[0]].category << "|" << masterList[excerpt[0]].message << '\n';
                cout << "..." << '\n'; 
                unsigned long int excerptSizeS = excerpt.size() - 1; 
                cout << excerptSizeS <<"|" << masterList[excerpt[excerptSizeS]].entryID << "|" << masterList[excerpt[excerptSizeS]].stringTimeStamp 
                << "|" << masterList[excerpt[excerptSizeS]].category << "|" << masterList[excerpt[excerptSizeS]].message << '\n';

                cout << "new ordering:" << '\n';
                sort(excerpt.begin(), excerpt.end());
                cout << "0|" << masterList[excerpt[0]].entryID << "|" << masterList[excerpt[0]].stringTimeStamp 
                << "|" << masterList[excerpt[0]].category << "|" << masterList[excerpt[0]].message << '\n';
                cout << "..." << '\n'; 
                unsigned long int excerptSizeS2 = excerpt.size() - 1; 
                cout << excerptSizeS <<"|" << masterList[excerpt[excerptSizeS2]].entryID << "|" << masterList[excerpt[excerptSizeS2]].stringTimeStamp 
                << "|" << masterList[excerpt[excerptSizeS2]].category << "|" << masterList[excerpt[excerptSizeS2]].message << '\n';
            }
        }
    } while (command != 'q');
}