#pragma once

#include <common.h>
#include <spm/camdrv.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <wii/tpl.h>

CPP_WRAPPER(spm::gxsub)

USING(spm::camdrv::CamEntry)
USING(wii::mtx::Mtx34)
USING(wii::gx::GXColor)
USING(wii::tpl::TPLHeader)

void gxsubInit_Tpl(TPLHeader * tpl);
void gxsubDrawTextureMtx(u32 texId, Mtx34 * mtx, const GXColor * colour);
void gxsubDrawTexture(u32 texId, f32 x, f32 y, f32 width, f32 height, const GXColor * colour);
f32 gxsubGetTexWidth(u32 texId);
f32 gxsubGetTexHeight(u32 texId);
void gxsubEnableTexAlpha();
void gxsubInit(); // general use
void gxsubInit_Cam(CamEntry * camera); // gxsubInit + specify active camera
void gxsubInit_Colour(); // sets vertex format to include colours
                         // (incompatible with normal functions)
void gxsubDrawQuad(f32 x, f32 y, f32 width, f32 height, const GXColor * colour);
void gxsubDrawQuadMtx(Mtx34 * mtx, f32 width, f32 height, const GXColor * colour);
void gxsubDrawQuadGradient(f32 x, f32 y, f32 width, f32 height, GXColor * startColour,
                           GXColor * endColour); // requires gxsubInit_Colour
void gxsubDrawLineSquare(f32 x, f32 y, f32 width, f32 height, u32 thickness,
                         const GXColor * colour);
void gxsubDrawLineSquareMtx(Mtx34 * mtx, f32 width, f32 height, u32 thickness,
                            const GXColor * colour);
void gxsubDrawDottedLine(Mtx34 * mtx, f32 width, f32 height, u32 thickness,
                         const GXColor * colour); // requires no init
void gxsubDrawLine(Mtx34 * mtx, f32 width, f32 height, u32 thickness, const GXColor * colour);
void gxsubDrawSphere(Mtx34 * mtx, const GXColor * colour);

CPP_WRAPPER_END()
