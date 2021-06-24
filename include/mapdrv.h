#ifndef MAPDRV_H
#define MAPDRV_H

#include <common.h>

void mapGrpFlagOn(bool allowSubname, const char * name, u32 mask);
void mapGrpFlagOff(bool allowSubname, const char * name, u32 mask);
void mapGrpFlag4On(bool allowSubname, const char * name, u32 mask);

#endif
