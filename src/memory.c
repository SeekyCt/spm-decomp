#include <common.h>
#include <mem.h>
#include <memory.h>
#include <os.h>
#include <system.h>

static MemWork memWork;
static MemWork * wp = &memWork;
static SmartWork smartWork;
static SmartWork * swp = &smartWork;
static bool memInitFlag = false;
bool g_bFirstSmartAlloc;

static HeapSize size_table[HEAP_COUNT] = {
    // MEM1
    { // 0
        .type = HEAPSIZE_ABSOLUTE_KB,
        .size = 0x2400
    },
    { // 1
        .type = HEAPSIZE_ABSOLUTE_KB,
        .size = 0x1800
    },
    { // 2
        .type = HEAPSIZE_PERCENT_REMAINING,
        .size = 100
    },

    // MEM2
    { // 3
        .type = HEAPSIZE_ABSOLUTE_KB,
        .size = 0x100
    },
    { // 4
        .type = HEAPSIZE_ABSOLUTE_KB,
        .size = 0x100
    },
    { // 5
        .type = HEAPSIZE_ABSOLUTE_KB,
        .size = 0x80
    },
    { // 6
        .type = HEAPSIZE_ABSOLUTE_KB,
        .size = 0x4400
    },
    { // 7
        .type = HEAPSIZE_PERCENT_REMAINING,
        .size = 100
    },
    { // 8
        .type = HEAPSIZE_ABSOLUTE_KB,
        .size = 1
    }
};

void memInit() {
    s32 i; // register usage closer to matching
    u32 max;
    u8 * min;

    // Instruction order doesn't match when geting size_table pointer at the start

    min = OSGetMem1ArenaLo();
    max = (u32) OSGetMem1ArenaHi();

    for (i = 0; i < MEM1_HEAP_COUNT; i++) {
        if (size_table[i].type == HEAPSIZE_ABSOLUTE_KB) {
            u32 size = (u32) size_table[i].size * 1024;
            wp->heapStart[i] = min;
            wp->heapEnd[i] = min + size;
            // "Error: Overheap of heap from arena [%d] \ n"
            assertf((u32)wp->heapEnd[i] <= max, "ERROR: アリーナからのヒープの取得オーバーです。[%d]\n", i);
            min += size;
        }
    }

    u32 space = (u32) max - (u32) min;
    for (i = 0; i < MEM1_HEAP_COUNT; i++) {
        if (size_table[i].type == HEAPSIZE_PERCENT_REMAINING) {
            u32 size = (u32) (((u64) space * size_table[i].size) / 100);
            // "ERROR: Excessive heap acquisition from arena\n"
            assert(size >= 32, "ERROR: アリーナからのヒープの取得オーバーです。\n");
            size -= size & 0x1f;
            wp->heapStart[i] = min;
            wp->heapEnd[i] = min + size;
            // "Error: Overheap of heap from arena [%d] \ n"
            assertf((u32)wp->heapEnd[i] <= max, "ERROR: アリーナからのヒープの取得オーバーです。[%d]\n", i);
            min += size;
        }
    }

    for (i = 0; i < MEM1_HEAP_COUNT; i++) {
        wp->heapHandle[i] = MEMCreateExpHeapEx(wp->heapStart[i], (u32)wp->heapEnd[i] - (u32)wp->heapStart[i], 0);
    }

    OSSetMem1ArenaLo((void *) max);
    // Register usage for min & max wrong way around from here
    min = OSGetMem2ArenaLo();
    max = (u32) OSGetMem2ArenaHi();

    for (i = MEM1_HEAP_COUNT; i < HEAP_COUNT; i++) {
        if (size_table[i].type == HEAPSIZE_ABSOLUTE_KB) {
            u32 size = (u32) size_table[i].size * 1024;
            wp->heapStart[i] = min;
            wp->heapEnd[i] = min + size;
            // "Error: Overheap of heap from arena [%d] \ n"
            assertf((u32)wp->heapEnd[i] <= max, "ERROR: アリーナからのヒープの取得オーバーです。[%d]\n", i);
            min += size;
        }
    }

    space = (u32) max - (u32) min;
    for (i = MEM1_HEAP_COUNT; i < HEAP_COUNT; i++) {
        if (size_table[i].type == HEAPSIZE_PERCENT_REMAINING) {
            u32 size = (u32) (((u64) space * size_table[i].size) / 100);
            // "ERROR: Excessive heap acquisition from arena\n"
            assert(size >= 32, "ERROR: アリーナからのヒープの取得オーバーです。\n");
            size -= size & 0x1f;
            wp->heapStart[i] = min;
            wp->heapEnd[i] = min + size;
            // "Error: Overheap of heap from arena [%d] \ n"
            assertf((u32)wp->heapEnd[i] <= max, "ERROR: アリーナからのヒープの取得オーバーです。[%d]\n", i);
            min += size;
        }
    }

    for (i = MEM1_HEAP_COUNT; i < HEAP_COUNT; i++) {
        wp->heapHandle[i] = MEMCreateExpHeapEx(wp->heapStart[i], (u32)wp->heapEnd[i] - (u32)wp->heapStart[i], 0);
    }

    OSSetMem2ArenaLo((void *) max);
    
    for (i = 0; i < HEAP_COUNT; i++) {
        // Maybe memClear inlined?
        if (i == 7) {
            MEMDestroyExpHeap(wp->heapHandle[i]);
            MEMCreateExpHeapEx(wp->heapStart[i], (u32)wp->heapEnd[i] - (u32)wp->heapStart[i], 4);
        }
        else {
            MEMDestroyExpHeap(wp->heapHandle[i]);
            MEMCreateExpHeapEx(wp->heapStart[i], (u32)wp->heapEnd[i] - (u32)wp->heapStart[i], 4 | 1);
        }
    }

    memInitFlag = true;
}

void memClear(s32 heapId) {
    if (heapId == 7) {
        MEMDestroyExpHeap(wp->heapHandle[heapId]);
        MEMCreateExpHeapEx(wp->heapStart[heapId], (u32)wp->heapEnd[heapId] - (u32)wp->heapStart[heapId], 4);
    }
    else {
        MEMDestroyExpHeap(wp->heapHandle[heapId]);
        MEMCreateExpHeapEx(wp->heapStart[heapId], (u32)wp->heapEnd[heapId] - (u32)wp->heapStart[heapId], 4 | 1);
    }
}

void * __memAlloc(s32 heapId, size_t size) {
    void * p = MEMAllocFromExpHeapEx(wp->heapHandle[heapId], size, 0x20);
    assertf(p, "メモリ確保エラー [id = %d][size = %d]", heapId, size);
    return p;
}

void __memFree(s32 heapId, void * ptr) {
    MEMFreeToExpHeap(wp->heapHandle[heapId], ptr);
}

void smartInit() {
    u32 size = MEMGetAllocatableSizeForExpHeapEx(wp->heapHandle[SMART_HEAP_ID], 4);
    void * p = MEMAllocFromExpHeapEx(wp->heapHandle[SMART_HEAP_ID], size, 0x20);
    assertf(p, "メモリ確保エラー [id = %d][size = %d]", SMART_HEAP_ID, size);
    swp->heapStart = p;
    swp->unknown_0xe008 = 0;
    swp->unknown_0xe00c = 0;
    swp->heapSize = MEMGetSizeForMBlockExpHeap(swp->heapStart);
    SmartAllocation * curAllocation = swp->allocations;
    for (int i = 0; i < SMART_ALLOCATION_MAX; i++) {
        curAllocation->next = curAllocation + 1;
        curAllocation->prev = curAllocation - 1;
        curAllocation++;
    }
    swp->firstFree = &swp->allocations[0];
    swp->firstFree->prev = NULL;
    swp->lastFree = &swp->allocations[SMART_ALLOCATION_MAX - 1];
    swp->lastFree->next = NULL;
    swp->unknown_0xe018 = 0;
    g_bFirstSmartAlloc = false;
}

// a lot
