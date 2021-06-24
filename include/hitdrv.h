#ifndef HITDRV_H
#define HITDRV_H

#include <common.h>

void hitGrpFlagOn(bool allowSubname, const char * name, u32 mask);
void hitGrpFlagOff(bool allowSubname, const char * name, u32 mask);
void hitGetMapEntryBbox(int mapEntryIdx, Vec3 * min, Vec3 * max);
void hitObjGetPos(const char * name, Vec3 * ret);
bool hitCheckFilter(float, float, float, float, float, float, void *, float *, float *, float *, float *, float *, float *, float *);

#endif
