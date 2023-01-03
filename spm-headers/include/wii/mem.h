/*
    Revolution SDK MEM Library
*/

#pragma once

#include <common.h>

CPP_WRAPPER(wii::mem)

typedef struct _MEMAllocation
{
/* 0x0 */ u16 magic; // 'UD' / 'FR'
/* 0x2 */ u16 flags;
/* 0x4 */ u32 size;
/* 0x8 */ struct _MEMAllocation * prev;
/* 0xC */ struct _MEMAllocation * next;
} MEMAllocation;
SIZE_ASSERT(MEMAllocation, 0x10)

typedef struct _MEMEXPHeap
{
/* 0x00 */ u32 magic; // 'EXPH'
/* 0x04 */ struct _MEMEXPHeap * prevHeap;
/* 0x08 */ struct _MEMEXPHeap * nextHeap;
/* 0x0C */ u8 unknown_0xc[0x18 -  0xc];
/* 0x18 */ void * start;
/* 0x1C */ void * end;
/* 0x20 */ u8 unknown_0x20[0x3c - 0x20];
/* 0x3C */ MEMAllocation * firstFree;
/* 0x40 */ MEMAllocation * lastFree;
/* 0x44 */ MEMAllocation * firstUsed;
/* 0x48 */ MEMAllocation * lastUsed;
/* 0x4C */ u8 unknown_0x4c[0x50 - 0x4c];
} MEMEXPHeap;
SIZE_ASSERT(MEMEXPHeap, 0x50)

#define MEM_FLAG_FILL_0 (1 << 0) // initialise allocated memory as 0
#define MEM_FLAG_THREAD_CONTROL (1 << 2) // use mutexes for access when handling heap

typedef void * MEMHeapHandle;

UNKNOWN_FUNCTION(FindContainHeap_);
UNKNOWN_FUNCTION(MEMiInitHeapHead);
UNKNOWN_FUNCTION(MEMiFinalizeHeap);
MEMHeapHandle MEMFindContainHeap(void * ptr);
UNKNOWN_FUNCTION(AllocUsedBlockFromFreeBlock_);
UNKNOWN_FUNCTION(AllocFromHead_);
UNKNOWN_FUNCTION(AllocFromTail_);
UNKNOWN_FUNCTION(RecycleRegion_);
MEMHeapHandle MEMCreateExpHeapEx(void * memory, size_t size, s32 flags);
void MEMDestroyExpHeap(MEMHeapHandle handle);
void * MEMAllocFromExpHeapEx(MEMHeapHandle handle, size_t size, u32 alignment);
void MEMFreeToExpHeap(MEMHeapHandle handle, void * ptr);
size_t MEMGetAllocatableSizeForExpHeapEx(MEMHeapHandle handle, u32 alignment);
size_t MEMGetSizeForMBlockExpHeap(void * ptr);
UNKNOWN_FUNCTION(MEMCreateFrmHeapEx);
UNKNOWN_FUNCTION(MEMDestroyFrmHeap);
UNKNOWN_FUNCTION(MEMAllocFromFrmHeapEx);
UNKNOWN_FUNCTION(MEMFreeToFrmHeap);
UNKNOWN_FUNCTION(MEMGetAllocatableSizeForFrmHeapEx);
UNKNOWN_FUNCTION(AllocatorAllocForExpHeap_);
UNKNOWN_FUNCTION(AllocatorFreeForExpHeap_);
UNKNOWN_FUNCTION(AllocatorAllocForFrmHeap_);
UNKNOWN_FUNCTION(AllocatorFreeForFrmHeap_);
UNKNOWN_FUNCTION(MEMAllocFromAllocator);
UNKNOWN_FUNCTION(MEMFreeToAllocator);
UNKNOWN_FUNCTION(MEMInitAllocatorForExpHeap);
UNKNOWN_FUNCTION(MEMInitAllocatorForFrmHeap);
UNKNOWN_FUNCTION(MEMInitList);
UNKNOWN_FUNCTION(MEMAppendListObject);
UNKNOWN_FUNCTION(MEMRemoveListObject);
UNKNOWN_FUNCTION(MEMGetNextListObject);

CPP_WRAPPER_END()
