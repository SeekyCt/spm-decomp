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
    u8 unknown_0x0[0xe - 0x0];
    u8 type;
    u8 unknown_0xf[0x14 - 0xf];
    struct _SmartAllocation * next;
    struct _SmartAllocation * prev;
} SmartAllocation;

#define SMART_ALLOCATION_MAX 2048

typedef struct {
    void * heapStart;
    SmartAllocation allocations[SMART_ALLOCATION_MAX];
    u32 heapSize;
    SmartAllocation * firstAllocated;
    SmartAllocation * lastAllocated;
    SmartAllocation * firstFree;
    SmartAllocation * lastFree;
    u32 unknown_0xe018;
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

extern bool g_bFirstSmartAlloc; // 805ae9ac

void memInit(); // 801a5dcc
void memClear(s32 heapId); // 801a61e4
void * __memAlloc(s32 heapId, size_t size); // 801a626c
void __memFree(s32 heapId, void * ptr); // 801a62f0
void smartInit(); // 801a6300
void smartAutoFree(s32 type); // 801a64f4
void smartFree(SmartAllocation * allocation); // 801a6598
// smartAlloc 801a6794
// smartGarbage 801a6b60
// smartTexObj 801a6cf0

// New to SPM so no symbols:
// 801a6d4c
// 801a6e34
// 801a6e8c

#endif