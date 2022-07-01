#pragma once

#include <common.h>
#include <spm/camdrv.h>
#include <wii/tpl.h>

void gxsubInit_Tpl(TPLHeader * tpl);
void gxsubDrawTextureMtx(u32 texId, Mtx34 * mtx, RGBA * colour);
void gxsubDrawTexture(u32 texId, f32 x, f32 y, f32 width, f32 height, RGBA * colour);
f32 gxsubGetTexWidth(u32 texId);
f32 gxsubGetTexHeight(u32 texId);
void gxsubEnableTexAlpha();
void gxsubInit();
void gxsubInit_Cam(CamEntry * camera);
void gxsubInit_Colour();
void gxsubDrawQuad(f32 x, f32 y, f32 width, f32 height, RGBA * colour);
void gxsubDrawQuadMtx(Mtx34 * mtx, f32 width, f32 height, RGBA * colour);
void gxsubDrawQuadGradient(f32 x, f32 y, f32 width, f32 height, RGBA * startColour, RGBA * endColour);
void gxsubDrawLineSquare(f32 x, f32 y, f32 width, f32 height, u32 thickness, RGBA * colour);
void gxsubDrawLineSquareMtx(Mtx34 * mtx, f32 width, f32 height, u32 thickness, RGBA * colour);
void gxsubDrawDottedLine(Mtx34 * mtx, f32 width, f32 height, u32 thickness, RGBA * colour);
void gxsubDrawLine(Mtx34 * mtx, f32 width, f32 height, u32 thickness, RGBA * colour);
void gxsubDrawSphere(Mtx34 * mtx, RGBA * colour);
