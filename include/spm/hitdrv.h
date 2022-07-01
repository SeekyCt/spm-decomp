#pragma once

#include <common.h>
#include <wii/types.h>

#define HITOBJ_FLAG_DISABLE 1


void hitInit();
void hitMain();
void hitRecalcMatrices();

void hitGrpFlagOn(bool allowSubname, const char * name, u32 mask);
void hitGrpFlagOff(bool allowSubname, const char * name, u32 mask);
void hitGetMapEntryBbox(s32 mapEntryIdx, Vec3 * min, Vec3 * max);
void hitObjGetPos(const char * name, Vec3 * ret);
bool hitCheckFilter(float, float, float, float, float, float, void *, float *, float *,
                    float *, float *, float *, float *, float *);
