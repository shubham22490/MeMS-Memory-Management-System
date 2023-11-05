/*
All the main functions with respect to the MeMS are inplemented here
read the function discription for more details

NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
you are only allowed to implement the functions 
you can also make additional helper functions a you wish

REFER DOCUMENTATION FOR MORE DETAILS ON FUNSTIONS AND THEIR FUNCTIONALITY
*/
// add other headers as required
#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
#include<stdbool.h>
#include<math.h>



/*
Use this macro where ever you need PAGE_SIZE.
As PAGESIZE can differ system to system we should have flexibility to modify this 
macro to make the output of all system same and conduct a fair evaluation. 
*/
#define PAGE_SIZE 4096
#define PROCESS true
#define HOLE false


/*
Initializing the structure of the block of the Main Linked List and the Sub Linked List.
The Type in SubBlock.
false - Hole
true - proccess
*/
struct MemBlock {
    size_t size;
    struct MemBlock* next;
    struct SubBlock* child;
    struct MemBlock* prev;
    void* PAD;
};

struct SubBlock {
    size_t size;
    bool type;
    struct SubBlock* next;
    struct SubBlock* prev;
};

struct MemBlock* freeHead;
int virtualStart;

/*
Initializes all the required parameters for the MeMS system. The main parameters to be initialized are:
1. the head of the free list i.e. the pointer that points to the head of the free list
2. the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
3. any other global variable that you want for the MeMS implementation can be initialized here.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_init(){
    freeHead = NULL;
    virtualStart = 0;
}


/*
This function will be called at the end of the MeMS system and its main job is to unmap the 
allocated memory using the munmap system call.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_finish(){
    
}


/*
Allocates memory of the specified size by reusing a segment from the free list if 
a sufficiently large segment is available. 

Else, uses the mmap system call to allocate more memory on the heap and updates 
the free list accordingly.

Note that while mapping using mmap do not forget to reuse the unused space from mapping
by adding it to the free list.
Parameter: The size of the memory the user program wants
Returns: MeMS Virtual address (that is created by MeMS)
*/ 

struct MemBlock* newBlock(struct MemBlock* ptr, size_t size, size_t processSize){
    struct SubBlock* tempSub;
    tempSub -> type = PROCESS;
    tempSub -> size = processSize;
    tempSub -> next = NULL; tempSub-> prev = NULL;

    if(size - processSize > 0){
        struct SubBlock* tempSub2;
        tempSub2 -> type = HOLE;
        tempSub2 -> size = size - processSize;
        tempSub2 -> next = NULL; tempSub2 -> prev = tempSub;
        tempSub -> next = tempSub2;
    }
    
    struct MemBlock* tempBlock;
    tempBlock->size = size;
    tempBlock->next = NULL; tempBlock -> prev = NULL;
    tempBlock->child = tempSub;

    if(ptr){
        ptr -> next = tempBlock;
        tempBlock -> prev = ptr;
    }

    return tempBlock;
}

struct SubBlock* checkHole(struct MemBlock* blockPtr, size_t size, int *virtualAddress){
    struct SubBlock *subPtr = blockPtr -> child;

    while(subPtr){
        if(subPtr -> type == HOLE && subPtr -> size >= size) break;
        *virtualAddress += subPtr -> size;
        subPtr = subPtr -> next;
    }

    return subPtr;
}

void* mems_malloc(size_t size){
    int virtualAddress = virtualStart;
    struct MemBlock* blockPtr = freeHead;

    // TO check if any hole could fulfil the requirement.
    while(blockPtr){
        struct SubBlock* reqdHole = checkHole(blockPtr, size, &virtualAddress);
        if(reqdHole){
            struct SubBlock *nextSub = reqdHole -> next;
            
            if(size == reqdHole -> size){
                reqdHole -> type = PROCESS;
                return virtualAddress;
            }

            struct SubBlock* tempSub;
            tempSub -> type = PROCESS;
            tempSub -> size = size;
            tempSub -> next = nextSub; tempSub-> prev = reqdHole;
            reqdHole -> next = tempSub;
            if(nextSub) nextSub -> prev = tempSub;
            reqdHole->size = reqdHole->size - size;
            virtualAddress += reqdHole->size;
            
            return virtualAddress;
        }
        if(blockPtr -> next == NULL) break;
        blockPtr = blockPtr -> next;
    }
    
    
    // In case there is no hole which could be used by the memory.
    size_t pageSize = (unsigned long)((double)size/PAGE_SIZE) * PAGE_SIZE;

    blockPtr = newBlock(blockPtr, pageSize, size);
    blockPtr -> PAD = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if(freeHead == NULL){
        freeHead = blockPtr;
    }

    return virtualAddress;
}


/*
this function print the stats of the MeMS system like
1. How many pages are utilised by using the mems_malloc
2. how much memory is unused i.e. the memory that is in freelist and is not used.
3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
Parameter: Nothing
Returns: Nothing but should print the necessary information on STDOUT
*/
void mems_print_stats(){

}


/*
Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
*/
void *mems_get(void*v_ptr){
    
}


/*
this function free up the memory pointed by our virtual_address and add it to the free list
Parameter: MeMS Virtual address (that is created by MeMS) 
Returns: nothing
*/
void mems_free(void *v_ptr){
    
}