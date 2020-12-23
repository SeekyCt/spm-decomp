#include <common.h>
#include <mem.h>
#include <memory.h>
#include <os.h>
#include <system.h>

static MemWork memWork;
static MemWork * wp = &memWork;
static HeapSize size_table[HEAP_COUNT];
static bool memInitFlag = false;

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
        if (i == 7) {
            MEMDestroyExpHeap(wp->heapHandle[i]);
            MEMCreateExpHeapEx(wp->heapStart[i], (u32)wp->heapEnd[i] - (u32)wp->heapStart[i], 4);
        }
        else {
            MEMDestroyExpHeap(wp->heapHandle[i]);
            MEMCreateExpHeapEx(wp->heapStart[i], (u32)wp->heapEnd[i] - (u32)wp->heapStart[i], 5);
        }
    }

    memInitFlag = true;
}
