/*
    Revolution SDK NAND library
*/

#ifndef NAND_H
#define NAND_H

#include <common.h>

#define NAND_CODE_OK 0
#define NAND_CODE_BUSY -3
#define NAND_CODE_EXISTS -6

#define NAND_PERMISSION_READ 0x10
#define NAND_PERMISSION_WRITE 0x20

#define NAND_MODE_READ 1
#define NAND_MODE_WRITE 1

typedef struct {
    u8 unknown_0x0[0x8c - 0x0];
} NANDFileInfo;

typedef struct {
    u32 magic;
    u32 flags;
    u16 iconSpeed;
    u8 unknown_0xa[0x20 - 0xa];
    wchar_t comment[2][32];
    u8 bannerTexture[0x6000];
    u8 iconTextures[8][0x1200];
} NANDBanner;

typedef struct {
    u8 unknown_0x0[0xb8 - 0x0];
} NANDCommandBlock;

typedef void (*NANDCallback)(s32 result, NANDCommandBlock * commandBlock);

void NANDInitBanner(NANDBanner * banner, u32 flags, wchar_t * title, wchar_t * comment); // 8029c438
void NANDCheckAsync(u32 blockCount, u32 inodeCount, u32 * answer, NANDCallback * callback, NANDCommandBlock * commandBlock); // 8029c5b0
// more

#endif
