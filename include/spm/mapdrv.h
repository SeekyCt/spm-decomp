#pragma once

#include <common.h>

#define MAPOBJ_FLAG_HIDE 1

extern BOOL __mapdrv_make_dl;

void mapInit();
void mapMain();
void mapDisp();

void mapGrpFlagOn(bool allowSubname, const char * name, u32 mask);
void mapGrpFlagOff(bool allowSubname, const char * name, u32 mask);
void mapGrpFlag4On(bool allowSubname, const char * name, u32 mask);

void func_8008f2a4(const char *);
void func_8008f324(const char *);
