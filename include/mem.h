/*
    Revolution SDK MEM Library
*/

#ifndef MEM_H
#define MEM_H

#include <common.h>

#define MEM_FLAG_FILL_0 (1 << 0) // initialise allocated memory as 0
#define MEM_FLAG_THREAD_CONTROL (1 << 2) // use mutexes for access when handling heap

typedef void * MEMHeapHandle;
MEMHeapHandle MEMFindContainHeap(void * ptr); // 8029540c
MEMHeapHandle MEMCreateExpHeapEx(void * memory, size_t size, s32 flags); // 80295a5c
void MEMDestroyExpHeap(MEMHeapHandle handle); // 80295b0c
void * MEMAllocFromExpHeapEx(MEMHeapHandle handle, size_t size, u32 alignment); // 80295b3c
void MEMFreeToExpHeap(MEMHeapHandle handle, void * ptr); // 80295bec
size_t MEMGetAllocatableSizeForExpHeapEx(MEMHeapHandle handle, u32 alignment); // 80295cb8
size_t MEMGetSizeForMBlockExpHeap(void * ptr); // 80295d90

#endif