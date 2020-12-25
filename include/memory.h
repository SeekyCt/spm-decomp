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

enum HeapSizeType {
    HEAPSIZE_PERCENT_REMAINING,
    HEAPSIZE_ABSOLUTE_KB
};

typedef struct {
    s32 type;
    s32 size;
} HeapSize;

void memInit(); // 801a5dcc
void memClear(s32 heapId); // 801a61e4
void * __memAlloc(s32 heapId, size_t size); // 801a626c
// __memFree 801a62f0
// smartInit 801a6300
// smartAutoFree 801a64f4
// smartFree 801a6598
// smartAlloc 801a6794
// smartGarbage 801a6b60
// smartTexObj 801a6cf0

// New to SPM so no symbols:
// 801a6d4c
// 801a6e34
// 801a6e8c

#endif