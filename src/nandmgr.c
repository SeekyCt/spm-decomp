#include <common.h>
#include <evtmgr_cmd.h>
#include <guide.h>
#include <lzss10.h>
#include <mario.h>
#include <memory.h>
#include <nand.h>
#include <nandmgr.h>
#include <npcdrv.h>
#include <os.h>
#include <string.h>
#include <system.h>
#include <tpl.h>

#define flag(value, mask) (value & mask) // needed for assert

static NandWork nandWork; // 80534fa8
static NandWork * wp = &nandWork; // 805ae1b0

// #include "files/saveImagesTpl.inc"
static const u8 saveImagesTpl[0x5058] = {1}; // not included in decomp for copyright reasons

// Register usage not matching, might need nandClearSave inlined?
void nandInit()
{
    memset(wp, 0, sizeof(*wp));

    wp->openingBufferSize = 0x4000;
    wp->openingBuffer = __memAlloc(0, wp->openingBufferSize);

    wp->saves = __memAlloc(0, sizeof(SaveFile) * 4);

    wp->tempSaveFile = __memAlloc(0, 0x25c0);
    wp->banner = __memAlloc(0, sizeof(NANDBanner));

    switch (gp->language)
    {
        case 0:
            NANDInitBanner(wp->banner, 0x10, // Some broken japanese strings?
                        L"\x30FB\x0058\x30FB\x005B\x30FB\x0070\x30FB\x005B\x30FB\x0079\x30FB\x005B\x30FB\x0070\x30FB\x005B\x30FB\x007D\x30FB\x30FB\x30FB\x0049",
                        L"\x30FB\x0079\x30FB\x30FB\x30FB\x0079\x30FB\x30FB\x30FB\x007D\x30FB\x30FB\x30FB\x0049\x30FB\xFF8C\x30FB\xFF74\x30FB\x30FB\x30FB\xFF73\x30FB\x30FB\x30FB\x0060\x30FB\xFF6F\x30FB\x0049");
            break;

        case 1:
        case 2:
        case 7:
            NANDInitBanner(wp->banner, 0x10, L"Super Paper Mario", L"An interdimensional adventure!");
            break;

        case 3:
            NANDInitBanner(wp->banner, 0x10, L"Super Paper Mario", L"Ein interdimensionales Abenteuer");
            break;

        case 4:
            NANDInitBanner(wp->banner, 0x10, L"Super Paper Mario", L"Une aventure interdimensionnelle");
            break;

        case 5:
            NANDInitBanner(wp->banner, 0x10, L"Super Paper Mario", L"\xA1Una aventura interdimensional!");
            break;

        case 6:
            NANDInitBanner(wp->banner, 0x10, L"Super Paper Mario", L"Un'avventura interdimensionale!");
            break;
    }

    TPLHeader * tpl = __memAlloc(0, lzss10GetDecompSize(saveImagesTpl));
    lzss10Decompress(saveImagesTpl, tpl);
    UnpackTexPalette(tpl);

    memcpy(wp->banner->bannerTexture, tpl->imageTable[0].image->data, sizeof(wp->banner->bannerTexture));
    memcpy(wp->banner->iconTextures[0], tpl->imageTable[1].image->data, sizeof(wp->banner->iconTextures[0]));
    memcpy(wp->banner->iconTextures[1], tpl->imageTable[1].image->data, sizeof(wp->banner->iconTextures[1]));
    memcpy(wp->banner->iconTextures[2], tpl->imageTable[2].image->data, sizeof(wp->banner->iconTextures[2]));

    wp->banner->iconSpeed = (u16) (wp->banner->iconSpeed & ~3 | 3);
    wp->banner->iconSpeed = (u16) (wp->banner->iconSpeed & ~0xc | 0xc);
    wp->banner->iconSpeed = (u16) (wp->banner->iconSpeed & ~0x30 | 0x20);
    wp->banner->iconSpeed = (u16) (wp->banner->iconSpeed & ~0xc0);

    __memFree(0, tpl);

    wp->bannerSize = sizeof(NANDBanner) - (5 * 0x1200); // don't include unused iconTextures

    for (int i = 0; i < 4; i++)
    {
        SaveFile * curSave = &wp->saves[i];
        memset(curSave, 0, sizeof(*curSave));

        curSave->flags |= 1;
        curSave->checksum = 0;
        curSave->checksumNOT = 0xffffffff;
        u32 checksum = 0;
        for (int j = 0; j < sizeof(*curSave); j += 2)
        {
            checksum += ((u8 *)curSave)[j];
            checksum += ((u8 *)curSave)[j+1];
        }
        curSave->checksum = checksum;
        curSave->checksumNOT = ~checksum;
    }
}

void nandMain()
{
    switch (wp->task)
    {
        case NANDMGR_TASK_CHECK:
            nandCheckMain();
            break;

        case NANDMGR_TASK_WRITE_BANNER:
            nandWriteBannerMain();
            break;

        case NANDMGR_TASK_WRITE_ALL_SAVES:
            nandWriteAllSavesMain();
            break;

        case NANDMGR_TASK_WRITE_SAVE:
            nandWriteSaveMain();
            break;

        case NANDMGR_TASK_WRITE_BANNER_LOAD_ALL_SAVES:
            nandWriteBannerLoadAllSavesMain();
            break;

        case NANDMGR_TASK_DELETE_SAVE:
            nandDeleteSaveMain();
            break;

        case NANDMGR_TASK_NONE:
            return;
    }
}

bool nandIsExec()
{
    if (wp->flag & NAND_FLAG_Exec)
        return 1;
    else
        return (bool) (wp->flag & NAND_FLAG_Waiting);
}

s32 nandGetCode()
{
    return wp->code;
}

SaveFile * nandGetSaveFiles()
{
    return wp->saves;
}

void nandCheck()
{
    // "Already running"
    assert(0x12c, !flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
 
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_CHECK;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0;
}

void nandWriteBanner()
{
    // "Already running"
    assert(0x139, !flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");

    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_WRITE_BANNER;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0;
}

void nandWriteAllSaves()
{
    // "Already running"
    assert(0x146, !flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");

    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_WRITE_ALL_SAVES;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0;
}

void nandWriteSave(s32 saveId)
{
    // "Already running"
    assert(0x153, !flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");

    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_WRITE_SAVE;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = saveId;
}

void nandWriteBannerLoadAllSaves()
{
    // "Already running"
    assert(0x160, !flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");

    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_WRITE_BANNER_LOAD_ALL_SAVES;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0;
}

void nandDeleteSave(s32 saveId)
{
    // "Already running"
    assert(0x16d, !flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_DELETE_SAVE;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0; // bug
    (void) saveId;
}

void nandCopySave(s32 sourceId, s32 destId)
{
    memcpy(&wp->saves[destId], &wp->saves[sourceId], sizeof(SaveFile));
}

void nandClearSave(s32 saveId)
{
    SaveFile * save = &wp->saves[saveId];

    memset(save, 0, sizeof(*save));

    save->flags |= 1;

    save->checksum = 0;
    save->checksumNOT = 0xffffffff;
    u32 checksum = 0;
    for (int i = 0; i < sizeof(*save); i += 2)
    {
        checksum += ((u8 *)save)[i];
        checksum += ((u8 *)save)[i+1]; // scheduling doesn't match with just one
    }
    save->checksum = checksum;
    save->checksumNOT = ~checksum;
}

void nandUpdateSave(s32 saveId)
{
    SaveFile * save = &wp->saves[saveId];
    MarioWork * mp = marioGetPtr();

    memset(save, 0, sizeof(SaveFile));

    SpmarioGlobals * _gp = gp; // a sign of inlining?
    _gp->lastSaveUpdateTime = OSGetTime();
    _gp->savePosition.x = mp->position.x;
    _gp->savePosition.y = mp->position.y;
    _gp->savePosition.z = mp->position.z;

    evtSetValue(NULL, GSWF(401), (s32) ((mp->flag4 >> 21) & 1));
    evtSetValue(NULL, GSWF(402), (s32) ((mp->flag4 >> 19) & 1));

    s32 temp = (guideGetWork()->flag0 & 0x80);
    evtSetValue(NULL, GSWF(403), temp == 0);

    evtSetValue(NULL, GSWF(404), (s32) ((mp->flag8 >> 23) & 1));
    evtSetValue(NULL, GSW(23), mp->character);

    gp->flags &= ~1;
    gp->flags &= ~2;
    gp->flags &= ~4;
    gp->flags &= ~8;

    gp->discIsEjected = 0;
    gp->gameSpeed = 1.0f;

    memcpy(&save->spmarioGlobals, gp, sizeof(SpmarioGlobals));
    memcpy(&save->pouch, pouchGetPtr(), sizeof(save->pouch));
    memcpy(&save->unknown_0x21b0, npcGetWorkPtr()->unknown_0x728, sizeof(save->unknown_0x21b0));

    save->flags &= ~3;

    save->checksum = 0;
    save->checksumNOT = 0xffffffff;
    u32 checksum = 0;
    for (int i = 0; i < sizeof(*save); i += 2)
    {
        checksum += ((u8 *)save)[i];
        checksum += ((u8 *)save)[i+1];
    }
    save->checksum = checksum;
    save->checksumNOT = ~checksum;
}

// Register usage not matching
void nandLoadSave(s32 saveId)
{
    SaveFile * save = &wp->saves[saveId];

    s32 language = gp->language;
    s32 fps = gp->fps;
    s32 lastSaveLoaded = gp->lastSaveLoaded;

    memcpy(gp, &save->spmarioGlobals, sizeof(SpmarioGlobals));
    memcpy(pouchGetPtr(), &save->pouch, sizeof(save->pouch));
    memcpy(npcGetWorkPtr()->unknown_0x728, &save->unknown_0x21b0, sizeof(save->unknown_0x21b0));

    gp->discIsEjected = 0;

    SpmarioGlobals * _gp = gp; // probably another inline function
    _gp->lastSaveLoadTime = OSGetTime();
    _gp->gameSpeed = 1.0f;

    gp->lastSaveLoaded = lastSaveLoaded;
    gp->language = language;
    gp->fps = fps;
    gp->unknown_0xe0 = 0;
}

void nandDisableSaving()
{
    wp->flag |= NAND_FLAG_NoSave;
}

void nandEnableSaving()
{
    wp->flag &= ~NAND_FLAG_NoSave;
}

bool nandCheckSaving()
{
    return (wp->flag & NAND_FLAG_NoSave) != 0;
}

void nandCheckMain()
{
    if (wp->flag & NAND_FLAG_Waiting)
        return;

    if (wp->code != NAND_CODE_OK)
    {
        wp->task = 0;
        wp->flag &= ~NAND_FLAG_Exec;
        wp->flag |= 4;
    }
    else
    {
        switch (wp->stage)
        {
            case 0:
                wp->flag |= NAND_FLAG_Waiting;
                s32 tries = 0;
                wp->answer = 0;

                while (NANDCheckAsync(NAND_BLOCK_COUNT, NAND_INODE_COUNT, &wp->answer,
                                      &checkCallback, &wp->commandBlock) == NAND_CODE_BUSY)
                {
                    if (++tries > NAND_ATTEMPTS_MAX)
                    {
                        wp->code = -128;
                        wp->flag &= ~NAND_FLAG_Waiting;
                        break;
                    }
                }
                break;

            case 1:
                wp->task = 0;
                wp->flag &= ~NAND_FLAG_Exec;
                break;
        }

        wp->stage++;
    }
}

// Register usage not matching
void nandWriteBannerMain()
{
    if (wp->flag & NAND_FLAG_Waiting)
        return;

    if ((wp->code != NAND_CODE_OK) && (wp->code != NAND_CODE_EXISTS))
    {
        wp->task = 0;
        wp->flag &= ~NAND_FLAG_Exec;
        wp->flag |= 4;
    }
    else
    {
        char * dir;
        s32 tries;
        NANDFileInfo * fileInfo;
        const char * filename;

        switch(wp->stage)
        {
            // Get game save directory
            case 0:
                dir = wp->homedir;
                wp->flag |= NAND_FLAG_Waiting;

                tries = 0;
                s32 code; // different since this function doesn't have a callback
                while (code = NANDGetHomeDir(dir), code == NAND_CODE_BUSY)
                {
                    if (++tries > NAND_ATTEMPTS_MAX)
                    {
                        code = -128;
                        break;
                    }
                }

                wp->code = code;
                wp->flag &= ~NAND_FLAG_Waiting;
                break;

            // Change to game save directory
            case 1:
                dir = wp->homedir;
                wp->flag |= NAND_FLAG_Waiting;

                tries = 0;
                while (NANDChangeDirAsync(dir, &genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
                {
                    if (++tries > NAND_ATTEMPTS_MAX)
                    {
                        wp->code = -128;
                        wp->flag &= ~NAND_FLAG_Waiting;
                        break;
                    }
                }
                break;

            // Create banner.bin, may already exist
            case 2:
                filename = "banner.bin";
                wp->flag |= NAND_FLAG_Waiting;

                tries = 0;
                while (NANDCreateAsync(filename, NAND_PERMISSION_READ_WRITE, 0, &genericCallback,
                                       &wp->commandBlock) == NAND_CODE_BUSY)
                {
                    if (++tries > NAND_ATTEMPTS_MAX)
                    {
                        wp->code = -128;
                        wp->flag &= ~NAND_FLAG_Waiting;
                        break;
                    }
                }
                break;

            // Open banner.bin
            case 3:
                fileInfo = &wp->fileInfo;
                filename = "banner.bin";
                wp->flag |= NAND_FLAG_Waiting;

                tries = 0;
                while (NANDSafeOpenAsync(filename, fileInfo, NAND_MODE_WRITE, wp->openingBuffer, wp->openingBufferSize,
                                         &genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
                {
                    if (++tries > NAND_ATTEMPTS_MAX)
                    {
                        wp->code = -128;
                        wp->flag &= ~NAND_FLAG_Waiting;
                        break;
                    }
                }
                break;

            // Write banner data into banner.bin
            case 4:
                fileInfo = &wp->fileInfo;
                NANDBanner * banner = wp->banner;
                u32 bannerSize = wp->bannerSize;
                wp->flag |= NAND_FLAG_Waiting;

                tries = 0;
                while (NANDWriteAsync(fileInfo, banner, bannerSize, &genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
                {
                    if (++tries > NAND_ATTEMPTS_MAX)
                    {
                        wp->code = -128;
                        wp->flag &= ~NAND_FLAG_Waiting;
                        break;
                    }
                }
                break;

            // Close banner.bin
            case 5:
                fileInfo = &wp->fileInfo;
                wp->flag |= NAND_FLAG_Waiting;
                tries = 0;
                while (NANDSafeCloseAsync(fileInfo, &genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
                {
                    if (++tries > NAND_ATTEMPTS_MAX)
                    {
                        wp->code = -128;
                        wp->flag &= ~NAND_FLAG_Waiting;
                        break;
                    }
                }
                break;

            // End
            case 6:
                wp->task = 0;
                wp->flag &= ~NAND_FLAG_Exec;
        }

        wp->stage++;
    }
}

// Unfinished, just for string pool
void nandWriteAllSavesMain()
{
    __dummy_string("wiimario%02d");
}

// void nandWriteSaveMain();
// void nandWriteBannerLoadAllSavesMain();
// void nandDeleteSaveMain();

void genericCallback(s32 result, NANDCommandBlock * commandBlock)
{
    (void) commandBlock;

    if (result > 0)
        result = 0;

    wp->flag &= ~NAND_FLAG_Waiting;
    wp->code = result;
}

void checkCallback(s32 result, NANDCommandBlock * commandBlock)
{
    (void) commandBlock;

    wp->flag &= ~NAND_FLAG_Waiting;
    wp->code = result;

    if (wp->code != 0)
        return;
    
    if (wp->answer & 0xa)
        wp->code = -11;
    if (wp->answer & 5)
        wp->code = -9;
}

#undef flag
