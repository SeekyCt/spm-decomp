#pragma once

#include <common.h>

CPP_WRAPPER(spm::wa1_02)

typedef struct 
{
/* 0x00 */ const char * mapName;
/* 0x04 */ u8 unknown_0x4[0x8 - 0x4];
/* 0x08 */ s32 alreadyBeatenLsw1;
/* 0x0C */ u8 unknown_0xc[0x10 - 0xc];
/* 0x10 */ s32 alreadyBeatenGsw0;
/* 0x14 */ u8 unknown_0x14[0x28 - 0x14];
} SammerDef;
SIZE_ASSERT(SammerDef, 0x28)

extern SammerDef sammerDefsCh6[30];
extern SammerDef sammerDefsEndgame[30];

// ...

CPP_WRAPPER_END()
