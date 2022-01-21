//  PROJECT IDENTIFIER: 3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
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
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <fstream>
#include <vector>
#include <limits>
#include <iomanip>

using namespace std; 

struct vertex {
    double a = 0; 
    double b = 0;
    bool visited = false;
};

struct mstData {
    int precedingVertex = -1; 
    bool visited = false; 
    double weight = numeric_limits<double>::infinity();
};

class OPT {
    
    public: 

    OPT(const vector<vertex> &points_in) {
        vertices = points_in;
    }

    int quadrant(const vertex &p){
        if (p.a > 0 and p.b > 0)
            return 1; 

        else if (p.a < 0 and p.a > 0)
            return 2; 

        else if (p.a < 0 and p.b < 0)
            return 3;

        else if (p.a > 0 and p.b < 0)
            return 4; 

        else if (p.a == 0 and p.b > 0)
            return 10; //positive y axis; 

        else if (p.a == 0 and p.b < 0)
            return 11; //negative y axis

        else if (p.b == 0 and p.b < 0)
            return 12; //"lies at negative x axis";

        else if (p.b == 0 and p.a > 0)
            return 13;  //"lies at positive x axis";

        else
            return 0; //origin
    }

    double distance(const vertex &a, const vertex &b){
        if((quadrant(a)) == 3){
            if(quadrant(b) == 1 || quadrant(b) == 2 || quadrant(b) == 4 || quadrant(b) == 10 || quadrant(b) == 13){
                return -1; 
            }
        }
        else if(quadrant(b) == 3){
            if(quadrant(a) == 1 || quadrant(a) == 2 || quadrant(a) == 4 || quadrant(a) == 10 || quadrant(a) == 13){
                return -1; 
            }
        }
        double aPythag = a.a - b.a;
        double bPythag = a.b - b.b;
        double aPythagM = aPythag * aPythag; 
        double bPythagM = bPythag * bPythag; 
        return sqrt(aPythagM + bPythagM);
    }

    double dist(const vertex &a, const vertex &b){
        double aPythag = a.a - b.a;
        double bPythag = a.b - b.b;
        double aPythagM = aPythag * aPythag; 
        double bPythagM = bPythag * bPythag; 
        return sqrt(aPythagM + bPythagM);
    }

    void craftAQuickTSP() {
        optimalProgress.push_back(0);
        vertices[0].visited = true;
        optimalProgress.push_back(1);
        vertices[1].visited = true;
        optimalProgress.push_back(2);
        vertices[2].visited = true;
        while (optimalProgress.size() != vertices.size()) {
            size_t a = 0; 
            while(a < vertices.size()){
                if (vertices[a].visited) {
                    a++;
                    continue;
                }
                pushToRAC(a);
                vertices[a].visited = true;
                a++;
            }
        }
        distanceFASTTSP = 0;
        size_t b = 0; 
        while(b < optimalProgress.size() - 1){
            distanceFASTTSP += dist(vertices[optimalProgress[b]], vertices[optimalProgress[b + 1]]);
            b++;
        }
        distanceFASTTSP += dist(vertices[optimalProgress[optimalProgress.size() - 1]], vertices[optimalProgress[0]]);
    }

    void pushToRAC(size_t dot) {
        double atPresentProgress = 0;
        double progressMinLength = numeric_limits<double>::infinity();
        size_t iteratorMin = 0;
        size_t a = 0; 
        while(a < optimalProgress.size() - 1){
            atPresentProgress = dist(vertices[optimalProgress[a]], vertices[dot]) + 
            dist(vertices[optimalProgress[a + 1]], vertices[dot]) - 
            dist(vertices[optimalProgress[a]], vertices[optimalProgress[a+ 1]]);
            if (atPresentProgress < progressMinLength) {
                progressMinLength = atPresentProgress;
                iteratorMin = a + 1;
            }
            a++;
        }
        optimalProgress.insert(optimalProgress.begin() + (int)iteratorMin, dot);
    }

    void makeFastTSP() {
        double atPresentDist = 0; 
        double atMinDist = numeric_limits<double>::infinity();
        size_t iterator = 0; 
        size_t j = 0; 
        size_t l = 0; 
        optimalProgress.push_back(0); 
        vertices[0].visited = true; 
        optimalProgress.push_back(1);
        vertices[1].visited = true;
        optimalProgress.push_back(2);
        vertices[2].visited = true;
        while(j < vertices.size()){
            if(vertices[j].visited == true) {
                j++;
                continue; 
            }
            atPresentDist = dist(vertices[0], vertices[j]);
            if(atPresentDist > atMinDist){}
            else if (atPresentDist < atMinDist) {
                atMinDist = atPresentDist;
                iterator = j;
            }
            j++;
        }
        vertices[iterator].visited = true;
        optimalProgress.push_back(iterator); 
        while(vertices.size() != optimalProgress.size()){
            j = 0; 
            while(j < vertices.size()){
                if (vertices[j].visited != true) {
                    l = j;
                    break;
                }
                j++;
            }
            iterator = 0;
            atMinDist = numeric_limits<double>::infinity();
            j = 0; 
            while(j < optimalProgress.size() - 1){
                atPresentDist = dist(vertices[optimalProgress[j]], vertices[l]) + 
                dist(vertices[optimalProgress[j + 1]], vertices[l]) - 
                dist(vertices[optimalProgress[j]], vertices[optimalProgress[j + 1]]);
                if (atPresentDist < atMinDist) {
                    atMinDist = atPresentDist;
                    iterator = j;
                } 
                j++;
            }
            optimalProgress.insert(optimalProgress.begin() + iterator + 1, l);
            vertices[l].visited = true;
        }
        distanceTSP = 0; 
        size_t a = 0; 
        while(a < optimalProgress.size() - 1){
            distanceTSP += dist(vertices[optimalProgress[a]], vertices[optimalProgress[a + 1]]);
            a++;
        }
        distanceTSP += dist(vertices[optimalProgress[optimalProgress.size() - 1]], vertices[optimalProgress[0]]);
    }

    void makeOPTTable() {
        twoDTable.resize(optimalProgress.size());
        size_t a = 0; 
        while(a < twoDTable.size()){
            twoDTable[a].resize(optimalProgress.size()); 
            size_t x = 0; 
            while(x < twoDTable[a].size()){
                twoDTable[a][x] = dist(vertices[a], vertices[x]);
                x++;
            }
            a++;
        }
    }

    void createOTP(){
        craftAQuickTSP();
        //makeFastTSP();
        vector<size_t> path_copy = optimalProgress;
        OPTTotalLength = distanceFASTTSP;
        makeOPTTable();
        genpermsFinal(path_copy, 1);
    }
    

    void printOTP() {
        cout << OPTTotalLength << '\n';
        size_t a = 0; 
        while(a < optimalProgress.size()){
            cout << optimalProgress[a] << " ";
            a++;
        }
        cout << '\n';
    }

    void genpermsFinal(vector<size_t>& progress, size_t permutationLength) {
        if (progress.size() == permutationLength) {
            double progressMass = 0;
            size_t a = 0; 
            while(a < permutationLength){
                if(permutationLength == a + 1){
                    progressMass += dist(vertices[progress[progress.size() - 1]], vertices[progress[0]]);
                }
                else{
                    progressMass += dist(vertices[progress[a]], vertices[progress[a + 1]]);
                }  
                a++; 
            }
            if (progressMass < OPTTotalLength) {
                OPTTotalLength = progressMass;
                optimalProgress = progress;
            }
            return;
        }
        if (!promisingFinal(progress, permutationLength)) {
            return;
        }
        for (size_t i = permutationLength; i < progress.size(); ++i) {
            swap(progress[permutationLength], progress[i]);
            genpermsFinal(progress, permutationLength + 1);
            swap(progress[i], progress[permutationLength]);
        }
    }

    bool promisingFinal(vector<size_t>& progress, size_t permutationLength) {
        size_t lengthLeft = progress.size() - permutationLength;
        size_t* progressLeft = &progress[permutationLength];
        optMass = 0;
        double backMin = numeric_limits<double>::infinity();
        double frontMin = numeric_limits<double>::infinity();
        double lengthBack = 0;
        double lengthFront = 0;
        if (!makeOPTTree(progress.size(), lengthLeft, progressLeft)) {
            return false;
        }
        size_t a = 0; 
        while(a < lengthLeft) {
            lengthBack = twoDTable[progressLeft[a]][progress.back()];
            lengthFront = twoDTable[progressLeft[a]][progress.front()];
            
            if (backMin > lengthBack) {
                backMin = lengthBack;
            }
            
            if (frontMin > lengthFront) {
                frontMin = lengthFront;
            }
            a++;
        }
        double progressAtPresent = 0;
        double optimalProgressLength = 0;
        for (size_t i = 0; i < permutationLength - 1; i++) {
            optimalProgressLength += twoDTable[progress[i]][progress[i + 1]];
            progressAtPresent += dist(vertices[progress[i]], vertices[progress[i + 1]]);
            assert(optimalProgressLength == progressAtPresent);
        }
        if (optMass + backMin + frontMin + progressAtPresent > OPTTotalLength) {
            return false;
        }
        return true;
    }

    bool makeOPTTree(size_t progressSize, size_t left, size_t* progressLeft) {
        mstOPT.clear();
        mstOPT.resize(progressSize);
        mstOPT[progressLeft[0]].weight = 0;
        double dist = 0;
        for (size_t iterator = 0; iterator < left; iterator++) {
            int parent = -1;
            double min = numeric_limits<double>::infinity();
            size_t b = 0; 
            while(b < left){
                if (!mstOPT[progressLeft[b]].visited) {
                    if (mstOPT[progressLeft[b]].weight < min) {
                        min = mstOPT[progressLeft[b]].weight;
                        parent = static_cast<int>(progressLeft[b]);
                    }
                }
                b++;
            }
            if (parent == -1) {
                break;
            }
            mstOPT[(size_t)parent].visited = true;
            b = 0; 
            while(b < left){
                if (!mstOPT[progressLeft[b]].visited) {
                    dist = twoDTable[(size_t)parent][progressLeft[b]];
                    if (mstOPT[progressLeft[b]].weight > dist) {
                        mstOPT[progressLeft[b]].weight = dist;
                        mstOPT[progressLeft[b]].precedingVertex = parent;
                    }
                }
                b++;
            }
        }
        optMass = 0;
        size_t a = 0; 
        while(a < mstOPT.size()){
           if (mstOPT[a].weight != numeric_limits<double>::infinity()) {
                optMass += mstOPT[a].weight;
            }
            a++; 
        }
        return true;
    }

    void makeAFASTTSP() {
        double atPresentDist = 0; 
        double atMinDist = numeric_limits<double>::infinity();
        size_t iterator = 0; 
        size_t l = 0; 
        optimalProgress.push_back(0); 
        vertices[0].visited = true; 
        optimalProgress.push_back(1);
        vertices[1].visited = true;
        optimalProgress.push_back(2);
        vertices[2].visited = true;
        size_t b = 0; 
        while(b < vertices.size()){
            if (vertices[b].visited) {
                b++;
                continue;
            }
            atPresentDist = dist(vertices[0], vertices[b]);
            if(atPresentDist > atMinDist){}
            else if (atPresentDist < atMinDist) {
                atMinDist = atPresentDist;
                iterator = b;
            }
            b++;
        }
        vertices[iterator].visited = true;
        optimalProgress.push_back(iterator); 
        while (optimalProgress.size() != vertices.size()) {
            size_t g = 0; 
            while(g < vertices.size()){
                if (vertices[g].visited != true) {
                    l = g;
                    break;
                }
                g++;
            }
            iterator = 0;
            atMinDist = numeric_limits<double>::infinity();
            size_t r = 0; 
            while(r < optimalProgress.size() -1){
                atPresentDist = dist(vertices[optimalProgress[r]], vertices[l]) + 
                dist(vertices[optimalProgress[r + 1]], vertices[l]) - 
                dist(vertices[optimalProgress[r]], vertices[optimalProgress[r + 1]]);
                if (atPresentDist < atMinDist) {
                    atMinDist = atPresentDist;
                    iterator = r;
                } 
                r++;
            }
            optimalProgress.insert(optimalProgress.begin() + iterator + 1, l);
            vertices[l].visited = true;
        }
        distanceFASTTSP = 0;
        size_t a = 0; 
        while(a < optimalProgress.size() -1){
            distanceFASTTSP += dist(vertices[optimalProgress[a]], vertices[optimalProgress[a + 1]]);
            a++;
        }
        distanceFASTTSP += dist(vertices[optimalProgress[optimalProgress.size() - 1]], vertices[optimalProgress[0]]);
    }

    void printFASTTSP(){
        cout << distanceFASTTSP 
            << '\n';
        size_t a = 0; 
        while(a < optimalProgress.size()){
            cout << optimalProgress[a] << " ";
            a++;
        }
        cout << '\n';
    }

    void makeMST(){
        mstVector.resize(vertices.size());
        mstVector[0].weight = 0; 
        size_t iterator = 0; 
        for (iterator = 0; iterator < vertices.size(); iterator++) {
            double lower = numeric_limits<double>::infinity();
            int predecessor = -1; 
            size_t s = 0; 
            while(s < vertices.size()){
                if(mstVector[s].visited != true){
                    if(mstVector[s].weight < lower){
                        lower = mstVector[s].weight;
                        predecessor = int(s); 
                    }
                }
                s++;
            }
            if(predecessor < 0) break; 
            mstVector[predecessor].visited = true; 
            size_t g = 0; 
            while(g < vertices.size()){
                if(mstVector[g].visited != true){
                    double inBetweenDist = distance(vertices[predecessor], vertices[g]);
                    if (inBetweenDist < 0) {
                        g++;
                        continue;
                    }
                    if (mstVector[g].weight > inBetweenDist) {
                        mstVector[g].weight = inBetweenDist;
                        mstVector[g].precedingVertex = predecessor;
                    }
                }
                g++;
            }
        }
        size_t a = 0; 
        while(a < mstVector.size()){
            if(mstVector[a].visited != true){
                cout << "Cannot construct MST" << '\n';
                exit(1);
            }
            a++;
        }
        treeWeight = 0;
        size_t b = 0; 
        while(b < mstVector.size()){
            if (mstVector[a].weight != numeric_limits<double>::infinity()) {
                treeWeight += mstVector[a].weight;
            }
            b++;
        }
    }

    void printMST(){
        cout << treeWeight << '\n';
        size_t a = 0; 
        while(a < mstVector.size()){
            if (mstVector[a].precedingVertex == -1) {
                a++;
                continue;
            }
            if (mstVector[a].weight != numeric_limits<double>::infinity()) {
                if (static_cast<int>(a) < mstVector[a].precedingVertex) {
                    cout << a << " " << mstVector[a].precedingVertex << "\n";
                }
                else {
                    cout << mstVector[a].precedingVertex << " " << a << "\n";
                }
            }
            a++;
        }
    }
    
    private: 
        vector<vertex> vertices; 
        vector<vector<double>> twoDTable; 
        vector<mstData> mstOPT;
        vector<size_t> optimalProgress;
        vector<mstData> mstVector;
        double treeWeight; 
        double OPTTotalLength;
        double distanceTSP;
        double distanceFASTTSP;
        double optMass;



};