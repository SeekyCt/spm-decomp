#ifndef MAPDRV_H
#define MAPDRV_H

#include <common.h>

#define MAPOBJ_FLAG_HIDE 1

void mapGrpFlagOn(bool allowSubname, const char * name, u32 mask); // 8008cf30
void mapGrpFlagOff(bool allowSubname, const char * name, u32 mask); // 8008d2fc
void mapGrpFlag4On(bool allowSubname, const char * name, u32 mask); // 8008e11c

void func_8008f2a4(const char *);
void func_8008f324(const char *);

#endif
