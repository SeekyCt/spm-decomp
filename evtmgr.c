#include <stddef.h>
#include <string.h>

#include "evtmgr.h"
#include "memory.h"
#include "somewhere.h"
#include "swdrv.h"
#include "system.h"

EvtWork evtWork;
int priTbl[EVT_ENTRY_MAX];
int priIdTbl[EVT_ENTRY_MAX];

int evtActiveEntryCount; // evtMax?
int priTblNum;
int evt_unk805ae8e; // runMainF?
int evtId = 1;
float evtSpeed = 1.0;

EvtWork * evtGetWork() {
    return &evtWork;
}

// not matching
void make_pri_table() {
    EvtWork * wp = &evtWork;
    EvtEntry * entry = wp->entries;
    // Add all active entries to index and id tables
    int priEntryCount = 0;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1) {
            priTbl[priEntryCount] = i;
            priIdTbl[priEntryCount] = entry->id;
            priEntryCount++;
        }
        i++;
        entry++;
    }

    priTblNum = priEntryCount;

    // Bubble sort tables
    i = 0; // wrong register used, stops matching from here
    while (i < priEntryCount - 1) {
        int j = i + 1;
        while (j < priEntryCount) {
            int slotI = priTbl[i];
            if (wp->entries[slotI].priority < wp->entries[priTbl[j]].priority) {
                priTbl[i] = priTbl[j];
                priTbl[j] = slotI;
                int idI = priIdTbl[i];
                priIdTbl[i] = priIdTbl[j];
                priIdTbl[j] = idI;
            }
            j++;
        }
        i++;
    }
}

void make_jump_table(EvtEntry * entry) {
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

        assert(cmdn >= 0, "EVTMGR:command line error") // missing double branch here
        assert(cmd < EVT_max, "EVTMGR:command line error") // missing double branch here

        int id = *pScriptHead;
        pScriptHead += cmdn;

        switch (cmd) {
            case 1:
                goto end; // break can't be used, return didn't match
            case 3:
                entry->labelIds[n] = id;
                entry->jumptable[n] = pScriptHead;
                n++;
        }

        assert(n < MAX_EVT_JMPTBL, "EVTMGR:Jump Table Overflow !![make_jump_table]") // missing double branch here
    }
    end: ;
}

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

// not matching
EvtEntry * evtEntry(int * script, uint8_t priority, uint8_t flags) {
    EvtEntry * entry = evtWork.entries;
    int i;
    for (i = 0; i < evtWork.entryCount; i++) {
        if (entry->flags & 1 == 0) break;
        entry++;
    }
    if (i >= evtWork.entryCount) {
        //assert(0, "EVTMGR:Pointer Table Overflow !![evtEntry]")
    }
    evtActiveEntryCount += 1;
    memset(entry, 0, sizeof(EvtEntry));
    entry->flags = flags | 1;
    entry->pCurInstruction = script;
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
    entry->type = 0xff;
    entry->unknown_0x19c = 0;
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
    if ((evt_unk805ae8e0 != 0) && (entry->flags & 0x20 != 0)) {
        priTbl[priTblNum] = i;
        priIdTbl[priTblNum++] = entry->id;
    }
    if (unknown_805ae9c8 == 1) {
        for (int i = 0; i < evtWork.entryCount; i++) {
            if (evtWork.entries[i].flags & 1 != 0) {
                evtStop(&evtWork.entries[i], 3);
            }
        }
    }
    else if (unknown_805ae9c8 > 0 && unknown_805ae9c8 < 3) {
        for (int i = 0; i < evtWork.entryCount; i++) {
            if (evtWork.entries[i].flags & 1 != 0) {
                evtStop(&evtWork.entries[i], 0xff);
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
//void evtmgrMain()
//void evtDelete(EvtEntry * entry)

void evtDeleteID(int id)
{
    EvtWork * wp = &evtWork;
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
    EvtWork * wp = &evtWork;
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 && entry->id == id) {
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
    entry->speed = multiplier * evtSpeed;
}

void evtSetType(EvtEntry * entry, uint8_t type) {
    entry->type = type;
}

void evtStop(EvtEntry * entry, uint8_t mask) {
    EvtWork * wp = &evtWork;
    if (entry->childEntry) {
        evtStop(entry->childEntry, mask);
    }
    EvtEntry * curEntry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 && curEntry->brotherEntry == entry) {
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
    EvtWork * wp = &evtWork;
    if (entry->childEntry) {
        evtStart(entry->childEntry, mask);
    }
    EvtEntry * curEntry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 && curEntry->brotherEntry == entry) {
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
    EvtWork * wp = &evtWork;
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 && entry->id == id) {
            evtStop(entry, 0xff);
        }
        i++;
        entry++;
    }
}

void evtStartID(int id) {
    EvtWork * wp = &evtWork;
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 && entry->id == id) {
            evtStart(entry, 0xff);
        }
        i++;
        entry++;
    }
}

void evtStopAll(uint8_t mask) {
    EvtWork * wp = &evtWork;
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1) {
            evtStop(entry, mask);
        }
        i++;
        entry++;
    }
}

void evtStartAll(uint8_t mask) {
    EvtWork * wp = &evtWork;
    EvtEntry * entry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1) {
            evtStart(entry, mask);
        }
        i++;
        entry++;
    }
}

void evtStopOther(EvtEntry * entry, uint8_t mask) {
    EvtWork * wp = &evtWork;
    EvtEntry * curEntry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 && curEntry != entry) {
            evtStop(curEntry, mask);
        }
        i++;
        curEntry++;
    }
}

void evtStartOther(EvtEntry * entry, uint8_t mask) {
    EvtWork * wp = &evtWork;
    EvtEntry * curEntry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 && curEntry != entry) {
            evtStart(curEntry, mask);
        }
        i++;
        curEntry++;
    }
}

EvtEntry * evtGetPtr(int index) {
    EvtWork * wp = &evtWork;
    EvtEntry * entry = &wp->entries[index];
    if (entry->flags & 1) {
        return entry;
    }
    return 0;
}

EvtEntry * evtGetPtrID(int id) {
    EvtWork * wp = &evtWork;
    EvtEntry * curEntry = wp->entries;
    int i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 && curEntry->id == id) {
            return curEntry;
        }
        curEntry++;
        i++;
    }
    return 0;
}