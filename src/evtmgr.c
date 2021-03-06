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
static float evtSpd = 1.0f; // 805ae01c

EvtWork * evtGetWork() // usually inlined in this file
{
    return &work;
}

static void make_pri_table() // 800d87f0
{
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
    for (n = 0; n < wp->entryCount; n++, entry++)
    {
        if (entry->flags & EVT_FLAG_IN_USE != 0)
        {
            priTbl[priEntryCount] = n;
            priIdTbl[priEntryCount] = entry->id;
            priEntryCount++;
        }
    }
    priTblNum = priEntryCount;

    // Bubble sort
    for (i = 0; i < priEntryCount - 1; i++)
    {
        for (j = i + 1; j < priEntryCount; j++)
        {
            slotJ = priTbl[j];
            slotI = priTbl[i];
            if (wp->entries[slotI].priority < wp->entries[slotJ].priority)
            {
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

// Not matching
static void make_jump_table(EvtEntry * entry) // 800d890c
{
    for (s32 i = 0; i < 16; i++)
    {
        entry->labelIds[i] = -1;
        entry->jumptable[i] = 0;
    }

    s32 n = 0;
    EvtScriptCode * pScriptHead = entry->pCurInstruction;
    while (true)
    {
        s32 cmd = *pScriptHead & 0xffff;
        s32 cmdn = *pScriptHead >> 16;
        pScriptHead++;

        assert(0x74, cmdn >= 0, "EVTMGR:command line error"); // missing double branch here
        assert(0x77, cmd < EVT_max, "EVTMGR:command line error"); // missing double branch here

        s32 id = *pScriptHead;
        pScriptHead += cmdn;

        switch (cmd)
        {
            case 1:
                goto end;
            case 3:
                entry->labelIds[n] = (s8) id;
                entry->jumptable[n] = pScriptHead;
                n++;
        }

        assert(0x88, n < MAX_EVT_JMPTBL, "EVTMGR:Jump Table Overflow !![make_jump_table]"); // missing double branch here
    }
    end: ; // didn't match when just using return
}

void evtmgrInit()
{
    work.entryCount = EVT_ENTRY_MAX;
    work.entries = (EvtEntry *) __memAlloc(0, work.entryCount * sizeof(EvtEntry));
    work.time = gp->time;
    memset(work.entries, 0, work.entryCount * sizeof(EvtEntry));
    memset(work.gw, 0, sizeof(work.gw));
    memset(work.gf, 0, sizeof(work.gf));
    evtMax = 0;
    priTblNum = 0;
    runMainF = 0;

    evt_msg_init();
}

void evtmgrReInit()
{
    work.time = gp->time;
    memset(work.entries, 0, work.entryCount * sizeof(EvtEntry));
    evtMax = 0;
    runMainF = 0;

    evt_msg_init();
}

// Not matching, includes inline evtEntryRunCheck
EvtEntry * evtEntry(EvtScriptCode * script, u32 priority, u8 flags)
{
    EvtEntry * entry = work.entries;
    s32 i;

    for (i = 0; i < work.entryCount; i++)
    {
        if (entry->flags & EVT_FLAG_IN_USE == 0)
            break;

        entry++;
    }
    if (i >= work.entryCount)
        assert(0x108, 0, "EVTMGR:Pointer Table Overflow !![evtEntry]");

    evtMax += 1;

    memset(entry, 0, sizeof(EvtEntry));
    entry->flags = (u8) (flags | EVT_FLAG_IN_USE);
    entry->pCurInstruction = script;
    entry->scriptStart = script;
    entry->pPrevInstruction = script;
    entry->curOpcode = EVT_OPC_END_EVT;
    entry->parent = NULL;
    entry->childEntry = NULL;
    entry->brotherEntry = NULL;
    entry->priority = (u8) priority;
    entry->id = evtId++;
    entry->dowhileDepth = -1;
    entry->switchDepth = -1;
    entry->type = 0xff;
    entry->name = NULL;
    entry->speed = 1.0f;
    entry->unknown_0x160 = 0.0f;
    entry->casedrvId = -1;
    entry->ownerNPC = 0;
    entry->lifetime = 0;

    for (s32 j = 0; j < 16; j++)
        entry->lw[i] = 0;
    for (s32 j = 0; j < 3; j++)
        entry->lf[j] = 0;

    make_jump_table(entry);

    if ((runMainF != 0) && ((entry->flags & 0x20) != 0))
    {
        priTbl[priTblNum] = i;
        priIdTbl[priTblNum++] = entry->id;
    }

    if (_spmarioSystemLevel == 1)
    {
        for (s32 i = 0; i < work.entryCount; i++)
        {
            if (work.entries[i].flags & EVT_FLAG_IN_USE != 0)
                evtStop(&work.entries[i], 3);
        }
    }
    else if ((_spmarioSystemLevel > 0) && (_spmarioSystemLevel < 3))
    {
        for (s32 i = 0; i < work.entryCount; i++)
        {
            if (work.entries[i].flags & EVT_FLAG_IN_USE != 0)
                evtStop(&work.entries[i], 0xff);
        }
    }

    if (evtId == 0)
        evtId = 1;

    return entry;
}

// Unfinished, just for string pool
EvtEntry * evtEntryType(EvtScriptCode * script, u32 priority, u8 flags, u8 type)
{
    (void) script;
    (void) priority;
    (void) flags;
    (void) type;

    __dummy_string("EVTMGR:Pointer Table Overflow !![evtEntryType]");

    return NULL;
}

// Unfinished, just for string pool
EvtEntry * evtChildEntry(EvtEntry * entry, EvtScriptCode * script, u8 flags)
{
    (void) entry;
    (void) script;
    (void) flags;

    __dummy_string("EVTMGR:Pointer Table Overflow !![evtChildEntry]");

    return NULL;
}

// Unfinished, just for string pool
EvtEntry * evtBrotherEntry(EvtEntry * entry, EvtScriptCode * script, u8 flags)
{
    (void) entry;
    (void) script;
    (void) flags;

    __dummy_string("EVTMGR:Pointer Table Overflow !![evtBrotherEntry]");

    return NULL;
}

// EvtEntry * evtRestart(EvtEntry * entry)

/* Unfinished
void evtmgrMain()
{
    EvtWork * wp = evtGetWork();
    runMainF = 1;
    s64 timeDif = gp->time - wp->time;
    if (timeDif < 0)
    {
        timeDif = 0;
    }
    s64 ms = OSTicksToMilliseconds(gp->time);
    if (ms > 500)
    {
        // this line is iffy
        timeDif = (1000 / gp->unknown_0x4) * (OSClockSpeed / 1000);
    }
    wp->time = gp->time;
    make_pri_table();
    for (s32 i = 0; i < priTblNum; i++)
    {
        EvtEntry * entry = &wp->entries[priTbl[i]];
        if (entry->flags & EVT_FLAG_IN_USE && entry->id == priIdTbl[i] && entry->flags & 0x92)
        {
            if (entry->flags & 4)
                entry->lifetime += ms;
            entry->unknown_0x160 += 1.0f;
            s32 shouldBreak = 0;
            while (entry->unknown_0x160 >= 1.0f)
            {
                entry->unknown_0x160 -= 1.0f;
                s32 ret = evtmgrCmd(entry);
                if (ret == 1)
                {
                    shouldBreak = 1;
                    break;
                }
                if (ret == -1)
                    break;
            }
            if (shouldBreak)
                break;
        }
    }
    runMainF = 0;
}
*/

void evtDelete(EvtEntry * entry)
{
    EvtWork * wp = evtGetWork();

    if (entry->flags & EVT_FLAG_IN_USE != 0)
    {
        if (entry->childEntry != NULL)
            evtDelete(entry->childEntry);

        EvtEntry * curEntry = wp->entries;
        for (s32 i = 0; i < wp->entryCount; i++, curEntry++)
        {
            if (((curEntry->flags & 1) != 0) && (curEntry->brotherEntry == entry))
                evtDelete(curEntry);
        }

        EvtEntry * parent = entry->parent;
        if (parent != NULL)
        {
            parent->flags &= ~0x10;
            parent->childEntry = NULL;

            for (s32 i = 0; i < 16; i++)
                parent->lw[i] = entry->lw[i];
            for (s32 i = 0; i < 3; i++)
                parent->lf[i] = entry->lf[i];

            parent->msgWindowId = entry->msgWindowId;
            parent->unknown_0x170 = entry->unknown_0x170;
            parent->unknown_0x178 = entry->unknown_0x178; // might be some arrays w/ unrolling here
            parent->unknown_0x17c = entry->unknown_0x17c;
            parent->unknown_0x180 = entry->unknown_0x180;
            parent->unknown_0x184 = entry->unknown_0x184;
            parent->msgPri = entry->msgPri;
        }

        entry->flags &= ~EVT_FLAG_IN_USE;
        memset(entry, 0, sizeof(EvtEntry));
        evtMax -= 1;
    }
}

void evtDeleteID(s32 id)
{
    EvtWork * wp = evtGetWork();

    EvtEntry * entry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, entry++)
    {
        if (((entry->flags & EVT_FLAG_IN_USE) != 0) && (entry->id == id))
            evtDelete(entry);
    }
}

bool evtCheckID(s32 id)
{
    EvtWork * wp = evtGetWork();

    EvtEntry * entry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, entry++)
    {
        if (((entry->flags & EVT_FLAG_IN_USE) != 0) && (entry->id == id))
            return true;
    }

    return false;
}

void evtSetPri(EvtEntry * entry, u32 priority)
{
    entry->priority = (u8) priority;
}

void evtSetSpeed(EvtEntry * entry, f32 multiplier)
{
    entry->speed = multiplier * evtSpd;
}

void evtSetType(EvtEntry * entry, u32 type)
{
    entry->type = (u8) type;
}

#pragma push
#pragma inline_max_auto_size(5)

void evtStop(EvtEntry * entry, u32 mask)
{
    EvtWork * wp = evtGetWork();

    if (entry->childEntry)
        evtStop(entry->childEntry, mask);

    EvtEntry * curEntry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, curEntry++)
    {
        if (((curEntry->flags & EVT_FLAG_IN_USE) != 0) && (curEntry->brotherEntry == entry))
            evtStop(curEntry, mask);
    }

    if (entry->type & mask)
        entry->flags |= EVT_FLAG_PAUSED;
}

void evtStart(EvtEntry * entry, u32 mask)
{
    EvtWork * wp = evtGetWork();

    if (entry->childEntry)
        evtStart(entry->childEntry, mask);

    EvtEntry * curEntry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, curEntry++)
    {
        if (((curEntry->flags & EVT_FLAG_IN_USE) != 0) && (curEntry->brotherEntry == entry))
            evtStart(curEntry, mask);
    }

    if (entry->type & mask)
        entry->flags &= ~EVT_FLAG_PAUSED;
}

#pragma pop

void evtStopID(s32 id)
{
    EvtWork * wp = evtGetWork();

    EvtEntry * entry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, entry++)
    {
        if (((entry->flags & EVT_FLAG_IN_USE) != 0) && (entry->id == id))
            evtStop(entry, 0xff);
    }
}

void evtStartID(s32 id)
{
    EvtWork * wp = evtGetWork();

    EvtEntry * entry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, entry++)
    {
        if (((entry->flags & EVT_FLAG_IN_USE) != 0) && (entry->id == id))
            evtStart(entry, 0xff);
    }
}

void evtStopAll(u32 mask)
{
    EvtWork * wp = evtGetWork();

    EvtEntry * entry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, entry++)
    {
        if ((entry->flags & EVT_FLAG_IN_USE) != 0)
            evtStop(entry, mask);
    }
}

void evtStartAll(u32 mask)
{
    EvtWork * wp = evtGetWork();

    EvtEntry * entry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, entry++)
    {
        if ((entry->flags & EVT_FLAG_IN_USE) != 0)
            evtStart(entry, mask);
    }
}

void evtStopOther(EvtEntry * entry, u32 mask)
{
    EvtWork * wp = evtGetWork();

    EvtEntry * curEntry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, curEntry++)
    {
        if (((curEntry->flags & EVT_FLAG_IN_USE) != 0) && (curEntry != entry))
            evtStop(curEntry, mask);
    }
}

void evtStartOther(EvtEntry * entry, u32 mask)
{
    EvtWork * wp = evtGetWork();

    EvtEntry * curEntry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, curEntry++)
    {
        if (((curEntry->flags & EVT_FLAG_IN_USE) != 0) && (curEntry != entry))
            evtStart(curEntry, mask);
    }
}

EvtEntry * evtGetPtr(s32 index)
{
    EvtWork * wp = evtGetWork();

    EvtEntry * entry = &wp->entries[index];
    if ((entry->flags & EVT_FLAG_IN_USE) != 0)
    {
        return entry;
    }

    return 0;
}

EvtEntry * evtGetPtrID(s32 id)
{
    EvtWork * wp = evtGetWork();

    EvtEntry * curEntry = wp->entries;
    for (s32 i = 0; i < wp->entryCount; i++, curEntry++)
    {
        if (curEntry->flags & EVT_FLAG_IN_USE != 0 && curEntry->id == id)
            return curEntry;
    }

    return 0;
}
