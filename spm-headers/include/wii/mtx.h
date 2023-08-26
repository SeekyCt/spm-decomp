/*
    Revolution SDK MTX library
*/

#pragma once

#include <common.h>

CPP_WRAPPER(wii::mtx)

#define DEG_TO_RAD(a) (a * 0.01745329238474369f)

typedef struct
{
    f32 x, y;
} Vec2;

typedef struct
{
    s32 x, y;
} Vec2i;

typedef struct
{
    f32 x, y, z;
} Vec3;

typedef struct
{
    s32 x, y, z;
} Vec3i;

typedef f32 Mtx34[3][4];
typedef f32 Mtx44[4][4];

void PSMTXIdentity(Mtx34 * dest);
UNKNOWN_FUNCTION(PSMTXCopy);
UNKNOWN_FUNCTION(PSMTXConcat);
UNKNOWN_FUNCTION(PSMTXInverse);
UNKNOWN_FUNCTION(PSMTXInvXpose);
UNKNOWN_FUNCTION(PSMTXRotRad);
UNKNOWN_FUNCTION(PSMTXRotTrig);
UNKNOWN_FUNCTION(__PSMTXRotAxisRadInternal);
UNKNOWN_FUNCTION(PSMTXRotAxisRad);
void PSMTXTrans(Mtx34 * dest, f32 x, f32 y, f32 z);
void PSMTXTransApply(Mtx34 * src, Mtx34 * dest, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx34 * dest, f32 x, f32 y, f32 z);
void PSMTXScaleApply(Mtx34 * src, Mtx34 * dest, f32 x, f32 y, f32 z);
UNKNOWN_FUNCTION(PSMTXQuat);
UNKNOWN_FUNCTION(C_MTXLookAt);
UNKNOWN_FUNCTION(C_MTXLightFrustum);
UNKNOWN_FUNCTION(C_MTXLightPerspective);
UNKNOWN_FUNCTION(C_MTXLightOrtho);
UNKNOWN_FUNCTION(PSMTXMultVec);
UNKNOWN_FUNCTION(PSMTXMultVecArray);
UNKNOWN_FUNCTION(PSMTXMultVecSR);
UNKNOWN_FUNCTION(C_MTXFrustum);
UNKNOWN_FUNCTION(C_MTXPerspective);
void C_MTXOrtho(Mtx44 * dest, f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far);
UNKNOWN_FUNCTION(PSMTX44Copy);
UNKNOWN_FUNCTION(PSMTX44MultVec);
UNKNOWN_FUNCTION(PSMTX44MultVecArray);
UNKNOWN_FUNCTION(PSVECAdd);
void PSVECSubtract(Vec3 * src1, Vec3 * src2, Vec3 * dest);
void PSVECScale(Vec3 * src, Vec3 * dest, f32 scale);
void PSVECNormalize(Vec3 * src, Vec3 * dest);
f32 PSVecSquareMag(Vec3 * src);
f32 PSVecMag(Vec3 * src);
f32 PSVECDotProduct(Vec3 * src1, Vec3 * src2);
void PSVECCrossProduct(Vec3 * src1, Vec3 * src2, Vec3 * dest);
UNKNOWN_FUNCTION(VECHalfAngle);
UNKNOWN_FUNCTION(VECReflect);
UNKNOWN_FUNCTION(PSVECSquareDistance);
UNKNOWN_FUNCTION(PSVECDistance);
UNKNOWN_FUNCTION(QUATMtx);
UNKNOWN_FUNCTION(QUATSlerp);

CPP_WRAPPER_END()
