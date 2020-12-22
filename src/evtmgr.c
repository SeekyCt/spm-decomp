/*
    Includes many weird while loops where for loops didn't match because of the
    variable incrementing order, maybe a sign of wrong compiler version?
    (Especially since PM64 decomp matched them with for loops)
*/

#include <common.h>
#include <evtmgr.h>
#include <evtmgr_cmd.h>
#include <evt_msg.h>
#include <spmario.h>
#include <memory.h>
#include <os.h>
#include <string.h>
#include <system.h>

static EvtWork work; // 8050c990
static s32 priTbl[EVT_ENTRY_MAX]; // 8050ca30
static s32 priIdTbl[EVT_ENTRY_MAX]; // 8050cc30

s32 evtMax;
static s32 priTblNum; // 805ae8dc
static s32 runMainF; // 805ae8e0

static s32 evtId = 1; // 805ae018
static float evtSpd = 1.0; // 805ae01c

EvtWork * evtGetWork() { // usually inlined in this file
    return &work;
}

static void make_pri_table() { // 800d87f0
    // Register usage didn't match otherwise
    EvtWork * wp;
    EvtEntry * entry;
    s32 n;
    s32 priEntryCount;
    s32 slotI;
    s32 slotJ;
    s32 i;
    s32 j;
    s32 idI;
    s32 idJ;

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
    for (s32 i = 0; i < 16; i++) {
        entry->labelIds[i] = -1;
        entry->jumptable[i] = 0;
    }

    s32 n = 0;
    EvtScriptCode * pScriptHead = entry->pCurInstruction;
    while (true) {
        s32 cmd = *pScriptHead & 0xffff;
        s32 cmdn = *pScriptHead >> 16;
        pScriptHead++;

        assert(cmdn >= 0, "EVTMGR:command line error"); // missing double branch here
        assert(cmd < EVT_max, "EVTMGR:command line error"); // missing double branch here

        s32 id = *pScriptHead;
        pScriptHead += cmdn;

        switch (cmd) {
            case 1:
                goto end;
            case 3:
                entry->labelIds[n] = (s8) id;
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

// not matching, includes inline evtEntryRunCheck
EvtEntry * evtEntry(EvtScriptCode * script, u8 priority, u8 flags) {
    EvtEntry * entry = work.entries;
    s32 i;
    for (i = 0; i < work.entryCount; i++) {
        if (entry->flags & 1 == 0) break;
        entry++;
    }
    if (i >= work.entryCount) {
        //assert(0, "EVTMGR:Pointer Table Overflow !![evtEntry]");
    }
    evtMax += 1;
    memset(entry, 0, sizeof(EvtEntry));
    entry->flags = (u8) (flags | 1);
    entry->pCurInstruction = script;
    entry->scriptStart = script;
    entry->pPrevInstruction = script;
    entry->curOpcode = EVT_OPC_END_EVT;
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
    entry->lifetime = 0;
    for (s32 j = 0; j < 16; j++) {
        entry->lw[i] = 0;
    }
    for (s32 j = 0; j < 3; j++) {
        entry->lw[j] = 0;
    }
    make_jump_table(entry);
    if ((runMainF != 0) && (entry->flags & 0x20 != 0)) {
        priTbl[priTblNum] = i;
        priIdTbl[priTblNum++] = entry->id;
    }
    if (_spmarioSystemLevel == 1) {
        for (s32 i = 0; i < work.entryCount; i++) {
            if (work.entries[i].flags & 1 != 0) {
                evtStop(&work.entries[i], 3);
            }
        }
    }
    else if (_spmarioSystemLevel > 0 && _spmarioSystemLevel < 3) {
        for (s32 i = 0; i < work.entryCount; i++) {
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

//EvtEntry * evtEntryType(EvtScriptCode * script, s32 param_2, s32 param_3, s32 param_4)
//EvtEntry * evtChildEntry(EvtEntry * entry, EvtScriptCode * script, u8 flags)
//EvtEntry * evtBrotherEntry(EvtEntry * entry, EvtScriptCode * script, u8 flags)
//EvtEntry * evtRestart(EvtEntry * entry)

// unfinished
void evtmgrMain() {
    EvtWork * wp = evtGetWork();
    runMainF = 1;
    s64 timeDif = gp->time - wp->time;
    if (timeDif < 0) {
        timeDif = 0;
    }
    s64 ms = OSTicksToMilliseconds(gp->time);
    if (ms > 500) {
        // this line is iffy
        timeDif = (1000 / gp->unknown_0x4) * (OSClockSpeed / 1000);
    }
    wp->time = gp->time;
    make_pri_table();
    for (s32 i = 0; i < priTblNum; i++) {
        EvtEntry * entry = &wp->entries[priTbl[i]];
        if (entry->flags & 1 && entry->id == priIdTbl[i] && entry->flags & 0x92) {
            if (entry->flags & 4) {
                entry->lifetime += ms;
            }
            entry->unknown_0x160 += 1.0f;
            s32 shouldBreak = 0;
            while (entry->unknown_0x160 >= 1.0f) {
                entry->unknown_0x160 -= 1.0f;
                s32 ret = evtmgrCmd(entry);
                if (ret == 1) {
                    shouldBreak = 1;
                    break;
                }
                if (ret == -1) break;
            }
            if (shouldBreak) break;
        }
    }
    runMainF = 0;
}

void evtDelete(EvtEntry * entry) {
    EvtWork * wp = evtGetWork();
    if (entry->flags & 1 != 0) {
        if (entry->childEntry != NULL) {
            evtDelete(entry->childEntry);
        }
        EvtEntry * curEntry = wp->entries;
        s32 i = 0;
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
            for (s32 i = 0; i < 16; i++) {
                parent->lw[i] = entry->lw[i];
            }
            for (s32 i = 0; i < 3; i++) {
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

void evtDeleteID(s32 id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    s32 i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0 && entry->id == id) {
            evtDelete(entry);
        }
        i++;
        entry++;
    }
}

bool evtCheckID(s32 id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    s32 i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0 && entry->id == id) {
            return true;
        }
        i++;
        entry++;
    }
    return false;
}

void evtSetPri(EvtEntry * entry, u8 priority) {
    entry->priority = priority;
}

void evtSetSpeed(EvtEntry * entry, f32 multiplier) {
    entry->speed = multiplier * evtSpd;
}

void evtSetType(EvtEntry * entry, u8 type) {
    entry->type = type;
}

void evtStop(EvtEntry * entry, u8 mask) {
    EvtWork * wp = evtGetWork();
    if (entry->childEntry) {
        evtStop(entry->childEntry, mask);
    }
    EvtEntry * curEntry = wp->entries;
    s32 i = 0;
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

void evtStart(EvtEntry * entry, u8 mask) {
    EvtWork * wp = evtGetWork();
    if (entry->childEntry) {
        evtStart(entry->childEntry, mask);
    }
    EvtEntry * curEntry = wp->entries;
    s32 i = 0;
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

void evtStopID(s32 id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    s32 i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0 && entry->id == id) {
            evtStop(entry, 0xff);
        }
        i++;
        entry++;
    }
}

void evtStartID(s32 id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    s32 i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0&& entry->id == id) {
            evtStart(entry, 0xff);
        }
        i++;
        entry++;
    }
}

void evtStopAll(u8 mask) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    s32 i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0) {
            evtStop(entry, mask);
        }
        i++;
        entry++;
    }
}

void evtStartAll(u8 mask) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = wp->entries;
    s32 i = 0;
    while (i < wp->entryCount) {
        if (entry->flags & 1 != 0) {
            evtStart(entry, mask);
        }
        i++;
        entry++;
    }
}

void evtStopOther(EvtEntry * entry, u8 mask) {
    EvtWork * wp = evtGetWork();
    EvtEntry * curEntry = wp->entries;
    s32 i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 != 0 && curEntry != entry) {
            evtStop(curEntry, mask);
        }
        i++;
        curEntry++;
    }
}

void evtStartOther(EvtEntry * entry, u8 mask) {
    EvtWork * wp = evtGetWork();
    EvtEntry * curEntry = wp->entries;
    s32 i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 != 0 && curEntry != entry) {
            evtStart(curEntry, mask);
        }
        i++;
        curEntry++;
    }
}

EvtEntry * evtGetPtr(s32 index) {
    EvtWork * wp = evtGetWork();
    EvtEntry * entry = &wp->entries[index];
    if (entry->flags & 1 != 0) {
        return entry;
    }
    return 0;
}

EvtEntry * evtGetPtrID(s32 id) {
    EvtWork * wp = evtGetWork();
    EvtEntry * curEntry = wp->entries;
    s32 i = 0;
    while (i < wp->entryCount) {
        if (curEntry->flags & 1 != 0 && curEntry->id == id) {
            return curEntry;
        }
        curEntry++;
        i++;
    }
    return 0;
}
