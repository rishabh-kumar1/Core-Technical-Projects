/*
 * Project 1
 * EECS 370 LC-2K Instruction-level simulator
 *
 * Make sure *not* to modify printState or any of the associated functions
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Machine Definitions
#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */

// File
#define MAXLINELENGTH 1000 /* MAXLINELENGTH is the max number of characters we read */

typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

extern void cache_init(int blockSize, int numSets, int blocksPerSet);
extern int cache_access(int addr, int write_flag, int write_data);
extern void printStats();
static stateType state;
static int num_mem_accesses = 0;

int mem_access(int addr, int write_flag, int write_data) {
    ++num_mem_accesses;
    if (write_flag) {
        state.mem[addr] = write_data;
    }
    return state.mem[addr];
}
int get_num_mem_accesses(){
	return num_mem_accesses;
}

void printState(stateType *);
int convertNum(int);
void build(stateType state);
void printFinalState(int numberOfInstructions, stateType state);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    for(int x = 0; x < NUMMEMORY; x++){
        state.mem[x] = 0;
    }
    
    for(int x = 0; x < NUMREGS; x++){
        state.reg[x] = 0;
    }

    /* read the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    build(state);

    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i=0; i<statePtr->numMemory; i++) {
              printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i=0; i<NUMREGS; i++) {
              printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

void build(stateType state){
    int currentOpcode, currentArgument0, currentArgument1, currentArgument2, currentOffset; 
    int numberOfInstructions = 0; 
    int keepBuilding = 0; 
    while(keepBuilding == 0){
        printState(&state);

        currentOpcode = state.mem[state.pc] >> 22; 
        currentArgument0 = (state.mem[state.pc] >> 19) & 0x7; 
        currentArgument1 = (state.mem[state.pc] >> 16) & 0x7;
        currentArgument2 = state.mem[state.pc] & 0x7;
        currentOffset = convertNum(state.mem[state.pc] & 65535) >> 0;

        if(currentOpcode == 7){
            state.pc++;
        }
        else if(currentOpcode == 6){
            state.pc++;
            keepBuilding = 2;
        }
        else if(currentOpcode == 5){
            state.reg[currentArgument1] = state.pc + 1; 
            state.pc = state.reg[currentArgument0];
        }
        else if(currentOpcode == 4){
            if(state.reg[currentArgument0] != state.reg[currentArgument1]) state.pc++;
            else if(state.reg[currentArgument0] == state.reg[currentArgument1]) state.pc = currentOffset + 1 + state.pc;
        }
        else if(currentOpcode == 3){
            state.mem[state.reg[currentArgument0] + currentOffset] = state.reg[currentArgument1];
            state.pc++;
        }
        else if(currentOpcode == 2){
            state.reg[currentArgument1] = state.mem[state.reg[currentArgument0] + currentOffset];
            state.pc++;
        }
        else if(currentOpcode == 1){
            state.reg[currentArgument2] = ~(state.reg[currentArgument1] | state.reg[currentArgument0]);
            state.pc++;
        }
        else if(currentOpcode == 0){
            state.reg[currentArgument2] = state.reg[currentArgument0] + state.reg[currentArgument1];
            state.pc++;
        }
        numberOfInstructions++;
    }
    printFinalState(numberOfInstructions, state);
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit Linux integer */
    if (num & (1<<15) ) {
        num -= (1<<16);
    }
    return(num);
}

void printFinalState(int numberOfInstructions, stateType state){
    printf("machine halted\n");
    printf("total of %d instructions executed\n", numberOfInstructions);
    printf("final state of machine:\n");
    printState(&state);
}

