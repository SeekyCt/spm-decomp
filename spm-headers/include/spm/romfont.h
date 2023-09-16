#pragma once

#include <common.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::romfont)

USING(wii::gx::GXColor)

// This file was heavily changed in the Korean version to be NW4R based
#ifndef SPM_KR0

typedef struct
{
/* 0x000 */ u8 image[0x120];
/* 0x120 */ s16 character;
/* 0x122 */ s16 width;
/* 0x124 */ u8 unknown_0x124[0x140 - 0x124];
} RomfontEntry;
SIZE_ASSERT(RomfontEntry, 0x140)

typedef struct
{
/* 0x0 */ RomfontEntry * entries;
/* 0x4 */ s32 entryCount;
/* 0x8 */ s32 useSJIS;
} RomfontWork;
SIZE_ASSERT(RomfontWork, 0xc)

DECOMP_STATIC(RomfontWork * romfont_wp)
DECOMP_STATIC(RomfontWork romfont_work)

const char * romFontGetMessage(s32 id);
void romFontInit();
DECOMP_STATIC(void romfont_romFontMake())
void ATTRIBUTE_FORMAT(printf, 5, 6) romFontPrintGX(f32 x, f32 y, f32 scale, const GXColor * colour,
                                                   const char * msg, ...);
s32 romFontGetWidth(const char * message);

#endif

CPP_WRAPPER_END()
