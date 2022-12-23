#pragma once

#include <common.h>

// TODO: probably called statuswindow

CPP_WRAPPER(spm::hud)

void hudInit();
void hudReInit();
UNKNOWN_FUNCTION(hudLoadStats);
void hudMain();
UNKNOWN_FUNCTION(hudGetPos);
UNKNOWN_FUNCTION(hudUnhideAlt);
void hudHide();
UNKNOWN_FUNCTION(func_80199b0c);
UNKNOWN_FUNCTION(func_80199b5c);
UNKNOWN_FUNCTION(hudUnhide);
UNKNOWN_FUNCTION(func_80199c74);
UNKNOWN_FUNCTION(func_80199c88);
UNKNOWN_FUNCTION(func_80199c9c);
UNKNOWN_FUNCTION(func_80199cb0);
typedef void (CountdownDoneCb)();
void hudStartCountdown(u32 length, CountdownDoneCb * cb);
UNKNOWN_FUNCTION(func_80199cf8);
UNKNOWN_FUNCTION(hudUpdateStats);
UNKNOWN_FUNCTION(hudDisp);
void func_8019af88();
void hudTurnOffFlipTimeBox(s32 idx);
UNKNOWN_FUNCTION(func_8019b0dc);
UNKNOWN_FUNCTION(func_8019be84);
UNKNOWN_FUNCTION(func_8019bea8);

CPP_WRAPPER_END()
