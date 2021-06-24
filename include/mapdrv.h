#ifndef MAPDRV_H
#define MAPDRV_H

#include <common.h>

void mapGrpFlagOn(bool allowSubname, const char * name, u32 mask);
void mapGrpFlagOff(bool allowSubname, const char * name, u32 mask);
void mapGrpFlag4On(bool allowSubname, const char * name, u32 mask);

void func_8008f2a4(const char *);
void func_8008f324(const char *);

#endif
