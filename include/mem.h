#ifndef MEM_H
#define MEM_H

#include <common.h>

typedef void * MEMHeapHandle;
MEMHeapHandle MEMCreateExpHeapEx(void * memory, size_t size, s32 flags); // 80295a5c
void MEMDestroyExpHeap(MEMHeapHandle handle); // 80295b0c
void * MEMAllocFromExpHeapEx(MEMHeapHandle handle, size_t size, u32 alignment); // 80295b3c
void MEMFreeToExpHeap(MEMHeapHandle handle, void * ptr); // 80295bec

#endif