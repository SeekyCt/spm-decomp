#include <spm/evtmgr_cmd.h>
#include <spm/memory.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <wii/string.h>
#include <wii/stdio.h>
#include <wii/os.h>

// .rodata
#include "orderdoubles/8032eca0_8032eca8.inc"
#include "orderstrings/8032eca8_8032efe1.inc"

// 0.5f comes early in this file's float pool, suggesting the rounding in evt_mod
// is an inline, though attempts to match with that failed
#include "orderfloats/805b1d50_805b1d64.inc"

static f32 check_float(s32 val) // always inlined
{
    f32 f;
    if (val <= EVTDAT_FLOAT_MAX)
    {
        val += EVTDAT_FLOAT_BASE;
        f = val;
        return f / 1024.0f;
    }
    else
    {
        f = val;
        return f;
    }
}

// change_float (inlined/unused)

s32 evt_end_evt(EvtEntry * entry)
{
    evtDelete(entry);

    return EVT_RET_END;
}

s32 evt_lbl(EvtEntry * entry)
{
    (void) entry;

    return EVT_RET_CONTINUE;
}

s32 evt_goto(EvtEntry * entry)
{
    s32 lbl;
    EvtScriptCode * r31;
    EvtScriptCode * dest;
    s32 n;

    lbl = evtGetValue(entry, entry->pCurData[0]);
    r31 = entry->pCurData;
    
    if (lbl < EVTDAT_ADDR_MAX) // TODO: evtSearchLabel inlined
    {
        dest = (EvtScriptCode *) lbl;
    }
    else
    {
        for (n = 0; n < MAX_EVT_JMPTBL; n++)
        {
            if (lbl == entry->labelIds[n])
            {
                r31 = entry->jumptable[n];
                break;
            }
        }
        if (n >= MAX_EVT_JMPTBL)
            assertf(0xf2e, 0, "EVTMGR_CMD:Jump Table Search error !!\n [lbl=%d, n=%d]", lbl, n);

        dest = r31;
    }

    entry->pCurInstruction = dest;

    return EVT_RET_CONTINUE;
}

s32 evt_do(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 count;
    s32 depth;

    p = entry->pCurData;
    count = *p++;

    entry->doWhileDepth += 1;
    depth = entry->doWhileDepth;
    if (depth >= 8)
        assert(0x67, 0, "EVTMGR_CMD:While Table Overflow !!");

    entry->doWhileStartPtrs[depth] = p;
    entry->doWhileCounters[depth] = count;

    return EVT_RET_CONTINUE;
}

s32 evt_while(EvtEntry * entry)
{
    s32 count;
    s32 depth;
    s32 ret;

    depth = entry->doWhileDepth;
    if (depth < 0)
        assert(0x7b, 0, "EVTMGR_CMD:While Table Underflow !!");

    count = entry->doWhileCounters[depth];
    if (count == 0)
    {
        entry->pCurInstruction = entry->doWhileStartPtrs[depth];

        return EVT_RET_CONTINUE;
    }
    else
    {
        if (count >= -10000000)
        {
            entry->doWhileCounters[depth] = --count;
        }
        else
        {
            ret = evtGetValue(entry, count);
            evtSetValue(entry, count, ret - 1);
            count = ret - 1;
        }

        if (count != 0)
        {
            entry->pCurInstruction = entry->doWhileStartPtrs[depth];

            return EVT_RET_CONTINUE;
        }
        else
        {
            entry->doWhileDepth--;

            return EVT_RET_CONTINUE;
        }
    }
}

s32 evt_do_break(EvtEntry * entry)
{
    if (entry->doWhileDepth < 0)
        assert(0xa1, 0, "EVTMGR_CMD:While Table Underflow !!");

    entry->pCurInstruction = evtSearchWhile(entry);
    entry->doWhileDepth -= 1;

    return EVT_RET_CONTINUE;
}

s32 evt_do_continue(EvtEntry * entry)
{
    if (entry->doWhileDepth < 0)
        assert(0xb0, 0, "EVTMGR_CMD:While Table Underflow !!");

    entry->pCurInstruction = evtSearchJustBeforeWhile(entry);

    return EVT_RET_CONTINUE;
}

s32 evt_wait_frm(EvtEntry * entry)
{
    EvtScriptCode * p;
    
    p = entry->pCurData;

    // Just an if didn't match
    switch (entry->blocked)
    {
        case false:
            entry->tempS[0] = evtGetValue(entry, p[0]);
            entry->blocked = true;
    }

    if (entry->tempS[0] == 0)
        return EVT_RET_CONTINUE;
    else
        return --entry->tempS[0] == 0 ? EVT_RET_CONTINUE_WEAK : EVT_RET_BLOCK_WEAK;
}

s32 evt_wait_msec(EvtEntry * entry)
{
    OSTime time;
    u64 tickDiff;
    s32 msecDiff;
    EvtScriptCode * args;

    args = entry->pCurData;
    time = entry->lifetime;
    switch (entry->blocked)
    {
        default:
            break;
        case false:
            entry->tempS[0] = evtGetValue(entry, args[0]);
            entry->tempU[1] = (u32) (((u64)time >> 32) & 0xffffffff);
            entry->tempU[2] = (u32) (time & 0xffffffff);
            entry->blocked = true;
            break;
    }
    if (entry->tempS[0] == 0)
    {
        return EVT_RET_CONTINUE;
    }

    tickDiff = (s32)time - (s32)*(OSTime *)&entry->tempS[1];
    msecDiff = (s32) OSTicksToMilliseconds(tickDiff);
    return msecDiff >= entry->tempS[0] ? EVT_RET_CONTINUE_WEAK : EVT_RET_BLOCK_WEAK;
}

s32 evt_halt(EvtEntry * entry)
{
   return evtGetValue(entry, entry->pCurData[0]) ? EVT_RET_BLOCK_WEAK : EVT_RET_CONTINUE;
}

s32 evt_if_str_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    char * s1;
    char * s2;
    
    p = entry->pCurData;

    s1 = (char *) evtGetValue(entry, p[0]);
    s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (strcmp(s1, s2))
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_str_not_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    char * s1;
    char * s2;
    
    p = entry->pCurData;

    s1 = (char *) evtGetValue(entry, p[0]);
    s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (!strcmp(s1, s2))
    {
        entry->pCurInstruction = evtSearchElse(entry);
    
        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_str_small(EvtEntry * entry)
{
    EvtScriptCode * p;
    char * s1;
    char * s2;
    
    p = entry->pCurData;

    s1 = (char *) evtGetValue(entry, p[0]);
    s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (strcmp(s1, s2) >= 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_str_large(EvtEntry * entry)
{
    EvtScriptCode * p;
    char * s1;
    char * s2;
    
    p = entry->pCurData;

    s1 = (char *) evtGetValue(entry, p[0]);
    s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (strcmp(s1, s2) <= 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}


s32 evt_if_str_small_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    char * s1;
    char * s2;
    
    p = entry->pCurData;

    s1 = (char *) evtGetValue(entry, p[0]);
    s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (strcmp(s1, s2) > 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_str_large_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    char * s1;
    char * s2;

    p = entry->pCurData;

    s1 = (char *) evtGetValue(entry, p[0]);
    s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (strcmp(s1, s2) < 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_iff_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    f32 f1;
    f32 f2;

    p = entry->pCurData;

    f1 = evtGetFloat(entry, p[0]);
    f2 = evtGetFloat(entry, p[1]);

    if (f1 != f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_iff_not_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    f32 f1;
    f32 f2;

    p = entry->pCurData;

    f1 = evtGetFloat(entry, p[0]);
    f2 = evtGetFloat(entry, p[1]);

    if (f1 == f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_iff_small(EvtEntry * entry)
{
    EvtScriptCode * p;
    f32 f1;
    f32 f2;

    p = entry->pCurData;

    f1 = evtGetFloat(entry, p[0]);
    f2 = evtGetFloat(entry, p[1]);

    if (f1 >= f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_iff_large(EvtEntry * entry)
{
    EvtScriptCode * p;
    f32 f1;
    f32 f2;

    p = entry->pCurData;

    f1 = evtGetFloat(entry, p[0]);
    f2 = evtGetFloat(entry, p[1]);

    if (f1 <= f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_iff_small_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    f32 f1;
    f32 f2;

    p = entry->pCurData;

    f1 = evtGetFloat(entry, p[0]);
    f2 = evtGetFloat(entry, p[1]);

    if (f1 > f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_iff_large_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    f32 f1;
    f32 f2;

    p = entry->pCurData;

    f1 = evtGetFloat(entry, p[0]);
    f2 = evtGetFloat(entry, p[1]);

    if (f1 < f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 val1;
    s32 val2;

    p = entry->pCurData;

    val1 = evtGetValue(entry, p[0]);
    val2 = evtGetValue(entry, p[1]);

    if (val1 != val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_not_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 val1;
    s32 val2;

    p = entry->pCurData;

    val1 = evtGetValue(entry, p[0]);
    val2 = evtGetValue(entry, p[1]);

    if (val1 == val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_small(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 val1;
    s32 val2;

    p = entry->pCurData;

    val1 = evtGetValue(entry, p[0]);
    val2 = evtGetValue(entry, p[1]);

    if (val1 >= val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_large(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 val1;
    s32 val2;

    p = entry->pCurData;

    val1 = evtGetValue(entry, p[0]);
    val2 = evtGetValue(entry, p[1]);

    if (val1 <= val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_small_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 val1;
    s32 val2;

    p = entry->pCurData;

    val1 = evtGetValue(entry, p[0]);
    val2 = evtGetValue(entry, p[1]);

    if (val1 > val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_large_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 val1;
    s32 val2;

    p = entry->pCurData;

    val1 = evtGetValue(entry, p[0]);
    val2 = evtGetValue(entry, p[1]);

    if (val1 < val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}


s32 evt_if_flag(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 val;
    s32 mask;

    p = entry->pCurData;

    val = evtGetValue(entry, p[0]);
    mask = p[1];

    if ((val & mask) == 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_if_not_flag(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 val;
    s32 mask;

    p = entry->pCurData;

    val = evtGetValue(entry, p[0]);
    mask = p[1];

    if ((val & mask) != 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_RET_CONTINUE;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_else(EvtEntry * entry)
{
    entry->pCurInstruction = evtSearchEndIf(entry);

    return EVT_RET_CONTINUE;
}

s32 evt_end_if(EvtEntry * entry)
{
    (void) entry;

    return EVT_RET_CONTINUE;
}

s32 evt_switch(EvtEntry * entry)
{
    s32 value;
    s32 depth;

    value = evtGetValue(entry, entry->pCurData[0]);

    entry->switchDepth += 1;
    depth = entry->switchDepth;
    if (depth >= 8)
        assert(0x31e, 0, "EVTMGR_CMD:Switch Table Overflow !!");

    entry->switchValues[depth] = value;
    entry->switchStates[depth] = 1;

    return EVT_RET_CONTINUE;
}

s32 evt_switchi(EvtEntry * entry)
{
    s32 depth;
    s32 value;

    value = entry->pCurData[0];
    entry->switchDepth += 1;
    depth = entry->switchDepth;
    if (depth >= 8)
        assert(0x336, 0, "EVTMGR_CMD:Switch Table Overflow !!");

    entry->switchValues[depth] = value;
    entry->switchStates[depth] = 1;

    return EVT_RET_CONTINUE;
}

s32 evt_case_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x34b, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    targetValue = evtGetValue(entry, p[0]);
    state = entry->switchStates[depth];
    inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        if (targetValue != inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_RET_CONTINUE;
    }
}

s32 evt_case_not_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x36c, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    targetValue = evtGetValue(entry, p[0]);
    state = entry->switchStates[depth];
    inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        if (targetValue == inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_RET_CONTINUE;
    }
}

s32 evt_case_small(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x38d, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    targetValue = evtGetValue(entry, p[0]);
    state = entry->switchStates[depth];
    inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        if (targetValue <= inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_RET_CONTINUE;
    }
}

s32 evt_case_small_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x3ae, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    targetValue = evtGetValue(entry, p[0]);
    state = entry->switchStates[depth];
    inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        if (targetValue < inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_RET_CONTINUE;
    }
}


s32 evt_case_large(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x3cf, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    targetValue = evtGetValue(entry, p[0]);
    state = entry->switchStates[depth];
    inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        if (targetValue >= inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_RET_CONTINUE;
    }
}

s32 evt_case_large_equal(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x3f0, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    targetValue = evtGetValue(entry, p[0]);
    state = entry->switchStates[depth];
    inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        if (targetValue > inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_RET_CONTINUE;
    }
}


s32 evt_case_between(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    s32 min;
    s32 max;
    s32 state;
    s32 inputValue;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x411, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    min = evtGetValue(entry, p[0]);
    max = evtGetValue(entry, p[1]);
    state = entry->switchStates[depth];
    inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        if ((min <= inputValue) && (inputValue <= max))
            entry->switchStates[depth] = 0;
        else
            entry->pCurInstruction = evtSearchCase(entry);

        return EVT_RET_CONTINUE;
    }
}

s32 evt_case_etc(EvtEntry * entry)
{
    s32 depth;
    s32 state;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x431, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    state = entry->switchStates[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        entry->switchStates[depth] = 0;

        return EVT_RET_CONTINUE;
    }
}

s32 evt_case_flag(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    u32 targetMask;
    s32 state;
    s32 inputValue;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x449, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    targetMask = (u32) p[0];
    state = entry->switchStates[depth];
    inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        if ((inputValue & targetMask) == 0)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_RET_CONTINUE;
    }
}

s32 evt_case_or(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    s32 targetValue;
    s32 inputValue;
    s8 state;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x46a, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    targetValue = evtGetValue(entry, p[0]);
    inputValue = entry->switchValues[depth];
    state = entry->switchStates[depth];
    
    if (state == 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        if (targetValue == inputValue)
            entry->switchStates[depth] = -1;
        else if (state != -1)
            entry->pCurInstruction = evtSearchCase(entry);

        return EVT_RET_CONTINUE;
    }
}

s32 evt_case_and(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    s32 targetValue;
    s32 inputValue;
    s8 state;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x48c, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    targetValue = evtGetValue(entry, p[0]);
    inputValue = entry->switchValues[depth];
    state = entry->switchStates[depth];
    
    if (state == 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else if (state == -2)
    {
        entry->pCurInstruction = evtSearchCase(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        if (targetValue == inputValue)
        {
            entry->switchStates[depth] = -1;
        }
        else
        {
            entry->switchStates[depth] = -2;
            entry->pCurInstruction = evtSearchCase(entry);
        }

        return EVT_RET_CONTINUE;
    }
}

s32 evt_case_end(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 depth;
    s32 inputValue;
    s8 state;

    p = entry->pCurData;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x4b1, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    inputValue = entry->switchValues[depth];
    state = entry->switchStates[depth];
    
    if (state == 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else if (state == -1)
    {
        entry->switchStates[depth] = 0;
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_RET_CONTINUE;
    }
    else
    {
        entry->switchStates[depth] = 1;
        entry->pCurInstruction = evtSearchCase(entry);

        return EVT_RET_CONTINUE;
    }
}

s32 evt_switch_break(EvtEntry * entry)
{
    if (entry->switchDepth < 0)
        assert(0x4cf, 0, "EVTMGR_CMD:Switch Table Underflow !!");
    
    entry->pCurInstruction = evtSearchEndSwitch(entry);

    return EVT_RET_CONTINUE;
}

s32 evt_end_switch(EvtEntry * entry)
{
    s32 depth;

    depth = entry->switchDepth;
    if (depth < 0)
        assert(0x4e0, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    entry->switchStates[depth] = 0;
    entry->switchDepth -= 1;

    return EVT_RET_CONTINUE;
}

s32 evt_set(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 value;

    p = entry->pCurData;
    destVar = p[0];
    value = evtGetValue(entry, p[1]);
    evtSetValue(entry, destVar, value);
    
    return EVT_RET_CONTINUE;
}

s32 evt_seti(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 value;

    p = entry->pCurData;
    destVar = p[0];
    value = p[1];
    evtSetValue(entry, destVar, value);
    
    return EVT_RET_CONTINUE;
}

s32 evt_setf(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    f32 value;

    p = entry->pCurData;
    destVar = p[0];
    value = evtGetFloat(entry, p[1]);
    evtSetFloat(entry, destVar, value);
    
    return EVT_RET_CONTINUE;
}

s32 evt_add(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 param;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = evtGetValue(entry, p[1]);
    result = evtGetValue(entry, destVar) + param;
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_sub(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 param;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = evtGetValue(entry, p[1]);
    result = evtGetValue(entry, destVar) - param;
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_mul(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 param;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = evtGetValue(entry, p[1]);
    result = evtGetValue(entry, destVar) * param;
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_div(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 param;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = evtGetValue(entry, p[1]);
    result = evtGetValue(entry, destVar) / param;
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_mod(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 param;
    s32 value;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = (s32) (evtGetValue(entry, p[1]) + 0.5f);
    value = (s32) (evtGetValue(entry, destVar) + 0.5f);
    result = value % param;
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_addf(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    f32 param;
    f32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = evtGetFloat(entry, p[1]);
    result = evtGetFloat(entry, destVar) + param;
    evtSetFloat(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_subf(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    f32 param;
    f32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = evtGetFloat(entry, p[1]);
    result = evtGetFloat(entry, destVar) - param;
    evtSetFloat(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}
s32 evt_mulf(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    f32 param;
    f32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = evtGetFloat(entry, p[1]);
    result = evtGetFloat(entry, destVar) * param;
    evtSetFloat(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_divf(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    f32 param;
    f32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = evtGetFloat(entry, p[1]);
    result = evtGetFloat(entry, destVar) / param;
    evtSetFloat(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_set_read(EvtEntry * entry)
{
    entry->readAddr = (s32 *) evtGetValue(entry, entry->pCurData[0]);

    return EVT_RET_CONTINUE;
}

s32 evt_set_readf(EvtEntry * entry)
{
    entry->readfAddr = (float *) evtGetValue(entry, entry->pCurData[0]);

    return EVT_RET_CONTINUE;
}

s32 evt_read(EvtEntry * entry)
{
    EvtScriptCode * p;

    p = entry->pCurData;
    evtSetValue(entry, p[0], *entry->readAddr++);

    return EVT_RET_CONTINUE;
}

s32 evt_read2(EvtEntry * entry)
{
    EvtScriptCode * p;

    p = entry->pCurData;
    evtSetValue(entry, p[0], *entry->readAddr++);
    evtSetValue(entry, p[1], *entry->readAddr++);

    return EVT_RET_CONTINUE;
}

s32 evt_read3(EvtEntry * entry)
{
    EvtScriptCode * p;

    p = entry->pCurData;
    evtSetValue(entry, p[0], *entry->readAddr++);
    evtSetValue(entry, p[1], *entry->readAddr++);
    evtSetValue(entry, p[2], *entry->readAddr++);

    return EVT_RET_CONTINUE;
}

s32 evt_read4(EvtEntry * entry)
{
    EvtScriptCode * p;

    p = entry->pCurData;
    evtSetValue(entry, p[0], *entry->readAddr++);
    evtSetValue(entry, p[1], *entry->readAddr++);
    evtSetValue(entry, p[2], *entry->readAddr++);
    evtSetValue(entry, p[3], *entry->readAddr++);

    return EVT_RET_CONTINUE;
}

s32 evt_read_n(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 var;
    s32 idx;

    p = entry->pCurData;
    var = p[0];
    idx = evtGetValue(entry, p[1]);
    evtSetValue(entry, var, entry->readAddr[idx]);

    return EVT_RET_CONTINUE;
}

s32 evt_readf(EvtEntry * entry)
{
    EvtScriptCode * p;

    p = entry->pCurData;
    evtSetFloat(entry, p[0], *entry->readfAddr++);

    return EVT_RET_CONTINUE;
}

s32 evt_readf2(EvtEntry * entry)
{
    EvtScriptCode * p;

    p = entry->pCurData;
    evtSetFloat(entry, p[0], *entry->readfAddr++);
    evtSetFloat(entry, p[1], *entry->readfAddr++);

    return EVT_RET_CONTINUE;
}

s32 evt_readf3(EvtEntry * entry)
{
    EvtScriptCode * p;

    p = entry->pCurData;
    evtSetFloat(entry, p[0], *entry->readfAddr++);
    evtSetFloat(entry, p[1], *entry->readfAddr++);
    evtSetFloat(entry, p[2], *entry->readfAddr++);

    return EVT_RET_CONTINUE;
}

s32 evt_readf4(EvtEntry * entry)
{
    EvtScriptCode * p;

    p = entry->pCurData;
    evtSetFloat(entry, p[0], *entry->readfAddr++);
    evtSetFloat(entry, p[1], *entry->readfAddr++);
    evtSetFloat(entry, p[2], *entry->readfAddr++);
    evtSetFloat(entry, p[3], *entry->readfAddr++);

    return EVT_RET_CONTINUE;
}

s32 evt_readf_n(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 var;
    s32 idx;

    p = entry->pCurData;
    var = p[0];
    idx = evtGetValue(entry, p[1]);
    evtSetFloat(entry, var, entry->readAddr[idx]); // bug using readAddr?

    return EVT_RET_CONTINUE;
}

s32 evt_clamp_int(EvtEntry * entry)
{
    EvtScriptCode * pData = entry->pCurData;
    s32 dest;
    s32 destVal;
    s32 minVal;
    s32 maxVal;

    dest = pData[0];
    destVal = evtGetValue(entry, dest);

    minVal = evtGetValue(entry, pData[1]);
    maxVal = evtGetValue(entry, pData[2]);

    if (destVal < minVal)
        evtSetValue(entry, dest, minVal);
    if (destVal > maxVal)
        evtSetValue(entry, dest, maxVal);
    
    return EVT_RET_CONTINUE;
}

s32 evt_set_user_wrk(EvtEntry * entry)
{
    entry->uw = (s32 *) evtGetValue(entry, entry->pCurData[0]);
    
    return EVT_RET_CONTINUE;
}

s32 evt_set_user_flg(EvtEntry * entry)
{
    entry->uf = (u32 *) evtGetValue(entry, entry->pCurData[0]);
    
    return EVT_RET_CONTINUE;
}

s32 evt_alloc_user_wrk(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 count;
    s32 destVar;

    p = entry->pCurData;
    count = evtGetValue(entry, p[0]);
    destVar = p[1];
    entry->uw = __memAlloc(1, count * sizeof(s32));
    evtSetValue(entry, destVar, (s32) entry->uw);

    return EVT_RET_CONTINUE;
}

s32 evt_and(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 param;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = evtGetValue(entry, p[1]);
    result = evtGetValue(entry, destVar) & param;
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_andi(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 param;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = p[1];
    result = evtGetValue(entry, destVar) & param;
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_or(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 param;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = evtGetValue(entry, p[1]);
    result = evtGetValue(entry, destVar) | param;
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_ori(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 param;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    param = p[1];
    result = evtGetValue(entry, destVar) | param;
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_set_frame_from_msec(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 msec;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    msec = evtGetValue(entry, p[1]);
    result = (msec * 60) / 1000; 
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_set_msec_from_frame(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 frame;
    s32 result;

    p = entry->pCurData;
    destVar = p[0];
    frame = evtGetValue(entry, p[1]);
    result = (frame * 1000) / 60; 
    evtSetValue(entry, destVar, result);
    
    return EVT_RET_CONTINUE;
}

s32 evt_set_ram(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 value;
    s32 * addr;


    p = entry->pCurData;
    value = evtGetValue(entry, p[0]);
    addr = (s32 *) p[1];
    *addr = value;
    
    return EVT_RET_CONTINUE;
}

s32 evt_set_ramf(EvtEntry * entry)
{
    EvtScriptCode * p;
    f32 value;
    f32 * addr;

    p = entry->pCurData;
    value = evtGetFloat(entry, p[0]);
    addr = (float *) p[1];
    *addr = value;
    
    return EVT_RET_CONTINUE;
}

s32 evt_get_ram(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 * addr;
    s32 value;

    p = entry->pCurData;
    addr = (s32 *) p[1];
    value = *addr;
    evtSetValue(entry, p[0], value);
    
    return EVT_RET_CONTINUE;
}

s32 evt_get_ramf(EvtEntry * entry)
{
    EvtScriptCode * p;
    f32 * addr;
    f32 value;

    p = entry->pCurData;
    addr = (f32 *) p[1];
    value = *addr;
    evtSetFloat(entry, p[0], value);
    
    return EVT_RET_CONTINUE;
}

s32 evt_setr(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    s32 value;

    p = entry->pCurData;
    destVar = evtGetValue(entry, p[0]);
    value = evtGetValue(entry, p[1]);
    evtSetValue(entry, destVar, value);

    return EVT_RET_CONTINUE;
}

s32 evt_setrf(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 destVar;
    f32 value;

    p = entry->pCurData;
    destVar = evtGetValue(entry, p[0]);
    value = evtGetFloat(entry, p[1]);
    evtSetFloat(entry, destVar, value);

    return EVT_RET_CONTINUE;
}

s32 evt_getr(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 sourceVar;
    s32 value;

    p = entry->pCurData;
    sourceVar = evtGetValue(entry, p[0]);
    value = evtGetValue(entry, sourceVar);
    evtSetValue(entry, p[1], value);

    return EVT_RET_CONTINUE;
}

s32 evt_getrf(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 sourceVar;
    f32 value;

    p = entry->pCurData;
    sourceVar = evtGetValue(entry, p[0]);
    value = evtGetFloat(entry, sourceVar);
    evtSetFloat(entry, p[1], value);

    return EVT_RET_CONTINUE;
}

s32 evt_user_func(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 ret;
    UserFunc * func;

    p = entry->pCurData;
    switch (entry->blocked)
    {
        case false:
            func = (UserFunc *) evtGetValue(entry, *p++);
            entry->userFunc = (UserFunc *) func;
            entry->curDataLength -= 1;
            entry->pCurData = p;
            entry->blocked = true;
            ret = (*func)(entry, true);
            break;
        default:
            ret = (*entry->userFunc)(entry, false);
            break;
    }
    return ret;
}

s32 evt_run_evt(EvtEntry * entry)
{
    EvtScriptCode * script;
    EvtEntry * evt;
    
    script = (EvtScriptCode *) evtGetValue(entry, entry->pCurData[0]);
    evt = evtEntryType(script, entry->priority, 0, entry->type);

    evt->ownerNPC = entry->ownerNPC;
    for (s32 i = 0; i < 16; i++)
        evt->lw[i] = entry->lw[i];
    for (s32 i = 0; i < 3; i++)
        evt->lf[i] = entry->lf[i];
    evt->uw = entry->uw;
    evt->uf = entry->uf;

    return EVT_RET_CONTINUE;
}

s32 evt_run_evt_id(EvtEntry * entry)
{
    EvtScriptCode * p;
    EvtScriptCode * script;
    s32 destVar;
    EvtEntry * evt;

    p = entry->pCurData;

    script = (EvtScriptCode *) evtGetValue(entry, p[0]);
    destVar = p[1];
    evt = evtEntryType(script, entry->priority, 0, entry->type);

    evt->ownerNPC = entry->ownerNPC;
    for (s32 i = 0; i < 16; i++)
        evt->lw[i] = entry->lw[i];
    for (s32 i = 0; i < 3; i++)
        evt->lf[i] = entry->lf[i];
    evt->uw = entry->uw;
    evt->uf = entry->uf;

    evtSetValue(entry, destVar, evt->id);

    return EVT_RET_CONTINUE;
}

s32 evt_run_child_evt(EvtEntry * entry)
{
    EvtScriptCode * script;
    EvtEntry * evt;
    
    script = (EvtScriptCode *) evtGetValue(entry, entry->pCurData[0]);
    evt = evtChildEntry(entry, script, 0);

    entry->curOpcode = EVT_OPC_NEXT;

    return EVT_RET_END;
}

s32 evt_restart_evt(EvtEntry * entry)
{
    EvtScriptCode * script;
    
    script = (EvtScriptCode *) evtGetValue(entry, entry->pCurData[0]);
    entry->scriptStart = script;
    evtRestart(entry);
    
    return EVT_RET_CONTINUE;
}

s32 evt_delete_evt(EvtEntry * entry)
{
    s32 id;
    
    id = evtGetValue(entry, entry->pCurData[0]);
    evtDeleteID(id);

    return EVT_RET_CONTINUE;
}

s32 evt_set_pri(EvtEntry * entry)
{
    u32 pri;
    
    pri = (u32) evtGetValue(entry, entry->pCurData[0]);
    evtSetPri(entry, pri);

    return EVT_RET_CONTINUE;
}

s32 evt_set_spd(EvtEntry * entry)
{
    f32 spd;
    
    spd = evtGetFloat(entry, entry->pCurData[0]);
    evtSetSpeed(entry, spd);

    return EVT_RET_CONTINUE;
}

s32 evt_set_type(EvtEntry * entry)
{
    u32 type;
    
    type = (u32) evtGetValue(entry, entry->pCurData[0]);
    evtSetType(entry, type);

    return EVT_RET_CONTINUE;
}

s32 evt_stop_all(EvtEntry * entry)
{
    u32 mask;
    
    mask = (u32) evtGetValue(entry, entry->pCurData[0]);
    evtStopAll(mask);

    return EVT_RET_CONTINUE;
}

s32 evt_start_all(EvtEntry * entry)
{
    u32 mask;
    
    mask = (u32) evtGetValue(entry, entry->pCurData[0]);
    evtStartAll(mask);

    return EVT_RET_CONTINUE;
}

s32 evt_stop_other(EvtEntry * entry)
{
    u32 mask;
    
    mask = (u32) evtGetValue(entry, entry->pCurData[0]);
    evtStopOther(entry, mask);

    return EVT_RET_CONTINUE;
}

s32 evt_start_other(EvtEntry * entry)
{
    u32 mask;
    
    mask = (u32) evtGetValue(entry, entry->pCurData[0]);
    evtStartOther(entry, mask);

    return EVT_RET_CONTINUE;
}

s32 evt_stop_id(EvtEntry * entry)
{
    s32 id;
    
    id = evtGetValue(entry, entry->pCurData[0]);
    evtStopID(id);

    return EVT_RET_CONTINUE;
}

s32 evt_start_id(EvtEntry * entry)
{
    s32 id;
    
    id = evtGetValue(entry, entry->pCurData[0]);
    evtStartID(id);

    return EVT_RET_CONTINUE;
}

s32 evt_chk_evt(EvtEntry * entry)
{
    EvtScriptCode * p;
    s32 id;
    s32 destVar;
    s32 result;

    p = entry->pCurData;
    id = evtGetValue(entry, entry->pCurData[0]);
    destVar = p[1];
    result = (s32) evtCheckID(id);
    evtSetValue(entry, destVar, result);

    return EVT_RET_CONTINUE;
}

s32 evt_inline_evt(EvtEntry * entry)
{
    EvtScriptCode * inlineEnd;
    s32 opc;
    EvtScriptCode * script;
    EvtEntry * evt;

    script = entry->pCurInstruction;

    inlineEnd = script;
    do
    {
        opc = *inlineEnd & 0xffff;
        inlineEnd += *inlineEnd++ >> 16;
    }
    while (opc != EVT_OPC_END_INLINE);
    entry->pCurInstruction = inlineEnd;

    evt = evtEntryType(script, entry->priority, 0x60, entry->type);
    evt->uw = entry->uw;
    evt->uf = entry->uf;
    evt->ownerNPC = entry->ownerNPC;
    evt->msgWindowId = entry->msgWindowId;
    evt->unknown_0x170 = entry->unknown_0x170;
    evt->unknown_0x178 = entry->unknown_0x178;
    evt->unknown_0x17c = entry->unknown_0x17c;
    evt->unknown_0x180 = entry->unknown_0x180;
    evt->unknown_0x184 = entry->unknown_0x184;
    evt->msgPri = entry->msgPri;
    for (s32 i = 0; i < 16; i++)
        evt->lw[i] = entry->lw[i];
    for (s32 i = 0; i < 3; i++)
        evt->lf[i] = entry->lf[i];

    return EVT_RET_CONTINUE;
}

s32 evt_inline_evt_id(EvtEntry * entry)
{
    EvtScriptCode * inlineEnd;
    s32 opc;
    EvtScriptCode * script;
    EvtEntry * evt;
    EvtScriptCode * p;
    s32 destVar;

    p = entry->pCurData;
    script = entry->pCurInstruction;
    destVar = p[0];

    inlineEnd = script;
    do
    {
        opc = *inlineEnd & 0xffff;
        inlineEnd += *inlineEnd++ >> 16;
    }
    while (opc != EVT_OPC_END_INLINE);
    entry->pCurInstruction = inlineEnd;

    evt = evtEntryType(script, entry->priority, 0x60, entry->type);
    evt->uw = entry->uw;
    evt->uf = entry->uf;
    evt->ownerNPC = entry->ownerNPC;
    evt->msgWindowId = entry->msgWindowId;
    evt->unknown_0x170 = entry->unknown_0x170;
    evt->unknown_0x178 = entry->unknown_0x178;
    evt->unknown_0x17c = entry->unknown_0x17c;
    evt->unknown_0x180 = entry->unknown_0x180;
    evt->unknown_0x184 = entry->unknown_0x184;
    evt->msgPri = entry->msgPri;
    for (s32 i = 0; i < 16; i++)
        evt->lw[i] = entry->lw[i];
    for (s32 i = 0; i < 3; i++)
        evt->lf[i] = entry->lf[i];

    evtSetValue(entry, destVar, evt->id);

    return EVT_RET_CONTINUE;
}

s32 evt_end_inline(EvtEntry * entry)
{
    evtDelete(entry);

    return EVT_RET_END;
}

s32 evt_brother_evt(EvtEntry * entry)
{
    EvtScriptCode * brotherEnd;
    s32 opc;
    EvtScriptCode * script;
    EvtEntry * evt;

    script = entry->pCurInstruction;

    brotherEnd = script;
    do
    {
        opc = *brotherEnd & 0xffff;
        brotherEnd += *brotherEnd++ >> 16;
    }
    while (opc != EVT_OPC_END_BROTHER);
    entry->pCurInstruction = brotherEnd;

    evt = evtBrotherEntry(entry, script, 0x60);
    evt->type = entry->type;
    evt->uw = entry->uw;
    evt->uf = entry->uf;
    evt->ownerNPC = entry->ownerNPC;
    evt->readAddr = entry->readAddr;

    return EVT_RET_CONTINUE;
}

s32 evt_brother_evt_id(EvtEntry * entry)
{
    EvtScriptCode * brotherEnd;
    s32 opc;
    EvtScriptCode * script;
    EvtEntry * evt;
    EvtScriptCode * p;
    s32 destVar;

    p = entry->pCurData;
    script = entry->pCurInstruction;
    destVar = p[0];

    brotherEnd = script;
    do
    {
        opc = *brotherEnd & 0xffff;
        brotherEnd += *brotherEnd++ >> 16;
    }
    while (opc != EVT_OPC_END_BROTHER);
    entry->pCurInstruction = brotherEnd;

    evt = evtBrotherEntry(entry, script, 0x60);
    evt->type = entry->type;
    evt->uw = entry->uw;
    evt->uf = entry->uf;
    evt->ownerNPC = entry->ownerNPC;

    evtSetValue(entry, destVar, evt->id);

    return EVT_RET_CONTINUE;
}

s32 evt_end_brother(EvtEntry * entry)
{
    evtDelete(entry);

    return EVT_RET_BLOCK_WEAK;
}

s32 evt_debug_put_msg(EvtEntry * entry)
{
    evtGetValue(entry, entry->pCurData[0]);

    return EVT_RET_CONTINUE;
}

s32 evt_debug_msg_clear(EvtEntry * entry)
{
    (void) entry;

    return EVT_RET_CONTINUE;
}

s32 evt_debug_put_reg(EvtEntry * entry)
{
    static char str[256];

    EvtScriptCode * p;
    EvtWork * wp;
    s32 reg;
    u32 mask;
    u32 dat;
    s32 val;
    f32 f;

    p = entry->pCurData;
    wp = evtGetWork();
    reg = *p;

    if (reg <= EVTDAT_ADDR_MAX)
    {
        sprintf(str, "ADDR     [%08X]", reg);
    }
    else if (reg <= EVTDAT_FLOAT_MAX)
    {
        f = check_float(reg);

        sprintf(str, "FLOAT    [%4.2f]", f);
    }
    else if (reg <= EVTDAT_UF_MAX)
    {
        reg += EVTDAT_UF_BASE;

        mask = 1U << (reg % 32);
        dat = entry->uf[reg / 32];

        sprintf(str, "UF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_UW_MAX)
    {
        reg += EVTDAT_UW_BASE;

        val = entry->uw[reg];

        if (val <= EVTDAT_ADDR_MAX)
        {
            sprintf(str, "UW(%3d)  [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX)
        {
            f = check_float(val);

            sprintf(str, "UW(%3d)  [%4.2f]", reg, f);
        }
        else
        {
            sprintf(str, "UW(%3d)  [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_GSW_MAX)
    {
        reg += EVTDAT_GSW_BASE;

        val = swByteGet(reg);

        if (val <= EVTDAT_ADDR_MAX)
        {
            sprintf(str, "GSW(%3d) [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX)
        {
            f = check_float(val);

            sprintf(str, "GSW(%3d) [%4.2f]", reg, f);
        }
        else
        {
            sprintf(str, "GSW(%3d) [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_LSW_MAX)
    {
        reg += EVTDAT_LSW_BASE;

        val = _swByteGet(reg);

        if (val <= EVTDAT_ADDR_MAX)
        {
            sprintf(str, "LSW(%3d) [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX)
        {
            f = check_float(val);

            sprintf(str, "LSW(%3d)  [%4.2f]", reg, f);
        }
        else
        {
            sprintf(str, "LSW(%3d) [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_GSWF_MAX)
    {
        reg += EVTDAT_GSWF_BASE;

        sprintf(str, "GSWF(%3d)[%d]", reg, swGet(reg));
    }
    else if (reg <= EVTDAT_LSWF_MAX)
    {
        reg += EVTDAT_LSWF_BASE;

        sprintf(str, "LSWF(%3d)[%d]", reg, _swGet(reg));
    }
    else if (reg <= EVTDAT_GF_MAX)
    {
        reg += EVTDAT_GF_BASE;

        mask = 1U << (reg % 32);
        dat = wp->gf[reg / 32];

        sprintf(str, "GF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_LF_MAX)
    {
        reg += EVTDAT_LF_BASE;

        mask = 1U << (reg % 32);
        dat = entry->lf[reg / 32];

        sprintf(str, "LF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_GW_MAX)
    {
        reg += EVTDAT_GW_BASE;

        val = wp->gw[reg];

        if (val <= EVTDAT_ADDR_MAX)
        {
            sprintf(str, "GW(%3d)  [%08X]", reg, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX)
        {
            f = check_float(val);
            sprintf(str, "GW(%3d)  [%4.2f]", reg, f);
        }
        else
        {
            sprintf(str, "GW(%3d)  [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_LW_MAX)
    {
        reg += EVTDAT_LW_BASE;

        val = entry->lw[reg];

        if (val <= EVTDAT_ADDR_MAX)
        {
            sprintf(str, "LW(%3d)  [%08X]", reg, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX)
        {
            f = check_float(val);

            sprintf(str, "LW(%3d)  [%4.2f]", reg, f);
        }
        else
        {
            sprintf(str, "LW(%3d)  [%d]", reg, val);
        }
    }
    else
    {
        sprintf(str, "         [%d]", reg);
    }

    return EVT_RET_CONTINUE;
}

s32 evt_debug_name(EvtEntry * entry)
{
    entry->name = (char *) entry->pCurData[0];

    return EVT_RET_CONTINUE;
}

s32 evt_debug_rem(EvtEntry * entry)
{
    (void) entry;

    return EVT_RET_CONTINUE;
}

s32 evt_debug_bp(EvtEntry * entry)
{
    s32 i;

    for (i = 0; i < EVT_ENTRY_MAX; i++)
    {
        if (entry == evtGetPtr(i))
            break;
    }

    return EVT_RET_CONTINUE_WEAK;
}

s32 evtmgrCmd(EvtEntry* entry)
{
    s32 argCount;
    s32 * p;
    s32 ret;

    while (true)
    {
        ret = EVT_RET_CONTINUE;
        switch (entry->curOpcode)
        {
            case EVT_OPC_NEXT:
                entry->pPrevInstruction = entry->pCurInstruction;
                p = entry->pCurInstruction;
                entry->curOpcode = (u8) *p;
                argCount = *p++ >> 0x10;
                entry->pCurData = p;
                p += argCount;
                entry->curDataLength = (u8) argCount;
                entry->pCurInstruction = p;
                entry->blocked = false;
                ret = EVT_RET_BLOCK;
                break;
            case EVT_OPC_END_SCRIPT:
                break;
            case EVT_OPC_END_EVT:
                ret = evt_end_evt(entry);
                break;
            case EVT_OPC_LBL:
                ret = evt_lbl(entry);
                break;
            case EVT_OPC_GOTO:
                ret = evt_goto(entry);
                break;
            case EVT_OPC_DO:
                ret = evt_do(entry);
                break;
            case EVT_OPC_WHILE:
                ret = evt_while(entry);
                break;
            case EVT_OPC_DO_BREAK:
                ret = evt_do_break(entry);
                break;
            case EVT_OPC_DO_CONTINUE:
                ret = evt_do_continue(entry);
                break;
            case EVT_OPC_WAIT_FRM:
                ret = evt_wait_frm(entry);
                break;
            case EVT_OPC_WAIT_MSEC:
                ret = evt_wait_msec(entry);
                break;
            case EVT_OPC_HALT:
                ret = evt_halt(entry);
                break;
            case EVT_OPC_IF_STR_EQUAL:
                ret = evt_if_str_equal(entry);
                break;
            case EVT_OPC_IF_STR_NOT_EQUAL:
                ret = evt_if_str_not_equal(entry);
                break;
            case EVT_OPC_IF_STR_SMALL:
                ret = evt_if_str_small(entry);
                break;
            case EVT_OPC_IF_STR_LARGE:
                ret = evt_if_str_large(entry);
                break;
            case EVT_OPC_IF_STR_SMALL_EQUAL:
                ret = evt_if_str_small_equal(entry);
                break;
            case EVT_OPC_IF_STR_LARGE_EQUAL:
                ret = evt_if_str_large_equal(entry);
                break;
            case EVT_OPC_IFF_EQUAL:
                ret = evt_iff_equal(entry);
                break;
            case EVT_OPC_IFF_NOT_EQUAL:
                ret = evt_iff_not_equal(entry);
                break;
            case EVT_OPC_IFF_SMALL:
                ret = evt_iff_small(entry);
                break;
            case EVT_OPC_IFF_LARGE:
                ret = evt_iff_large(entry);
                break;
            case EVT_OPC_IFF_SMALL_EQUAL:
                ret = evt_iff_small_equal(entry);
                break;
            case EVT_OPC_IFF_LARGE_EQUAL:
                ret = evt_iff_large_equal(entry);
                break;
            case EVT_OPC_IF_EQUAL:
                ret = evt_if_equal(entry);
                break;
            case EVT_OPC_IF_NOT_EQUAL:
                ret = evt_if_not_equal(entry);
                break;
            case EVT_OPC_IF_SMALL:
                ret = evt_if_small(entry);
                break;
            case EVT_OPC_IF_LARGE:
                ret = evt_if_large(entry);
                break;
            case EVT_OPC_IF_SMALL_EQUAL:
                ret = evt_if_small_equal(entry);
                break;
            case EVT_OPC_IF_LARGE_EQUAL:
                ret = evt_if_large_equal(entry);
                break;
            case EVT_OPC_IF_FLAG:
                ret = evt_if_flag(entry);
                break;
            case EVT_OPC_IF_NOT_FLAG:
                ret = evt_if_not_flag(entry);
                break;
            case EVT_OPC_ELSE:
                ret = evt_else(entry);
                break;
            case EVT_OPC_END_IF:
                ret = evt_end_if(entry);
                break;
            case EVT_OPC_SWITCH:
                ret = evt_switch(entry);
                break;
            case EVT_OPC_SWITCHI:
                ret = evt_switchi(entry);
                break;
            case EVT_OPC_CASE_EQUAL:
                ret = evt_case_equal(entry);
                break;
            case EVT_OPC_CASE_NOT_EQUAL:
                ret = evt_case_not_equal(entry);
                break;
            case EVT_OPC_CASE_SMALL:
                ret = evt_case_small(entry);
                break;
            case EVT_OPC_CASE_SMALL_EQUAL:
                ret = evt_case_small_equal(entry);
                break;
            case EVT_OPC_CASE_LARGE:
                ret = evt_case_large(entry);
                break;
            case EVT_OPC_CASE_LARGE_EQUAL:
                ret = evt_case_large_equal(entry);
                break;
            case EVT_OPC_CASE_ETC:
                ret = evt_case_etc(entry);
                break;
            case EVT_OPC_SWITCH_BREAK:
                ret = evt_switch_break(entry);
                break;
            case EVT_OPC_CASE_OR:
                ret = evt_case_or(entry);
                break;
            case EVT_OPC_CASE_END:
                ret = evt_case_end(entry);
                break;
            case EVT_OPC_CASE_AND:
                ret = evt_case_and(entry);
                break;
            case EVT_OPC_CASE_FLAG:
                ret = evt_case_flag(entry);
                break;
            case EVT_OPC_CASE_BETWEEN:
                ret = evt_case_between(entry);
                break;
            case EVT_OPC_END_SWITCH:
                ret = evt_end_switch(entry);
                break;
            case EVT_OPC_SET:
                ret = evt_set(entry);
                break;
            case EVT_OPC_SETI:
                ret = evt_seti(entry);
                break;
            case EVT_OPC_SETF:
                ret = evt_setf(entry);
                break;
            case EVT_OPC_ADD:
                ret = evt_add(entry);
                break;
            case EVT_OPC_SUB:
                ret = evt_sub(entry);
                break;
            case EVT_OPC_MUL:
                ret = evt_mul(entry);
                break;
            case EVT_OPC_DIV:
                ret = evt_div(entry);
                break;
            case EVT_OPC_MOD:
                ret = evt_mod(entry);
                break;
            case EVT_OPC_ADDF:
                ret = evt_addf(entry);
                break;
            case EVT_OPC_SUBF:
                ret = evt_subf(entry);
                break;
            case EVT_OPC_MULF:
                ret = evt_mulf(entry);
                break;
            case EVT_OPC_DIVF:
                ret = evt_divf(entry);
                break;
            case EVT_OPC_SET_READ:
                ret = evt_set_read(entry);
                break;
            case EVT_OPC_READ:
                ret = evt_read(entry);
                break;
            case EVT_OPC_READ2:
                ret = evt_read2(entry);
                break;
            case EVT_OPC_READ3:
                ret = evt_read3(entry);
                break;
            case EVT_OPC_READ4:
                ret = evt_read4(entry);
                break;
            case EVT_OPC_READ_N:
                ret = evt_read_n(entry);
                break;
            case EVT_OPC_SET_READF:
                ret = evt_set_readf(entry);
                break;
            case EVT_OPC_READF:
                ret = evt_readf(entry);
                break;
            case EVT_OPC_READF2:
                ret = evt_readf2(entry);
                break;
            case EVT_OPC_READF3:
                ret = evt_readf3(entry);
                break;
            case EVT_OPC_READF4:
                ret = evt_readf4(entry);
                break;
            case EVT_OPC_READF_N:
                ret = evt_readf_n(entry);
                break;
            case EVT_OPC_CLAMP_INT:
                ret = evt_clamp_int(entry);
                break;
            case EVT_OPC_SET_USER_WRK:
                ret = evt_set_user_wrk(entry);
                break;
            case EVT_OPC_SET_USER_FLG:
                ret = evt_set_user_flg(entry);
                break;
            case EVT_OPC_ALLOC_USER_WRK:
                ret = evt_alloc_user_wrk(entry);
                break;
            case EVT_OPC_DELETE_EVT:
                ret = evt_delete_evt(entry);
                break;
            case EVT_OPC_AND:
                ret = evt_and(entry);
                break;
            case EVT_OPC_ANDI:
                ret = evt_andi(entry);
                break;
            case EVT_OPC_OR:
                ret = evt_or(entry);
                break;
            case EVT_OPC_ORI:
                ret = evt_ori(entry);
                break;
            case EVT_OPC_SET_FRAME_FROM_MSEC:
                evt_set_frame_from_msec(entry);
                break;
            case EVT_OPC_SET_MSEC_FROM_FRAME:
                evt_set_msec_from_frame(entry);
                break;
            case EVT_OPC_SET_RAM:
                evt_set_ram(entry);
                break;
            case EVT_OPC_SET_RAMF:
                evt_set_ramf(entry);
                break;
            case EVT_OPC_GET_RAM:
                evt_get_ram(entry);
                break;
            case EVT_OPC_GET_RAMF:
                evt_get_ramf(entry);
                break;
            case EVT_OPC_SETR:
                evt_setr(entry);
                break;
            case EVT_OPC_SETRF:
                evt_setrf(entry);
                break;
            case EVT_OPC_GETR:
                evt_getr(entry);
                break;
            case EVT_OPC_GETRF:
                evt_getrf(entry);
                break;
            case EVT_OPC_USER_FUNC:
                ret = evt_user_func(entry);
                break;
            case EVT_OPC_RUN_EVT:
                ret = evt_run_evt(entry);
                break;
            case EVT_OPC_RUN_EVT_ID:
                ret = evt_run_evt_id(entry);
                break;
            case EVT_OPC_RUN_CHILD_EVT:
                ret = evt_run_child_evt(entry);
                break;
            case EVT_OPC_SET_PRI:
                ret = evt_set_pri(entry);
                break;
            case EVT_OPC_SET_SPD:
                ret = evt_set_spd(entry);
                break;
            case EVT_OPC_SET_TYPE:
                ret = evt_set_type(entry);
                break;
            case EVT_OPC_RESTART_EVT:
                ret = evt_restart_evt(entry);
                break;
            case EVT_OPC_STOP_ALL:
                ret = evt_stop_all(entry);
                break;
            case EVT_OPC_START_ALL:
                ret = evt_start_all(entry);
                break;
            case EVT_OPC_STOP_OTHER:
                ret = evt_stop_other(entry);
                break;
            case EVT_OPC_START_OTHER:
                ret = evt_start_other(entry);
                break;
            case EVT_OPC_STOP_ID:
                ret = evt_stop_id(entry);
                break;
            case EVT_OPC_START_ID:
                ret = evt_start_id(entry);
                break;
            case EVT_OPC_CHK_EVT:
                ret = evt_chk_evt(entry);
                break;
            case EVT_OPC_INLINE_EVT:
                ret = evt_inline_evt(entry);
                break;
            case EVT_OPC_END_INLINE:
                ret = evt_end_inline(entry);
                break;
            case EVT_OPC_INLINE_EVT_ID:
                ret = evt_inline_evt_id(entry);
                break;
            case EVT_OPC_BROTHER_EVT:
                ret = evt_brother_evt(entry);
                break;
            case EVT_OPC_BROTHER_EVT_ID:
                ret = evt_brother_evt_id(entry);
                break;
            case EVT_OPC_END_BROTHER:
                ret = evt_end_brother(entry);
                break;
            case EVT_OPC_DEBUG_PUT_MSG:
                ret = evt_debug_put_msg(entry);
                break;
            case EVT_OPC_DEBUG_MSG_CLEAR:
                ret = evt_debug_msg_clear(entry);
                break;
            case EVT_OPC_DEBUG_PUT_REG:
                ret = evt_debug_put_reg(entry);
                break;
            case EVT_OPC_DEBUG_NAME:
                ret = evt_debug_name(entry);
                break;
            case EVT_OPC_DEBUG_REM:
                ret = evt_debug_rem(entry);
                break;
            case EVT_OPC_DEBUG_BP:
                ret = evt_debug_bp(entry);
                break;
            default:
                assert(0xd07, 0, "EVTMGR_CMD:Command Undefined !!");
                break;
        }

        if (ret == EVT_RET_BLOCK)
            continue;

        if (ret == EVT_RET_END)
            return -1;
        
        if (ret < 0)
            return 1;
        
        if (ret == EVT_RET_BLOCK_WEAK)
            break;
        else if (ret == EVT_RET_CONTINUE_WEAK)
        {
            entry->curOpcode = 0;
            break;
        }
        else if (ret == EVT_RET_CONTINUE)
        {
            entry->curOpcode = 0;
            continue;            
        }
    }
    return 0;
}

// Regswap in the check_float inlines
// https://decomp.me/scratch/nFkPA
#ifdef NON_MATCHING
s32 evtGetValue(EvtEntry * entry, s32 reg)
{
    EvtWork * wp;
    u32 mask;
    u32 dat;
    s32 val;

    wp = evtGetWork();

    if (reg <= EVTDAT_ADDR_MAX)
    {
        return reg;
    }
    else if (reg <= -270000000)
    {
        return reg;
    }
    else if (reg <= EVTDAT_FLOAT_MAX)
    {
        val = (s32) check_float(reg);
        return val;
    }
    else if (reg <= EVTDAT_UF_MAX)
    {
        reg += EVTDAT_UF_BASE;

        mask = 1U << (reg % 32);
        dat = entry->uf[reg / 32];
        reg = (s32) (mask & dat);

        return reg != 0;
    }
    else if (reg <= EVTDAT_UW_MAX)
    {
        reg += EVTDAT_UW_BASE;

        val = entry->uw[reg];

        if (val <= EVTDAT_ADDR_MAX)
            return val;

        if (val <= EVTDAT_FLOAT_MAX)
            val = (s32) check_float(val);

        return val;
    }
    else if (reg <= EVTDAT_GSW_MAX)
    {
        reg += EVTDAT_GSW_BASE;

        return swByteGet(reg);
    }
    else if (reg <= EVTDAT_LSW_MAX)
    {
        reg += EVTDAT_LSW_BASE;

        return _swByteGet(reg);
    }
    else if (reg <= EVTDAT_GSWF_MAX)
    {
        reg += EVTDAT_GSWF_BASE;

        return swGet(reg);
    }
    else if (reg <= EVTDAT_LSWF_MAX)
    {
        reg += EVTDAT_LSWF_BASE;

        return _swGet(reg);
    }
    else if (reg <= EVTDAT_GF_MAX)
    {
        reg += EVTDAT_GF_BASE;

        mask = 1U << (reg % 32);
        dat = wp->gf[reg / 32];
        reg = (s32) (mask & dat);

        return reg != 0;
    }
    else if (reg <= EVTDAT_LF_MAX)
    {
        reg += EVTDAT_LF_BASE;

        mask = 1U << (reg % 32);
        dat = entry->lf[reg / 32];
        reg = (s32) (mask & dat);

        return reg != 0;
    }
    else if (reg <= EVTDAT_GW_MAX)
    {
        reg += EVTDAT_GW_BASE;

        val = wp->gw[reg];

        if (val <= EVTDAT_ADDR_MAX)
            return val;
        
        if (val <= EVTDAT_FLOAT_MAX)
            val = (s32) check_float(val);

        return val;
    }
    else if (reg <= EVTDAT_LW_MAX)
    {
        reg += EVTDAT_LW_BASE;

        val = entry->lw[reg];

        if (val <= EVTDAT_ADDR_MAX)
            return val;
        
        if (val <= EVTDAT_FLOAT_MAX)
            val = (s32) check_float(val);

        return val;
    }
    else
    {
        return reg;
    }
}
#else
asm s32 evtGetValue(EvtEntry * entry, s32 variable)
{
    #include "asm/800de594.s"
}
#endif

// Regswap in the check_float inlines
// https://decomp.me/scratch/nFkPA
#ifdef NON_MATCHING
s32 evtSetValue(EvtEntry * entry, s32 reg, s32 value)
{
    EvtWork * wp;
    s32 shift;
    s32 ret;

    wp = evtGetWork();

    if (reg <= EVTDAT_ADDR_MAX)
    {
        return value;
    }
    else if (reg <= EVTDAT_FLOAT_MAX)
    {
        return (s32) check_float(value);
    }
    else if (reg <= EVTDAT_UF_MAX)
    {
        reg += EVTDAT_UF_BASE;

        shift = reg % 32;
        if (value)
            entry->uf[reg / 32] |= (1 << shift);
        else
            entry->uf[reg / 32] &= ~(1 << shift);

        return value;
    }
    else if (reg <= EVTDAT_UW_MAX)
    {
        reg += EVTDAT_UW_BASE;
        ret = entry->uw[reg];
        entry->uw[reg] = value;
        return ret;
    }
    else if (reg <= EVTDAT_GSW_MAX)
    {
        reg += EVTDAT_GSW_BASE;
        ret = swByteGet(reg);
        swByteSet(reg, value);
        return ret;
    }
    else if (reg <= EVTDAT_LSW_MAX)
    {
        reg += EVTDAT_LSW_BASE;
        ret = _swByteGet(reg);
        _swByteSet(reg, (u8)value);
        return ret;
    }
    else if (reg <= EVTDAT_GSWF_MAX)
    {
        reg += EVTDAT_GSWF_BASE;
        ret = swGet(reg);
        if (value)
            swSet(reg);
        else
            swClear(reg);
        return ret;
    }
    else if (reg <= EVTDAT_LSWF_MAX)
    {
        reg += EVTDAT_LSWF_BASE;
        ret = _swGet(reg);
        if (value)
            _swSet(reg);
        else
            _swClear(reg);
        return ret;
    }
    else if (reg <= EVTDAT_GF_MAX)
    {
        reg += EVTDAT_GF_BASE;

        shift = reg % 32;
        if (value)
            wp->gf[reg / 32] |= (1 << shift);
        else
            wp->gf[reg / 32] &= ~(1 << shift);

        return value;
    }
    else if (reg <= EVTDAT_LF_MAX)
    {
        reg += EVTDAT_LF_BASE;

        shift = reg % 32;
        if (value)
            entry->lf[reg / 32] |= (1 << shift);
        else
            entry->lf[reg / 32] &= ~(1 << shift);

        return value;
    }
    else if (reg <= EVTDAT_GW_MAX)
    {
        reg += EVTDAT_GW_BASE;
        ret = wp->gw[reg];
        wp->gw[reg] = value;
        return ret;
    }
    else if (reg <= EVTDAT_LW_MAX)
    {
        reg += EVTDAT_LW_BASE;
        ret = entry->lw[reg];
        entry->lw[reg] = value;
        return ret;
    }
    else
    {
        return value;
    }
}
#else
asm s32 evtSetValue(EvtEntry * entry, s32 variable, s32 value)
{
    #include "asm/800de9b8.s"
}
#endif

asm float evtGetFloat(EvtEntry * entry, s32 variable)
{
    #include "asm/800dedb8.s"
}

asm float evtSetFloat(EvtEntry * entry, s32 variable, float value)
{
    #include "asm/800df1fc.s"
}

EvtScriptCode * evtSearchElse(EvtEntry * entry)
{
    s32 ifDepth;
    EvtScriptCode * pInstr;
    s32 opc;

    ifDepth = 0;
    pInstr = entry->pCurInstruction;

    while (true)
    {
        opc = *pInstr & 0xffff;
        pInstr += *pInstr++ >> 16;

        switch (opc)
        {
            case EVT_OPC_END_SCRIPT:
                assert(0xf4b, 0, "EVTMGR_CMD:'ELSE' Search Error !!");

            case EVT_OPC_END_IF:
                if (--ifDepth >= 0)
                    break;
                else
                    return pInstr;

            case EVT_OPC_IF_STR_EQUAL ... EVT_OPC_IF_NOT_FLAG:
                ifDepth += 1;
                break;

             case EVT_OPC_ELSE:
                if (ifDepth == 0)
                    return pInstr;
                else
                    break;
       }
    }
}

EvtScriptCode * evtSearchEndIf(EvtEntry * entry)
{
    s32 ifDepth;
    EvtScriptCode * pInstr;
    s32 opc;

    ifDepth = 0;
    pInstr = entry->pCurInstruction;

    while (true)
    {
        opc = *pInstr & 0xffff;
        pInstr += *pInstr++ >> 16;

        switch (opc)
        {
            case EVT_OPC_END_SCRIPT:
                assert(0xf89, 0, "EVTMGR_CMD:'END_IF' Search Error !!");

            case EVT_OPC_END_IF:
                if (--ifDepth >= 0)
                    break;
                else
                    return pInstr;

            case EVT_OPC_IF_STR_EQUAL ... EVT_OPC_IF_NOT_FLAG:
                ifDepth += 1;
                break;
       }
    }
}

EvtScriptCode * evtSearchEndSwitch(EvtEntry * entry)
{
    EvtScriptCode * pInstr;
    s32 switchDepth;
    s32 opc;
    EvtScriptCode * ret;

    pInstr = entry->pCurInstruction;
    switchDepth = 1;

    while (true)
    {
        opc = *pInstr & 0xffff;
        ret = pInstr;
        pInstr += *pInstr++ >> 16;

        switch (opc)
        {
            case EVT_OPC_END_SCRIPT:
                assert(0xfc9, 0, "EVTMGR_CMD:'END_SWITCH' Search Error !!");

            case EVT_OPC_SWITCH:
                switchDepth += 1;
                break;

            case EVT_OPC_END_SWITCH:
                if (--switchDepth == 0)
                    return ret;
                else
                    break;
        }
    }
}

EvtScriptCode * evtSearchCase(EvtEntry * entry)
{
    EvtScriptCode * pInstr;
    s32 switchDepth;
    s32 opc;
    EvtScriptCode * ret;

    pInstr = entry->pCurInstruction;
    switchDepth = 1;

    while (true)
    {
        opc = *pInstr & 0xffff;
        ret = pInstr;
        pInstr += *pInstr++ >> 16;

        switch (opc)
        {
            case EVT_OPC_END_SCRIPT:
                assert(0xff1, 0, "EVTMGR_CMD:'CASE' Search Error !!");

            case EVT_OPC_SWITCH:
                switchDepth += 1;
                break;

            case EVT_OPC_END_SWITCH:
                if (--switchDepth == 0)
                    return ret;
                else
                    break;

            case EVT_OPC_CASE_EQUAL ... EVT_OPC_CASE_BETWEEN:
                if (switchDepth == 1)
                    return ret;
                else
                    break;
        }
    }
}

EvtScriptCode * evtSearchWhile(EvtEntry * entry)
{
    s32 doWhileDepth;
    EvtScriptCode * pInstr;
    s32 opc;

    doWhileDepth = 0;
    pInstr = entry->pCurInstruction;

    while (true)
    {
        opc = *pInstr & 0xffff;
        pInstr += *pInstr++ >> 16;

        switch (opc)
        {
            case EVT_OPC_END_SCRIPT:
                assert(0x1027, 0, "EVTMGR_CMD:'WHILE' Search Error !!");

            case EVT_OPC_WHILE:
                if (--doWhileDepth >= 0)
                    break;
                else
                    return pInstr;

            case EVT_OPC_DO:
                doWhileDepth += 1;
                break;
        }
    }
}

EvtScriptCode * evtSearchJustBeforeWhile(EvtEntry * entry)
{
    s32 doWhileDepth;
    EvtScriptCode * pInstr;
    s32 opc;

    doWhileDepth = 0;
    pInstr = entry->pCurInstruction;

    while (true)
    {
        opc = *pInstr & 0xffff;

        switch (opc)
        {
            case EVT_OPC_END_SCRIPT:
                assert(0x1049, 0, "EVTMGR_CMD:just before 'WHILE' Search Error !!");

            case EVT_OPC_WHILE:
                if (--doWhileDepth >= 0)
                    break;
                else
                    return pInstr;

            case EVT_OPC_DO:
                doWhileDepth += 1;
                break;
        }

        pInstr += *pInstr++ >> 16;
    }
}
