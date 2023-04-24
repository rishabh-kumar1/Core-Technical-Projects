/*
 * EECS 370, University of Michigan
 * Project 4: LC-2K Cache Simulator
 * Instructions are found in the project spec.
 */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define MAX_CACHE_SIZE 256
#define MAX_BLOCK_SIZE 256

extern int mem_access(int addr, int write_flag, int write_data);
extern int get_num_mem_accesses();

enum actionType
{
    cacheToProcessor,
    processorToCache,
    memoryToCache,
    cacheToMemory,
    cacheToNowhere
};

typedef struct blockStruct
{
    int data[MAX_BLOCK_SIZE];
    int valid; 
    int dirty;
    int lruLabel;
    int set;
    int tag;
} blockStruct;

typedef struct cacheStruct
{
    blockStruct blocks[MAX_CACHE_SIZE];
    int blockSize;
    int SObits; 
    int numSets;
    int blocksPerSet;
    int BObits; 
    int Tbits; 

} cacheStruct;

/* Global Cache variable */
cacheStruct cache;
int addrC; 
int write_dataC; 
int write_flagC; 
int blockOffset, setOffset, tag, position; 
int cacheBlocksPerSet; 
int cacheBlockSize; 
int present(int); 
int notPresent(int);
void printAction(int, int, enum actionType);
bool checkPresent(int);
void editLRU(int, int);
int checkLRU(int); 
void printCache();

/*
 * Set up the cache with given command line parameters. This is 
 * called once in main(). You must implement this function.
 */
void cache_init(int blockSize, int numSets, int blocksPerSet){
    int sixteen = 16; 
    cache.blockSize = blockSize; 
    cache.numSets = numSets; 
    cache.blocksPerSet = blocksPerSet;
    cacheBlocksPerSet = cache.blocksPerSet; 
    cacheBlockSize = cache.blockSize;
    cache.SObits = log2(cache.numSets); 
    cache.BObits = log2(cache.blockSize);
    for(int x = 0; x < MAX_CACHE_SIZE; x++){
        cache.blocks[x].tag = -1;
        cache.blocks[x].dirty = 0; 
        cache.blocks[x].valid = 0; 
        cache.blocks[x].lruLabel = 0; 
    }
    cache.Tbits = sixteen - cache.SObits - cache.BObits; 
}


bool checkPresent(int addr){
    int present = 0; 
    position = 0; 

    int holder = (1 << cache.SObits) - 1; 
    setOffset = (addr >> cache.BObits) & holder; 
   
    holder = (1 << cache.BObits) - 1; 
    blockOffset = addr & holder; 

    holder = (1 << cache.Tbits) - 1; 
    int combo = cache.BObits + cache.SObits; 
    tag = (addr >> combo) & holder; 

    int x = 0; 

    while(x < cacheBlocksPerSet){
        int placeHolder = cache.blocks[setOffset * cacheBlocksPerSet + x].tag; 
        if((cache.blocks[setOffset * cacheBlocksPerSet + x].valid == 1) && (tag == placeHolder)){
            present = 1; 
            position = x; 
            break; 
        }
        x++;
    }

    if(present == 1){
        return true; 
    }
    else{
        return false; 
    }

}

int present(int addr){
    int SOmBlocksPerSet = setOffset * cache.blocksPerSet; 
    editLRU(SOmBlocksPerSet, position);
    int SOmBlocksPerSetPlusPosition = SOmBlocksPerSet + position; 

    if(write_flagC == 1){
        cache.blocks[SOmBlocksPerSetPlusPosition].data[blockOffset] = write_dataC; 
        cache.blocks[SOmBlocksPerSetPlusPosition].dirty = 1; 
        printAction(addr, 1, processorToCache); 
        return 0; 
    } 
    else if(write_flagC == 0){
        printAction(addr, 1, cacheToProcessor);
        return cache.blocks[SOmBlocksPerSetPlusPosition].data[blockOffset];
    }

    return 0; 
}

int notPresent(int addr){
    int start = addr - blockOffset; 
    int placeHolderBlock[MAX_BLOCK_SIZE];
    
    int inject = -1; 

    int x = 0; 
    while(x < cacheBlockSize){
        placeHolderBlock[x] = mem_access(start + x, 0, 0);
        x++;
    }

    x = 0; 
    while (x < cacheBlocksPerSet){
        if(cache.blocks[setOffset*cacheBlocksPerSet + x].valid == 0){
            inject = setOffset*cacheBlocksPerSet + x; 
        }
        x++; 
    }

    if(inject != -1){
        x = 0; 
        while(x < cacheBlockSize){
            cache.blocks[inject].data[x] = placeHolderBlock[x];
            x++;
        }
        printAction(start, cacheBlockSize, memoryToCache);
    }
    else{
        inject = checkLRU(setOffset*cacheBlocksPerSet);
        int original = (cache.blocks[inject].tag * pow(2, cache.BObits + cache.SObits)) + (setOffset * pow(2, cache.BObits));
        if(cache.blocks[inject].dirty == 1){
            x = 0; 
            while(x < cacheBlockSize){
                mem_access(original + x, 1, cache.blocks[inject].data[x]);
                x++;
            }
            printAction(original, cacheBlockSize, cacheToMemory);
            cache.blocks[inject].dirty = 0; 
        }
        else{
            printAction(original, cacheBlockSize, cacheToNowhere); 
        }
        x = 0; 
        while(x < cacheBlockSize){
            cache.blocks[inject].data[x] = placeHolderBlock[x];
            x++;
        }

        cache.blocks[inject].tag = tag; 
        printAction(start, cacheBlockSize, memoryToCache);
        cache.blocks[inject].valid = 1; 
    }

    cache.blocks[inject].tag = tag; 
    cache.blocks[inject].valid = 1; 

    editLRU(setOffset * cacheBlocksPerSet, inject - setOffset*cacheBlocksPerSet);

    if(write_flagC == 1){
        cache.blocks[inject].data[blockOffset] = write_dataC; 
        cache.blocks[inject].dirty = 1; 
        printAction(addr, 1, processorToCache);
        return 0; 
    }
    else if(write_flagC == 0){
        printAction(addr, 1, cacheToProcessor);
        return cache.blocks[inject].data[blockOffset];
    }

    return 0; 

}

/*
 * Access the cache. This is the main part of the project,
 * and should call printAction as is appropriate.
 * It should only call mem_access when absolutely necessary.
 * addr is a 16-bit LC2K word address.
 * write_flag is 0 for reads (fetch/lw) and 1 for writes (sw).
 * write_data is a word, and is only valid if write_flag is 1.
 * The return of mem_access is undefined if write_flag is 1.
 * Thus the return of cache_access is undefined if write_flag i)s 1.
 */
int cache_access(int addr, int write_flag, int write_data) {
    addrC = addr; 
    write_flagC = write_flag; 
    write_dataC = write_data; 
    bool checker = checkPresent(addr); 
    
    if(checker == true){
        return present(addr); 
    }
    else { //checker is false 
        return notPresent(addr); 
    }
}


int checkLRU(int begin){
    int top = -1; 
    int position = 0;
    
    int x = 0; 
    while(x < cacheBlocksPerSet){
        if(cache.blocks[begin + x].lruLabel > top){
            top = cache.blocks[begin + x].lruLabel; 
            position = x; 
        }
        x++;
    }
    return position + begin; 
}

void editLRU(int begin, int update){
    int currentLRU = cache.blocks[begin + update].lruLabel; 
    int x = 0; 
    while(x < cacheBlocksPerSet){
        if((cache.blocks[x + begin].lruLabel <= currentLRU) && (x != update)){
            cache.blocks[begin + x].lruLabel++;
        }
        x++;
    }
    cache.blocks[begin + update].lruLabel = 0; 
}
    





/*
 * print end of run statistics like in the spec. This is not required,
 * but is very helpful in debugging.
 * This should be called once a halt is reached.
 * DO NOT delete this function, or else it won't compile.
 * DO NOT print $$$ in this function
 */
void printStats(){
    return;
}

/*
 * Log the specifics of each cache action.
 *
 * address is the starting word address of the range of data being transferred.
 * size is the size of the range of data being transferred.
 * type specifies the source and destination of the data being transferred.
 *  -    cacheToProcessor: reading data from the cache to the processor
 *  -    processorToCache: writing data from the processor to the cache
 *  -    memoryToCache: reading data from the memory to the cache
 *  -    cacheToMemory: evicting cache data and writing it to the memory
 *  -    cacheToNowhere: evicting cache data and throwing it away
 */
void printAction(int address, int size, enum actionType type)
{
    printf("$$$ transferring word [%d-%d] ", address, address + size - 1);

    if (type == cacheToProcessor) {
        printf("from the cache to the processor\n");
    }
    else if (type == processorToCache) {
        printf("from the processor to the cache\n");
    }
    else if (type == memoryToCache) {
        printf("from the memory to the cache\n");
    }
    else if (type == cacheToMemory) {
        printf("from the cache to the memory\n");
    }
    else if (type == cacheToNowhere) {
        printf("from the cache to nowhere\n");
    }
}

/*
 * Prints the cache based on the configurations of the struct
 * This is for debugging only and is not graded, so you may
 * modify it, but that is not recommended.
 */
void printCache()
{
    printf("\ncache:\n");
    for (int set = 0; set < cache.numSets; ++set) {
        printf("\tset %i:\n", set);
        for (int block = 0; block < cache.blocksPerSet; ++block) {
            printf("\t\t[ %i ]: {", block);
            for (int index = 0; index < cache.blockSize; ++index) {
                printf(" %i", cache.blocks[set * cache.blocksPerSet + block].data[index]);
            }
            printf(" }\n");
        }
    }
    printf("end cache\n");
}

