#pragma once

#include <common.h>

CPP_WRAPPER(spm::extdrv)

void extInit();
UNKNOWN_FUNCTION(extInit);
UNKNOWN_FUNCTION(extEntry);
UNKNOWN_FUNCTION(extMakeTexture);
UNKNOWN_FUNCTION(extReset);
void extMain();
UNKNOWN_FUNCTION(compare);
UNKNOWN_FUNCTION(extGetPosePtr);
UNKNOWN_FUNCTION(func_80065848);
UNKNOWN_FUNCTION(extDraw);
UNKNOWN_FUNCTION(extLoadRenderMode);
UNKNOWN_FUNCTION(extLoadVertex);
UNKNOWN_FUNCTION(extLoadTexture);
UNKNOWN_FUNCTION(extLoadTextureExit);
UNKNOWN_FUNCTION(extLoadTev);

CPP_WRAPPER_END()
