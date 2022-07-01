/*
    Game's custom library for handling memory allocation
    Heap 7 is the 'smart' heap, which regularly tries to move allocations forwards in
    the heap so that free space is in a larger block at the end. Allocations are accessed
    by structs instead of direct pointers, since the data location can change.
*/

#pragma once

#include <common.h>
#include <wii/mem.h>

#define MEM1_HEAP_COUNT 3
#define MEM2_HEAP_COUNT 6
#define HEAP_COUNT 9
#define SMART_HEAP_ID 7
#define SMART_ALLOCATION_MAX 2048

typedef struct
{
    MEMHeapHandle heapHandle[HEAP_COUNT];
    void * heapStart[HEAP_COUNT];
    void * heapEnd[HEAP_COUNT];
} MemWork;

typedef struct _SmartAllocation
{
    void * data; // space on the smart heap for user to put their data
    size_t size; // size of the space for data on the smart heap
    struct _FileEntry * fileEntry; // allows special treatment if this allocation is for a file
    u16 flag; // 1 for in use, 0 otherwise
    u8 type; // used to group for deallocation
    u8 unknown_0xf; // padding?
    size_t spaceAfter; // amount of free space in the smart heap after the data of this allocation
    struct _SmartAllocation * next; // next item in the allocated or free linked list
    struct _SmartAllocation * prev; // previous item in the allocated or free linked list
} SmartAllocation;

typedef struct
{
    void * heapStart; // pointer to the block of allocated memory to work with
    SmartAllocation allocations[SMART_ALLOCATION_MAX];
    size_t heapStartSpace; // free space at the beginning of the heap
    SmartAllocation * allocatedStart; // first item in the allocated linked list
    SmartAllocation * allocatedEnd; // last item in the allocated linked list
    SmartAllocation * freeStart; // first item in free allocation linked list
    SmartAllocation * freeEnd; // last item in free allocation linked list
    u32 freedThisFrame; // number of allocations freed this frame
} SmartWork;

enum HeapSizeType
{
    HEAPSIZE_PERCENT_REMAINING,
    HEAPSIZE_ABSOLUTE_KB
};

typedef struct
{
    s32 type;
    s32 size;
} HeapSize;

extern s32 g_bFirstSmartAlloc;

/*
    Initialise heaps
*/
void memInit();

/*
    Clear the contents of a heap
*/
void memClear(s32 heapId);

/*
    Allocate a block of memory from a heap
*/
void * __memAlloc(s32 heapId, size_t size);

/*
    Free a block of memory back to a heap
*/
void __memFree(s32 heapId, void * ptr);

/*
    Initialise the smart heap (requires memInit to have ran)
*/
void smartInit();

/*
    Frees all allocations of a certain type
    Freeing type 3 also frees type 4
*/  
void smartAutoFree(s32 type);

/*
    Frees a smart allocation back to the smart heap
*/
void smartFree(SmartAllocation * lp);

/*
    Allocates memory from the smart heap of a certain type
*/
SmartAllocation * smartAlloc(size_t size, u8 type);

/*
    Moves allocations forwards in the heap where possible to collect empty space
    into a larger block at the end
*/
void smartGarbage();

/*
    Sets up a GXTexObj with an image stored behind a smart allocation
*/
void * smartTexObj(void * texObj, SmartAllocation * imageAllocation);

// New to SPM:

/*
    C++ operator new
*/
void * __nw__FUl(size_t size);

/*
    C++ operator delete
*/
void __dl__FPv(void * ptr);

/*
    Frees a block of memory back to heap 0
*/
void __sys_free(void * ptr);
