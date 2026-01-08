#pragma once

#include <common.h>
#include <wii/os.h>
#include <wii/mtx.h>
#include <wii/gx.h>
#include <spm/filemgr.h>

CPP_WRAPPER(spm::animdrv)

USING(wii::os::OSTime)
USING(wii::mtx::Mtx34)
USING(wii::gx::GXColor)
USING(spm::filemgr::FileEntry)

typedef void (AnimPoseDisplayCb)(void * param, s32 animGroupIdx, s32 param_3);

typedef struct
{
/* 0x00 */ u32 dataSize;
/* 0x04 */ u32 loopCount;
/* 0x08 */ u32 keyframeCount;
/* 0x0c */ u32 vertexPositionDeltaCount;
/* 0x10 */ u32 vertexNormalDeltaCount;
/* 0x14 */ u32 textureCoordinateTransformDeltaCount;
/* 0x18 */ u32 visibilityGroupDeltaCount;
/* 0x1c */ u32 groupTransformDeltaCount;
/* 0x20 */ u32 animDataType8Count;
/* 0x24 */ void * loopData;
/* 0x28 */ void * keyframes;
/* 0x2c */ void * vertexPositionDeltas;
/* 0x30 */ void * vertexNormalDeltras;
/* 0x34 */ void * textureCoordinateTransformDeltas;
/* 0x38 */ void * visibilityGroupDeltas;
/* 0x3c */ void * groupTransformDataDeltas;
/* 0x40 */ void * animDataType8Data;
/* 0x44 */ u8 unknown_0x44[0x5c - 0x44];
} AnimPoseData_AnimData;
SIZE_ASSERT(AnimPoseData_AnimData, 0x5c)

typedef struct 
{
/* 0x00 */ char name[60];
/* 0x3C */ AnimPoseData_AnimData * data;
} AnimPoseData_AnimTableEntry;
SIZE_ASSERT(AnimPoseData_AnimTableEntry, 0x40)

/*
    Header of an AnimPose file from the /a/ folder
*/
typedef struct 
{
/* 0x000 */ u32 headerSize;
/* 0x004 */ char modelName[64];
/* 0x044 */ char textureName[64];
/* 0x084 */ char buildTime[64];
/* 0x0C4 */ u32 flags;
/* 0x0C8 */ u32 radius;
/* 0x0CC */ u32 height;
/* 0x0D0 */ float bboxMin[3];
/* 0x0DC */ float bboxMax[3];
/* 0x0E8 */ u32 shapeCount;
/* 0x0EC */ u32 polygonCount;
/* 0x0F0 */ u32 vertexPositionCount;
/* 0x0F4 */ u32 vertexPositionIndexCount;
/* 0x0F8 */ u32 vertexNormalCount;
/* 0x0FC */ u32 vertexNormalIndexCount;
/* 0x100 */ u32 vertexColorCount;
/* 0x104 */ u32 vertexColorIndexCount;
/* 0x108 */ u32 vertexTextureCoordinate0IndexCount;
/* 0x10C */ u32 vertexTextureCoordinate1IndexCount;
/* 0x110 */ u32 vertexTextureCoordinate2IndexCount;
/* 0x114 */ u32 vertexTextureCoordinate3IndexCount;
/* 0x118 */ u32 vertexTextureCoordinate4IndexCount;
/* 0x11C */ u32 vertexTextureCoordinate5IndexCount;
/* 0x120 */ u32 vertexTextureCoordinate6IndexCount;
/* 0x124 */ u32 vertexTextureCoordinate7IndexCount;
/* 0x128 */ u32 vertexTextureCoordinateCount;
/* 0x12C */ u32 textureCoordinateTransformCount;
/* 0x130 */ u32 samplerCount;
/* 0x134 */ u32 textureCount;
/* 0x138 */ u32 subshapeCount;
/* 0x13C */ u32 visibilityGroupCount;
/* 0x140 */ u32 groupTransformDataCount;
/* 0x144 */ u32 groupCount;
/* 0x148 */ s32 animCount;
/* 0x14C */ void * shapes;
/* 0x150 */ void * polygons;
/* 0x154 */ void * vertexPositions;
/* 0x158 */ void * vertexPositionIndices;
/* 0x15C */ void * vertexNormals;
/* 0x160 */ void * vertexNormalIndices;
/* 0x164 */ void * vertexColors;
/* 0x168 */ void * vertexColorIndices;
/* 0x16C */ void * vertexTextureCoordinate0Indices;
/* 0x170 */ void * vertexTextureCoordinate1Indices;
/* 0x174 */ void * vertexTextureCoordinate2Indices;
/* 0x178 */ void * vertexTextureCoordinate3Indices;
/* 0x17C */ void * vertexTextureCoordinate4Indices;
/* 0x180 */ void * vertexTextureCoordinate5Indices;
/* 0x184 */ void * vertexTextureCoordinate6Indices;
/* 0x188 */ void * vertexTextureCoordinate7Indices;
/* 0x18C */ void * vertexTextureCoordinates;
/* 0x190 */ void * textureCoordinateTransforms;
/* 0x194 */ void * samplers;
/* 0x198 */ void * textures;
/* 0x19C */ void * subshapes;
/* 0x1A0 */ void * visibilityGroups;
/* 0x1A4 */ void * groupTransformData;
/* 0x1A8 */ void * groups;
/* 0x1AC */ AnimPoseData_AnimTableEntry * anims;
} AnimPoseData;
SIZE_ASSERT(AnimPoseData, 0x1b0)

typedef struct
{
/* 0x00 */ s32 inUse;
/* 0x04 */ s32 refCnt;
/* 0x08 */ FileEntry * file;
} TextureGroup;
SIZE_ASSERT(TextureGroup, 0xc)

typedef struct
{
/* 0x00 */ s32 inUse;
/* 0x04 */ s32 refCnt;
/* 0x08 */ FileEntry * file;
/* 0x0C */ s32 textureGroupId;
} AnimGroup;
SIZE_ASSERT(AnimGroup, 0x10)

typedef struct
{
/* 0x000 */ AnimGroup * animGroups;
/* 0x004 */ s32 animGroupNum;
/* 0x008 */ TextureGroup * textureGroups;
/* 0x00C */ s32 textureGroupNum;
/* 0x008 */ u8 unknown_0x8[0x110 - 0x010];
} AnimWork; // Uncertain size
SIZE_ASSERT(AnimWork, 0x110)

DECOMP_STATIC(AnimWork * animdrv_wp)

typedef struct 
{
/* 0x000 */ u32 flag;
/* 0x004 */ s32 paperPoseFlag; // typeFlags
/* 0x008 */ Unk paperPoseSharedCnt; // refCount
/* 0x00C */ s32 releaseType;
/* 0x010 */ s32 animGroupId;
/* 0x014 */ Unk curAnimIndex;
/* 0x018 */ u8 unknown_0x18[0x020 - 0x018];
/* 0x020 */ f32 frmCounter; // unconfirmed
/* 0x024 */ u8 unknown_0x24[0x188 - 0x024];
} AnimPose;
SIZE_ASSERT(AnimPose, 0x188)

AnimWork * animGetPtr();
OSTime animTimeGetTime();
DECOMP_STATIC(void * animdrv_testAlloc(u32 size))
void animInit();
void animMain();
UNKNOWN_FUNCTION(textureGroupEntry)
UNKNOWN_FUNCTION(animGroupEntry)
UNKNOWN_FUNCTION(animPoseRefresh)
s32 animPoseEntry(const char * filename, s32 releaseType);
UNKNOWN_FUNCTION(animPaperPoseEntry)
UNKNOWN_FUNCTION(func_80042ec8)
UNKNOWN_FUNCTION(animPosePeraOff)
UNKNOWN_FUNCTION(animPoseSetLocalTimeRate)
UNKNOWN_FUNCTION(animPoseSetLocalTime)
UNKNOWN_FUNCTION(animPoseSetStartTime)
void animPoseSetAnim(s32 id, const char * animName, bool forceReset);
UNKNOWN_FUNCTION(animPaperPoseGetId)
UNKNOWN_FUNCTION(animPoseSetPaperAnimGroup)
UNKNOWN_FUNCTION(animPoseSetPaperAnim)
UNKNOWN_FUNCTION(func_80043b90)
UNKNOWN_FUNCTION(func_80043ca4)
UNKNOWN_FUNCTION(animPoseSetEffect)
UNKNOWN_FUNCTION(animPoseSetEffectAnim)
UNKNOWN_FUNCTION(animPoseSetGXFunc)
f32 animPoseGetLoopTimes(s32 animPoseId);
UNKNOWN_FUNCTION(animPoseSetFlagF0On)
UNKNOWN_FUNCTION(animPoseSetFlagF0Off)
UNKNOWN_FUNCTION(animPoseSetFlagF4On)
UNKNOWN_FUNCTION(animPoseSetFlagF4Off)
UNKNOWN_FUNCTION(animPoseSetMaterialLightFlagOn)
UNKNOWN_FUNCTION(animPoseSetMaterialLightFlagOff)

void animPoseSetMaterialFlagOn(s32 animPoseId, u32 flag);

void animPoseSetMaterialFlagOff(s32 animPoseId, u32 flag);

UNKNOWN_FUNCTION(animPoseSetMaterialAnmColor)

GXColor animPoseGetMaterialEvtColor(s32 animPoseId);

u32 animPoseGetMaterialFlag(s32 animPoseId);

UNKNOWN_FUNCTION(animPoseGetFlagF4)

void animPoseSetMaterialEvtColor(s32 animPoseId, GXColor color);

void animPoseSetDispCallback2(s32 id, void * func, void * evt);

UNKNOWN_FUNCTION(func_800451c4)
void animPoseMain(s32 id);
UNKNOWN_FUNCTION(pushGXModelMtx_TransformNode__)
UNKNOWN_FUNCTION(pushGXModelMtx_JointNode__)
UNKNOWN_FUNCTION(func_80047170)
UNKNOWN_FUNCTION(animSetMaterial_Texture)
UNKNOWN_FUNCTION(animSetMaterial_ChangeTexture)
UNKNOWN_FUNCTION(materialProc)
UNKNOWN_FUNCTION(renderProc)
UNKNOWN_FUNCTION(dispProc)
UNKNOWN_FUNCTION(animPoseDraw)
UNKNOWN_FUNCTION(_animPoseDrawMtx)
void animPoseDrawMtx(s32 id, Mtx34 mtx, s32 xluStage, f32 rotY, f32 scale);
UNKNOWN_FUNCTION(animSetPaperTexObj)
s32 animPoseRelease(s32 id);
UNKNOWN_FUNCTION(animPaperPoseRelease)
void animPoseAutoRelease(s32 releaseType);
UNKNOWN_FUNCTION(animPaperPoseDisp)
UNKNOWN_FUNCTION(animPaperPoseDispSub)
UNKNOWN_FUNCTION(animPoseDisp_MakeExtTexture)
UNKNOWN_FUNCTION(animSetPaperTexMtx)
u32 animGroupBaseAsync(const char * animPoseName, s32 param_2, void * readDoneCb);
AnimPose * animPoseGetAnimPosePtr(s32 animPoseId);
UNKNOWN_FUNCTION(animPoseGetAnimDataPtr)
AnimPoseData * animPoseGetAnimBaseDataPtr(s32 id);
UNKNOWN_FUNCTION(animPoseGetCurrentAnim)
UNKNOWN_FUNCTION(func_8004ca08)
UNKNOWN_FUNCTION(func_8004caec)
UNKNOWN_FUNCTION(func_8004cd30)
UNKNOWN_FUNCTION(func_8004cfa8)
UNKNOWN_FUNCTION(evalProc)
UNKNOWN_FUNCTION(animPoseWorldPositionEvalOn)
UNKNOWN_FUNCTION(animPoseWorldMatrixEvalOn)
void animPoseSetDispCallback(s32 animPoseId, AnimPoseDisplayCb * cb, void * param);
UNKNOWN_FUNCTION(animPoseDrawShape)
UNKNOWN_FUNCTION(func_8004d96c)
UNKNOWN_FUNCTION(animPoseGetShapeIdx)
s32 animPoseGetGroupIdx(s32 id, const char * name);
UNKNOWN_FUNCTION(animPoseGetGroupIdxSubname)

CPP_WRAPPER_END()
