#pragma once

#include <common.h>

// TODO: probably called statuswindow

CPP_WRAPPER(spm::hud)

typedef struct
{
/* 0x000 */ u32 flags;
/* 0x004 */ u8 unknown_0x004[0x178 - 0x004];
/* 0x178 */ s32 countdownTimer;
/* 0x17C */ void * countdownCallback;
/* 0x180 */ u8 unknown_0x180[0x190 - 0x180];
} HudWork;
SIZE_ASSERT(HudWork, 0x190)

DECOMP_STATIC(HudWork * hud_wp)

void hudInit();
void hudReInit();
UNKNOWN_FUNCTION(hudLoadStats)
void hudMain();
UNKNOWN_FUNCTION(hudGetPos)
UNKNOWN_FUNCTION(hudUnhideAlt)
void hudHide();
UNKNOWN_FUNCTION(func_80199b0c)
UNKNOWN_FUNCTION(func_80199b5c)
UNKNOWN_FUNCTION(hudUnhide)
UNKNOWN_FUNCTION(func_80199c74)
UNKNOWN_FUNCTION(func_80199c88)
UNKNOWN_FUNCTION(func_80199c9c)
UNKNOWN_FUNCTION(func_80199cb0)
typedef void (CountdownDoneCb)();
void hudStartCountdown(u32 length, CountdownDoneCb * cb);
UNKNOWN_FUNCTION(func_80199cf8)
UNKNOWN_FUNCTION(hudUpdateStats)
UNKNOWN_FUNCTION(hudDisp)
void func_8019af88();
void hudTurnOffFlipTimeBox(s32 idx);
UNKNOWN_FUNCTION(func_8019b0dc)
void func_8019be84();
UNKNOWN_FUNCTION(func_8019bea8)

CPP_WRAPPER_END()
