/*
    Revolution SDK MEM Library
*/

#pragma once

#include <common.h>

#define MEM_FLAG_FILL_0 (1 << 0) // initialise allocated memory as 0
#define MEM_FLAG_THREAD_CONTROL (1 << 2) // use mutexes for access when handling heap

typedef void * MEMHeapHandle;
MEMHeapHandle MEMFindContainHeap(void * ptr);
MEMHeapHandle MEMCreateExpHeapEx(void * memory, size_t size, s32 flags);
void MEMDestroyExpHeap(MEMHeapHandle handle);
void * MEMAllocFromExpHeapEx(MEMHeapHandle handle, size_t size, u32 alignment);
void MEMFreeToExpHeap(MEMHeapHandle handle, void * ptr);
size_t MEMGetAllocatableSizeForExpHeapEx(MEMHeapHandle handle, u32 alignment);
size_t MEMGetSizeForMBlockExpHeap(void * ptr);
