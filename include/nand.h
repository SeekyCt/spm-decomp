/*
    Revolution SDK NAND library
*/

#ifndef NAND_H
#define NAND_H

#include <common.h>

typedef struct {
    u32 magic;
    u32 flags;
    u16 iconSpeed;
    u8 unknown_0xa[0x20 - 0xa];
    wchar_t comment[2][32];
    u8 bannerTexture[0x6000];
    u8 iconTextures[8][0x1200];
} NANDBanner;

void NANDInitBanner(NANDBanner * banner, u32 flags, wchar_t * title, wchar_t * comment); // 8029c438
// more

#endif
