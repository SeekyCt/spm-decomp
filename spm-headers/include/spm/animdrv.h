#pragma once

#include <common.h>
#include <wii/os.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::animdrv)

USING(wii::os::OSTime)
USING(wii::mtx::Mtx34)

typedef void (AnimPoseDisplayCb)(void * param, s32 animGroupIdx, s32 param_3);

typedef struct 
{
/* 0x00 */ char name[60];
/* 0x3C */ void * data;
} AnimationModelFileAnimTableEntry;
SIZE_ASSERT(AnimationModelFileAnimTableEntry, 0x40)

typedef struct 
{
/* 0x000 */ u8 unknown_0x0[0x148 - 0x0];
/* 0x148 */ u32 animCount;
/* 0x14C */ u8 unknown_0x14c[0x1ac - 0x14c];
/* 0x1AC */ AnimationModelFileAnimTableEntry * anims;
} AnimationModelFileHeader;
SIZE_ASSERT(AnimationModelFileHeader, 0x1b0)

typedef struct
{
/* 0x000 */ u8 unknown_0x0[0x110 - 0x0];
} AnimWork; // Uncertain size;

AnimWork * animGetPtr();
OSTime animTimeGetTime();
DECOMP_STATIC(void * animdrv_testAlloc(u32 size))
void animInit();
void animMain();
UNKNOWN_FUNCTION(textureGroupEntry);
UNKNOWN_FUNCTION(animGroupEntry);
UNKNOWN_FUNCTION(animPoseRefresh);
s32 animPoseEntry(const char * filename, s32 releaseType);
UNKNOWN_FUNCTION(animPaperPoseEntry);
UNKNOWN_FUNCTION(func_80042ec8);
UNKNOWN_FUNCTION(animPosePeraOff);
UNKNOWN_FUNCTION(animPoseSetLocalTimeRate);
UNKNOWN_FUNCTION(animPoseSetLocalTime);
UNKNOWN_FUNCTION(animPoseSetStartTime);
void animPoseSetAnim(s32 id, const char * animName, bool forceReset);
UNKNOWN_FUNCTION(animPaperPoseGetId);
UNKNOWN_FUNCTION(animPoseSetPaperAnimGroup);
UNKNOWN_FUNCTION(animPoseSetPaperAnim);
UNKNOWN_FUNCTION(func_80043b90);
UNKNOWN_FUNCTION(func_80043ca4);
UNKNOWN_FUNCTION(animPoseSetEffect);
UNKNOWN_FUNCTION(animPoseSetEffectAnim);
UNKNOWN_FUNCTION(animPoseSetGXFunc);
UNKNOWN_FUNCTION(animPoseGetLoopTimes);
UNKNOWN_FUNCTION(animPoseSetFlagF0On);
UNKNOWN_FUNCTION(animPoseSetFlagF0Off);
UNKNOWN_FUNCTION(animPoseSetFlagF4On);
UNKNOWN_FUNCTION(animPoseSetFlagF4Off);
UNKNOWN_FUNCTION(animPoseSetMaterialLightFlagOn);
UNKNOWN_FUNCTION(animPoseSetMaterialLightFlagOff);
UNKNOWN_FUNCTION(animPoseSetMaterialEvtColor);
UNKNOWN_FUNCTION(animPoseSetMaterialAnmColor);
UNKNOWN_FUNCTION(animPoseGetFlagF0);
UNKNOWN_FUNCTION(animPoseGetFlagF4);
UNKNOWN_FUNCTION(animPoseGetMaterialEvtColor);
UNKNOWN_FUNCTION(animPoseSetDispCallBack2);
UNKNOWN_FUNCTION(func_800451c4);
void animPoseMain(s32 id);
UNKNOWN_FUNCTION(pushGXModelMtx_TransformNode__);
UNKNOWN_FUNCTION(pushGXModelMtx_JointNode__);
UNKNOWN_FUNCTION(func_80047170);
UNKNOWN_FUNCTION(animSetMaterial_Texture);
UNKNOWN_FUNCTION(animSetMaterial_ChangeTexture);
UNKNOWN_FUNCTION(materialProc);
UNKNOWN_FUNCTION(renderProc);
UNKNOWN_FUNCTION(dispProc);
UNKNOWN_FUNCTION(animPoseDraw);
UNKNOWN_FUNCTION(_animPoseDrawMtx);
void animPoseDrawMtx(s32 id, Mtx34 * mtx, s32 xluStage, f32 rotY, f32 scale);
UNKNOWN_FUNCTION(animSetPaperTexObj);
s32 animPoseRelease(s32 id);
UNKNOWN_FUNCTION(animPaperPoseRelease);
void animPoseAutoRelease(s32 releaseType);
UNKNOWN_FUNCTION(animPaperPoseDisp);
UNKNOWN_FUNCTION(animPaperPoseDispSub);
UNKNOWN_FUNCTION(animPoseDisp_MakeExtTexture);
UNKNOWN_FUNCTION(animSetPaperTexMtx);
UNKNOWN_FUNCTION(animGroupBaseAsync);
UNKNOWN_FUNCTION(animPoseGetAnimPosePtr);
UNKNOWN_FUNCTION(animPoseGetAnimDataPtr);
AnimationModelFileHeader * animPoseGetAnimBaseDataPtr(s32 id);
UNKNOWN_FUNCTION(animPoseGetCurrentAnim);
UNKNOWN_FUNCTION(func_8004ca08);
UNKNOWN_FUNCTION(func_8004caec);
UNKNOWN_FUNCTION(func_8004cd30);
UNKNOWN_FUNCTION(func_8004cfa8);
UNKNOWN_FUNCTION(evalProc);
UNKNOWN_FUNCTION(animPoseWorldPositionEvalOn);
UNKNOWN_FUNCTION(animPoseWorldMatrixEvalOn);
void animPoseSetDispCallback(s32 animPoseId, AnimPoseDisplayCb * cb, void * param);
UNKNOWN_FUNCTION(animPoseDrawShape);
UNKNOWN_FUNCTION(func_8004d96c);
UNKNOWN_FUNCTION(animPoseGetShapeIdx);
s32 animPoseGetGroupIdx(s32 id, const char * name);
UNKNOWN_FUNCTION(animPoseGetGroupIdxSubname);

CPP_WRAPPER_END()
