#include <common.h>
#include <evtmgr_cmd.h>
#include <evtmgr.h>
#include <os.h>
#include <stdio.h>
#include <string.h>
#include <swdrv.h>
#include <system.h>

static float check_float(s32 val) // always inlined
{
    if (val <= EVTDAT_FLOAT_MAX)
        return (val + EVTDAT_FLOAT_BASE) / 1024.0f;
    else
        return val;
}

// change_float (inlined/unused)

int evt_end_evt(EvtEntry * entry)
{
    evtDelete(entry);

    return EVT_END;
}

int evt_lbl(EvtEntry * entry)
{
    (void) entry;

    return EVT_CONTINUE;
}

// Matching but not good
int evt_goto(EvtEntry * entry)
{
    s32 lbl = evtGetValue(entry, entry->pCurData[0]);
    EvtScriptCode * r31 = entry->pCurData;
    EvtScriptCode * dest;

    if (lbl < EVTDAT_ADDR_MAX) // TODO: evtSearchLabel inlined
    {
        dest = (EvtScriptCode *) lbl;
    }
    else
    {
        s32 n;
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

    return EVT_CONTINUE;
}

int evt_do(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;
    s32 count = *p++;

    entry->dowhileDepth += 1;
    s32 depth = entry->dowhileDepth;
    if (depth >= 8)
        assert(0x67, 0, "EVTMGR_CMD:While Table Overflow !!");

    entry->dowhileStartPtrs[depth] = p;
    entry->dowhileCounters[depth] = count;

    return EVT_CONTINUE;
}

int evt_while(EvtEntry * entry)
{
    s32 count; // only way the register usage of depth's sign extend matched

    s32 depth = entry->dowhileDepth;
    if (depth < 0)
        assert(0x7b, 0, "EVTMGR_CMD:While Table Underflow !!");

    count = entry->dowhileCounters[depth];
    if (count == 0)
    {
        entry->pCurInstruction = entry->dowhileStartPtrs[depth];

        return EVT_CONTINUE;
    }
    else
    {
        if (count >= -10000000)
        {
            entry->dowhileCounters[depth] = --count;
        }
        else
        {
            s32 ret = evtGetValue(entry, count);
            evtSetValue(entry, count, ret - 1);
            count = ret - 1;
        }

        if (count != 0)
        {
            entry->pCurInstruction = entry->dowhileStartPtrs[depth];

            return EVT_CONTINUE;
        }
        else
        {
            entry->dowhileDepth--;

            return EVT_CONTINUE;
        }
    }
}

int evt_do_break(EvtEntry * entry)
{
    if (entry->dowhileDepth < 0)
        assert(0xa1, 0, "EVTMGR_CMD:While Table Underflow !!");

    entry->pCurInstruction = evtSearchWhile(entry);
    entry->dowhileDepth -= 1;

    return EVT_CONTINUE;
}

int evt_do_continue(EvtEntry * entry)
{
    if (entry->dowhileDepth < 0)
        assert(0xb0, 0, "EVTMGR_CMD:While Table Underflow !!");

    entry->pCurInstruction = evtSearchJustBeforeWhile(entry);

    return EVT_CONTINUE;
}

int evt_wait_frm(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    // Just an if didn't match
    switch (entry->blocked)
    {
        case false:
            entry->tempS[0] = evtGetValue(entry, *p);
            entry->blocked = 1;
    }

    if (entry->tempS[0] == 0)
        return EVT_CONTINUE;
    else
        return !--entry->tempS[0];
}

/* Unfinished
int evt_wait_msec(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;
    s64 time = entry->lifetime;
    switch(entry->blocked)
    {
        case false:
            entry->tempS[0] = evtGetValue(entry, *p);
            entry->tempS[1] = (s32) ((time & 0xffffffff00000000) >> 32);
            entry->tempS[2] = (s32) (time & 0xffffffff);
            entry->blocked = true;
    }

    if (entry->tempU[0] == 0)
        return EVT_CONTINUE;
    else
        // Wrong
        return (OSTicksToMilliseconds((time & 0xffffffff) - entry->tempU[2]) >= entry->tempU[0]);
}
*/

int evt_halt(EvtEntry * entry)
{
   return evtGetValue(entry, entry->pCurData[0]) ? EVT_BLOCK_WEAK : EVT_CONTINUE;
}

int evt_if_str_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (strcmp(s1, s2))
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_str_not_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (!strcmp(s1, s2))
    {
        entry->pCurInstruction = evtSearchElse(entry);
    
        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_str_small(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (strcmp(s1, s2) >= 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_str_large(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (strcmp(s1, s2) <= 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}


int evt_if_str_small_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (strcmp(s1, s2) > 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_str_large_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    char * s1 = (char *) evtGetValue(entry, p[0]);
    char * s2 = (char *) evtGetValue(entry, p[1]);

    if (s1 == NULL)
        s1 = "";
    if (s2 == NULL)
        s2 = "";

    if (strcmp(s1, s2) < 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_iff_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    float f1 = evtGetFloat(entry, p[0]);
    float f2 = evtGetFloat(entry, p[1]);

    if (f1 != f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_iff_not_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    f32 f1 = evtGetFloat(entry, p[0]);
    f32 f2 = evtGetFloat(entry, p[1]);

    if (f1 == f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_iff_small(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    f32 f1 = evtGetFloat(entry, p[0]);
    f32 f2 = evtGetFloat(entry, p[1]);

    if (f1 >= f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_iff_large(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    f32 f1 = evtGetFloat(entry, p[0]);
    f32 f2 = evtGetFloat(entry, p[1]);

    if (f1 <= f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_iff_small_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    f32 f1 = evtGetFloat(entry, p[0]);
    f32 f2 = evtGetFloat(entry, p[1]);

    if (f1 > f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_iff_large_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    f32 f1 = evtGetFloat(entry, p[0]);
    f32 f2 = evtGetFloat(entry, p[1]);

    if (f1 < f2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 val1 = evtGetValue(entry, p[0]);
    s32 val2 = evtGetValue(entry, p[1]);

    if (val1 != val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_not_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 val1 = evtGetValue(entry, p[0]);
    s32 val2 = evtGetValue(entry, p[1]);

    if (val1 == val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_small(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 val1 = evtGetValue(entry, p[0]);
    s32 val2 = evtGetValue(entry, p[1]);

    if (val1 >= val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_large(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 val1 = evtGetValue(entry, p[0]);
    s32 val2 = evtGetValue(entry, p[1]);

    if (val1 <= val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_small_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 val1 = evtGetValue(entry, p[0]);
    s32 val2 = evtGetValue(entry, p[1]);

    if (val1 > val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_large_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 val1 = evtGetValue(entry, p[0]);
    s32 val2 = evtGetValue(entry, p[1]);

    if (val1 < val2)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}


int evt_if_flag(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 val = evtGetValue(entry, p[0]);
    s32 mask = p[1];

    if ((val & mask) == 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_if_not_flag(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 val = evtGetValue(entry, p[0]);
    s32 mask = p[1];

    if ((val & mask) != 0)
    {
        entry->pCurInstruction = evtSearchElse(entry);

        return EVT_CONTINUE;
    }

    return EVT_CONTINUE;
}

int evt_else(EvtEntry * entry)
{
    entry->pCurInstruction = evtSearchEndIf(entry);

    return EVT_CONTINUE;
}

int evt_end_if(EvtEntry * entry)
{
    (void) entry;

    return EVT_CONTINUE;
}

int evt_switch(EvtEntry * entry)
{
    s32 value = evtGetValue(entry, entry->pCurData[0]);

    entry->switchDepth += 1;
    s32 depth = entry->switchDepth;
    if (depth >= 8)
        assert(0x31e, 0, "EVTMGR_CMD:Switch Table Overflow !!");

    entry->switchValues[depth] = value;
    entry->switchStates[depth] = 1;

    return EVT_CONTINUE;
}

int evt_switchi(EvtEntry * entry)
{
    // Required for register usage
    s32 depth;
    s32 value;

    value = entry->pCurData[0];
    entry->switchDepth += 1;
    depth = entry->switchDepth;
    if (depth >= 8)
        assert(0x336, 0, "EVTMGR_CMD:Switch Table Overflow !!");

    entry->switchValues[depth] = value;
    entry->switchStates[depth] = 1;

    return EVT_CONTINUE;
}

int evt_case_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x34b, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 targetvalue = evtGetValue(entry, p[0]);
    s32 state = entry->switchStates[depth];
    s32 inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        if (targetvalue != inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_CONTINUE;
    }
}

int evt_case_not_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x36c, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 targetValue = evtGetValue(entry, p[0]);
    s32 state = entry->switchStates[depth];
    s32 inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        if (targetValue == inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_CONTINUE;
    }
}

int evt_case_small(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x38d, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 targetValue = evtGetValue(entry, p[0]);
    s32 state = entry->switchStates[depth];
    s32 inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        if (targetValue <= inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_CONTINUE;
    }
}

int evt_case_small_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x3ae, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 targetValue = evtGetValue(entry, p[0]);
    s32 state = entry->switchStates[depth];
    s32 inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        if (targetValue < inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_CONTINUE;
    }
}


int evt_case_large(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x3cf, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 targetValue = evtGetValue(entry, p[0]);
    s32 state = entry->switchStates[depth];
    s32 inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        if (targetValue >= inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_CONTINUE;
    }
}

int evt_case_large_equal(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x3f0, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 targetValue = evtGetValue(entry, p[0]);
    s32 state = entry->switchStates[depth];
    s32 inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        if (targetValue > inputValue)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_CONTINUE;
    }
}


int evt_case_between(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x411, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 min = evtGetValue(entry, p[0]);
    s32 max = evtGetValue(entry, p[1]);
    s32 state = entry->switchStates[depth];
    s32 inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        if ((min <= inputValue) && (inputValue <= max))
            entry->switchStates[depth] = 0;
        else
            entry->pCurInstruction = evtSearchCase(entry);

        return EVT_CONTINUE;
    }
}

int evt_case_etc(EvtEntry * entry)
{
    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x431, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 state = entry->switchStates[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        entry->switchStates[depth] = 0;

        return EVT_CONTINUE;
    }
}

int evt_case_flag(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x449, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    u32 targetMask = (u32) p[0];
    s32 state = entry->switchStates[depth];
    s32 inputValue = entry->switchValues[depth];

    if (state <= 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        if ((inputValue & targetMask) == 0)
            entry->pCurInstruction = evtSearchCase(entry);
        else
            entry->switchStates[depth] = 0;

        return EVT_CONTINUE;
    }
}

int evt_case_or(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x46a, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 targetvalue = evtGetValue(entry, p[0]);
    s32 inputValue = entry->switchValues[depth];
    s8 state = entry->switchStates[depth];
    
    if (state == 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        if (targetvalue == inputValue)
            entry->switchStates[depth] = -1;
        else if (state != -1)
            entry->pCurInstruction = evtSearchCase(entry);

        return EVT_CONTINUE;
    }
}

int evt_case_and(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x48c, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 targetvalue = evtGetValue(entry, p[0]);
    s32 inputValue = entry->switchValues[depth];
    s8 state = entry->switchStates[depth];
    
    if (state == 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else if (state == -2)
    {
        entry->pCurInstruction = evtSearchCase(entry);

        return EVT_CONTINUE;
    }
    else
    {
        if (targetvalue == inputValue)
        {
            entry->switchStates[depth] = -1;
        }
        else
        {
            entry->switchStates[depth] = -2;
            entry->pCurInstruction = evtSearchCase(entry);
        }

        return EVT_CONTINUE;
    }
}

int evt_case_end(EvtEntry * entry)
{
    EvtScriptCode * p = entry->pCurData;

    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x4b1, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    s32 inputValue = entry->switchValues[depth];
    s8 state = entry->switchStates[depth];
    
    if (state == 0)
    {
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else if (state == -1)
    {
        entry->switchStates[depth] = 0;
        entry->pCurInstruction = evtSearchEndSwitch(entry);

        return EVT_CONTINUE;
    }
    else
    {
        entry->switchStates[depth] = 1;
        entry->pCurInstruction = evtSearchCase(entry);

        return EVT_CONTINUE;
    }
}

int evt_switch_break(EvtEntry * entry)
{
    if (entry->switchDepth < 0)
        assert(0x4cf, 0, "EVTMGR_CMD:Switch Table Underflow !!");
    
    entry->pCurInstruction = evtSearchEndSwitch(entry);

    return EVT_CONTINUE;
}

int evt_end_switch(EvtEntry * entry)
{
    s32 depth = entry->switchDepth;
    if (depth < 0)
        assert(0x4e0, 0, "EVTMGR_CMD:Switch Table Underflow !!");

    entry->switchStates[depth] = 0;
    entry->switchDepth -= 1;

    return EVT_CONTINUE;
}

int evt_set(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 variable = p[0];
    s32 value = evtGetValue(entry, p[1]);
    evtSetValue(entry, variable, value);
    
    return EVT_CONTINUE;
}

int evt_seti(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 variable = p[0];
    s32 value = p[1];
    evtSetValue(entry, variable, value);
    
    return EVT_CONTINUE;
}

int evt_setf(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 variable = p[0];
    float value = evtGetFloat(entry, p[1]);
    evtSetFloat(entry, variable, value);
    
    return EVT_CONTINUE;
}

int evt_add(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 destVar = p[0];
    s32 param = evtGetValue(entry, p[1]);
    s32 result = evtGetValue(entry, destVar) + param;
    evtSetValue(entry, destVar, result);
    
    return EVT_CONTINUE;
}

int evt_sub(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 destVar = p[0];
    s32 param = evtGetValue(entry, p[1]);
    s32 result = evtGetValue(entry, destVar) - param;
    evtSetValue(entry, destVar, result);
    
    return EVT_CONTINUE;
}

int evt_mul(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 destVar = p[0];
    s32 param = evtGetValue(entry, p[1]);
    s32 result = evtGetValue(entry, destVar) * param;
    evtSetValue(entry, destVar, result);
    
    return EVT_CONTINUE;
}

int evt_div(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 destVar = p[0];
    s32 param = evtGetValue(entry, p[1]);
    s32 result = evtGetValue(entry, destVar) / param;
    evtSetValue(entry, destVar, result);
    
    return EVT_CONTINUE;
}

int evt_mod(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 destVar = p[0];
    s32 param = (s32) (evtGetValue(entry, p[1]) + 0.5f);
    s32 value = (s32) (evtGetValue(entry, destVar) + 0.5f);
    s32 result = value % param;
    evtSetValue(entry, destVar, result);
    
    return EVT_CONTINUE;
}

int evt_addf(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 destVar = p[0];
    f32 param = evtGetFloat(entry, p[1]);
    f32 result = evtGetFloat(entry, destVar) + param;
    evtSetFloat(entry, destVar, result);
    
    return EVT_CONTINUE;
}

int evt_subf(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 destVar = p[0];
    f32 param = evtGetFloat(entry, p[1]);
    f32 result = evtGetFloat(entry, destVar) - param;
    evtSetFloat(entry, destVar, result);
    
    return EVT_CONTINUE;
}
int evt_mulf(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 destVar = p[0];
    f32 param = evtGetFloat(entry, p[1]);
    f32 result = evtGetFloat(entry, destVar) * param;
    evtSetFloat(entry, destVar, result);
    
    return EVT_CONTINUE;
}

int evt_divf(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    s32 destVar = p[0];
    f32 param = evtGetFloat(entry, p[1]);
    f32 result = evtGetFloat(entry, destVar) / param;
    evtSetFloat(entry, destVar, result);
    
    return EVT_CONTINUE;
}

int evt_set_read(EvtEntry * entry)
{
    entry->readAddr = (int *) evtGetValue(entry, entry->pCurData[0]);

    return EVT_CONTINUE;
}

int evt_set_readf(EvtEntry * entry)
{
    entry->readAddr = (int *) evtGetValue(entry, entry->pCurData[0]);

    return EVT_CONTINUE;
}

int evt_read(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    evtSetValue(entry, p[0], *entry->readAddr++);

    return EVT_CONTINUE;
}

int evt_read2(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    evtSetValue(entry, p[0], *entry->readAddr++);
    evtSetValue(entry, p[1], *entry->readAddr++);

    return EVT_CONTINUE;
}

int evt_read3(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    evtSetValue(entry, p[0], *entry->readAddr++);
    evtSetValue(entry, p[1], *entry->readAddr++);
    evtSetValue(entry, p[2], *entry->readAddr++);

    return EVT_CONTINUE;
}

int evt_read4(EvtEntry * entry)
{
    s32 * p = entry->pCurData;
    evtSetValue(entry, p[0], *entry->readAddr++);
    evtSetValue(entry, p[1], *entry->readAddr++);
    evtSetValue(entry, p[2], *entry->readAddr++);
    evtSetValue(entry, p[3], *entry->readAddr++);

    return EVT_CONTINUE;
}

// int evt_read_n(EvtEntry * entry)
// int evt_readf(EvtEntry * entry)
// int evt_readf2(EvtEntry * entry)
// int evt_readf3(EvtEntry * entry)
// int evt_readf4(EvtEntry * entry)
// int evt_readf_n(EvtEntry * entry)

int evt_clamp_int(EvtEntry * entry)
{
    s32 * pData = entry->pCurData;

    s32 dest = pData[0];
    s32 destVal = evtGetValue(entry, dest);

    s32 minVal = evtGetValue(entry, pData[1]);
    s32 maxVal = evtGetValue(entry, pData[2]);

    if (destVal < minVal)
    {
        evtSetValue(entry, dest, minVal);
    }

    if (destVal > maxVal)
    {
        evtSetValue(entry, dest, maxVal);
    }
    
    return EVT_CONTINUE;
}

// int evt_set_user_wrk(EvtEntry * entry)
// int evt_set_user_flg(EvtEntry * entry)
// int evt_alloc_user_wrk(EvtEntry * entry)
// int evt_and(EvtEntry * entry)
// int evt_andi(EvtEntry * entry)
// int evt_or(EvtEntry * entry)
// int evt_ori(EvtEntry * entry)
// int evt_set_frame_from_msec(EvtEntry * entry)
// int evt_set_msec_from_frame(EvtEntry * entry)
// int evt_set_ram(EvtEntry * entry)
// int evt_set_ramf(EvtEntry * entry)
// int evt_get_ram(EvtEntry * entry)
// int evt_get_ramf(EvtEntry * entry)
// int evt_setr(EvtEntry * entry)
// int evt_setrf(EvtEntry * entry)
// int evt_getr(EvtEntry * entry)
// int evt_getrf(EvtEntry * entry)
// int evt_user_func(EvtEntry * entry)
// int evt_run_evt(EvtEntry * entry)
// int evt_run_evt_id(EvtEntry * entry)
// int evt_run_child_evt(EvtEntry * entry)
// int evt_restart_evt(EvtEntry * entry)
// int evt_delete_evt(EvtEntry * entry)
// int evt_set_pri(EvtEntry * entry)
// int evt_set_spd(EvtEntry * entry)
// int evt_set_type(EvtEntry * entry)
// int evt_stop_all(EvtEntry * entry)
// int evt_start_all(EvtEntry * entry)
// int evt_stop_other(EvtEntry * entry)
// int evt_start_other(EvtEntry * entry)
// int evt_stop_id(EvtEntry * entry)
// int evt_start_id(EvtEntry * entry)
// int evt_chk_evt(EvtEntry * entry)
// int evt_inline_evt(EvtEntry * entry)
// int evt_inline_evt_id(EvtEntry * entry)
// int evt_end_inline(EvtEntry * entry)
// int evt_brother_evt(EvtEntry * entry)
// int evt_brother_evt_id(EvtEntry * entry)
// int evt_end_brother(EvtEntry * entry)

int evt_debug_put_msg(EvtEntry * entry)
{
    evtGetValue(entry, entry->pCurData[0]);

    return EVT_CONTINUE;
}

int evt_debug_msg_clear(EvtEntry * entry)
{
    (void) entry;

    return EVT_CONTINUE;
}

int evt_debug_put_reg(EvtEntry * entry)
{
    static char str[256]; // 8050ce30

    EvtScriptCode * p = entry->pCurData;
    EvtWork * wp = evtGetWork();
    s32 reg = *p;

    if (reg <= EVTDAT_ADDR_MAX)
    {
        sprintf(str, "ADDR     [%08X]", reg);
    }
    else if (reg <= EVTDAT_FLOAT_MAX)
    {
        f32 f = check_float(reg);

        sprintf(str, "FLOAT    [%4.2f]", f);
    }
    else if (reg <= EVTDAT_UF_MAX)
    {
        reg += EVTDAT_UF_BASE;

        u32 mask = 1U << (reg % 32);
        u32 dat = entry->uf[reg / 32];

        sprintf(str, "UF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_UW_MAX)
    {
        reg += EVTDAT_UW_BASE;

        s32 val = entry->uw[reg];

        if (val <= EVTDAT_ADDR_MAX)
        {
            sprintf(str, "UW(%3d)  [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX)
        {
            f32 f = check_float(val);

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

        s32 val = swByteGet(reg);

        if (val <= EVTDAT_ADDR_MAX)
        {
            sprintf(str, "GSW(%3d) [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX)
        {
            f32 f = check_float(val);

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

        s32 val = _swByteGet(reg);

        if (val <= EVTDAT_ADDR_MAX)
        {
            sprintf(str, "LSW(%3d) [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX)
        {
            f32 f = check_float(val);

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

        u32 mask = 1U << (reg % 32);
        u32 dat = wp->gf[reg / 32];

        sprintf(str, "GF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_LF_MAX)
    {
        reg += EVTDAT_LF_BASE;

        u32 mask = 1U << (reg % 32);
        u32 dat = entry->lf[reg / 32];

        sprintf(str, "LF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_GW_MAX)
    {
        reg += EVTDAT_GW_BASE;

        s32 val = wp->gw[reg];

        if (val <= EVTDAT_ADDR_MAX)
        {
            sprintf(str, "GW(%3d)  [%08X]", reg, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX)
        {
            f32 f = check_float(val);
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

        s32 val = entry->lw[reg];

        if (val <= EVTDAT_ADDR_MAX)
        {
            sprintf(str, "LW(%3d)  [%08X]", reg, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX)
        {
            f32 f = check_float(val);

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

    return EVT_CONTINUE;
}

int evt_debug_name(EvtEntry * entry)
{
    entry->name = (char *) entry->pCurData[0];

    return EVT_CONTINUE;
}

int evt_debug_rem(EvtEntry * entry)
{
    (void) entry;

    return EVT_CONTINUE;
}

int evt_debug_bp(EvtEntry * entry)
{
    for (s32 i = 0; i < EVT_ENTRY_MAX; i++)
    {
        if (entry == evtGetPtr(i))
            break;
    }

    return EVT_CONTINUE_WEAK;
}

// Unfinished, just for string pool
int evtmgrCmd(EvtEntry * entry)
{
    (void) entry;

    __dummy_string("EVTMGR_CMD:Command Undefined !!");

    return 0;
}

// evtGetValue
// evtGetNumber (inlined/unused)
// evtSetValue
// evtGetFloat
// evtSetFloat

// evtSearchLabel (inlined)

EvtScriptCode * evtSearchElse(EvtEntry * entry)
{
    s32 ifDepth = 0;
    EvtScriptCode * pInstr = entry->pCurInstruction;

    while (true)
    {
        s32 opc = *pInstr & 0xffff;
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
    s32 ifDepth = 0;
    EvtScriptCode * pInstr = entry->pCurInstruction;

    while (true)
    {
        s32 opc = *pInstr & 0xffff;
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
    EvtScriptCode * pInstr = entry->pCurInstruction;
    s32 switchDepth = 1;

    while (true)
    {
        s32 opc = *pInstr & 0xffff;
        EvtScriptCode * ret = pInstr;
        pInstr += *pInstr++ >> 16;

        switch(opc)
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
    EvtScriptCode * pInstr = entry->pCurInstruction;
    s32 switchDepth = 1;

    while (true)
    {
        s32 opc = *pInstr & 0xffff;
        EvtScriptCode * ret = pInstr;
        pInstr += *pInstr++ >> 16;

        switch(opc)
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
    s32 dowhileDepth = 0;
    EvtScriptCode * pInstr = entry->pCurInstruction;

    while (true)
    {
        s32 opc = *pInstr & 0xffff;
        pInstr += *pInstr++ >> 16;

        switch (opc)
        {
            case EVT_OPC_END_SCRIPT:
                assert(0x1027, 0, "EVTMGR_CMD:'WHILE' Search Error !!");

            case EVT_OPC_WHILE:
                if (--dowhileDepth >= 0)
                    break;
                else
                    return pInstr;

            case EVT_OPC_DO:
                dowhileDepth += 1;
                break;
        }
    }
}

EvtScriptCode * evtSearchJustBeforeWhile(EvtEntry * entry)
{
    s32 dowhileDepth = 0;
    EvtScriptCode * pInstr = entry->pCurInstruction;

    while (true)
    {
        s32 opc = *pInstr & 0xffff;

        switch (opc)
        {
            case EVT_OPC_END_SCRIPT:
                assert(0x1049, 0, "EVTMGR_CMD:just before 'WHILE' Search Error !!");

            case EVT_OPC_WHILE:
                if (--dowhileDepth >= 0)
                    break;
                else
                    return pInstr;

            case EVT_OPC_DO:
                dowhileDepth += 1;
                break;
        }

        pInstr += *pInstr++ >> 16;
    }
}
