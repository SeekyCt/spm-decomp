/*
    WARNING: Not fully decompiled
    This file is currently not linked into the final dol
*/

#include <common.h>
#include <spm/filemgr.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/gx.h>
#include <wii/os.h>
#include <wii/mem.h>
#include <msl/new>
#include <msl/string.h>

extern "C" {

// .data
static HeapSize size_table[HEAP_COUNT] =
{
#ifdef SPM_KR0
    #error "TODO"
#elif (defined SPM_EU0) || (defined SPM_EU1)
    // MEM1
    { // 0
        HEAPSIZE_ABSOLUTE_KB,
        0x2400
    },
    { // 1
        HEAPSIZE_ABSOLUTE_KB,
        0x1800
    },
    { // 2
        HEAPSIZE_PERCENT_REMAINING,
        100
    },

    // MEM2
    { // 3
        HEAPSIZE_ABSOLUTE_KB,
        0x100
    },
    { // 4
        HEAPSIZE_ABSOLUTE_KB,
        0x100
    },
    { // 5
        HEAPSIZE_ABSOLUTE_KB,
        0x80
    },
    { // 6
        HEAPSIZE_ABSOLUTE_KB,
        0x4400
    },
    { // 7 - smart heap
        HEAPSIZE_PERCENT_REMAINING,
        100
    },
    { // 8
        HEAPSIZE_ABSOLUTE_KB,
        1
    }
#else
    // MEM1
    { // 0
        HEAPSIZE_ABSOLUTE_KB,
        0x2400
    },
    { // 1
        HEAPSIZE_ABSOLUTE_KB,
        0x1800
    },
    { // 2
        HEAPSIZE_ABSOLUTE_KB,
        0x100
    },
    { // 3
        HEAPSIZE_ABSOLUTE_KB,
        0x100
    },
    { // 4
        HEAPSIZE_PERCENT_REMAINING,
        100
    },

    // MEM2
    { // 5
        HEAPSIZE_ABSOLUTE_KB,
        0x80
    },
    { // 6
        HEAPSIZE_ABSOLUTE_KB,
        0x4400
    },
    { // 7 - smart heap
        HEAPSIZE_PERCENT_REMAINING,
        100
    },
    { // 8
        HEAPSIZE_ABSOLUTE_KB,
        1
    }
#endif
};

// .bss
static MemWork work;
static SmartWork smartWork;

// .sdata
static MemWork * wp = &work;
static SmartWork * swp = &smartWork;

// .sbss
static bool memInitFlag = false;
s32 g_bFirstSmartAlloc;
static MEMHeapHandle fallbackHeap;

#define IS_FIRST_SMART_ALLOC(allocation) (allocation->prev == NULL)
#define IS_LAST_SMART_ALLOC(allocation) (allocation->next == NULL)
#define GET_SMART_HEAP_SIZE() (MEMGetSizeForMBlockExpHeap(swp->heapStart))

// NON_MATCHING - https://decomp.me/scratch/fZFB1
void memInit()
{
    // Register usage not matching
    s32 i;
    u32 max;
    u32 min;
    u32 size;
    u32 space;

    // Instruction order doesn't match when geting size_table pointer at the start

    min = (u32) OSGetMEM1ArenaLo();
    max = (u32) OSGetMEM1ArenaHi();

    // Set up MEM1 absolute size heaps
    for (i = 0; i < MEM1_HEAP_COUNT; i++)
    {
        if (size_table[i].type == HEAPSIZE_ABSOLUTE_KB)
        {
            size = (u32) (size_table[i].size * 1024);

            wp->heapStart[i] = (void *) min;
            wp->heapEnd[i] = (void* ) (min + size);

            // "Error: Overheap of heap from arena [%d] \ n"
            assertf(97, (u32)wp->heapEnd[i] <= max, "ERROR: アリーナからのヒープの取得オーバーです。[%d]\n", i);

            min += size;
        }
    }

    // Set up MEM1 relative size heaps with remaining space
    space = max - min;
    for (i = 0; i < MEM1_HEAP_COUNT; i++)
    {
        if (size_table[i].type == HEAPSIZE_PERCENT_REMAINING)
        {
            size = (u32) (((u64) space * size_table[i].size) / 100);

            // "ERROR: Excessive heap acquisition from arena\n"
            assert(111, size >= 32, "ERROR: アリーナからのヒープの取得オーバーです。\n");

            size -= size & 0x1f;

            wp->heapStart[i] = (void *) min;
            wp->heapEnd[i] = (void *) (min + size);

            // "ERROR: Overheap of heap from arena [%d] \ n"
            assertf(117, (u32)wp->heapEnd[i] <= max, "ERROR: アリーナからのヒープの取得オーバーです。[%d]\n", i);

            min += size;
        }
    }

    // Pass MEM1 heaps into MEM library
    for (i = 0; i < MEM1_HEAP_COUNT; i++)
    {
        size = (u32)wp->heapEnd[i] - (u32)wp->heapStart[i];
        wp->heapHandle[i]  = MEMCreateExpHeapEx(wp->heapStart[i], size, 0);
    }

    OSSetMEM1ArenaLo((void *) max);

    // Register usage for min & max wrong way around from here
    min = (u32) OSGetMEM2ArenaLo();
    max = (u32) OSGetMEM2ArenaHi();

    // Set up MEM2 absolute size heaps
    for (i = MEM1_HEAP_COUNT; i < HEAP_COUNT; i++)
    {
        if (size_table[i].type == HEAPSIZE_ABSOLUTE_KB)
        {
            size = (u32) (size_table[i].size * 1024);

            wp->heapStart[i] = (void *) min;
            wp->heapEnd[i] = (void *) (min + size);

            // "Error: Overheap of heap from arena [%d] \ n"
            assertf(148, (u32)wp->heapEnd[i] <= max, "ERROR: アリーナからのヒープの取得オーバーです。[%d]\n", i);

            min += size;
        }
    }

    // Set up MEM2 relative size heaps with remaining space
    space = max - min;
    for (i = MEM1_HEAP_COUNT; i < HEAP_COUNT; i++)
    {
        if (size_table[i].type == HEAPSIZE_PERCENT_REMAINING)
        {
            size = (u32) (((u64) space * size_table[i].size) / 100);

            // "ERROR: Excessive heap acquisition from arena\n"
            assert(162, size >= 32, "ERROR: アリーナからのヒープの取得オーバーです。\n");

            size -= size & 0x1f;

            wp->heapStart[i] = (void *) min;
            wp->heapEnd[i] = (void *) (min + size);

            // "Error: Overheap of heap from arena [%d] \ n"
            assertf(168, (u32)wp->heapEnd[i] <= max, "ERROR: アリーナからのヒープの取得オーバーです。[%d]\n", i);

            min += size;
        }
    }

    // Pass MEM2 heaps into MEM library
    for (i = MEM1_HEAP_COUNT; i < HEAP_COUNT; i++)
    {
        size = (u32)wp->heapEnd[i] - (u32)wp->heapStart[i];
        wp->heapHandle[i]  = MEMCreateExpHeapEx(wp->heapStart[i], size, 0);
    }

    OSSetMEM2ArenaLo((void *) max);
    
    // Clear all heaps
    for (i = 0; i < HEAP_COUNT; i++)
        memClear(i);

    memInitFlag = true;
}

void memClear(s32 heapId)
{
    if (heapId == SMART_HEAP_ID)
    {
        MEMDestroyExpHeap(wp->heapHandle[heapId]);
        MEMCreateExpHeapEx(wp->heapStart[heapId], (u32)wp->heapEnd[heapId] - (u32)wp->heapStart[heapId], MEM_FLAG_THREAD_CONTROL);
    }
    else
    {
        MEMDestroyExpHeap(wp->heapHandle[heapId]);
        MEMCreateExpHeapEx(wp->heapStart[heapId], (u32)wp->heapEnd[heapId] - (u32)wp->heapStart[heapId], MEM_FLAG_THREAD_CONTROL | MEM_FLAG_FILL_0);
    }
}

void * __memAlloc(s32 heapId, size_t size)
{
    void * p = MEMAllocFromExpHeapEx(wp->heapHandle[heapId], size, 0x20);

    // "Memory allocation error"
    assertf(221, p, "メモリ確保エラー [id = %d][size = %d]", heapId, size);

    return p;
}

void __memFree(s32 heapId, void * ptr)
{
    MEMFreeToExpHeap(wp->heapHandle[heapId], ptr);
}

void smartInit()
{
    // Allocate the entire heap for custom use
    u32 size = MEMGetAllocatableSizeForExpHeapEx(wp->heapHandle[SMART_HEAP_ID], 4);
    void * p = __memAlloc(SMART_HEAP_ID, size);

    // Space at the start of the heap is the whole heap, allocated list is empty
    swp->heapStart = p;
    swp->allocatedStart = NULL;
    swp->allocatedEnd = NULL;
    swp->heapStartSpace = GET_SMART_HEAP_SIZE();

    // Initialise free list
    SmartAllocation * curAllocation = swp->allocations;
    for (s32 i = 0; i < SMART_ALLOCATION_MAX; i++)
    {
        curAllocation->next = curAllocation + 1;
        curAllocation->prev = curAllocation - 1;
        curAllocation++;
    }
    swp->freeStart = &swp->allocations[0];
    swp->freeStart->prev = NULL;
    swp->freeEnd = &swp->allocations[SMART_ALLOCATION_MAX - 1];
    swp->freeEnd->next = NULL;
    
    swp->freedThisFrame = 0;
    g_bFirstSmartAlloc = false;
}

void smartAutoFree(s32 type)
{
    SmartAllocation * curAllocation;
    SmartAllocation * next;

    for (curAllocation = swp->allocatedStart; curAllocation; curAllocation = next)
    {
        next = curAllocation->next;

        // TODO: type weirdness
        if (curAllocation->type == (u16)type)
            smartFree(curAllocation);
    }

    if (type == 3)
        smartAutoFree(4);
}

void smartFree(SmartAllocation * lp)
{
    // "Invalid pointer. p=0x%x"
    assertf(403, lp, "無効なポインタです。p=0x%x\n", lp);

    // "Already free. p=0x%x"
    assertf(404, lp->flag != 0, "すでに開放されています。p=0x%x\n", lp);

    if (lp->type == 4)
    {
        lp->type = 3;
    }
    else
    {
        // Update previous item in allocated list
        if (IS_FIRST_SMART_ALLOC(lp))
        {
            swp->allocatedStart = lp->next;
            if (swp->allocatedStart)
                swp->allocatedStart->prev = NULL;
        }
        else
        {
            lp->prev->next = lp->next;
        }

        // Update next item in allocated list
        if (IS_LAST_SMART_ALLOC(lp))
        {
            swp->allocatedEnd = lp->prev;
            if (swp->allocatedEnd)
                swp->allocatedEnd->next = NULL;
        }
        else
        {
            lp->next->prev = lp->prev;
        }
        
        // Update spaceAfter of previous item in allocated list
        size_t spaceFreed = lp->size + lp->spaceAfter;
        if (!IS_FIRST_SMART_ALLOC(lp))
            lp->prev->spaceAfter += spaceFreed;
        else
            swp->heapStartSpace += spaceFreed;

        // Append to free list
        if (swp->freeStart == NULL)
        {
            swp->freeStart = lp;
            lp->prev = NULL;
        }
        else
        {
            // "The list structure is broken"
            // was getting a compiler error with plain sjis
            assert(446, swp->freeEnd, "リスト" "\x8d\x5c\x91\xA2" "が壊れています");

            swp->freeEnd->next = lp;
            lp->prev = swp->freeEnd;
        }

        lp->flag = 0;
        lp->next = NULL;
        lp->size = 0;
        lp->spaceAfter = 0;
        lp->fileEntry = NULL;
        swp->freeEnd = lp;
        swp->freedThisFrame++;
    }
}

SmartAllocation * smartAlloc(size_t size, u8 type)
{
    // Special behaviour if this is the first time running
    if (!g_bFirstSmartAlloc)
    {
        g_bFirstSmartAlloc = true;
        smartAutoFree(3); // inline
    }

    // Wait if any allocations were freed this frame
    if (swp->freedThisFrame != 0)
    {
        sysWaitDrawSync();
        swp->freedThisFrame = 0;
    }

    // Pick a free SmartAllocation to use
    SmartAllocation * new_lp = swp->freeStart;

    // "Heap list shortage"
    assert(482, new_lp, "ヒープリスト足りない\n");
    
    // Update previous item in the free list
    if (IS_FIRST_SMART_ALLOC(new_lp))
    {
        swp->freeStart = new_lp->next;
        if (swp->freeStart)
            swp->freeStart->prev = NULL;
    }
    else
    {
        new_lp->prev->next = new_lp->next;
    }

    // Update next item in the free list
    if (IS_LAST_SMART_ALLOC(new_lp))
    {
        swp->freeEnd = new_lp->prev;
        if (swp->freeEnd)
            swp->freeEnd->next = NULL;
    }
    else
    {
        new_lp->next->prev = new_lp->prev;
    }

    // Round up 0x20
    if (size & 0x1f)
        size += 0x20 - (size & 0x1f);

    // Fill in some of the new allocation
    new_lp->flag = 1;
    new_lp->type = type;
    new_lp->size = size;
    new_lp->fileEntry = NULL;

    if (swp->heapStartSpace >= size)
    {
        // If it'll fit at the start of the heap, insert at the start of the list
        new_lp->data = swp->heapStart;
        new_lp->spaceAfter = swp->heapStartSpace - size;
        new_lp->next = swp->allocatedStart;
        new_lp->prev = NULL;

        swp->heapStartSpace = 0;

        if (swp->allocatedStart)
            swp->allocatedStart->prev = new_lp;

        swp->allocatedStart = new_lp;

        if(IS_LAST_SMART_ALLOC(new_lp))
        {
            u32 heapSize = GET_SMART_HEAP_SIZE();
            new_lp->spaceAfter = (u32)swp->heapStart + heapSize
                                 - (u32)new_lp->data - new_lp->size;

            swp->allocatedEnd = new_lp;
        }

        return new_lp;
    }
    else
    {
        // If it'll fit after any existing allocations, insert there
        SmartAllocation * lp = swp->allocatedStart;
        while (lp != NULL)
        {
            if (lp->spaceAfter >= size)
            {
                new_lp->data = (void *) ((u32)lp->data + lp->size);
                new_lp->spaceAfter = lp->spaceAfter - size;
                new_lp->next = lp->next;
                new_lp->prev = lp;

                lp->spaceAfter = 0;

                if (!IS_LAST_SMART_ALLOC(lp))
                    lp->next->prev = new_lp;
                else
                    swp->allocatedEnd = new_lp;

                lp->next = new_lp;

                return new_lp;
            }
            lp = lp->next;
        }

        // Try freeing space as a last resort
        for (s32 i = 0; i < 3; i++)
        {
            switch (i)
            {
                case 0:
                    // required to match
                    break;
                case 1:
                    _fileGarbage(1);
                    break;
                case 2:
                    _fileGarbage(0);
                    break;
            }
            smartGarbage();

            lp = swp->allocatedEnd;

            if (lp->spaceAfter >= size)
            {
                new_lp->data = (void *) ((u32)lp->data + lp->size);
                new_lp->spaceAfter = lp->spaceAfter - size;
                new_lp->next = lp->next;
                new_lp->prev = lp;

                lp->spaceAfter = 0;

                if (!IS_LAST_SMART_ALLOC(lp))
                    lp->next->prev = new_lp;
                else
                    swp->allocatedEnd = new_lp;

                lp->next = new_lp;

                return new_lp;
            }
        }

        // "Garbage collect, but not enough heap"
        assert(640, 0, "smartAlloc: ガーベージコレクトしたけどヒープ足りない\n");

        return NULL;
    }
}

void smartGarbage()
{
    sysWaitDrawSync();

    // Update space at the start of the heap
    if (swp->allocatedStart != 0)
        swp->heapStartSpace = 0;
    else
        swp->heapStartSpace = GET_SMART_HEAP_SIZE();

    // Move all allocations closer together if possible
    SmartAllocation * prevAllocation = NULL;
    void * space = swp->heapStart;
    for (SmartAllocation * curAllocation = swp->allocatedStart; curAllocation; prevAllocation = curAllocation, curAllocation = curAllocation->next)
    {
        // If this allocation can be moved forwards
        if (curAllocation->data != space)
        {
            // Try move
            if (curAllocation->fileEntry != NULL)
            {
                // Don't move if dvd is being read into it?
                if ((curAllocation->fileEntry->state == 3) && (curAllocation->fileEntry->dvdEntry != 0))
                {
                    // Update previous allocation since assumption there'd be no space after it is false
                    if (prevAllocation != NULL)
                        prevAllocation->spaceAfter = (u32) curAllocation->data - (u32) space;
                    else
                        swp->heapStartSpace = (u32) curAllocation->data - (u32) space;

                    // Do the exact things the continue skips anyway
                    space = (void *) ((u32)curAllocation->data + curAllocation->size);
                    curAllocation->spaceAfter = 0;
                    continue;
                }
                else
                {
                    // Move the memory in a different way
                    fileGarbageMoveMem(space, curAllocation->fileEntry);
                }
            }
            else
            {
                // Move the memory and update allocation with new pointer
                memmove(space, curAllocation->data, curAllocation->size);
                curAllocation->data = space;
            }
        }
        // Assume there'll be no space after this, will be updated by the allocation after if this ends up being not true
        curAllocation->spaceAfter = 0;
        space = (void *) ((u32)curAllocation->data + curAllocation->size);
    }

    // Update spaceAfter of final allocation, since assumption it'd be 0 wasn't true
    if (swp->allocatedEnd != NULL)
    {
        u32 heapSize = GET_SMART_HEAP_SIZE();
        swp->allocatedEnd->spaceAfter = (u32) swp->heapStart + heapSize
                                        - (u32) swp->allocatedEnd->data - swp->allocatedEnd->size;
    }

    // Flush heap from cache
    DCFlushRange(swp->heapStart, GET_SMART_HEAP_SIZE());
}

GXTexObj * smartTexObj(GXTexObj * texObj, SmartAllocation * imageAllocation)
{
    if (imageAllocation)
        GXInitTexObjData(texObj, imageAllocation->data);
    else
        // "There is no smart memory information"
        assert(725, 0, "スマートメモリの情報がないよ\n");

    return texObj;
}

}

#ifdef __CLANGD__
void * operator new(size_t size) throw(std::bad_alloc)
#else
void * operator new(size_t size) throw()
#endif
{
    if (!memInitFlag)
    {
        if (fallbackHeap == 0) {
            void * memory = OSGetMEM1ArenaLo();
            fallbackHeap = MEMCreateExpHeapEx(memory, 0x2000, MEM_FLAG_THREAD_CONTROL | MEM_FLAG_FILL_0);
            OSSetMEM1ArenaLo((void *)((u32)memory + 0x2000));
        }
        return MEMAllocFromExpHeapEx(fallbackHeap, size, 4);
    }
    else
    {
        void * p = MEMAllocFromExpHeapEx(wp->heapHandle[0], size, 0x20);
        assertf(221, p, "メモリ確保エラー [id = %d][size = %d]", 0, size);
        return p;
    }
}

void operator delete(void * ptr) throw()
{
    MEMHeapHandle heap = MEMFindContainHeap(ptr);
    MEMFreeToExpHeap(heap, ptr);
}

extern "C" {

void __sys_free(void * ptr)
{
    MEMFreeToExpHeap(wp->heapHandle[0], ptr);
}

}
