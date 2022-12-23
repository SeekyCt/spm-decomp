#pragma once

#include <common.h>

CPP_WRAPPER(spm::bgdrv)

void bgInit();
void bgMain();
UNKNOWN_FUNCTION(func_8004e10c);
UNKNOWN_FUNCTION(bgLoadTpl);
UNKNOWN_FUNCTION(func_8004e608);
UNKNOWN_FUNCTION(bgSetColor);
UNKNOWN_FUNCTION(func_8004e738);
UNKNOWN_FUNCTION(bgFlagOn);
UNKNOWN_FUNCTION(bgFlagOff);
UNKNOWN_FUNCTION(bgFlag8On);
UNKNOWN_FUNCTION(bgFlag8Off);
UNKNOWN_FUNCTION(bgMain);
UNKNOWN_FUNCTION(func_8004ec64);
UNKNOWN_FUNCTION(func_8004f53c);
void func_8004f5c8(const char *);
void func_8004f5f0(const char *);

CPP_WRAPPER_END()
