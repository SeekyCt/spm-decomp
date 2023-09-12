/*
    memory is the game's custom library for handling memory allocation
    Heap 7 is the 'smart' heap, which regularly tries to move allocations forwards in
    the heap so that free space is in a larger block at the end. Allocations are accessed
    by structs instead of direct pointers, since the data location can change.
*/

#pragma once

#include <common.h>
#include <wii/gx.h>
#include <wii/mem.h>

CPP_WRAPPER(spm::filemgr)

struct _FileEntry;

CPP_WRAPPER_END()

CPP_WRAPPER(spm::memory)

USING(spm::filemgr::_FileEntry)
USING(wii::gx::GXTexObj)
USING(wii::mem::MEMHeapHandle)

#define MEM1_HEAP_COUNT 3
#define MEM2_HEAP_COUNT 6
#define HEAP_COUNT 9
#define SMART_HEAP_ID 7
#define SMART_ALLOCATION_MAX 2048

enum Heap
{
/* 0x0 */ HEAP_MAIN,
/* 0x1 */ HEAP_MAP,
/* 0x2 */ HEAP_MEM1_UNUSED,
/* 0x3 */ HEAP_EXT,
/* 0x4 */ HEAP_EFFECT,
/* 0x5 */ HEAP_WPAD,
/* 0x6 */ HEAP_SOUND,
/* 0x7 */ HEAP_SMART,
/* 0x8 */ HEAP_MEM2_UNUSED
};

enum HeapSizeType
{
/* 0x0 */ HEAPSIZE_PERCENT_REMAINING,
/* 0x1 */ HEAPSIZE_ABSOLUTE_KB
};

typedef struct
{
/* 0x0 */ s32 type; // see enum above
/* 0x4 */ s32 size;
} HeapSize;
SIZE_ASSERT(HeapSize, 0x8)

DECOMP_STATIC(HeapSize memory_size_table[9])

typedef struct
{
/* 0x00 */ MEMHeapHandle heapHandle[HEAP_COUNT]; // pointer to the MEM library's heap struct
/* 0x24 */ void * heapStart[HEAP_COUNT]; // pointer to the start of the heap
/* 0x48 */ void * heapEnd[HEAP_COUNT]; // pointer to the end of the heap
} MemWork;
SIZE_ASSERT(MemWork, 0x6c)

DECOMP_STATIC(MemWork memory_work)
DECOMP_STATIC(MemWork * memory_wp)

typedef struct _SmartAllocation
{
/* 0x00 */ void * data; // space on the smart heap for the user to put their data,
                        // may change during lifetime
/* 0x04 */ size_t size; // size of the space for data on the smart heap
/* 0x08 */ struct _FileEntry * fileEntry; // set if this allocation is being used for a file,
                                          // gives special behaviour
/* 0x0C */ u16 flag; // 1 for in use, 0 otherwise
/* 0x0E */ u8 type; // used to group for de-allocation
/* 0x0F */ u8 unknown_0xf; // padding?
/* 0x10 */ size_t spaceAfter; // space after the data before the next allocated data
                              // (or the end of the heap)
/* 0x14 */ struct _SmartAllocation * next; // next item in the allocated or free linked list
/* 0x18 */ struct _SmartAllocation * prev; // previous item in the allocated or free linked list
} SmartAllocation;
SIZE_ASSERT(SmartAllocation, 0x1c)

typedef struct
{
/* 0x0000 */ void * heapStart; // pointer to the block of allocated memory to work with
/* 0x0004 */ SmartAllocation allocations[SMART_ALLOCATION_MAX];
/* 0xE004 */ size_t heapStartSpace; // free space at the beginning of the heap
/* 0xE008 */ SmartAllocation * allocatedStart; // first item in the allocated linked list
/* 0xE00C */ SmartAllocation * allocatedEnd; // last item in the allocated linked list
/* 0xE010 */ SmartAllocation * freeStart; // first item in free allocation linked list
/* 0xE014 */ SmartAllocation * freeEnd; // last item in free allocation linked list
/* 0xE018 */ u32 freedThisFrame; // number of allocations freed this frame
} SmartWork;
SIZE_ASSERT(SmartWork, 0xe01c)

DECOMP_STATIC(SmartWork memory_smartWork)
DECOMP_STATIC(SmartWork * memory_swp)

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
GXTexObj * smartTexObj(GXTexObj * texObj, SmartAllocation * imageAllocation);

// New to SPM:

// TODO: C++

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

CPP_WRAPPER_END()
