#pragma once

#include <common.h>
#include <wii/gx.h>

CPP_WRAPPER(wii::DEMOInit)

extern s32 DemoUseMEMHeap;

void DEMOInit(GXRenderModeObj * renderMode);
UNKNOWN_FUNCTION(DEMOSetRenderMode)
void DEMOBeforeRender();
void DEMODoneRender();
UNKNOWN_FUNCTION(DEMOGetRenderModeObj)

CPP_WRAPPER_END()
