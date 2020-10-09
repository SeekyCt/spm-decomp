/*
    Todo: evtEntryRunCheck is inlined in evtEntry, but the code was just put in directly
*/

#include <os.h>
#include <stddef.h>
#include <string.h>

#include "evtmgr.h"
#include "memory.h"
#include "somewhere.h"
#include "mariost.h"
#include "system.h"

static EvtWork work; // 8050c990
static int priTbl[EVT_ENTRY_MAX]; // 8050ca30
static int priIdTbl[EVT_ENTRY_MAX]; // 8050cc30

int evtMax;
static int priTblNum; // 805ae8dc
static int runMainF; // 805ae8e0
static int evtId = 1; // 805ae018
static float evtSpd = 1.0; // 805ae01c

EvtWork * evtGetWork() { // usually inlined in this file but not elsewhere
    return &work;
}

static void make_pri_table() { // 800d87f0
    // Register usage didn't match otherwise
    EvtWork * wp;
    EvtEntry * entry;
    int n;
    int priEntryCount;
    int slotI;
    int slotJ;
    int i;
    int j;
    int idI;
    int idJ;

    wp = evtGetWork();
    entry = wp->entries;
    priEntryCount = 0;
    n = 0;
    while (n < wp->entryCount) {
        if (entry->flags & 1 != 0) {
            priTbl[priEntryCount] = n;
            priIdTbl[priEntryCount] = entry->id;
            priEntryCount++;
        }
        n++;
        entry++;
    }
    priTblNum = priEntryCount;

    // Bubble sort
    for (i = 0; i < priEntryCount - 1; i++) {
        for (j = i + 1; j < priEntryCount; j++) {
            slotJ = priTbl[j];
            slotI = priTbl[i];
            if (wp->entries[slotI].priority < wp->entries[slotJ].priority) {
                idI = priIdTbl[i];
                idJ = priIdTbl[j];
                priTbl[i] = slotJ;
                priIdTbl[i] = idJ;
                priTbl[j] = slotI;
                priIdTbl[j] = idI;
            }
        }
    }
}

static void make_jump_table(EvtEntry * entry) { // 800d890c
    for (int i = 0; i < 16; i++) {
        entry->labelIds[i] = -1;
        entry->jumptable[i] = 0;
    }

    int n = 0;
    int * pScriptHead = entry->pCurInstruction;
    while (true) {
        int cmd = *pScriptHead & 0xffff;
        int cmdn = *pScriptHead >> 16;
        pScriptHead++;

        assert(cmdn >= 0, "EVTMGR:command line error"); // missing double branch here
        assert(cmd < EVT_max, "EVTMGR:command line error"); // missing double branch here

        int id = *pScriptHead;
        pScriptHead += cmdn;

        switch (cmd) {
            case 1:
                goto end;
            case 3:
                entry->labelIds[n] = (int8_t) id;
                entry->jumptable[n] = pScriptHead;
                n++;
        }

        assert(n < MAX_EVT_JMPTBL, "EVTMGR:Jump Table Overflow !![make_jump_table]"); // missing double branch here
    }
    end: ; // didn't match when just using return
}

void evtmgrInit() {
    work.entryCount = EVT_ENTRY_MAX;
    work.entries = (EvtEntry *) __MemAlloc(0, work.entryCount * sizeof(EvtEntry));
    work.time = gp->time;
    memset(work.entries, 0, work.entryCount * sizeof(EvtEntry));
    memset(work.gw, 0, sizeof(work.gw));
    memset(work.gf, 0, sizeof(work.gf));
    evtMax = 0;
    priTblNum = 0;
    runMainF = 0;
    evt_msg_init();
}

void evtmgrReInit() {
    work.time = gp->time;
    memset(work.entries, 0, work.entryCount * sizeof(EvtEntry));
    evtMax = 0;
    runMainF = 0;
    evt_msg_init();
}

// not matching
EvtEntry * evtEntry(int * script, uint8_t priority, uint8_t flags) {
    EvtEntry * entry = work.entries;
    int i;
    for (i = 0; i < work.entryCount; i++) {
        if (entry->flags & 1 == 0) break;
        entry++;
    }
    if (i >= work.entryCount) {
        //assert(0, "EVTMGR:Pointer Table Overflow !![evtEntry]");
    }
    evtMax += 1;
    memset(entry, 0, sizeof(EvtEntry));
    entry->flags = (uint8_t) (flags | 1);
    entry->pCurInstruction = script;
    entry->scriptStart = script;
    entry->pPrevInstruction = script;
    entry->curOpcode = 2; // TODO: make 'next'
    entry->parent = NULL;
    entry->childEntry = NULL;
    entry->brotherEntry = NULL;
    entry->priority = priority;
    entry->id = evtId++;
    entry->dowhileDepth = -1;
    entry->unknown_0xf = -1;
    entry->type = 0xff;
    entry->name = NULL;
    entry->speed = 1.0f;
    entry->unknown_0x160 = 0.0f;
    entry->unknown_0x164 = -1;
    entry->unknown_0x168 = 0;
    entry->unknown_0x4 = 0;
    entry->unknown_0x0 = 0;
    for (int j = 0; j < 16; j++) {
        entry->lw[i] = 0;
    }
    for (int j = 0; j < 3; j++) {
        entry->lw[j] = 0;
    }
    make_jump_table(entry);
    if ((runMainF != 0) && (entry->flags & 0x20 != 0)) {
        priTbl[priTblNum] = i;
        priIdTbl[priTblNum++] = entry->id;
    }
    if (unknown_805ae9c8 == 1) {
        for (int i = 0; i < work.entryCount; i++) {
            if (work.entries[i].flags & 1 != 0) {
                evtStop(&work.entries[i], 3);
            }
        }
    }
    else if (unknown_805ae9c8 > 0 && unknown_805ae9c8 < 3) {
        for (int i = 0; i < work.entryCount; i++) {
            if (work.entries[i].flags & 1 != 0) {
                evtStop(&work.entries[i], 0xff);
            }
        }
    }
    if (evtId == 0) {
        evtId = 1;
    }
    return entry;
}

//EvtEntry * evtEntryType(int * script, int param_2, int param_3, int param_4)
//EvtEntry * evtChildEntry(EvtEntry * entry, int * script, uint8_t flags)
//EvtEntry * evtBrotherEntry(EvtEntry * entry, int * script, uint8_t flags)
//EvtEntry * evtRestart(EvtEntry * entry)

// unfinished
void evtmgrMain() {
    EvtWork * wp = evtGetWork();
    int64_t timeDif = gp->time - wp->time;
    if (timeDif < 0) {
        timeDif = 0;
    }
    int64_t ms = OSTicksToMilliseconds(gp->time);
    if (ms > 500) {
        // this line is iffy
        timeDif = (1000 / gp->unknown_0x4) * (OSClockSpeed / 1000);
    }
    wp->time = gp->time;
    make_pri_table();
    for (int i = 0; i < priTblNum; i++) {
        
    }
}

void evtDelete(EvtEntry * entry) {
    EvtWork * wp = evtGetWork();
    if (entry->flags & 1 != 0) {
        if (entry->childEntry != NULL) {
            evtDelete(entry->childEntry);
        }
        EvtEntry * curEntry = wp->entries;
        int i = 0;
        while(i < wp->entryCount) {
            if (curEntry->flags & 1 != 0 && curEntry->brotherEntry == entry) {
                evtDelete(curEntry);
            }
            i++;
            curEntry++;
        }

        EvtEntry * parent = entry->parent;
        if (parent != NULL) {
            parent->flags &= ~0x10;
            parent->childEntry = NULL;
            for (int i = 0; i < 16; i++) {
                parent->lw[i] = entry->lw[i];
            }
            for (int i = 0; i < 3; i++) {
                parent->lf[i] = entry->lf[i];
            }
            parent->unknown_0x174 = entry->unknown_0x174; // might be some arrays w/ unrolling here
            parent->unknown_0x170 = entry->unknown_0x170;
            parent->unknown_0x178 = entry->unknown_0x178;
            parent->unknown_0x17c = entry->unknown_0x17c;
            parent->unknown_0x180 = entry->unknown_0x180;
            parent->unknown_0x184 = entry->unknown_0x184;
            parent->unknown_0x188 = entry->unknown_0x188;
        }
        entry->flags &= ~1;
        memset(entry, 0, sizeof(EvtEntry));
        evtMax -= 1;
    }
}

void evtDeleteID(int id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0 && entry->id == id) {
            evtDelete(entry);
        }
        i++;
        entry++;
    }
}

bool evtCheckID(int id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0 && entry->id == id) {
            return true;
        }
        i++;
        entry++;
    }
    return false;
}

void evtSetPri(EvtEntry * entry, uint8_t priority) {
    entry->priority = priority;
}

void evtSetSpeed(EvtEntry * entry, float multiplier) {
    entry->speed = multiplier * evtSpd;
}

void evtSetType(EvtEntry * entry, uint8_t type) {
    entry->type = type;
}

void evtStop(EvtEntry * entry, uint8_t mask) {
    EvtWork * wp = evtGetWork();
    if (entry->childEntry) {
        evtStop(entry->childEntry, mask);
    }
    EvtEntry * curEntry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 != 0 && curEntry->brotherEntry == entry) {
            evtStop(curEntry, mask);
        }
        i++;
        curEntry++;
    }
    if (entry->type & mask) {
        entry->flags |= 2;
    }
}

void evtStart(EvtEntry * entry, uint8_t mask) {
    EvtWork * wp = evtGetWork();
    if (entry->childEntry) {
        evtStart(entry->childEntry, mask);
    }
    EvtEntry * curEntry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 != 0 && curEntry->brotherEntry == entry) {
            evtStart(curEntry, mask);
        }
        i++;
        curEntry++;
    }
    if (entry->type & mask) {
        entry->flags &= ~2;
    }
}

void evtStopID(int id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0 && entry->id == id) {
            evtStop(entry, 0xff);
        }
        i++;
        entry++;
    }
}

void evtStartID(int id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0&& entry->id == id) {
            evtStart(entry, 0xff);
        }
        i++;
        entry++;
    }
}

void evtStopAll(uint8_t mask) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0) {
            evtStop(entry, mask);
        }
        i++;
        entry++;
    }
}

void evtStartAll(uint8_t mask) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0) {
            evtStart(entry, mask);
        }
        i++;
        entry++;
    }
}

void evtStopOther(EvtEntry * entry, uint8_t mask) {
    EvtWork * wp = evtGetWork();
    EvtEntry * curEntry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 != 0 && curEntry != entry) {
            evtStop(curEntry, mask);
        }
        i++;
        curEntry++;
    }
}

void evtStartOther(EvtEntry * entry, uint8_t mask) {
    EvtWork * wp = evtGetWork();
    EvtEntry * curEntry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 != 0 && curEntry != entry) {
            evtStart(curEntry, mask);
        }
        i++;
        curEntry++;
    }
}

EvtEntry * evtGetPtr(int index) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = &wp->entries[index];
    if (entry->flags & 1 != 0) {
        return entry;
    }
    return 0;
}

EvtEntry * evtGetPtrID(int id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * curEntry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 != 0 && curEntry->id == id) {
            return curEntry;
        }
        curEntry++;
        i++;
    }
    return 0;
}