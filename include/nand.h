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
#define NAND_PERMISSION_READ_WRITE (NAND_PERMISSION_READ | NAND_PERMISSION_WRITE)

#define NAND_MODE_READ 1
#define NAND_MODE_WRITE 2

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

typedef void (NANDCallback)(s32 result, NANDCommandBlock * commandBlock);

s32 NANDCreateAsync(const char * filename, u8 permissions, u8 attributes, NANDCallback * callback, NANDCommandBlock * commandBlock); // 8029983c
s32 NANDWriteAsync(NANDFileInfo * fileInfo, void * buffer, u32 size, NANDCallback * callback, NANDCommandBlock * commandBlock); // 80299cbc
s32 NANDSafeOpenAsync(const char * path, NANDFileInfo * fileInfo, u8 mode, void * buffer, u32 bufferSize, NANDCallback * callback, NANDCommandBlock * commandBlock); // 8029a73c
s32 NANDSafeCloseAsync(NANDFileInfo * fileInfo, NANDCallback * callback, NANDCommandBlock * commandBlock); // 8029ac98
s32 NANDChangeDirAsync(const char * path, NANDCallback * callback, NANDCommandBlock * commandBlock); // 8029bbe4
s32 NANDGetHomeDir(char * path); // 8029bdf8
void NANDInitBanner(NANDBanner * banner, u32 flags, wchar_t * title, wchar_t * comment); // 8029c438
s32 NANDCheckAsync(u32 blockCount, u32 inodeCount, u32 * answer, NANDCallback * callback, NANDCommandBlock * commandBlock); // 8029c5b0
// more

#endif
