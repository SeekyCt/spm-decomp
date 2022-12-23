#pragma once

#include <common.h>

CPP_WRAPPER(spm::shadowdrv)

void shadowInit();
void shadowMain();
UNKNOWN_FUNCTION(shadowDisp);
UNKNOWN_FUNCTION(shadowCharShadowInit);
UNKNOWN_FUNCTION(polygonInit);
UNKNOWN_FUNCTION(textureInit);
UNKNOWN_FUNCTION(_filter);
UNKNOWN_FUNCTION(shadowCharShadowDisp_Collision);
UNKNOWN_FUNCTION(shadowCharShadowDisp_Projection);
UNKNOWN_FUNCTION(shadowCharShadowDisp_Polygon);
UNKNOWN_FUNCTION(shadowCharShadowDisp_Texture);
UNKNOWN_FUNCTION(cylinder);
UNKNOWN_FUNCTION(cylinderShadowDraw);
UNKNOWN_FUNCTION(shadowEntry);
UNKNOWN_FUNCTION(shadowEntryMode);
UNKNOWN_FUNCTION(shadowSetType);
UNKNOWN_FUNCTION(func_800375e4);
UNKNOWN_FUNCTION(func_800375fc);
UNKNOWN_FUNCTION(func_8003763c);
UNKNOWN_FUNCTION(func_8003765c);
UNKNOWN_FUNCTION(shadowSetVolumeDecay);
UNKNOWN_FUNCTION(shadowSetSize);
UNKNOWN_FUNCTION(shadowSetCollision);
UNKNOWN_FUNCTION(shadowFlagOff);
UNKNOWN_FUNCTION(shadowGetCharShadowTexObj);
UNKNOWN_FUNCTION(shadowGetCharShadowProjTexMtx);
UNKNOWN_FUNCTION(shadowGetCharShadowColor);
UNKNOWN_FUNCTION(shadowGetCharShadowProjection);
UNKNOWN_FUNCTION(shadowGetRamp8TexObj);
UNKNOWN_FUNCTION(shadowGetRamp16TexObj);
UNKNOWN_FUNCTION(shadowDepthShadowDrawCountUp);
UNKNOWN_FUNCTION(shadowGetDepthShadowTexObj);
UNKNOWN_FUNCTION(shadowGetDepthShadowDepthMtx);
UNKNOWN_FUNCTION(shadowGetDepthShadowProjTexMtx);
UNKNOWN_FUNCTION(shadowGetDepthShadowColor);
UNKNOWN_FUNCTION(shadowGetDepthShadowConfig);
UNKNOWN_FUNCTION(shadowProjShadowDrawCountUp);
UNKNOWN_FUNCTION(shadowGetProjShadowTexObj);
UNKNOWN_FUNCTION(shadowGetProjShadowProjTexMtx);
UNKNOWN_FUNCTION(shadowGetProjShadowColor);
UNKNOWN_FUNCTION(shadowGetProjShadowConfig);
UNKNOWN_FUNCTION(func_800378c8);
UNKNOWN_FUNCTION(projShadowEnd);
UNKNOWN_FUNCTION(depthShadowEnd);

CPP_WRAPPER_END()
