#include <stdint.h>
#include <stdio.h>

#include "evtmgr_cmd.h"
#include "evtmgr.h"
#include "swdrv.h"
#include "system.h"

static char debugMsgBuf[256];

static float check_float(int val) { // usually inlined
    if (val <= EVTDAT_FLOAT_MAX) {
        return (val + 240000000) / 1024.0f;
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
    int id = *p++;
    int depth = ++entry->dowhileDepth; // missing rlwinm here
    if (depth >= 8) {
        assert(0, "EVTMGR_CMD:While Table Overflow !!");
    }
    entry->dowhileStartPtrs[depth] = p;
    entry->dowhileIds[depth] = id;
    return EVT_CONTINUE;
}

// EVT_CMD_FN(while)

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

// a lot

// I think 1:1? Need to double check + improve quality
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
        unsigned int val = entry->uf[(reg + 210000000) / 32] & (1 << ((reg + 210000000) % 32));
        sprintf(debugMsgBuf, "UF(%3d)  [%d]", reg + 210000000, val);
    }
    else if (reg <= EVTDAT_UW_MAX) {
        int val = entry->uw[reg + 190000000];
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "UW(%3d)  [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f = check_float(val);
            sprintf(debugMsgBuf, "UW(%3d)  [%4.2f]", reg + 190000000, f);
        }
        else {
            sprintf(debugMsgBuf, "UW(%3d)  [%d]", reg + 190000000, val);
        }
    }
    else if (reg <= EVTDAT_GSW_MAX) {
        int val = swByteGet(reg + 170000000);
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "GSW(%3d) [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f = check_float(val);
            sprintf(debugMsgBuf, "GSW(%3d) [%4.2f]", reg + 170000000, f);
        }
        else {
            sprintf(debugMsgBuf, "GSW(%3d) [%d]", reg + 170000000, val);
        }
    }
    else if (reg <= EVTDAT_LSW_MAX) {
        int val = _swByteGet(reg + 150000000);
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "LSW(%3d) [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f = check_float(val);
            sprintf(debugMsgBuf, "LSW(%3d)  [%4.2f]", reg + 150000000, f);
        }
        else {
            sprintf(debugMsgBuf, "LSW(%3d) [%d]", reg + 150000000, val);
        }
    }
    else if (reg <= EVTDAT_GSWF_MAX) {
        sprintf(debugMsgBuf, "GSWF(%3d)[%d]", reg + 130000000, swGet(reg + 130000000));
    }
    else if (reg <= EVTDAT_LSWF_MAX) {
        sprintf(debugMsgBuf, "LSWF(%3d)[%d]", reg + 110000000, _swGet(reg + 110000000));
    }
    else if (reg <= EVTDAT_GF_MAX) {
        unsigned int val = wp->gf[(reg + 90000000) / 32] & (1 << ((reg + 90000000) % 32));
        sprintf(debugMsgBuf, "GF(%3d)  [%d]", reg + 90000000, val);
    }
    else if (reg <= EVTDAT_LF_MAX) {
        unsigned int val = entry->lf[(reg + 70000000) / 32] & (1 << ((reg + 70000000) % 32));
        sprintf(debugMsgBuf, "LF(%3d)  [%d]", reg + 70000000, val);
    }
    else if (reg <= EVTDAT_GW_MAX) {
        int val = wp->gw[reg + 50000000];
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "GW(%3d)  [%08X]", reg + 50000000, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f = check_float(val);
            sprintf(debugMsgBuf, "GW(%3d)  [%4.2f]", reg + 50000000, f);
        }
        else {
            sprintf(debugMsgBuf, "GW(%3d)  [%d]", reg + 50000000, val);
        }
    }
    else if (reg <= EVTDAT_LW_MAX) {
        int val = entry->lw[reg + 30000000];
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "LW(%3d)  [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f = check_float(val);
            sprintf(debugMsgBuf, "LW(%3d)  [%4.2f]", reg + 30000000, f);
        }
        else {
            sprintf(debugMsgBuf, "LW(%3d)  [%d]", reg + 30000000, val);
        }
    }
    else {
        sprintf(debugMsgBuf, "         [%d]", reg);
    }

    return EVT_CONTINUE;
}