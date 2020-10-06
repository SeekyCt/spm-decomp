#include <stdint.h>
#include <stdio.h>

#include "evtmgr_cmd.h"
#include "evtmgr.h"
#include "system.h"

static char debugMsgBuf[256];

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
    if (lbl < EVTDAT_ADDR_MAX) {
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
            assertf(0, "EVTMGR_CMD:Jump Table Search error !!\n [lbl=%d, n=%d]", lbl, n)
        }
        dest = r31;
    }
    entry->pCurInstruction = dest;
    return EVT_CONTINUE;
}

// not matching
EVT_CMD_FN(do) {
    int depth = entry->dowhileDepth + 1;
    entry->dowhileDepth = (int8_t) depth;
    int * p = entry->pCurData;
    int id = *p++;

    if (depth >= 8) {
        assert(0, "EVTMGR_CMD:While Table Overflow !!")
    }
    entry->dowhileStartPtrs[depth] = p;
    entry->dowhileIds[depth] = id;
    return EVT_CONTINUE;
}

// EVT_CMD_FN(while)

EVT_CMD_FN(do_break) {
    if (entry->dowhileDepth < 0) {
        assert(0, "EVTMGR_CMD:While Table Underflow !!")
    }
    entry->pCurInstruction = evtSearchWhile(entry);
    entry->dowhileDepth -= 1;
    return EVT_CONTINUE;
}

EVT_CMD_FN(do_continue) {
    if (entry->dowhileDepth < 0) {
        assert(0, "EVTMGR_CMD:While Table Underflow !!")
    }
    entry->pCurInstruction = evtSearchJustBeforeWhile(entry);
    return EVT_CONTINUE;
}

// a lot

// unfinished
EVT_CMD_FN(debug_put_reg) {
    int reg = entry->pCurData[0];
    EvtWork * wp = evtGetWork();
    if (reg <= EVTDAT_ADDR_MAX) {
        sprintf(debugMsgBuf, "ADDR     [%08X]", reg);
    }
    else if (reg <= EVTDAT_FLOAT_MAX) {
        float f;
        if (reg <= EVTDAT_FLOAT1024_MAX) {
            f = (reg + 240000000) / 1024.0f;
        }
        else {
            f = reg;
        }
        sprintf(debugMsgBuf, "FLOAT    [%4.2f]", f);
    }
    else if (reg <= EVTDAT_UF_MAX) {
        int n = reg + 210000000;
        unsigned int val = entry->uf[n / 32] & (1 << (n % 32));
        sprintf(debugMsgBuf, "UF(%3d)  [%d]", reg + 210000000, val);
    }
    else if (reg <= EVTDAT_UW_MAX) {
        int val = entry->uw[reg + 190000000];
        if (val <= EVTDAT_ADDR_MAX) {
            sprintf(debugMsgBuf, "UW(%3d)  [%08X]", val, val);
        }
        else if (val <= EVTDAT_FLOAT_MAX) {
            float f;
            if (val <= EVTDAT_FLOAT1024_MAX) {
                f = (val + 240000000) / 1024.0f;
            }
            else {
                f = val;
            }
            sprintf(debugMsgBuf, "UW(%3d)  [%4.2f]", reg + 190000000, f);
        }
        else {
            sprintf(debugMsgBuf, "UW(%3d)  [%d]", reg + 190000000, val);
        }
    }

    return EVT_CONTINUE;
}