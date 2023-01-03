#pragma once

#include <common.h>

CPP_WRAPPER(spm::homebuttondrv)

#define HOMEBUTTON_FLAG_OPEN 0x10
#define HOMEBUTTON_FLAG_CONTROLLER_DC 0x20
#define HOMEBUTTON_FLAG_FORBIDDEN 0x40

typedef struct
{
/* 0x00 */ s32 state;
/* 0x04 */ u16 flags;
/* 0x06 */ u8 unknown_0x6[0xa0 - 0x6];
} HomeButtonWork;
SIZE_ASSERT(HomeButtonWork, 0xa0)

DECOMP_STATIC(HomeButtonWork * homebuttondrv_wp)

void homebuttonInit();
UNKNOWN_FUNCTION(func_802418c8);
bool homebuttonMain();
bool func_80242198();
UNKNOWN_FUNCTION(homebuttonAllowOpen);
UNKNOWN_FUNCTION(homebuttonForbidOpen);
UNKNOWN_FUNCTION(func_802421dc);
UNKNOWN_FUNCTION(func_802421f0);
UNKNOWN_FUNCTION(homebuttonDumpEfb);
UNKNOWN_FUNCTION(homebuttonDisp);

CPP_WRAPPER_END()
