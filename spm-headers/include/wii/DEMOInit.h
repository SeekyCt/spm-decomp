#pragma once

#include <common.h>

CPP_WRAPPER(wii::DEMOInit)

extern s32 DemoUseMEMHeap;

void DEMOInit();
UNKNOWN_FUNCTION(DEMOSetRenderMode);
void DEMOBeforeRender();
void DEMODoneRender();
UNKNOWN_FUNCTION(DEMOGetRenderModeObj);

CPP_WRAPPER_END()
