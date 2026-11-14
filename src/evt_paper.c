#include "spm/animdrv.h"
#include "spm/evtmgr_cmd.h"
#include <spm/evt_paper.h>

extern "C" {

s32 evt_paper_entry(EvtEntry * script, bool isFirstCall)
{
    (void) isFirstCall;
    const char * filename = (const char *) (evtGetValue(script, script->pCurData[0]));
    u32 animAsyncResult = animGroupBaseAsync(filename, 0, 0);
    if (animAsyncResult == 0)
    {
        return EVT_RET_BLOCK_WEAK;
    }
    else
    {
        animPaperPoseEntry(filename, 0);
        return EVT_RET_CONTINUE;
    }
}

s32 evt_paper_delete(EvtEntry * script, bool isFirstCall)
{
    (void) isFirstCall;
    const char * filename = (const char *) evtGetValue(script, script->pCurData[0]);
    s32 id = animPaperPoseGetId(filename, 0);
    animPaperPoseRelease(id);
    return EVT_RET_CONTINUE;
}
}
