#ifndef MEMORY_H
#define MEMORY_H

#include <common.h>
#include <mem.h>

#define MEM1_HEAP_COUNT 3
#define MEM2_HEAP_COUNT 6
#define HEAP_COUNT 9

typedef struct {
    MEMHeapHandle heapHandle[HEAP_COUNT];
    void * heapStart[HEAP_COUNT];
    void * heapEnd[HEAP_COUNT];
} MemWork;

typedef struct _SmartAllocation {
    void * data;
    size_t size;
    u32 unknown_0x8;
    u16 flag;
    u8 type;
    u8 unknown_0xf; // padding?
    size_t spaceAfter;
    struct _SmartAllocation * next;
    struct _SmartAllocation * prev;
} SmartAllocation;

#define SMART_ALLOCATION_MAX 2048

typedef struct {
    void * heapStart; // pointer to the block of allocated memory to work with
    SmartAllocation allocations[SMART_ALLOCATION_MAX];
    size_t heapStartSpace; // free space at the beginning of the heap
    SmartAllocation * allocatedStart; // first item in the used linked list
    SmartAllocation * allocatedEnd; // last item in the used linked list
    SmartAllocation * freeStart; // first item in free allocation linked list
    SmartAllocation * freeEnd; // last item in free allocation linked list
    u32 freedThisFrame; // number of allocations freed this frame
} SmartWork;

enum HeapSizeType {
    HEAPSIZE_PERCENT_REMAINING,
    HEAPSIZE_ABSOLUTE_KB
};

typedef struct {
    s32 type;
    s32 size;
} HeapSize;

#define SMART_HEAP_ID 7

extern s32 g_bFirstSmartAlloc; // 805ae9ac

void memInit(); // 801a5dcc
void memClear(s32 heapId); // 801a61e4
void * __memAlloc(s32 heapId, size_t size); // 801a626c
void __memFree(s32 heapId, void * ptr); // 801a62f0

void smartInit(); // 801a6300
void smartAutoFree(s32 type); // 801a64f4
void smartFree(SmartAllocation * lp); // 801a6598
SmartAllocation * smartAlloc(size_t size, u8 type); // 801a6794
void smartGarbage(); // 801a6b60
void * smartTexObj(void * texObj, SmartAllocation * imageAllocation); // 801a6cf0

// New to SPM so no symbols:
// 801a6d4c
// 801a6e34
// 801a6e8c

#endif