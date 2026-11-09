/*
    WARNING: Not fully decompiled
    This file is currently not linked into the final dol
*/

#include "spm/animdrv.h"
#include "spm/evtmgr_cmd.h"
#include <spm/evt_paper.h>

extern "C" {

s32 evt_paper_entry(EvtEntry* script, bool isFirstCall) {
    const char* filename = (const char*)(evtGetValue(script, script->pCurData[0]));
    u32 animAsyncResult = animGroupBaseAsync(filename, 0, 0);
    if (animAsyncResult == 0) {
        return isFirstCall && false;
    } else {
        animPaperPoseEntry(filename, 0);
        return 2;
    }
}

s32 evt_paper_delete(EvtEntry* script, bool isFirstCall) {
    /*
    r3: arg0Value
    r4: 0
    */
    isFirstCall &= false;
    const char* filename = (const char*)evtGetValue(script, script->pCurData[0]);
    s32 id = animPaperPoseGetId(filename, 0);
    animPaperPoseRelease(id);
    return 2;
}

}
