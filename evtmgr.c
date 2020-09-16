#include <stddef.h>
#include <string.h>

#include "evtmgr.h"
#include "memory.h"
#include "swdrv.h"
#include "system.h"

// TODO: move
void evt_msg_init();

EvtWork evtWork;
int priTbl[EVT_ENTRY_MAX];
int priIdTbl[EVT_ENTRY_MAX];

int evtActiveEntryCount; // evtMax?
int priTblNum;
int evt_unk805ae8e; // runMainF?
int evtId = 1;

EvtWork * evtGetWork() {
    return &evtWork;
}

void make_pri_table() {
    // Add all active entries to index and id tables
    int priEntryCount = 0;
    for (int i = 0; i < evtWork.entryCount; i++) {
        if ((evtWork.entries[i].flags & 1) != 0) {
            priTbl[priEntryCount] = i;
            priIdTbl[priEntryCount] = evtWork.entries[i].id;
            priEntryCount++;
        }
    }

    priTblNum = priEntryCount;

    // Bubble sort tables
    for (int i = 0; i < priEntryCount - 1; i++) {
        for (int j = i + 1; j < priEntryCount; j++) {
            int slotI = priTbl[i];
            if (evtWork.entries[slotI].priority < evtWork.entries[priTbl[j]].priority) {
                priTbl[i] = priTbl[j];
                priTbl[j] = slotI;
                int idI = priIdTbl[i];
                priIdTbl[i] = priIdTbl[j];
                priIdTbl[j] = idI;
            }
        }
    }
}

// make_jump_table

void evtmgrInit() {
    evtWork.entryCount = EVT_ENTRY_MAX;
    evtWork.entries = (EvtEntry *) __MemAlloc(0, evtWork.entryCount * sizeof(EvtEntry));
    evtWork.time = swdrv->time;
    memset(evtWork.entries, 0, evtWork.entryCount * sizeof(EvtEntry));
    memset(evtWork.gw, 0, sizeof(evtWork.gw));
    memset(evtWork.gf, 0, sizeof(evtWork.gf));
    evtActiveEntryCount = 0;
    priTblNum = 0;
    evt_unk805ae8e0 = 0;
    evt_msg_init();
}

void evtmgrReInit() {
    evtWork.time = swdrv->time;
    memset(evtWork.entries, 0, evtWork.entryCount * sizeof(EvtEntry));
    evtActiveEntryCount = 0;
    evt_unk805ae8e0 = 0;
    evt_msg_init();
}

EvtEntry * evtEntry(void * script, uint8_t priority, int8_t flags) {
    EvtEntry * entry = evtWork.entries;
    int i;
    for (i = 0; i < evtWork.entryCount; i++) {
        if (entry->flags & 1 == 0) break;
        entry++;
    }
    if (i >= evtWork.entryCount) {
        assert(0, "EVTMGR:Pointer Table Overflow !![evtEntry]")
    }
    evtActiveEntryCount += 1;
    memset(entry, 0, sizeof(EvtEntry));
    entry->flags |= 1;
    entry->curInstructionPtr = script;
    entry->scriptStart = script;
    entry->prevInstructionPtr = script;
    entry->curOpcode = 2; // TODO: make 'next'
    entry->parent = NULL;
    entry->childEntry = NULL;
    entry->brotherEntry = NULL;
    entry->priority = priority;
    entry->id = evtId++;
    entry->dowhileDepth = -1;
    entry->unknown_0xf = -1;
    entry->type = -1;
    entry->unknown_0x19c = 0;
    entry->speed = 1.0f;
    entry->unknown_0x160 = 0.0f;
    entry->unknown_0x164 = -1;
    entry->unknown_0x168 = 0;
    entry->unknown_0x4 = 0;
    entry->unknown_0x0 = 0;
    for (i = 0; i < 16; i++) {
        entry->lw[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        entry->lw[i] = 0;
    }
    make_jump_table(entry);
    if ((evt_unk805ae8e0 != 0) && (entry->flags & 0x20 != 0)) {

    }
    (void) flags;
    return 0;
}
