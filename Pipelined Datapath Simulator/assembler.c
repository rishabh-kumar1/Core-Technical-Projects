/**
 * Project 2
 * Assembler code fragment for LC-2K 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int globalLabelChecker(char *labelGiven);
int anyUndefinedLabels(char *labelGiven);
int anyGlobals(char *labelGiven);

struct Label{
    char providedOpcode[6];
    int address; 
    char symbolicLabel[7];
    int offsetSize; 
    char presentSection; 
};
int currentAddress = 0; 
int currentTextSectionSize = 0; 
int currentDataSectionSize = 0; 
int howMuchReloMvmt = 0; 
int sizeOfLocals = 0; 
int sizeOfGlobals = 0; 
int sizeOfUndefineds = 0; 
struct Label reloMvmt[MAXLINELENGTH];
struct Label locallyDefinedLabels[MAXLINELENGTH];
struct Label globallyDefinedLabels[MAXLINELENGTH];
struct Label undefinedGlobalLabels[MAXLINELENGTH]; 

int addressLocator(int thisAddress, int whatType, char* arg0) {
    int addressTemporary = -100; 
    if(!globalLabelChecker(arg0)){
        int x = 0; 
        while(x < sizeOfLocals){
            if (!strcmp(locallyDefinedLabels[x].symbolicLabel, arg0)){
                addressTemporary = locallyDefinedLabels[x].address;
                break;
            }
            x++;
        }
    }
    else if(globalLabelChecker(arg0)){  
        int x = 0; 
        while(x < sizeOfUndefineds){
            if(strcmp(undefinedGlobalLabels[x].symbolicLabel, arg0)){
                //do nothing
            }
            else if(!strcmp(undefinedGlobalLabels[x].symbolicLabel, arg0)){
                if(whatType == 4){
                    printf("undefined address\n");
                    exit(1);
                }
                else {
                    addressTemporary = 0; 
                    break; 
                }
            }
            x++;
        }
        int b = 0; 
        while(b < sizeOfGlobals){
            if(strcmp(globallyDefinedLabels[b].symbolicLabel, arg0)){
                //do nothing
            }
            else if (!strcmp(globallyDefinedLabels[b].symbolicLabel, arg0)){
                addressTemporary = globallyDefinedLabels[b].address;
                break; 
            }
            b++;
        }
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
    int currentOffset = 0; 
    char temporaryPlaceholder = 'T';
    while (readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        if (!isNumber(arg2) && (!strcmp(opcode, "sw") || !strcmp(opcode, "lw"))){
            reloMvmt[howMuchReloMvmt].offsetSize = currentOffset;
            strcpy(reloMvmt[howMuchReloMvmt].providedOpcode, opcode);
            strcpy(reloMvmt[howMuchReloMvmt].symbolicLabel, arg2);
            howMuchReloMvmt++;
        }
        if(!strcmp(opcode, ".fill")){
            if(temporaryPlaceholder == 'T'){
                temporaryPlaceholder = 'D';
                currentOffset = 0; 
            }
            if(!isNumber(arg0)){
                reloMvmt[howMuchReloMvmt].offsetSize = currentOffset; 
                strcpy(reloMvmt[howMuchReloMvmt].symbolicLabel, arg0);
                strcpy(reloMvmt[howMuchReloMvmt].providedOpcode, opcode);
                howMuchReloMvmt++;
                if((anyUndefinedLabels(arg0) == -1) && !anyGlobals(arg0) && globalLabelChecker(arg0)){
                    undefinedGlobalLabels[sizeOfUndefineds].address = 0; 
                    undefinedGlobalLabels[sizeOfUndefineds].presentSection = temporaryPlaceholder;
                    strcpy (undefinedGlobalLabels[sizeOfUndefineds].symbolicLabel, arg0);
                    strcpy (undefinedGlobalLabels[sizeOfUndefineds].providedOpcode, opcode);
                    sizeOfUndefineds++;
                }
            }
            else if(isNumber(arg0)){
                //do nothing - failsafe
            }
            currentDataSectionSize++;
        }
        if(!strlen(label)){}
        if(strlen(label)){
            if(!globalLabelChecker(label)){
                locallyDefinedLabels[sizeOfLocals].address = currentAddress; 
                locallyDefinedLabels[sizeOfLocals].offsetSize = currentOffset; 
                locallyDefinedLabels[sizeOfLocals].presentSection = temporaryPlaceholder; 
                strcpy(locallyDefinedLabels[sizeOfLocals].symbolicLabel, label);
                strcpy(locallyDefinedLabels[sizeOfLocals].providedOpcode, opcode);
                sizeOfLocals++;
            }
            else if(globalLabelChecker(label)){
                if(anyUndefinedLabels(label) != 0){
                    int calc = anyUndefinedLabels(label);
                    while(calc < sizeOfUndefineds){
                        undefinedGlobalLabels[calc] = undefinedGlobalLabels[calc + 1];
                        calc++;
                    }
                    --sizeOfUndefineds;
                }
                globallyDefinedLabels[sizeOfGlobals].presentSection = currentAddress; 
                globallyDefinedLabels[sizeOfGlobals].address = temporaryPlaceholder; 
                globallyDefinedLabels[sizeOfGlobals].offsetSize = currentOffset; 
                strcpy(globallyDefinedLabels[sizeOfGlobals].symbolicLabel, label);
                strcpy(globallyDefinedLabels[sizeOfGlobals].providedOpcode, opcode);
                sizeOfGlobals++;    
            }
        }
        if((anyUndefinedLabels(arg2) == -1) && globalLabelChecker(arg2) && !anyGlobals(arg2)){
            undefinedGlobalLabels[sizeOfUndefineds].presentSection = temporaryPlaceholder;
            undefinedGlobalLabels[sizeOfUndefineds].address = 0; 
            strcpy(undefinedGlobalLabels[sizeOfUndefineds].symbolicLabel, arg2);
            strcpy(undefinedGlobalLabels[sizeOfUndefineds].providedOpcode, opcode);
            sizeOfUndefineds++;
        }
        if (strcmp(opcode, ".fill")){
            currentTextSectionSize++;
        }
        currentAddress++;
        currentOffset++;
    }

    for(int outsideIterator = 0; outsideIterator < sizeOfLocals; outsideIterator++){
        for(int insideIterator = outsideIterator + 1; insideIterator < sizeOfLocals; insideIterator++){
            if(!strcmp(locallyDefinedLabels[outsideIterator].symbolicLabel, locallyDefinedLabels[insideIterator].symbolicLabel)){
                printf("Same label found\n");
                exit(1);
            }
            else{
                //do nothing
            }
        }
    }

    for(int outsideIterator = 0; outsideIterator < sizeOfGlobals; outsideIterator++){
        for(int insideIterator = outsideIterator + 1; insideIterator < sizeOfGlobals; insideIterator++){
            if(!strcmp(globallyDefinedLabels[outsideIterator].symbolicLabel, globallyDefinedLabels[insideIterator].symbolicLabel)){
                printf("Same label found\n");
                exit(1);
            }
            else{
                //do nothing
            }
        }
    }
    int totalSymbolicLabelSize = sizeOfGlobals + sizeOfUndefineds; 
    rewind(inFilePtr);
    
    currentAddress = 0; 
    fprintf(outFilePtr, "%d ", currentTextSectionSize);
    fprintf(outFilePtr, "%d ", currentDataSectionSize);
    fprintf(outFilePtr, "%d ", totalSymbolicLabelSize);
    fprintf(outFilePtr, "%d\n", howMuchReloMvmt);

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
                argument0 = addressLocator(currentAddress, -1, arg0);
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
                machineCodeResult += addressLocator(currentAddress, 2, arg2);
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
                machineCodeResult += addressLocator(currentAddress, 3, arg2);
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
                machineCodeResult += addressLocator(currentAddress, 4, arg2);
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
    
    int d = 0; 
    while(d < sizeOfGlobals){
        fprintf(outFilePtr, "%s ", globallyDefinedLabels[d].symbolicLabel);
        fprintf(outFilePtr, "%c ", globallyDefinedLabels[d].presentSection);
        fprintf(outFilePtr, "%d\n", globallyDefinedLabels[d].offsetSize);
        d++;
    }
    
    d = 0; 
    while(d < sizeOfUndefineds){
        fprintf(outFilePtr, "%s ", undefinedGlobalLabels[d].symbolicLabel);
        fprintf(outFilePtr, "%c ", 'U');
        fprintf(outFilePtr, "%d\n", 0);
        d++;
    }
    d = 0; 
    while(d < howMuchReloMvmt){
        fprintf(outFilePtr, "%d ", reloMvmt[d].offsetSize);
        fprintf(outFilePtr, "%s ", reloMvmt[d].providedOpcode);
        fprintf(outFilePtr, "%s\n", reloMvmt[d].symbolicLabel);
        d++;
    }
    return 0; 
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

int globalLabelChecker(char *labelGiven){
    //uppercase letters
    if (labelGiven[0] >= 'A' && labelGiven[0] <= 'Z'){
        return 1;
    }
    else {
        return 0;
    }
}

int anyUndefinedLabels(char *labelGiven){
    int i = 0; 
    while(i < sizeOfUndefineds){
        if(strcmp(undefinedGlobalLabels[i].symbolicLabel, labelGiven)){

        }
        else if(!strcmp(undefinedGlobalLabels[i].symbolicLabel, labelGiven)){
            return i; 
        }
        i++;
    }
    return -1; 
}

int anyGlobals(char *labelGiven){
    int i = 0; 
    while(i < sizeOfGlobals){
        if(strcmp(globallyDefinedLabels[i].symbolicLabel, labelGiven)){

        }
        else if(!strcmp(globallyDefinedLabels[i].symbolicLabel, labelGiven)){
            return 1; 
        }
        i++;
    }
    return 0; 
}
