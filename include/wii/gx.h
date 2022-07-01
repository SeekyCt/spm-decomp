/*
    Revolution SDK GX Library
*/

#pragma once

#include <common.h>
#include <wii/types.h>

typedef struct
{
    u8 unknown_0x0[0x3c - 0x0];
} GXRenderModeObj;


// Parameters from libogc's gx.h

void GXSetVtxDesc(u8 attr, u8 type);
void GXClearVtxDesc();
void GXSetVtxAttrFmt(u8 vtxfmt, u32 vtxattr, u32 comptype, u32 compsize, u32 frac);
UNKNOWN_FUNCTION(GXSetTexCoordGen2);
UNKNOWN_FUNCTION(GXSetNumTexGens);
UNKNOWN_FUNCTION(GXSetDrawSync);
UNKNOWN_FUNCTION(GXReadDrawSync);
void GXBegin(u8 primitive, u8 vtxfmt, u16 vtxcnt);
void GXSetCullMode(u8 mode);
void GXSetNumChans(u8 num);
void GXInitTexObjData(void * texObj, void * image);
UNKNOWN_FUNCTION(GXGetTexObjWidth);
UNKNOWN_FUNCTION(GXGetTexObjHeight);
UNKNOWN_FUNCTION(GXLoadTexObj);
UNKNOWN_FUNCTION(GXSetTevColorIn);
UNKNOWN_FUNCTION(GXSetTevAlphaIn);
UNKNOWN_FUNCTION(GXSetTevColorOp);
UNKNOWN_FUNCTION(GXSetTevAlphaOp);
UNKNOWN_FUNCTION(GXSetTevColor);
void GXSetTevSwapMode(u8 tevstage, u8 ras_sel, u8 tex_sel);
UNKNOWN_FUNCTION(GXSetAlphaCompare);
void GXSetTevOrder(u8 tevstage, u8 texcoord, u32 texmap, u8 color);
void GXSetNumTevStages(u8 num);
UNKNOWN_FUNCTION(GXSetFog);
UNKNOWN_FUNCTION(GXSetBlendMode);
void GXSetZMode(u8 enable, u8 func, u8 update_enable);
UNKNOWN_FUNCTION(GXSetZCompLoc);
void GXLoadPosMtxImm(Mtx34 * mtx, u32 pnidx);
void GXSetCurrentMtx(u32 mtx);
UNKNOWN_FUNCTION(GXLoadTexMtxImm);

void GXSetChanCtrl(s32 chan, u8 enable, u8 ambsrc, u8 matsrc, u8 litmask, u8 diff_fn, u8 attn_fn);
void GXSetChanMatColor(s32 channel, RGBA * colour);
void GXSetTevOp(u8 tevstage, u8 mode);

UNKNOWN_FUNCTION(GXSetCopyClear);
UNKNOWN_FUNCTION(GXSetProjection);
