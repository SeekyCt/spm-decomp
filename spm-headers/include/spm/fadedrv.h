#pragma once

#include <common.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::fadedrv)

USING(wii::gx::GXColor)

void fadeInit();
void fadeEntry(u32 transitionType, s32 param_2, GXColor colour);
UNKNOWN_FUNCTION(func_80066558);
void fadeMain();
UNKNOWN_FUNCTION(func_80066e4c);
UNKNOWN_FUNCTION(func_80067588);
bool fadeIsFinish();
UNKNOWN_FUNCTION(func_80067824);
UNKNOWN_FUNCTION(func_8006783c);
UNKNOWN_FUNCTION(func_800678d0);
UNKNOWN_FUNCTION(func_800678dc);

/*
    Sets the transition to be used on the next map change
    Doesn't affect anything internally in fadedrv, it's just stored with a getter
    for callers (in map change and minigames) of fadeEntry to use
*/
void fadeSetMapChangeTransition(s32 in, s32 out);

UNKNOWN_FUNCTION(func_800678fc);
UNKNOWN_FUNCTION(func_80067908);
UNKNOWN_FUNCTION(func_80067914);
UNKNOWN_FUNCTION(func_80067928);
UNKNOWN_FUNCTION(func_800679d0);
UNKNOWN_FUNCTION(func_800679d8);

CPP_WRAPPER_END()
