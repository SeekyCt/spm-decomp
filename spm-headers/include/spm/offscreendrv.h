#pragma once

#include <common.h>

CPP_WRAPPER(spm::offscreendrv)

void offscreenInit();
UNKNOWN_FUNCTION(offscreenReset);
UNKNOWN_FUNCTION(offscreenEntry);
UNKNOWN_FUNCTION(offscreenDisp);
void offscreenMain();
UNKNOWN_FUNCTION(offscreenAddBoundingBox);
UNKNOWN_FUNCTION(func_800350a0);
UNKNOWN_FUNCTION(func_800350fc);
UNKNOWN_FUNCTION(offscreenNameToId);
UNKNOWN_FUNCTION(func_8003521c);
UNKNOWN_FUNCTION(func_800352b4);
UNKNOWN_FUNCTION(func_800352cc);
UNKNOWN_FUNCTION(func_800353c4);
UNKNOWN_FUNCTION(func_80035478);
UNKNOWN_FUNCTION(func_800354d8);

CPP_WRAPPER_END()
