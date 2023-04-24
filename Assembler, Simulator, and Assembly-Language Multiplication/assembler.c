/*
 * Assembler code fragment for LC-2K 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

struct Label{
    int address; 
    char symbolicLabel[10];
};

struct Label labels[MAXLINELENGTH];

int addressLocator(int thisAddress, int currentNumberOfLabels, int whatType, char* arg0) {
    int addressTemporary = -100; 
    int a = 0;
    while(a < currentNumberOfLabels){
        if (!strcmp(labels[a].symbolicLabel, arg0)){
            addressTemporary = labels[a].address;
            break;
        }
        a++;
    }
    if(addressTemporary == -100){
        printf("There was an error in finding the address\n");
        exit(1);
    }
    if (whatType == 4) {
        addressTemporary = (65535 & (addressTemporary - thisAddress - 1));
    }
    else if (whatType == 3 || whatType == 2) {
        addressTemporary &= 65535;
    }
    return addressTemporary;
}

int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    int numberOfLabelsUsed = 0; 
    for(int address = 0; readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2); address++){
        if(strlen(label) == 0){
            continue; 
        }
        else if(strlen(label) > 0){
            strcpy(labels[numberOfLabelsUsed].symbolicLabel, label);
            labels[numberOfLabelsUsed].address = address; 
            numberOfLabelsUsed++;
        }
    }
    
    for(int x = 0; x <= numberOfLabelsUsed; x++){
        for(int y = x+1; y < numberOfLabelsUsed; y++){
            if(!strcmp(labels[x].symbolicLabel, labels[y].symbolicLabel)){
                printf("There is a duplicate Label being used here\n");
                exit(1);
            }
        }
    }

    rewind(inFilePtr);
     
    int currentAddress = 0; 

    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        int machineCodeResult = 0; 
        if(!strcmp(opcode, "nor")){
            unsigned int argument0, argument1, argument2; 
            machineCodeResult += (1<<22);
            argument0 = atoi(arg0);
            argument1 = atoi(arg1);
            argument2 = atoi(arg2);
            machineCodeResult += (argument0 << 19); 
            machineCodeResult += (argument1 << 16);
            machineCodeResult += argument2;
        }   
        else if(!strcmp(opcode, "add")){
            unsigned int argument0, argument1, argument2; 
            machineCodeResult += (0 << 22);
            argument0 = atoi(arg0);
            argument1 = atoi(arg1);
            argument2 = atoi(arg2); 
            machineCodeResult += (argument0 << 19);
            machineCodeResult += (argument1 << 16);
            machineCodeResult += argument2; 
        }
        else if(!strcmp(opcode, "halt")){
            machineCodeResult += (6 << 22);
        }
        else if(!strcmp(opcode, "noop")){
            machineCodeResult += (7 << 22); 
        }
        else if(!strcmp(opcode, ".fill")){
            int argument0; 
            if(!isNumber(arg0)){
                argument0 = addressLocator(currentAddress, numberOfLabelsUsed, -1, arg0);
            }
            else if(isNumber(arg0)){
                argument0 = atoi(arg0);
                // if (argument0<= -32769 || argument0 >= 32768){
                //     printf("Invalid Field Size\n");
                //     exit(1);
                // }
            }
            machineCodeResult += argument0; 
        }
        else if(!strcmp(opcode, "jalr")){
            int argument0, argument1; 
            machineCodeResult += (5 << 22);
            argument0 = atoi(arg0);
            argument1 = atoi(arg1);
            machineCodeResult += (argument0 << 19);
            machineCodeResult += (argument1 << 16);
        }
        else if(!strcmp(opcode, "lw")){
            int argument0, argument1, argument2; 
            machineCodeResult += (1 << 23);
            argument0 = atoi(arg0);
            argument1 = atoi(arg1); 
            machineCodeResult += (argument0 << 19);
            machineCodeResult += (argument1 << 16);
            if(!isNumber(arg2)){
                machineCodeResult += addressLocator(currentAddress, numberOfLabelsUsed, 2, arg2);
            }
            else if(isNumber(arg2)){
                argument2 = atoi(arg2);
                if (argument2 >= 32768 || argument2 <= -32769 ){
                    printf("This field is larger than allowed\n");
                    exit(1);
                }
                argument2 &= 0xFFFF;
                machineCodeResult += argument2; 
            }
        }
        else if(!strcmp(opcode, "sw")){
            int argument0, argument1, argument2; 
            machineCodeResult += (3 << 22);
            argument0 = atoi(arg0);
            argument1 = atoi(arg1);
            machineCodeResult += (argument0 << 19); 
            machineCodeResult += (argument1 << 16);
            if(!isNumber(arg2)){
                machineCodeResult += addressLocator(currentAddress, numberOfLabelsUsed, 3, arg2);
            }
            else if(isNumber(arg2)){
                argument2 = atoi(arg2);
                if (argument2 >= 32768 || argument2 <= -32769 ){
                    printf("This field is larger than allowed\n");
                    exit(1);
                }
                argument2 &= 0xFFFF;
                machineCodeResult += argument2; 
            }
        }
        else if(!strcmp(opcode, "beq")){
            int argument0, argument1, argument2; 
            machineCodeResult += (4 << 22);
            argument0 = atoi(arg0);
            argument1 = atoi(arg1);
            machineCodeResult += (argument0 << 19);
            machineCodeResult += (argument1 << 16);
            if(!isNumber(arg2)){
                machineCodeResult += addressLocator(currentAddress, numberOfLabelsUsed, 4, arg2);
            }
            else if(isNumber(arg2)){
                argument2 = atoi(arg2);
                if(argument2 >= 32768 || argument2 <= -32769){
                    printf("BEQ Field is too big");
                    exit(1);
                }
                argument2 &= 0xFFFF;
                machineCodeResult += argument2;
            }
        }
        else {
            printf("%s", opcode);
            printf("Undefined opcode error");
            exit(1);
        }
        currentAddress++; 
        fprintf(outFilePtr, "%d\n", machineCodeResult);
    }
    return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);
    return(1);
}

int
isNumber(char *string)
{
    /* return 1 if string is a number */
    int i;
    return( (sscanf(string, "%d", &i)) == 1);
}


