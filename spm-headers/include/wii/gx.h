/*
    Revolution SDK GX Library
*/

#pragma once

#include <common.h>
#include <wii/mtx.h>

CPP_WRAPPER(wii::gx)

USING(wii::mtx::Mtx34)
USING(wii::mtx::Mtx44)

typedef struct
{
    u8 r, g, b, a;
} GXColor;

typedef struct
{
/* 0x0 */ u8 unknown_0x0[0x3c - 0x0];
} GXRenderModeObj;
SIZE_ASSERT(GXRenderModeObj, 0x3c)

typedef struct
{
/* 0x0 */ u8 unknown_0x0[0x20 - 0x0];
} GXTexObj;
SIZE_ASSERT(GXTexObj, 0x20)

// Parameters from libogc's gx.h

UNKNOWN_FUNCTION(__GXDefaultTexRegionCallback);
UNKNOWN_FUNCTION(__GXDefaultTlutRegionCallback);
UNKNOWN_FUNCTION(__GXShutdown);
UNKNOWN_FUNCTION(__GXInitRevisionBits);
UNKNOWN_FUNCTION(GXInit);
UNKNOWN_FUNCTION(__GXInitGX);
UNKNOWN_FUNCTION(GXCPInterruptHandler);
UNKNOWN_FUNCTION(GXInitFifoBase);
UNKNOWN_FUNCTION(CPGPLinkCheck);
UNKNOWN_FUNCTION(GXSetCPUFifo);
UNKNOWN_FUNCTION(GXSetGPFifo);
UNKNOWN_FUNCTION(__GXSaveFifo);
UNKNOWN_FUNCTION(__GXIsGPFifoReady);
UNKNOWN_FUNCTION(GXGetCPUFifo);
UNKNOWN_FUNCTION(GXGetFifoCount);
UNKNOWN_FUNCTION(GXGetFifoWrap);
UNKNOWN_FUNCTION(GXSetBreakPtCallback);
UNKNOWN_FUNCTION(__GXFifoInit);
UNKNOWN_FUNCTION(__GXCleanGPFifo);
UNKNOWN_FUNCTION(GXResetOverflowCount);
void GXSetVtxDesc(u8 attr, u8 type);
UNKNOWN_FUNCTION(GXSetVtxDescv);
UNKNOWN_FUNCTION(__GXSetVCD);
UNKNOWN_FUNCTION(__GXCalculateVLim);
UNKNOWN_FUNCTION(GXGetVtxDesc);
UNKNOWN_FUNCTION(GXGetVtxDescv);
void GXClearVtxDesc();
void GXSetVtxAttrFmt(u8 vtxfmt, u32 vtxattr, u32 comptype, u32 compsize, u32 frac);
UNKNOWN_FUNCTION(GXSetVtxAttrFmtv);
UNKNOWN_FUNCTION(__GXSetVAT);
UNKNOWN_FUNCTION(GXGetVtxAttrFmt);
UNKNOWN_FUNCTION(GXGetVtxAttrFmtv);
UNKNOWN_FUNCTION(GXSetArray);
UNKNOWN_FUNCTION(GXInvalidateVtxCache);
UNKNOWN_FUNCTION(GXSetTexCoordGen2);
void GXSetNumTexGens(u32 nr);
UNKNOWN_FUNCTION(GXSetMisc);
UNKNOWN_FUNCTION(GXFlush);
UNKNOWN_FUNCTION(GXResetWriteGatherPipe);
UNKNOWN_FUNCTION(__GXAbort);
UNKNOWN_FUNCTION(GXAbortFrame);
void GXSetDrawSync(u16 token);
u16 GXReadDrawSync();
UNKNOWN_FUNCTION(GXDrawDone);
UNKNOWN_FUNCTION(GXPixModeSync);
UNKNOWN_FUNCTION(GXTexModeSync);
UNKNOWN_FUNCTION(GXPokeAlphaMode);
UNKNOWN_FUNCTION(GXPokeAlphaRead);
UNKNOWN_FUNCTION(GXPokeAlphaUpdate);
UNKNOWN_FUNCTION(GXPokeBlendMode);
UNKNOWN_FUNCTION(GXPokeColorUpdate);
UNKNOWN_FUNCTION(GXPokeDstAlpha);
UNKNOWN_FUNCTION(GXPokeDither);
UNKNOWN_FUNCTION(GXPokeZMode);
UNKNOWN_FUNCTION(GXSetDrawSyncCallback);
UNKNOWN_FUNCTION(GXTokenInterruptHandler);
UNKNOWN_FUNCTION(GXSetDrawDoneCallback);
UNKNOWN_FUNCTION(GXFinishInterruptHandler);
UNKNOWN_FUNCTION(__GXPEInit);
UNKNOWN_FUNCTION(__GXSetDirtyState);
void GXBegin(u8 primitive, u8 vtxfmt, u16 vtxcnt);
UNKNOWN_FUNCTION(__GXSendFlushPrim);
UNKNOWN_FUNCTION(GXSetLineWidth);
UNKNOWN_FUNCTION(GXSetPointSize);
UNKNOWN_FUNCTION(GXEnableTexOffsets);
void GXSetCullMode(u8 mode);
UNKNOWN_FUNCTION(GXGetCullMode);
UNKNOWN_FUNCTION(GXSetCoPlanar);
UNKNOWN_FUNCTION(GXAdjustForOverscan);
UNKNOWN_FUNCTION(GXSetDispCopySrc);
UNKNOWN_FUNCTION(GXSetTexCopySrc);
UNKNOWN_FUNCTION(GXSetDispCopyDst);
UNKNOWN_FUNCTION(GXSetTexCopyDst);
UNKNOWN_FUNCTION(GXSetDispCopyFrame2Field);
UNKNOWN_FUNCTION(GXSetCopyClamp);
UNKNOWN_FUNCTION(GXGetYScaleFactor);
UNKNOWN_FUNCTION(GXSetDispCopyYScale);
UNKNOWN_FUNCTION(GXSetCopyClear);
UNKNOWN_FUNCTION(GXSetCopyFilter);
UNKNOWN_FUNCTION(GXSetDispCopyGamma);
UNKNOWN_FUNCTION(GXCopyDisp);
UNKNOWN_FUNCTION(GXCopyTex);
UNKNOWN_FUNCTION(GXClearBoundingBox);
UNKNOWN_FUNCTION(GXReadBoundingBox);
UNKNOWN_FUNCTION(GXInitLightAttn);
UNKNOWN_FUNCTION(GXInitLightSpot);
UNKNOWN_FUNCTION(GXInitLightDistAttn);
UNKNOWN_FUNCTION(GXInitLightPos);
UNKNOWN_FUNCTION(GXInitLightDir);
UNKNOWN_FUNCTION(GXInitLightColor);
UNKNOWN_FUNCTION(GXLoadLightObjImm);
UNKNOWN_FUNCTION(GXSetChanAmbColor);
void GXSetChanMatColor(s32 channel, const GXColor * colour);
void GXSetNumChans(u8 num);
void GXSetChanCtrl(s32 chan, u8 enable, u8 ambsrc, u8 matsrc, u8 litmask, u8 diff_fn, u8 attn_fn);
UNKNOWN_FUNCTION(GXGetTexBufferSize);
UNKNOWN_FUNCTION(__GetImageTileCount);
UNKNOWN_FUNCTION(GXInitTexObj);
UNKNOWN_FUNCTION(GXInitTexObjCI);
UNKNOWN_FUNCTION(GXInitTexObjLOD);
void GXInitTexObjData(GXTexObj * texObj, void * image);
UNKNOWN_FUNCTION(GXInitTexObjWrapMode);
UNKNOWN_FUNCTION(GXInitTexObjTlut);
UNKNOWN_FUNCTION(GXInitTexObjFilter);
UNKNOWN_FUNCTION(GXGetTexObjWidth);
UNKNOWN_FUNCTION(GXGetTexObjHeight);
UNKNOWN_FUNCTION(GXGetTexObjFmt);
UNKNOWN_FUNCTION(GXGetTexObjMipMap);
UNKNOWN_FUNCTION(GXGetTexObjTlut);
UNKNOWN_FUNCTION(GXLoadTexObjPreLoaded);
UNKNOWN_FUNCTION(GXLoadTexObj);
UNKNOWN_FUNCTION(GXInitTlutObj);
UNKNOWN_FUNCTION(GXLoadTlut);
UNKNOWN_FUNCTION(GXInitTexCacheRegion);
UNKNOWN_FUNCTION(GXInitTexPreLoadRegion);
UNKNOWN_FUNCTION(GXInitTlutRegion);
UNKNOWN_FUNCTION(GXInvalidateTexAll);
UNKNOWN_FUNCTION(GXSetTexRegionCallback);
UNKNOWN_FUNCTION(GXSetTlutRegionCallback);
UNKNOWN_FUNCTION(GXPreLoadEntireTexture);
UNKNOWN_FUNCTION(__SetSURegs);
UNKNOWN_FUNCTION(__GXSetSUTexRegs);
UNKNOWN_FUNCTION(__GXSetTmemConfig);
UNKNOWN_FUNCTION(GXSetTevIndirect);
UNKNOWN_FUNCTION(GXSetIndTexMtx);
UNKNOWN_FUNCTION(GXSetIndTexCoordScale);
UNKNOWN_FUNCTION(GXSetIndTexOrder);
UNKNOWN_FUNCTION(GXSetNumIndStages);
UNKNOWN_FUNCTION(GXSetTevDirect);
UNKNOWN_FUNCTION(GXSetTevIndWarp);
UNKNOWN_FUNCTION(__GXUpdateBPMask);
UNKNOWN_FUNCTION(__GXSetIndirectMask);
UNKNOWN_FUNCTION(__GXFlushTextureState);
void GXSetTevOp(u8 tevstage, u8 mode);
UNKNOWN_FUNCTION(GXSetTevColorIn);
UNKNOWN_FUNCTION(GXSetTevAlphaIn);
UNKNOWN_FUNCTION(GXSetTevColorOp);
UNKNOWN_FUNCTION(GXSetTevAlphaOp);
UNKNOWN_FUNCTION(GXSetTevColor);
UNKNOWN_FUNCTION(GXSetTevColorS10);
UNKNOWN_FUNCTION(GXSetTevKColor);
UNKNOWN_FUNCTION(GXSetTevKColorSel);
UNKNOWN_FUNCTION(GXSetTevKAlphaSel);
void GXSetTevSwapMode(u8 tevstage, u8 ras_sel, u8 tex_sel);
UNKNOWN_FUNCTION(GXSetTevSwapModeTable);
void GXSetAlphaCompare(u8 comp0, u8 ref0, u8 aop, u8 comp1, u8 ref1);
UNKNOWN_FUNCTION(GXSetZTexture);
void GXSetTevOrder(u8 tevstage, u8 texcoord, u32 texmap, u8 color);
void GXSetNumTevStages(u8 num);
void GXSetFog(u8 type, f32 startz, f32 endz, f32 nearz, f32 farz, const GXColor * col);
UNKNOWN_FUNCTION(GXSetFogRangeAdj);
void GXSetBlendMode(u8 type, u8 src_fact, u8 dst_fact, u8 op);
UNKNOWN_FUNCTION(GXSetColorUpdate);
UNKNOWN_FUNCTION(GXSetAlphaUpdate);
void GXSetZMode(u8 enable, u8 func, u8 update_enable);
void GXSetZCompLoc(u8 before_tex);
UNKNOWN_FUNCTION(GXSetPixelFmt);
UNKNOWN_FUNCTION(GXSetDither);
UNKNOWN_FUNCTION(GXSetDstAlpha);
UNKNOWN_FUNCTION(GXSetFieldMask);
UNKNOWN_FUNCTION(GXSetFieldMode);
UNKNOWN_FUNCTION(GXDrawSphere);
UNKNOWN_FUNCTION(GXDrawCubeFace);
UNKNOWN_FUNCTION(GXDrawCube);
UNKNOWN_FUNCTION(GXBeginDisplayList);
UNKNOWN_FUNCTION(GXEndDisplayList);
UNKNOWN_FUNCTION(GXCallDisplayList);
UNKNOWN_FUNCTION(GXProject);
UNKNOWN_FUNCTION(__GXSetProjection);
void GXSetProjection(Mtx44 * mtx, u32 type);
UNKNOWN_FUNCTION(GXSetProjectionv);
UNKNOWN_FUNCTION(GXGetProjectionv);
void GXLoadPosMtxImm(Mtx34 * mtx, u32 pnidx);
UNKNOWN_FUNCTION(GXLoadNrmMtxImm);
void GXSetCurrentMtx(u32 mtx);
UNKNOWN_FUNCTION(GXLoadTexMtxImm);
UNKNOWN_FUNCTION(__GXSetViewport);
UNKNOWN_FUNCTION(GXSetViewportJitter);
UNKNOWN_FUNCTION(GXSetViewport);
UNKNOWN_FUNCTION(GXGetViewportv);
UNKNOWN_FUNCTION(GXSetZScaleOffset);
UNKNOWN_FUNCTION(GXSetScissor);
UNKNOWN_FUNCTION(GXGetScissor);
UNKNOWN_FUNCTION(GXSetScissorBoxOffset);
UNKNOWN_FUNCTION(GXSetClipMode);
UNKNOWN_FUNCTION(__GXSetMatrixIndex);
UNKNOWN_FUNCTION(GXSetGPMetric);
UNKNOWN_FUNCTION(GXClearGPMetric);

inline void GXEnd() { }

CPP_WRAPPER_END()
