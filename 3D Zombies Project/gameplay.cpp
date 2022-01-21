// Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

// EECS 281, Project 2
// rishkum

#include <queue>
#include <getopt.h>
#include <string>
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include "commandLine.cpp"
#include "P2random.h"

using namespace std;


class gamePlay{

public: 
    uint32_t round, numRandomZombies, numNamedZombies, totalZombies, currentRound; 
    bool humanLiving, verboseOutput, medianOutput, statsOutput, zombiesLeft, firstRoundNotOne,
    nextRoundPresent;
    string killerZombie, lastZombieKilledName, info;
    uint32_t quiverCapacity, randomSeed, maxRandDistance, 
    maxRandSpeed, maxRandHealth, currentQuiverNumber, lastZombieKilledRound, 
    zomsAliveNumber, statisticsNumber, nextRoundToCreateZombies; 

gamePlay() {
    firstRoundNotOne = false; 
    lastZombieKilledName = "";
    lastZombieKilledRound = 1; 
    killerZombie = "";
    round = 0; 
    numRandomZombies = 0; 
    numNamedZombies = 0; 
    totalZombies = 0; 
    currentRound = 0; 
    humanLiving = true; 
    zombiesLeft = true;
    verboseOutput = false; 
    medianOutput = false; 
    statsOutput = false; 
    quiverCapacity = 0; 
    randomSeed = 0; 
    maxRandDistance = 0; 
    maxRandSpeed = 0; 
    maxRandHealth = 0; 
    currentQuiverNumber = 0; 
    zomsAliveNumber = 0; 
    nextRoundToCreateZombies = 0;
    statisticsNumber = 0; 
    info = "";
}

struct zombie{ 
    string name; 
    uint32_t distance, speed, health, rounds, eta; 
    uint32_t roundsActive = 1;  
    bool dead; 
}; 

class zombieComparatorLessThan {
    public:

    bool operator()(const zombie* a, const zombie* b) const {
        if (a->roundsActive == b->roundsActive) {
            return a->name > b->name; 
        }
        return a->roundsActive < b->roundsActive;
    }

};

class zombieComparatorGreaterThan {
    public:

    bool operator()(const zombie* a, const zombie* b) const {
        if (a->roundsActive == b->roundsActive) {
            return a->name > b->name; 
        }
        return a->roundsActive > b->roundsActive;
    }

};

class zombieComparator {
    public:

    bool operator()(const zombie* a, const zombie* b) const {
        if(a->eta == b->eta){
            if(a->health == b->health){
                return a->name > b->name; 
            }
            else{
                return a->health > b->health; 
            }
        }
        else{
            return a->eta > b->eta; 
        }
    }
};
vector<zombie*> everyZombie;
priority_queue<zombie*, vector<zombie*>, zombieComparator> zombies;
vector<string> killedZombies; 
priority_queue<uint32_t, vector<uint32_t>, greater<uint32_t>> upper;
priority_queue<uint32_t, vector<uint32_t>, less<uint32_t>> lower;  

void readCreateRounds(){
    string tempLine;
    string namedZombieName; 
    uint32_t distance; 
    uint32_t speed; 
    uint32_t health; 
    //store next round where theres creation of more zombies
    if(nextRoundToCreateZombies != 0){
        if(currentRound == nextRoundToCreateZombies){
            nextRoundToCreateZombies = 0;
            if(firstRoundNotOne == true){
                firstRoundNotOne = false; 
            }
        }
        else{
            return;
        }
    }
    while(cin >> info){ 
        if(info[0] == '-'){
            break; 
        }
        if(info == "round:" || info == "r"){
            cin >> round; 
        }
        if(currentRound == 1){
            if(round != currentRound){
                nextRoundToCreateZombies = round; 
                firstRoundNotOne = true; 
                break;
            }
        }
        if(round != currentRound){
            nextRoundToCreateZombies = round;
            break; 
        }
        if(info == "random-zombies:" || info == "rndzmbs"){
            cin >> numRandomZombies;
            if(numRandomZombies > 0){
                for(uint32_t i = 0; i < numRandomZombies; i++){
                    zombie* a = new zombie(); 
                    a->name = P2random::getNextZombieName();
                    a->distance = P2random::getNextZombieDistance();
                    a->speed = P2random::getNextZombieSpeed(); 
                    a->health = P2random::getNextZombieHealth();
                    a->eta = a->distance/ a->speed; 
                    everyZombie.push_back(a);
                    zombies.push(a);
                    if (verboseOutput == true) {
                        cout << "Created: " << a->name << " (distance: " << a->distance << ", speed: " << a->speed << ", health: " << a->health << ")\n";
                    }
                }//hey qt ;) 
            }
        }
        if(info == "named-zombies:" || info == "named"){
            cin >> numNamedZombies; 
            if(numNamedZombies > 0){
                for(uint32_t i = 0; i < numNamedZombies; i++){
                    cin >> namedZombieName; 
                    zombie* a = new zombie;
                    a->name = namedZombieName; 
                    cin >> info >> distance; 
                    a->distance = distance; 
                    cin >> info >> speed; 
                    a->speed = speed; 
                    cin >> info >> health; 
                    a->health = health; 
                    a->eta = a->distance/ a->speed; 
                    everyZombie.push_back(a);
                    zombies.push(a);
                    if (verboseOutput == true) {
                        cout << "Created: " << a->name << " (distance: " << a->distance << ", speed: " << a->speed << ", health: " << a->health << ")\n";
                    }
                }
            }
        }
    }//while  
}//funtion 

void shootRounds(){
    while(currentQuiverNumber > 0){
        if(zombies.empty() == true){
            break; 
        }
        zombie* a = zombies.top();  
        if(a->health > currentQuiverNumber){
            a->health = a->health - currentQuiverNumber;
            break; 
        }
        else if(a->health <= currentQuiverNumber){ //if health is less than quiver, shoot zombie until destroyed
            currentQuiverNumber -= a->health;
            a->health = 0; 
            a->dead = true; 
            zombies.pop(); //<--"kill zombie"
            //record "round where last zombie was killed and its name"
            lastZombieKilledName = a->name; 
            lastZombieKilledRound = currentRound; 
            if(verboseOutput == true){
                cout << "Destroyed: " << a->name << " (distance: " << a->distance << ", speed: " << a->speed << ", health: " << a->health << ")\n";
            }
            if(medianOutput == true || statsOutput == true){
                killedZombies.push_back(lastZombieKilledName);
            }
            if (medianOutput) {
                pushMedian(a->roundsActive);
            }
        }
    }
}

void moveZombies() {
    for(zombie* a : everyZombie){
        if(!a->dead){
            if(a->distance <= a->speed){
                a->distance = 0; 
                a->eta = 0; 
            }
            else{
                a->distance -= a->speed; 
                a->eta = a->distance/a->speed;
            }
            if (verboseOutput == true) {
                cout << "Moved: " << a->name << " (distance: " << a->distance << ", speed: " << a->speed << ", health: " << a->health << ")\n";
            }
            a->roundsActive += 1; 
            if (a->distance == 0 && killerZombie.empty()) {
                humanLiving = false; 
                killerZombie = a->name;
            }
        }
    }
}

void playGame() {
    string checkNextRound;
    while(humanLiving || zombiesLeft){
        currentRound+= 1; 
        if(verboseOutput != false){
            cout << "Round: " << currentRound << endl;
        }
        if(humanLiving == false){
            zomsAliveNumber = (uint32_t)zombies.size();
            break; 
        }
        currentQuiverNumber = quiverCapacity; 
        moveZombies();
        if(humanLiving == false){
            zomsAliveNumber = (uint32_t)zombies.size();
            break; 
        }
        readCreateRounds();
        if(humanLiving == false){
            zomsAliveNumber = (uint32_t)zombies.size(); 
            break; 
        }
        shootRounds();
        zomsAliveNumber = (uint32_t)zombies.size(); 
        if (medianOutput != false && !killedZombies.empty()) {
            cout << "At the end of round " << currentRound << ", the median zombie lifetime is " << medianReturn() << endl;
        }
        if(zombies.empty() == true && firstRoundNotOne != true){
            zombiesLeft = false; 
            if(info == "---"){
                zombiesLeft = true;
            }
            else if(nextRoundToCreateZombies != 0){
                zombiesLeft = true;
            }
            else{
                break;
            }
        }
    }
}

void printFirstNZombies(){
    uint32_t finalFirstNNumber = 0; 
    uint32_t x = 0; 
    if(statisticsNumber > killedZombies.size()){
        finalFirstNNumber = (uint32_t)killedZombies.size(); 
    }
    else if(statisticsNumber <= killedZombies.size()){
        finalFirstNNumber = statisticsNumber; 
    }
    for (const string& a : killedZombies) {
        cout << '\n' << a << " " << ++x;
        if (x == finalFirstNNumber) {
            break;
        }
    }
}

void printLastNZombies(){
    int finalLastNNumber; 

    if((int)statisticsNumber > (int)killedZombies.size()){
        finalLastNNumber = int(killedZombies.size()); 
    }
    else if((int)statisticsNumber <= (int)killedZombies.size()){
        finalLastNNumber = (int)statisticsNumber; 
    }
    int x = finalLastNNumber; 
    for(int i = int(killedZombies.size() - 1); i >= int(killedZombies.size() - finalLastNNumber); i--){
        cout << '\n' << killedZombies[i] << " " << x;
        x--;
    }
}

void printMostActiveZombies(){
    uint32_t finalMostActiveNumber; 
    if(statisticsNumber > everyZombie.size()){
        finalMostActiveNumber = (uint32_t)everyZombie.size(); 
    }
    else if(statisticsNumber <= everyZombie.size()){
        finalMostActiveNumber = statisticsNumber; 
    }
    priority_queue<zombie*, vector<zombie*>, zombieComparatorLessThan> mostActive(everyZombie.begin(), everyZombie.end());
    for(uint32_t x = 0; x < finalMostActiveNumber; x++){
        zombie* a = mostActive.top();
        mostActive.pop();
        cout << '\n' << a->name << " " << a->roundsActive; 
    }
}

void printLeastActiveZombies(){
    uint32_t finalLeastActiveNumber; 
    if(statisticsNumber > everyZombie.size()){
        finalLeastActiveNumber = (uint32_t)everyZombie.size(); 
    }
    else if(statisticsNumber <= everyZombie.size()){
        finalLeastActiveNumber = statisticsNumber; 
    }
    priority_queue<zombie*, vector<zombie*>, zombieComparatorGreaterThan> leastActive(everyZombie.begin(), everyZombie.end());
    for (uint32_t x = 0; x < finalLeastActiveNumber; x++) {
        zombie* a = leastActive.top();
        leastActive.pop();
        cout << '\n' << a->name << " " << a->roundsActive;
    }
}

void printEndingMessages(){
    if(humanLiving == false){
        cout << "DEFEAT IN ROUND " << currentRound << "! " << killerZombie << " ate your brains!\n"; 
    }
    else if(humanLiving != false){
        uint32_t victoryRound = currentRound; 
        if(lastZombieKilledRound < currentRound - 1){ //acount for the fact that even after last zombie is 
            victoryRound = round; 
        }
        cout << "VICTORY IN ROUND " << victoryRound << "! " << lastZombieKilledName << " was the last zombie.\n";
    }
    if(statsOutput == true){
        cout << "Zombies still active: " << zomsAliveNumber << '\n'; 
        cout << "First zombies killed:";
        printFirstNZombies();
        cout << '\n' << "Last zombies killed:";
        printLastNZombies();
        cout << '\n' << "Most active zombies:";
        printMostActiveZombies();
        cout << '\n' << "Least active zombies:";
        printLeastActiveZombies();
        cout << '\n';
    }

}

void destructor() {
    for (uint32_t i = 0; i < everyZombie.size(); i++) { //go through and delete everyZombie vector
        delete everyZombie[i]; //"destructor"
    }
}


 void pushMedian(uint32_t activeRounds) {
        if (upper.empty() || activeRounds >= upper.top()) {
            upper.push(activeRounds);
        }
        else {
            lower.push(activeRounds);
        }

        if ((int)(upper.size() - lower.size()) > 1) {
            lower.push(upper.top());
            upper.pop();
        }
        else if ((int)(lower.size() - upper.size()) > 1) {
            upper.push(lower.top());
            lower.pop();
        }
    }

    uint32_t medianReturn() const {
        uint32_t median = 0;
        if (!upper.empty() || !lower.empty()) {
            if (upper.size() == lower.size()) {
                median = (upper.top() + lower.top()) / 2;
            }
            else if (upper.size() > lower.size()) {
                median = upper.top();
            }
            else {
                median = lower.top();
            }
        }
        return median;
    }

};


int main(int argc, char *argv[]) {
    string commentLine; 
    string info;
    commandLine cL = commandLine(); 
    gamePlay gp =  gamePlay(); 
    cL.getMode(argc, argv); 
    gp.verboseOutput = cL.returnVerbose(); 
    gp.medianOutput = cL.returnMedian(); 
    gp.statsOutput = cL.returnStatistics(); 
    gp.statisticsNumber = cL.returnStatsNumber();
    getline(cin, commentLine);
    cin >> info >> gp.quiverCapacity; 
    cin >> info >> gp.randomSeed; 
    cin >> info >> gp.maxRandDistance; 
    cin >> info >> gp.maxRandSpeed; 
    cin >> info >> gp.maxRandHealth; 
    cin >> info; //first --
    P2random::initialize(gp.randomSeed, gp.maxRandDistance, gp.maxRandSpeed, gp.maxRandHealth);
    gp.playGame(); 
    gp.printEndingMessages(); 
    gp.destructor();
}
