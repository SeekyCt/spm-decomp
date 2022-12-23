#pragma once

#include <common.h>

CPP_WRAPPER(spm::lightdrv)

void lightInit();
void lightReInit();
void lightMain();
UNKNOWN_FUNCTION(lightEntry);
UNKNOWN_FUNCTION(lightNameToPtr);
DECOMP_STATIC(UNKNOWN_FUNCTION(lightdrv_sort))
UNKNOWN_FUNCTION(lightGetNearObj);
UNKNOWN_FUNCTION(lightGetAmbient);
UNKNOWN_FUNCTION(lightGetCharaAmbient);
UNKNOWN_FUNCTION(lightCheckCharaLight);
UNKNOWN_FUNCTION(lightGetPaper);

CPP_WRAPPER_END()
