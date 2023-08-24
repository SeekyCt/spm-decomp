/*
    Revolution SDK NAND library
*/

#pragma once

#include <common.h>

CPP_WRAPPER(wii::nand)

#define NAND_ALIGN 32

#define NAND_CODE_OK 0
#define NAND_CODE_BUSY -3
#define NAND_CODE_5 -5
#define NAND_CODE_EXISTS -6
#define NAND_CODE_9 -9
#define NAND_CODE_11 -11
#define NAND_CODE_NOT_EXIST -12
#define NAND_CODE_15 -15
#define NAND_CODE_FATAL -128

#define NAND_ANSWER_FLAG_1 1
#define NAND_ANSWER_FLAG_2 2
#define NAND_ANSWER_FLAG_4 4
#define NAND_ANSWER_FLAG_8 8

#define NAND_PERMISSION_READ 0x10
#define NAND_PERMISSION_WRITE 0x20
#define NAND_PERMISSION_READ_WRITE (NAND_PERMISSION_READ | NAND_PERMISSION_WRITE)

#define NAND_MODE_READ 1
#define NAND_MODE_WRITE 2

#define NAND_SET_ICON_SPEED(banner, icon, speed) banner->iconSpeed = (u16) ((banner->iconSpeed & ~(3 << 2 * (icon))) | ((speed) << 2 * (icon)))

#define NAND_PATH_LENGTH 64
#define NAND_FILENAME_LENGTH 13

#define NAND_BANNER_FLAG_NO_COPY 0x1
#define NAND_BANNER_FLAG_BOUNCE 0x10

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x8c - 0x0];
} NANDFileInfo;
SIZE_ASSERT(NANDFileInfo, 0x8c)

typedef struct
{
/* 0x0000 */ u32 magic;
/* 0x0004 */ u32 flags;
/* 0x0008 */ u16 iconSpeed;
/* 0x000A */ u8 unknown_0xa[0x20 - 0xa];
/* 0x0020 */ wchar16_t comment[2][32];
/* 0x00A0 */ u8 bannerTexture[0x6000];
/* 0x60A0 */ u8 iconTextures[8][0x1200];
} NANDBanner;
SIZE_ASSERT(NANDBanner, 0xf0a0)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0xb8 - 0x0];
#ifdef SPM_KR0
/* 0xB0 */ u8 unknown_0xb8[0xbc - 0xb8];
#endif
} NANDCommandBlock;
#ifdef SPM_KR0
SIZE_ASSERT(NANDCommandBlock, 0xbc)
#else
SIZE_ASSERT(NANDCommandBlock, 0xb8)
#endif

typedef void (NANDCallback)(s32 result, NANDCommandBlock * commandBlock);

UNKNOWN_FUNCTION(nandCreate);
s32 NANDCreateAsync(const char * filename, u8 permissions, u8 attributes, NANDCallback * callback,
                    NANDCommandBlock * commandBlock);
UNKNOWN_FUNCTION(NANDPrivateCreateAsync);
UNKNOWN_FUNCTION(NANDDelete);
s32 NANDDeleteAsync(const char * filename, NANDCallback * callback,
                    NANDCommandBlock * commandBlock);
UNKNOWN_FUNCTION(NANDPrivateDeleteAsync);
UNKNOWN_FUNCTION(NANDRead);
s32 NANDReadAsync(NANDFileInfo * fileinfo, void * buffer, u32 length, NANDCallback * callback,
                  NANDCommandBlock * commandBlock);
UNKNOWN_FUNCTION(NANDWrite);
s32 NANDWriteAsync(NANDFileInfo * fileInfo, void * buffer, u32 size, NANDCallback * callback,
                   NANDCommandBlock * commandBlock);
s32 NANDSeekAsync(NANDFileInfo * fileInfo, u32 offset, u8 mode, NANDCallback * callback,
                  NANDCommandBlock * commandBlock);
UNKNOWN_FUNCTION(nandCreateDir);
UNKNOWN_FUNCTION(NANDPrivateCreateDirAsync);
UNKNOWN_FUNCTION(nandComposePerm);
UNKNOWN_FUNCTION(nandSplitPerm);
UNKNOWN_FUNCTION(nandGetStatus);
UNKNOWN_FUNCTION(nandGetStatusCallback);
UNKNOWN_FUNCTION(NANDPrivateGetStatusAsync);
UNKNOWN_FUNCTION(nandOpen);
UNKNOWN_FUNCTION(NANDOpen);
UNKNOWN_FUNCTION(NANDPrivateOpen);
UNKNOWN_FUNCTION(NANDOpenAsync);
UNKNOWN_FUNCTION(NANDPrivateOpenAsync);
UNKNOWN_FUNCTION(nandOpenCallback);
UNKNOWN_FUNCTION(NANDClose);
UNKNOWN_FUNCTION(NANDCloseAsync);
s32 NANDSafeOpenAsync(const char * path, NANDFileInfo * fileInfo, u8 mode, void * buffer,
                      u32 bufferSize, NANDCallback * callback, NANDCommandBlock * commandBlock);
UNKNOWN_FUNCTION(nandSafeOpenAsync);
UNKNOWN_FUNCTION(nandSafeOpenCallback);
UNKNOWN_FUNCTION(nandReadOpenCallback);
s32 NANDSafeCloseAsync(NANDFileInfo * fileInfo, NANDCallback * callback,
                       NANDCommandBlock * commandBlock);
UNKNOWN_FUNCTION(nandSafeCloseCallback);
UNKNOWN_FUNCTION(nandReadCloseCallback);
UNKNOWN_FUNCTION(nandCloseCallback);
UNKNOWN_FUNCTION(nandRemoveTailToken);
UNKNOWN_FUNCTION(nandGetHeadToken);
UNKNOWN_FUNCTION(nandGetRelativeName);
UNKNOWN_FUNCTION(nandConvertPath);
UNKNOWN_FUNCTION(nandIsPrivatePath);
UNKNOWN_FUNCTION(nandIsUnderPrivatePath);
UNKNOWN_FUNCTION(nandIsInitialised);
UNKNOWN_FUNCTION(nandReportErrorCode);
UNKNOWN_FUNCTION(nandConvertErrorCode);
UNKNOWN_FUNCTION(nandGenerateAbsPath);
UNKNOWN_FUNCTION(nandGetParentDirectory);
UNKNOWN_FUNCTION(NANDInit);
UNKNOWN_FUNCTION(nandOnShutdown);
UNKNOWN_FUNCTION(nandShutdownCallback);
UNKNOWN_FUNCTION(nandChangeDir);
s32 NANDChangeDirAsync(const char * path, NANDCallback * callback, NANDCommandBlock * commandBlock);
UNKNOWN_FUNCTION(nandChangeDirCallback);
s32 NANDGetHomeDir(char * path);
UNKNOWN_FUNCTION(nandCallback);
UNKNOWN_FUNCTION(nandGetType);
UNKNOWN_FUNCTION(NANDPrivateGetTypeAsync);
UNKNOWN_FUNCTION(nandGetTypeCallback);
UNKNOWN_FUNCTION(nandGetHomeDir);
void NANDInitBanner(NANDBanner * banner, u32 flags, wchar16_t * title, wchar16_t * comment);
UNKNOWN_FUNCTION(_ES_GetDataDir);
s32 NANDCheckAsync(u32 blockCount, u32 inodeCount, u32 * answer, NANDCallback * callback,
                   NANDCommandBlock * commandBlock);
UNKNOWN_FUNCTION(nandUserAreaCallback);

CPP_WRAPPER_END()
