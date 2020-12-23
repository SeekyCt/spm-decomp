#ifndef MEM_H
#define MEM_H

#include <common.h>

typedef void * MEMHeapHandle;
MEMHeapHandle MEMCreateExpHeapEx(void * memory, u32 size, s32 flags);
void MEMDestroyExpHeap(MEMHeapHandle handle);

#endif