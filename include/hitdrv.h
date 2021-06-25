#ifndef HITDRV_H
#define HITDRV_H

#include <common.h>

void hitGrpFlagOn(bool allowSubname, const char * name, u32 mask); // 8006c4b4
void hitGrpFlagOff(bool allowSubname, const char * name, u32 mask); // 8006c6ac
void hitGetMapEntryBbox(int mapEntryIdx, Vec3 * min, Vec3 * max); // 800703b0
void hitObjGetPos(const char * name, Vec3 * ret); // 8006ffe4
bool hitCheckFilter(float, float, float, float, float, float, void *, float *, float *,
                    float *, float *, float *, float *, float *); // 8006ee2c

#endif
