// Project Identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A

// EECS 281, Project 1
// rishkum


#include <deque>
#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stack>
using namespace std;

class spaceShip{
    public:

    size_t numRows, numCols, numLevels, levelSize;
    size_t level, row, column; 
    char inputType, specialCharacter;  
    bool solutionPresent = false; 
    size_t startLevel, startRow, startCol; 
    size_t endLevel, endRow, endCol; 

    spaceShip(){    
       numRows = levelSize; 
       numCols = levelSize; 
       inputType = 0;
        numLevels = 0;
        numRows = 0; 
        numCols = 0; 
        numLevels = 0; 
        level = 0; 
        row = 0;
        column = 0; 
        inputType = '.';
        specialCharacter = '.';
        startLevel = 0; 
        startRow = 0; 
        startCol = 0; 
        endLevel = 0; 
        endRow = 0; 
        endCol = 0;
       map.resize(numLevels,vector<vector<FloorSquare>>(numRows,vector<FloorSquare>(numCols)));
    }

    spaceShip(char inputType1, size_t numLevels1, size_t levelSize1){
        inputType = inputType1; 
        numLevels = numLevels1; 
        numRows = levelSize1; 
        numCols = levelSize1; 
        level = 0; 
        row = 0;
        column = 0; 
        specialCharacter = '.';
        startLevel = 0; 
        startRow = 0; 
        startCol = 0; 
        endLevel = 0; 
        endRow = 0; 
        endCol = 0; 
        map.resize(numLevels,vector<vector<FloorSquare>>(numRows,vector<FloorSquare>(numCols)));
    }

    struct FloorSquare{
        bool discovered; 
        char element;
        char directionDiscovered; 
    };
    vector<vector<vector<FloorSquare>>> map;
    deque<vector<size_t>> searchContainer;
    stack<vector<size_t>> backtraceContainer;

    void readMapToMemory(){
        string tempLine; 
        bool first = true;
        for(size_t level = 0; level < numLevels; ++level){
            for(size_t row = 0; row < numRows; ++row) {
                for(size_t col = 0; col < numCols; ++col){
                    map[level][row][col].element = '.';
                }    
            }
        }
        for(size_t level = 0; level < numLevels; ++level){
            for(size_t row = 0; row < numRows; ++row) {
                if(first == true){
                    getline(cin, tempLine);
                    first = false; 
                }
                getline(cin, tempLine);
                if(tempLine.at(0) != '/'){
                    for(size_t col = 0; col < numCols; ++col){
                        if((tempLine[col] == 'S' || tempLine[col] == '#' || tempLine[col] == 'H' 
                        || tempLine[col] == '.' || tempLine[col] == 'E') && (level < numLevels && row < numRows && col < numCols)) {
                            map[level][row][col].element = tempLine[col];
                        }//if
                        else {
                            cerr << "Invalid map input character\n"; 
                            exit(1);
                        }//else
                        if(tempLine[col] == 'S'){
                            startLevel = level; 
                            startRow = row; 
                            startCol = col;
                        }//if
                    }//for
                }//if
                else {
                    --row;
                }//else
            }//for
        }//for
    }//readMapToMemory

    void readListToMemory(){
        //string line; 
        char first; 
        string line; 
        //char temp; 
        char useless; //commas and parenthesis
        for(size_t level = 0; level < numLevels; ++level){
            for(size_t row = 0; row < numRows; ++row) {
                for(size_t col = 0; col < numCols; ++col){
                    map[level][row][col].element = '.';
                }    
            }
        }

        while(cin >> first){  //getline(cin, listLine)
            if(first == '/'){
                getline(cin, line);
            }
            else {
                cin >> level >> useless >> row >> useless >> column >> useless >> specialCharacter >> useless; 
                //cout << "fdfjh" << level << " " << row << " " << column << " " << specialCharacter; 
                if((specialCharacter == 'S' || specialCharacter == '#' || specialCharacter == 'H' || specialCharacter == '.' 
                || specialCharacter == 'E') && (level < numLevels && row < numRows && column < numCols)){
                    map[level][row][column].element = specialCharacter;
                }//if
                else {
                    cerr << "Invalid list input character\n"; 
                    exit(1);
                }//else
                if(specialCharacter == 'S'){
                    startLevel = level; 
                    startRow = row; 
                    startCol = column;
                }//if
            }//else
        }//while
    }//readlistToMemory

    char convertInttoChar(int intLevel){ 
        char num_char[10];
        char level; 
        std::sprintf(num_char, "%d", intLevel);
        level = *num_char; 
        return level; 
    } 

    void searchContainerQueue() {
        //Number of iterations needed to search through all of the map
        //size_t iterations = numLevels * numRows * numCols; 
        size_t currentLevel, currentRow, currentCol;
        //size_t lastLevel; 
        bool foundH = false; 
        //put starting location into deque
        searchContainer.push_back({startLevel, startRow, startCol});
        map[startLevel][startRow][startCol].discovered = true; 
        while(!searchContainer.empty() && foundH == false){ 
            currentLevel = searchContainer[0][0];
            currentRow = searchContainer[0][1];
            currentCol = searchContainer[0][2];
            searchContainer.pop_front();
            if(map[currentLevel][currentRow][currentCol].element == 'H'){
                foundH = true; 
                solutionPresent = true; 
                endLevel = currentLevel; 
                endRow = currentRow; 
                endCol = currentCol;  
                //north
                // if((currentRow < numRows - 1) && (map[currentLevel][currentRow + 1][currentCol].discovered == true)){
                //      map[currentLevel][currentRow][currentCol].directionDiscovered = 'n'; 
                // }
                // //east
                // else if((currentCol > 0) && (map[currentLevel][currentRow][currentCol - 1].discovered == true)){
                //     map[currentLevel][currentRow][currentCol].directionDiscovered = 'e';
                // }
                // //south 
                // else if((currentRow > 0) && (map[currentLevel][currentRow - 1][currentCol].discovered == true)){
                //     map[currentLevel][currentRow][currentCol].directionDiscovered = 's';
                // }
                // //west
                // else if((currentCol < numCols - 1) && (map[currentLevel][currentRow][currentCol + 1].discovered == true)){
                //     map[currentLevel][currentRow][currentCol].directionDiscovered = 'w';
                // }
                // break; 
            }
            //now check north, east, south, and west of each currentlocation using map 3d floorspaces
            if(currentRow != 0){ //checking North
                if(map[currentLevel][currentRow - 1][currentCol].element != '#' && (map[currentLevel][currentRow - 1][currentCol].discovered != true)){
                    searchContainer.push_back({currentLevel, currentRow - 1, currentCol});
                    map[currentLevel][currentRow - 1][currentCol].discovered = true; 
                    map[currentLevel][currentRow - 1][currentCol].directionDiscovered = 'n'; 
                }//if North valid
            }//if edge case
            if(currentCol < numCols - 1){ //checking East
                if(map[currentLevel][currentRow][currentCol + 1].element != '#' && (map[currentLevel][currentRow][currentCol + 1].discovered != true)){
                    searchContainer.push_back({currentLevel, currentRow, currentCol + 1});
                    map[currentLevel][currentRow][currentCol + 1].discovered = true; 
                    map[currentLevel][currentRow][currentCol + 1].directionDiscovered = 'e'; 
                }//if East Valid
            }//if edge case
            if(currentRow < numRows -1){ //checking South
                if(map[currentLevel][currentRow + 1][currentCol].element != '#' && (map[currentLevel][currentRow + 1][currentCol].discovered != true)){
                    searchContainer.push_back({currentLevel, currentRow + 1, currentCol});
                    map[currentLevel][currentRow + 1][currentCol].discovered = true; 
                    map[currentLevel][currentRow + 1][currentCol].directionDiscovered = 's'; 
                }//if South Valid
            }//if edge case
            if(currentCol > 0){ //checking West
                if(map[currentLevel][currentRow][currentCol - 1].element != '#' && (map[currentLevel][currentRow][currentCol - 1].discovered != true)){
                    searchContainer.push_back({currentLevel, currentRow, currentCol - 1});
                    map[currentLevel][currentRow][currentCol - 1].discovered = true; 
                    map[currentLevel][currentRow][currentCol - 1].directionDiscovered = 'w'; 
                }//if South Valid
            }//if edge case
            if(map[currentLevel][currentRow][currentCol].element == 'E'){
                for(size_t level = 0; level < numLevels; ++level){
                    if(map[level][currentRow][currentCol].element == 'E' && (map[level][currentRow][currentCol].discovered != true)){
                        searchContainer.push_back({level, currentRow, currentCol});
                        map[level][currentRow][currentCol].discovered = true; 
                        int intLevel = static_cast<int>(currentLevel);
                        map[level][currentRow][currentCol].directionDiscovered = convertInttoChar(intLevel);
                    } //checking Elevator
                }
            }
             //remove first element of deque to examine the next one in the queue
            //Then update current location and go into the next loop  
            //When you find the hangar tile you stop searching 
        }//while 
    }

    void searchContainerStack(){
        size_t currentLevel, currentRow, currentCol;
        //size_t lastRow, lastCol;
        char currentLevelChar;  
        bool foundH = false;
        //put starting location into search deque
        searchContainer.push_back({startLevel, startRow, startCol});
        map[startLevel][startRow][startCol].discovered = true; 
        while(!searchContainer.empty() && foundH == false){ 
            currentLevel = searchContainer[searchContainer.size() - 1][0];
            currentRow = searchContainer[searchContainer.size() - 1][1];
            currentCol = searchContainer[searchContainer.size() - 1][2];
            searchContainer.pop_back();
            if(map[currentLevel][currentRow][currentCol].element == 'H'){
                foundH = true; 
                solutionPresent = true;
                endLevel = currentLevel; 
                endRow = currentRow; 
                endCol = currentCol;   
                //north
               /*  if((currentRow > lastRow) && (currentCol == lastCol)){
                     map[currentLevel][currentRow][currentCol].directionDiscovered = 'n'; 
                }
                //east
                else if((currentCol > lastCol) && (currentRow == lastRow)){
                    map[currentLevel][currentRow][currentCol].directionDiscovered = 'e';
                }
                //south 
                else if((currentRow < lastRow) && (currentCol == lastCol)){
                    map[currentLevel][currentRow][currentCol].directionDiscovered = 's';
                }
                //west
                else if((currentCol < lastCol) && (currentRow == lastRow)){
                    map[currentLevel][currentRow][currentCol].directionDiscovered = 'w';
                }
                break;  */
            }
            if(map[currentLevel][currentRow][currentCol].element != '#'){
                if(currentRow != 0){ //checking North
                    if(map[currentLevel][currentRow - 1][currentCol].element != '#' && (map[currentLevel][currentRow - 1][currentCol].discovered != true)){
                        searchContainer.push_back({currentLevel, currentRow - 1, currentCol});
                        map[currentLevel][currentRow - 1][currentCol].discovered = true; 
                        map[currentLevel][currentRow - 1][currentCol].directionDiscovered = 'n'; 
                    }//if North valid
                }//if edge case
                if(currentCol < numCols - 1){ //checking East
                    if(map[currentLevel][currentRow][currentCol + 1].element != '#' && (map[currentLevel][currentRow][currentCol + 1].discovered != true)){
                        searchContainer.push_back({currentLevel, currentRow, currentCol + 1});
                        map[currentLevel][currentRow][currentCol + 1].discovered = true; 
                        map[currentLevel][currentRow][currentCol + 1].directionDiscovered = 'e'; 
                    }//if East Valid
                }//if edge case
                if(currentRow < numRows - 1){ //checking South    
                    if(map[currentLevel][currentRow + 1][currentCol].element != '#' && (map[currentLevel][currentRow + 1][currentCol].discovered != true)){
                        searchContainer.push_back({currentLevel, currentRow + 1, currentCol});
                        map[currentLevel][currentRow + 1][currentCol].discovered = true; 
                        map[currentLevel][currentRow + 1][currentCol].directionDiscovered = 's'; 
                    }//if South Valid
                }//if edge case
                if(currentCol > 0){ //checking West //ADD IF CURRENT ROW IS GREATER THAN ONE
                    if(map[currentLevel][currentRow][currentCol - 1].element != '#' && (map[currentLevel][currentRow][currentCol - 1].discovered != true)){
                        searchContainer.push_back({currentLevel, currentRow, currentCol - 1});
                        map[currentLevel][currentRow][currentCol - 1].discovered = true; 
                        map[currentLevel][currentRow][currentCol - 1].directionDiscovered = 'w'; 
                    }//if South Valid
                }//if edge case
                if(map[currentLevel][currentRow][currentCol].element == 'E'){
                    for(size_t level = 0; level < numLevels; ++level){
                        if(map[level][currentRow][currentCol].element == 'E' && (map[level][currentRow][currentCol].discovered != true)){
                            searchContainer.push_back({level, currentRow, currentCol});
                            map[level][currentRow][currentCol].discovered = true; 
                            currentLevelChar = static_cast<char>('0' + currentLevel);
                            map[level][currentRow][currentCol].directionDiscovered = currentLevelChar;
                        } //checking Elevator
                    }
                }
            }//if
        }//while
    }//searchContainerStack

    void backTracing(){
        size_t currentLevel, currentRow, currentCol;
        size_t elevatorLevel, elevatorRow, elevatorCol; 
        char currentLevelChar; 
        bool elevatorFound = false; 
        bool foundS = false; 
        currentLevel = endLevel; 
        currentRow = endRow; 
        currentCol = endCol; 
        if(solutionPresent == true){
            while(foundS == false){
                //if to check if current FloorSquare is Start TODO
                if(currentLevel == startLevel && currentRow == startRow && currentCol == startCol){
                    foundS = true; 
                }
                if(elevatorFound == true){
                    for(size_t level = 0; level < numLevels; ++level){
                        if(map[level][elevatorRow][elevatorCol].element == 'E' && (map[level][elevatorRow][elevatorCol].discovered == true)){
                            currentLevelChar = static_cast<char>('0' + elevatorLevel);
                            map[level][elevatorRow][elevatorCol].element = currentLevelChar; 
                            currentLevel = level; 
                            backtraceContainer.push({level, elevatorRow, elevatorCol});
                        } 
                    }
                    elevatorFound = false; 
                }
                //north 
                if(map[currentLevel][currentRow][currentCol].directionDiscovered == 'n'){
                    //update currentLevel, row, col to be used in next loop
                    //update stack with this floorsquare
                    if(currentRow < numRows - 1){
                        if(map[currentLevel][currentRow + 1][currentCol].element == 'E'){
                            elevatorFound = true; 
                            elevatorRow = currentRow + 1; 
                            elevatorCol = currentCol; 
                        }
                        map[currentLevel][currentRow + 1][currentCol].element = 'n';
                        backtraceContainer.push({currentLevel, currentRow + 1, currentCol});
                        currentRow = currentRow + 1; 
                    }
                    else {
                        continue; 
                    }
                }
                //east 
                else if(map[currentLevel][currentRow][currentCol].directionDiscovered == 'e'){
                    //update currentLevel, row, col to be used in next loop
                    //update stack with this floorsquare
                    if(currentCol > 0){
                        if(map[currentLevel][currentRow][currentCol - 1].element == 'E'){
                            elevatorFound = true; 
                            elevatorLevel = currentLevel; 
                            elevatorRow = currentRow; 
                            elevatorCol = currentCol - 1; 
                        }
                        map[currentLevel][currentRow][currentCol - 1].element = 'e';
                        backtraceContainer.push({currentLevel, currentRow, currentCol - 1});
                        currentCol = currentCol - 1; 
                    }
                     else {
                        continue; 
                    }
                   
                }
                //south 
                else if(map[currentLevel][currentRow][currentCol].directionDiscovered == 's'){
                    //update currentLevel, row, col to be used in next loop
                    //update stack with this floorsquare
                    if(currentRow > 0){
                        if(map[currentLevel][currentRow - 1][currentCol].element == 'E'){
                            elevatorFound = true; 
                            elevatorLevel = currentLevel; 
                            elevatorRow = currentRow - 1; 
                            elevatorCol = currentCol; 
                        }
                        map[currentLevel][currentRow - 1][currentCol].element = 's';
                        backtraceContainer.push({currentLevel, currentRow - 1, currentCol});
                        currentRow = currentRow - 1; 
                    }
                     else {
                        continue; 
                    }
                }
                //west
                else if(map[currentLevel][currentRow][currentCol].directionDiscovered == 'w'){
                    //update currentLevel, row, col to be used in next loop
                    //update stack with this floorsquare
                    if(currentCol < numRows - 1){
                        if(map[currentLevel][currentRow][currentCol + 1].element == 'E'){
                            elevatorFound = true; 
                            elevatorLevel = currentLevel; 
                            elevatorRow = currentRow; 
                            elevatorCol = currentCol + 1; 
                        }
                        map[currentLevel][currentRow][currentCol + 1].element = 'w';
                        backtraceContainer.push({currentLevel, currentRow, currentCol + 1});
                        currentCol = currentCol + 1; 
                    }
                     else {
                        continue; 
                    }
                }
            }
        }
    }
    
    void printMapOutput(){
        if(solutionPresent == false){
            for(size_t level = 0; level < numLevels; ++level){
                cout << "//level " << level << '\n'; 
                for(size_t row = 0; row < numRows; ++row) {
                    for(size_t col = 0; col < numCols; ++col){
                        cout << map[level][row][col].element;                        
                    }
                    cout << '\n';
                }
            }
        }
        else if(solutionPresent == true){
            cout << "Start in level " << startLevel << ", row " << startRow << ", column " << startCol << '\n';
            for(size_t level = 0; level < numLevels; ++level){
                cout << "//level " << level << '\n'; 
                for(size_t row = 0; row < numRows; ++row) {
                    for(size_t col = 0; col < numCols; ++col){
                        cout << map[level][row][col].element;
                    }
                    cout << '\n';
                }
            }
        }
    }

    void printListOutput(){
        vector<size_t> out; 
        if(solutionPresent == false){
            cout << "//path taken\n"; 
        }
        else if(solutionPresent == true){
            cout << "//path taken\n"; 
            while(!backtraceContainer.empty()){
                out = backtraceContainer.top();
                // ( << level << , << row << , << col << , << element
                cout << "(" << out[0] << "," << out[1] << "," << out[2] << "," << map[out[0]][out[1]][out[2]].element << ")\n"; 
                backtraceContainer.pop(); 
            }
        }
    }

};

void printHelp(char *argv[]) {
    //use backslash n
    cout << "Usage: " << argv[0] << " \n";
    cout << "This program is designed to take in a map or list of the ship\n";
    cout << "and generate a viable path through the various obstacles\n"; 
    cout << "to the ship's hangar. Output will either be a list or a map\n";
    cout << "depending on usage flagged on command line\n";
} // printHelp()

string getMode(int argc, char * argv[]) {
    // These are used with getopt_long()
    opterr = false; // Let us handle all error output for command line options
    int choice;
    int option_index = 0;
    string mode; 
    char outputType; 
    string routingScheme; 
    string checkInput; 
    bool outputClassification = false; 
    
    option long_options[] = {
        {"stack",      no_argument,   nullptr, 's'},
        {"queue",        no_argument, nullptr, 'q'},
        {"help",        no_argument, nullptr,  'h'},
        {"output", required_argument, nullptr, 'o'},
        { nullptr, 0,         nullptr, '\0' }
    };

    // Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "sqho:", long_options, &option_index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(argv);
            exit(0);

        case 'o':
            outputClassification = true; 
            checkInput = optarg;
            if(checkInput == "M"){
                 outputType = 'M';
            }
            else if (checkInput == "L"){
                outputType = 'L';
            }
            else if (checkInput.size() == 0){
                outputType = 'M';
            }
            break;
        case 'q':
            routingScheme = 'Q';
            break;
        case 's':
            routingScheme = "S";
            break; 

        default:
            cerr << "Error: invalid option" << endl;
            exit(1);
        } // switch++
    } // while

    if(outputClassification == true){
        mode = routingScheme + outputType; 
    }
    else {
        mode = routingScheme; 
    }
    return mode;
} // getMode()


int main(int argc, char *argv[]) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);
    string mode;
    char inputType;
    size_t levelSize, numLevels; 
    mode = getMode(argc, argv);
    cin >> inputType; 
    cin >> numLevels; 
    cin >> levelSize;
    spaceShip ship(inputType, numLevels, levelSize);
    if(mode == "SM"){
        if(ship.inputType == 'M'){
            ship.readMapToMemory(); 
        }
        else if(ship.inputType == 'L'){
            ship.readListToMemory();
        }
        ship.searchContainerStack();
        ship.backTracing(); 
        ship.printMapOutput();
    }
    else if(mode == "SL"){
        if(ship.inputType == 'M'){
            ship.readMapToMemory(); 
        }
        else if(ship.inputType == 'L'){
            ship.readListToMemory();
        } 
        ship.searchContainerStack();
        ship.backTracing();
        ship.printListOutput();
    }
    else if(mode =="QM"){
        if(ship.inputType == 'M'){
            ship.readMapToMemory(); 
        }
        else if(ship.inputType == 'L'){
            ship.readListToMemory();
        }
        ship.searchContainerQueue();
        ship.backTracing();
        ship.printMapOutput();
    }
    else if(mode == "QL"){
        if(ship.inputType == 'M'){
            ship.readMapToMemory(); 
        }
        else if(ship.inputType == 'L'){
            ship.readListToMemory();
        }
        ship.searchContainerQueue();
        ship.backTracing();
        ship.printListOutput();
    }
   

    return 0;
} // main()


