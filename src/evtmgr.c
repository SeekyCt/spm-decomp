#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_msg.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <spm/spmario.h>
#include <wii/os.h>
#include <wii/string.h>

// .bss
static EvtWork work;
static s32 priTbl[EVT_ENTRY_MAX];
static s32 priIdTbl[EVT_ENTRY_MAX];

// .sdata
static s32 evtId = 1;
static f32 evtSpd = 1.0f;
s32 evtMax = 0;
static s32 priTblNum = 0;
static s32 runMainF = 0;

EvtWork * evtGetWork() // usually inlined in this file
{
    return &work;
}

static void make_pri_table()
{
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

static void make_jump_table(EvtEntry * entry)
{
    s32 i;
    s32 n;
    EvtScriptCode * pScriptHead;
    s32 cmd;
    s32 cmdn;
    s32 id;

    for (i = 0; i < 16; i++)
    {
        entry->labelIds[i] = -1;
        entry->jumptable[i] = 0;
    }

    n = 0;
    pScriptHead = entry->pCurInstruction;
    while (true)
    {
        cmd = *pScriptHead & 0xffff;
        cmdn = *pScriptHead >> 16;
        pScriptHead++;

        if (cmdn < 0)
            assert(0x74, cmdn >= 0, "EVTMGR:command line error");
        if (cmd >= EVT_max)
            assert(0x77, cmd < EVT_max, "EVTMGR:command line error");

        id = *pScriptHead;
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

        if (n >= MAX_EVT_JMPTBL)
            assert(0x88, n < MAX_EVT_JMPTBL, "EVTMGR:Jump Table Overflow !![make_jump_table]");
    }
    end: ; // didn't match when just using return
}

void evtmgrInit()
{
    EvtWork * wp;
    
    wp = evtGetWork();
    
    wp->entryCount = EVT_ENTRY_MAX;
    wp->entries = (EvtEntry *) __memAlloc(0, wp->entryCount * sizeof(EvtEntry));
    wp->time = gp->time;
    memset(wp->entries, 0, wp->entryCount * sizeof(EvtEntry));
    memset(wp->gw, 0, sizeof(wp->gw));
    memset(wp->gf, 0, sizeof(wp->gf));
    evtMax = 0;
    priTblNum = 0;
    runMainF = 0;

    evt_msg_init();
}

void evtmgrReInit()
{
    EvtWork * wp;

    wp = evtGetWork();    
    wp->time = gp->time;
    memset(wp->entries, 0, wp->entryCount * sizeof(EvtEntry));
    evtMax = 0;
    runMainF = 0;

    evt_msg_init();
}

// Always inlined
void evtEntryRunCheck()
{
    switch (_spmarioSystemLevel) {
        case 0:
            break;
        case 1:
            evtStopAll(3);
            break;
        case 2:
            evtStopAll(0xff);
            break;
    }
}

EvtEntry * evtEntry(s32* script, u32 priority, u8 flags) {
    EvtEntry* entry;
    s32 j;
    s32 i;
    EvtWork * wp;

    wp = evtGetWork();

    entry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, entry++)
    {
        if ((entry->flags & EVT_FLAG_IN_USE) == 0)
            break;
    }
    if (i >= wp->entryCount)
        assert(0x108, 0, "EVTMGR:Pointer Table Overflow !![evtEntry]");
    evtMax += 1;
    memset(entry, 0, sizeof(*entry));
    entry->flags = (u8) (flags | EVT_FLAG_IN_USE);
    entry->pCurInstruction = script;
    entry->scriptStart = script;
    entry->pPrevInstruction = script;
    entry->curOpcode = EVT_OPC_NEXT;
    entry->parent = NULL;
    entry->childEntry = NULL;
    entry->brotherEntry = NULL;
    entry->priority = (u8) priority;
    entry->id = evtId++;
    entry->doWhileDepth = -1;
    entry->switchDepth = -1;
    entry->type = 0xFF;
    entry->name = NULL;
    entry->speed = evtSpd;
    entry->timeToRun = 0.0f;
    entry->casedrvId = -1;
    entry->ownerNPC = NULL;
    entry->lifetime = 0;
    for (j = 0; j < 16; j++)
        entry->lw[j] = 0;
    for (j = 0; j < 3; j++)
        entry->lf[j] = 0;
    make_jump_table(entry);
    if (runMainF && ((entry->flags & EVT_FLAG_START_IMMEDIATE) != 0))
    {
        priTbl[priTblNum] = i;
        priIdTbl[priTblNum] = entry->id;
        priTblNum++;
    }
    evtEntryRunCheck();
    if (evtId == 0)
        evtId = 1;
    return entry;
}

EvtEntry * evtEntryType(s32* script, u32 priority, u8 flags, u8 type) {
    EvtEntry* entry;
    s32 j;
    s32 i;
    EvtWork * wp;

    wp = evtGetWork();

    entry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, entry++)
    {
        if ((entry->flags & EVT_FLAG_IN_USE) == 0)
            break;
    }
    if (i >= wp->entryCount)
        assert(0x155, 0, "EVTMGR:Pointer Table Overflow !![evtEntryType]");
    evtMax += 1;
    memset(entry, 0, sizeof(*entry));
    entry->flags = (u8) (flags | EVT_FLAG_IN_USE);
    entry->pCurInstruction = script;
    entry->scriptStart = script;
    entry->pPrevInstruction = script;
    entry->curOpcode = EVT_OPC_NEXT;
    entry->parent = NULL;
    entry->childEntry = NULL;
    entry->brotherEntry = NULL;
    entry->priority = (u8) priority;
    entry->id = evtId++;
    entry->doWhileDepth = -1;
    entry->switchDepth = -1;
    entry->type = type;
    entry->name = NULL;
    entry->speed = evtSpd;
    entry->timeToRun = 0.0f;
    entry->casedrvId = -1;
    entry->ownerNPC = NULL;
    entry->lifetime = 0;
    for (j = 0; j < 16; j++)
        entry->lw[j] = 0;
    for (j = 0; j < 3; j++)
        entry->lf[j] = 0;
    make_jump_table(entry);
    if (runMainF && ((entry->flags & EVT_FLAG_START_IMMEDIATE) != 0))
    {
        priTbl[priTblNum] = i;
        priIdTbl[priTblNum] = entry->id;
        priTblNum++;
    }
    evtEntryRunCheck();
    if (evtId == 0)
        evtId = 1;
    return entry;
}

EvtEntry * evtChildEntry(EvtEntry * parent, EvtScriptCode * script, u8 flags)
{
    EvtEntry* entry;
    s32 j;
    s32 i;
    EvtWork * wp;

    wp = evtGetWork();

    entry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, entry++)
    {
        if ((entry->flags & EVT_FLAG_IN_USE) == 0)
            break;
    }
    if (i >= wp->entryCount)
        assert(0x1a6, 0, "EVTMGR:Pointer Table Overflow !![evtChildEntry]");
    evtMax += 1;
    parent->childEntry = entry;
    parent->flags |= EVT_FLAG_WAIT_CHILD;
    memset(entry, 0, sizeof(*entry));
    entry->flags = (u8) (flags | EVT_FLAG_IN_USE);
    entry->pCurInstruction = script;
    entry->scriptStart = script;
    entry->pPrevInstruction = script;
    entry->curOpcode = EVT_OPC_NEXT;
    entry->parent = parent;
    entry->childEntry = NULL;
    entry->brotherEntry = NULL;
    entry->priority = (u8) (parent->priority + 1);
    entry->id = evtId++;
    entry->doWhileDepth = -1;
    entry->switchDepth = -1;
    entry->type = parent->type;
    entry->name = NULL;
    entry->uw = parent->uw;
    entry->uf = parent->uf;
    entry->speed = evtSpd;
    entry->timeToRun = 0.0f;
    entry->casedrvId = -1;
    entry->ownerNPC = parent->ownerNPC;
    entry->lifetime = 0;
    entry->msgWindowId = parent->msgWindowId;
    entry->unknown_0x170 = parent->unknown_0x170;
    entry->unknown_0x178 = parent->unknown_0x178;
    entry->unknown_0x17c = parent->unknown_0x17c;
    entry->unknown_0x180 = parent->unknown_0x180;
    entry->unknown_0x184 = parent->unknown_0x184;
    entry->msgPri = parent->msgPri;
    for (j = 0; j < 16; j++)
        entry->lw[j] = parent->lw[j];
    for (j = 0; j < 3; j++)
        entry->lf[j] = parent->lf[j];
    make_jump_table(entry);
    if (runMainF)
    {
        priTbl[priTblNum] = i;
        priIdTbl[priTblNum] = entry->id;
        priTblNum++;
    }
    evtEntryRunCheck();
    if (evtId == 0)
        evtId = 1;
    return entry;
}

EvtEntry * evtBrotherEntry(EvtEntry * brother, EvtScriptCode * script, u8 flags)
{
    EvtEntry* entry;
    s32 j;
    s32 i;
    EvtWork * wp;

    wp = evtGetWork();

    entry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, entry++)
    {
        if ((entry->flags & EVT_FLAG_IN_USE) == 0)
            break;
    }
    if (i >= wp->entryCount)
        assert(0x204, 0, "EVTMGR:Pointer Table Overflow !![evtBrotherEntry]");
    evtMax += 1;
    memset(entry, 0, sizeof(*entry));
    entry->flags = (u8) (flags | EVT_FLAG_IN_USE);
    entry->pCurInstruction = script;
    entry->scriptStart = script;
    entry->pPrevInstruction = script;
    entry->curOpcode = EVT_OPC_NEXT;
    entry->parent = NULL;
    entry->brotherEntry = brother;
    entry->childEntry = NULL;
    entry->priority = (u8) brother->priority;
    entry->id = evtId++;
    entry->doWhileDepth = -1;
    entry->switchDepth = -1;
    entry->type = brother->type;
    entry->name = NULL;
    entry->uw = brother->uw;
    entry->uf = brother->uf;
    entry->speed = evtSpd;
    entry->timeToRun = 0.0f;
    entry->casedrvId = -1;
    entry->ownerNPC = brother->ownerNPC;
    entry->lifetime = 0;
    for (j = 0; j < 16; j++)
        entry->lw[j] = brother->lw[j];
    for (j = 0; j < 3; j++)
        entry->lf[j] = brother->lf[j];
    make_jump_table(entry);
    if (runMainF)
    {
        priTbl[priTblNum] = i;
        priIdTbl[priTblNum] = entry->id;
        priTblNum++;
    }
    if (evtId == 0)
        evtId = 1;
    evtEntryRunCheck();
    return entry;
}

EvtEntry* evtRestart(EvtEntry* entry)
{
    EvtWork * wp;
    EvtScriptCode * start;

    wp = evtGetWork();
    start = entry->scriptStart;
    entry->curOpcode = EVT_OPC_NEXT;
    entry->pCurInstruction = start;
    entry->pPrevInstruction = start;
    entry->speed = 1.0f;
    entry->timeToRun = 0.0f;
    entry->doWhileDepth = -1;
    entry->switchDepth = -1;
    entry->speed = evtSpd;
    entry->timeToRun = 0.0f;
    entry->lifetime = 0;
    make_jump_table(entry);
    evtEntryRunCheck();
    return entry;
}

void evtmgrMain()
{
    EvtWork * wp;
    OSTime delta;
    s32 i;
    EvtEntry * entry;
    s32 shouldBreak;
    s32 ret;

    wp = evtGetWork();

    // Mark main function as running
    runMainF = 1;

    // Clamp time delta
    delta = gp->time - wp->time;
    if (delta < 0)
        delta = 0;
    if (OSTicksToMilliseconds(delta) > 500)
        delta = OSMillisecondsToTicks(1000 / gp->fps);

    // Update time
    wp->time = gp->time;

    // Sort scripts by priority
    make_pri_table();

    // Execute scripts
    for (i = 0; i < priTblNum; i++)
    {
        // Get script
        entry = wp->entries + priTbl[i];

        // Check if script needs to be ran
        if (((entry->flags & EVT_FLAG_IN_USE) != 0) && entry->id == priIdTbl[i] &&
            ((entry->flags & (EVT_FLAG_80 | EVT_FLAG_WAIT_CHILD | EVT_FLAG_PAUSED)) == 0))
        {
            // Update lifetiem
            if ((entry->flags & EVT_FLAG_STOP_TIME) == 0)
                entry->lifetime += delta;

            // Run instructions until told to stop or time runs out
            entry->timeToRun += entry->speed;
            shouldBreak = 0;
            while (true)
            {
                // Check if time has ran out
                if (entry->timeToRun < 1.0f)
                    break;

                // Decrement time
                entry->timeToRun -= 1.0f;

                // Execute instruction
                ret = evtmgrCmd(entry);

                // Handle return
                if (ret == EVTCMD_RET_END_ALL)
                {
                    shouldBreak = 1;
                    break;
                }
                if (ret == EVTCMD_RET_END)
                    break;
            }

            // Check if any more scripts should run
            if (shouldBreak)
                break;
        }
    }
    runMainF = 0;
}

void evtDelete(EvtEntry * entry)
{
    EvtWork * wp;
    EvtEntry * curEntry;
    EvtEntry * parent;
    s32 i;

    wp = evtGetWork();
    if (entry->flags & EVT_FLAG_IN_USE != 0)
    {
        if (entry->childEntry != NULL)
            evtDelete(entry->childEntry);

        curEntry = wp->entries;
        for (i = 0; i < wp->entryCount; i++, curEntry++)
        {
            if (((curEntry->flags & EVT_FLAG_IN_USE) != 0) && (curEntry->brotherEntry == entry))
                evtDelete(curEntry);
        }

        parent = entry->parent;
        if (parent != NULL)
        {
            parent->flags &= ~EVT_FLAG_WAIT_CHILD;
            parent->childEntry = NULL;

            for (i = 0; i < 16; i++)
                parent->lw[i] = entry->lw[i];
            for (i = 0; i < 3; i++)
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
    EvtWork * wp;
    EvtEntry * entry;
    s32 i;

    wp = evtGetWork();
    entry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, entry++)
    {
        if (((entry->flags & EVT_FLAG_IN_USE) != 0) && (entry->id == id))
            evtDelete(entry);
    }
}

bool evtCheckID(s32 id)
{
    EvtWork * wp;
    EvtEntry * entry;
    s32 i;

    wp = evtGetWork();
    entry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, entry++)
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


void evtStop(EvtEntry * entry, u32 mask)
{
    EvtWork * wp;
    EvtEntry * curEntry;
    s32 i;

    wp = evtGetWork();
    if (entry->childEntry)
        evtStop(entry->childEntry, mask);
    else
        // FAKEMATCH: stops recursive inlining
        (void) 0;

    curEntry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, curEntry++)
    {
        if (((curEntry->flags & EVT_FLAG_IN_USE) != 0) && (curEntry->brotherEntry == entry))
            evtStop(curEntry, mask);
    }

    if (entry->type & mask)
        entry->flags |= EVT_FLAG_PAUSED;
}

void evtStart(EvtEntry * entry, u32 mask)
{
    EvtWork * wp;
    EvtEntry * curEntry;
    s32 i;

    wp = evtGetWork();
    if (entry->childEntry)
        evtStart(entry->childEntry, mask);
    else
        // FAKEMATCH: stops recursive inlining
        (void) 0;

    curEntry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, curEntry++)
    {
        if (((curEntry->flags & EVT_FLAG_IN_USE) != 0) && (curEntry->brotherEntry == entry))
            evtStart(curEntry, mask);
    }

    if (entry->type & mask)
        entry->flags &= ~EVT_FLAG_PAUSED;
}

void evtStopID(s32 id)
{
    EvtWork * wp;
    EvtEntry * entry;
    s32 i;

    wp = evtGetWork();
    entry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, entry++)
    {
        if (((entry->flags & EVT_FLAG_IN_USE) != 0) && (entry->id == id))
            evtStop(entry, 0xff);
    }
}

void evtStartID(s32 id)
{
    EvtWork * wp;
    EvtEntry * entry;
    s32 i;

    wp = evtGetWork();
    entry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, entry++)
    {
        if (((entry->flags & EVT_FLAG_IN_USE) != 0) && (entry->id == id))
            evtStart(entry, 0xff);
    }
}

void evtStopAll(u32 mask)
{
    EvtWork * wp;
    EvtEntry * entry;
    s32 i;

    wp = evtGetWork();
    entry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, entry++)
    {
        if ((entry->flags & EVT_FLAG_IN_USE) != 0)
            evtStop(entry, mask);
    }
}

void evtStartAll(u32 mask)
{
    EvtWork * wp;
    EvtEntry * entry;
    s32 i;

    wp = evtGetWork();
    entry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, entry++)
    {
        if ((entry->flags & EVT_FLAG_IN_USE) != 0)
            evtStart(entry, mask);
    }
}

void evtStopOther(EvtEntry * entry, u32 mask)
{
    EvtWork * wp;
    EvtEntry * curEntry;
    s32 i;

    wp = evtGetWork();
    curEntry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, curEntry++)
    {
        if (((curEntry->flags & EVT_FLAG_IN_USE) != 0) && (curEntry != entry))
            evtStop(curEntry, mask);
    }
}

void evtStartOther(EvtEntry * entry, u32 mask)
{
    EvtWork * wp;
    EvtEntry * curEntry;
    s32 i;

    wp = evtGetWork();
    curEntry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, curEntry++)
    {
        if (((curEntry->flags & EVT_FLAG_IN_USE) != 0) && (curEntry != entry))
            evtStart(curEntry, mask);
    }
}

EvtEntry * evtGetPtr(s32 index)
{
    EvtWork * wp;
    EvtEntry * entry;

    wp = evtGetWork();
    entry = &wp->entries[index];
    if ((entry->flags & EVT_FLAG_IN_USE) != 0)
    {
        return entry;
    }

    return 0;
}

EvtEntry * evtGetPtrID(s32 id)
{
    EvtWork * wp;
    EvtEntry * curEntry;
    s32 i;

    wp = evtGetWork();
    curEntry = wp->entries;
    for (i = 0; i < wp->entryCount; i++, curEntry++)
    {
        if (curEntry->flags & EVT_FLAG_IN_USE != 0 && curEntry->id == id)
            return curEntry;
    }

    return 0;
}
