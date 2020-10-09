#include <stdint.h>
#include <stdio.h>

#include "evtmgr_cmd.h"
#include "evtmgr.h"
#include "swdrv.h"
#include "system.h"

static char debugMsgBuf[256];

static float check_float(int val) { // usually inlined
    if (val <= EVTDAT_FLOAT_MAX) {
        return (val + EVTDAT_FLOAT_BASE) / 1024.0f;
    }
    else {
        return val;
    }
}

// change_float

EVT_CMD_FN(end_evt) {
    evtDelete(entry);
    return EVT_END;
}

EVT_CMD_FN(lbl) {
    (void) entry;
    return EVT_CONTINUE;
}

// Matching but not good
EVT_CMD_FN(goto) {
    int lbl = evtGetValue(entry, entry->pCurData[0]);
    int * r31 = entry->pCurData;
    int * dest;
    if (lbl < EVTDAT_ADDR_MAX) { // likely evtSearchLabel inlined
        dest = (int *) lbl;
    }
    else {
        int n;
        for (n = 0; n < MAX_EVT_JMPTBL; n++) {
            if (lbl == entry->labelIds[n]) {
                r31 = entry->jumptable[n];
                break;
            }
        }
        if (n >= MAX_EVT_JMPTBL) {
            assertf(0, "EVTMGR_CMD:Jump Table Search error !!\n [lbl=%d, n=%d]", lbl, n);
        }
        dest = r31;
    }
    entry->pCurInstruction = dest;
    return EVT_CONTINUE;
}

EVT_CMD_FN(do) {
    int * p = entry->pCurData;
    int count = *p++;
    int depth = ++entry->dowhileDepth; // missing rlwinm here
    if (depth >= 8) {
        assert(0, "EVTMGR_CMD:While Table Overflow !!");
    }
    entry->dowhileStartPtrs[depth] = p;
    entry->dowhileCounters[depth] = count;
    return EVT_CONTINUE;
}

EVT_CMD_FN(while) {
    int count; // only way the register usage of depth's sign extend matched
    int depth = entry->dowhileDepth;
    if (depth < 0) {
        assert(0, "EVTMGR_CMD:While Table Underflow !!");
    }
    count = entry->dowhileCounters[depth];
    if (count == 0) {
        entry->pCurInstruction = entry->dowhileStartPtrs[depth];
        return EVT_CONTINUE;
    }
    else {
        if (count >= -10000000) {
            entry->dowhileCounters[depth] = --count;
        }
        else {
            int ret = evtGetValue(entry, count);
            evtSetValue(entry, count, ret - 1);
            count = ret - 1;
        }
        if (count == 0) {
            entry->pCurInstruction = entry->dowhileStartPtrs[depth];
            return EVT_CONTINUE;
        }
        else {
            entry->dowhileDepth--;
            return EVT_CONTINUE;
        }
    }
}

EVT_CMD_FN(do_break) {
    if (entry->dowhileDepth < 0) {
        assert(0, "EVTMGR_CMD:While Table Underflow !!");
    }
    entry->pCurInstruction = evtSearchWhile(entry);
    entry->dowhileDepth -= 1;
    return EVT_CONTINUE;
}

EVT_CMD_FN(do_continue) {
    if (entry->dowhileDepth < 0) {
        assert(0, "EVTMGR_CMD:While Table Underflow !!");
    }
    entry->pCurInstruction = evtSearchJustBeforeWhile(entry);
    return EVT_CONTINUE;
}

EVT_CMD_FN(wait_frm) {
    int * p = entry->pCurData;
    if (!entry->blocked) { // wrong branch setup here
        entry->unknown_0x74 = evtGetValue(entry, *p);
        entry->blocked = 1;
    }

    if (entry->unknown_0x74 == 0) {
        return EVT_CONTINUE;
    }
    else {
        return !--entry->unknown_0x74;
    }
}

// a lot

EVT_CMD_FN(debug_put_msg) {
    evtGetValue(entry, *entry->pCurData);
    return EVT_CONTINUE;
}

EVT_CMD_FN(debug_msg_clear) {
    return EVT_CONTINUE;
}

EVT_CMD_FN(debug_put_reg) {
    int * p = entry->pCurData;
    EvtWork * wp = evtGetWork();
    int reg = *p;
    if (reg <= EVTDAT_ADDR_MAX) {
        sprintf(debugMsgBuf, "ADDR     [%08X]", reg);
    }
    else if (reg <= EVTDAT_FLOAT_MAX) {
        float f = check_float(reg);
        sprintf(debugMsgBuf, "FLOAT    [%4.2f]", f);
    }
    else if (reg <= EVTDAT_UF_MAX) {
        reg += EVTDAT_UF_BASE;
        unsigned int mask = 1U << ((reg) % 32);
        unsigned int dat = entry->uf[(reg) / 32];
        sprintf(debugMsgBuf, "UF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_UW_MAX) {
        reg += EVTDAT_UW_BASE;
        int val = entry->uw[reg];
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "UW(%3d)  [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f = check_float(val);
            sprintf(debugMsgBuf, "UW(%3d)  [%4.2f]", reg, f);
        }
        else {
            sprintf(debugMsgBuf, "UW(%3d)  [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_GSW_MAX) {
        reg += EVTDAT_GSW_BASE;
        int val = swByteGet(reg);
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "GSW(%3d) [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f = check_float(val);
            sprintf(debugMsgBuf, "GSW(%3d) [%4.2f]", reg, f);
        }
        else {
            sprintf(debugMsgBuf, "GSW(%3d) [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_LSW_MAX) {
        reg += EVTDAT_LSW_BASE;
        int val = _swByteGet(reg);
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "LSW(%3d) [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f = check_float(val);
            sprintf(debugMsgBuf, "LSW(%3d)  [%4.2f]", reg, f);
        }
        else {
            sprintf(debugMsgBuf, "LSW(%3d) [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_GSWF_MAX) {
        reg += EVTDAT_GSWF_BASE;
        sprintf(debugMsgBuf, "GSWF(%3d)[%d]", reg, swGet(reg));
    }
    else if (reg <= EVTDAT_LSWF_MAX) {
        reg += EVTDAT_LSWF_BASE;
        sprintf(debugMsgBuf, "LSWF(%3d)[%d]", reg, _swGet(reg));
    }
    else if (reg <= EVTDAT_GF_MAX) {
        reg += EVTDAT_GF_BASE;
        unsigned int mask = 1U << ((reg) % 32);
        unsigned int dat = wp->gf[(reg) / 32];
        sprintf(debugMsgBuf, "GF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_LF_MAX) {
        reg += EVTDAT_LF_BASE;
        unsigned int mask = 1U << ((reg) % 32);
        unsigned int dat = entry->lf[(reg) / 32];
        sprintf(debugMsgBuf, "LF(%3d)  [%d]", reg, mask & dat);
    }
    else if (reg <= EVTDAT_GW_MAX) {
        reg += EVTDAT_GW_BASE;
        int val = wp->gw[reg];
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "GW(%3d)  [%08X]", reg, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f = check_float(val);
            sprintf(debugMsgBuf, "GW(%3d)  [%4.2f]", reg, f);
        }
        else {
            sprintf(debugMsgBuf, "GW(%3d)  [%d]", reg, val);
        }
    }
    else if (reg <= EVTDAT_LW_MAX) {
        reg += EVTDAT_LW_BASE;
        int val = entry->lw[reg];
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "LW(%3d)  [%08X]", reg, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f = check_float(val);
            sprintf(debugMsgBuf, "LW(%3d)  [%4.2f]", reg, f);
        }
        else {
            sprintf(debugMsgBuf, "LW(%3d)  [%d]", reg, val);
        }
    }
    else {
        sprintf(debugMsgBuf, "         [%d]", reg);
    }

    return EVT_CONTINUE;
}