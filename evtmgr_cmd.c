#include <stdint.h>
#include <stdio.h>

#include "evtmgr_cmd.h"
#include "evtmgr.h"

char debugMsgBuf[256];

EVT_CMD_FN(end_evt) {
    evtDelete(entry);
    return EVT_END;
}

EVT_CMD_FN(lbl) {
    (void) entry;
    return EVT_CONTINUE;
}

// unfinished
EVT_CMD_FN(goto) {
    int x = evtGetValue(entry, entry->pCurData[0]);
    if (x < -290000000) {
        evtGetValue(0, 0);
    }
    return EVT_CONTINUE;
}

// a lot

// unfinished
EVT_CMD_FN(debug_put_reg) {
    int reg = entry->pCurData[0];
    EvtWork * wp = evtGetWork();
    if (reg < -290000000) {
        sprintf(debugMsgBuf, "ADDR     [%08X]", reg);
    }
    else if (reg <= -220000000) {
        float f;
        if (reg <= -220000000) {
            f = (reg + 240000000) / 1024.0f;
        }
        else {
            f = reg;
        }
        sprintf(debugMsgBuf, "FLOAT    [%4.2f]", f);
    }
    else if (reg <= -200000000) {

    }

    return EVT_CONTINUE;
}