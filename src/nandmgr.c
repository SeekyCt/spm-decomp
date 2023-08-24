#include <common.h>
#include <spm/evtmgr_cmd.h>
#include <spm/guide.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/memory.h>
#include <spm/nandmgr.h>
#include <spm/npcdrv.h>
#include <spm/system.h>
#include <wii/cx.h>
#include <wii/nand.h>
#include <wii/os.h>
#include <wii/tpl.h>
#include <msl/stdio.h>
#include <msl/string.h>

// .rodata
const u8 lz_saveImagesTpl[] = {
    #include "assets/saveImages.tpl.lz.inc"
};
#include "orderstrings/803494e8_80349537.inc"

// .bss
static NandWork work;

// .sdata
static NandWork * wp = &work;

#define flag(value, mask) (value & mask) // needed for assert

static void genericCallback(s32 result, NANDCommandBlock * commandBlock);
static void checkCallback(s32 result, NANDCommandBlock * commandBlock);

static void nand_check();
static void nand_get_home_dir(char * homedir);
static void nand_change_dir(const char * dir);
static void nand_create(const char * path);
static void nand_delete(const char * path);
static void nand_seek(NANDFileInfo * fileInfo, u32 offset, u8 mode);
static void nand_open(const char * path, NANDFileInfo * fileInfo, u8 mode);
static void nand_read(NANDFileInfo * fileInfo, void * data, u32 length);
static void nand_write(NANDFileInfo * fileInfo, void * data, u32 length);
static void nand_close(NANDFileInfo * fileInfo);

// For some reason nandInit won't match without this
void dummy(s32 x);
void dummy(s32 x)
{
    memcpy(&wp->saves[x], 0, 0);
}

static u32 nandCalcChecksum(SaveFile * save)
{
    u32 checksum;
    u32 i;
    
    checksum = 0;
    for (i = 0; i < sizeof(*save); i++)
        checksum += ((u8 *)save)[i];

    return checksum;
}

void nandInit()
{
    TPLHeader *tpl;
    s32 i;

    // Init work
    memset(wp, 0, sizeof(*wp));
    wp->openingBufferSize = 0x4000;
    wp->openingBuffer = __memAlloc(HEAP_MAIN, wp->openingBufferSize);
    wp->saves = __memAlloc(HEAP_MAIN, sizeof(SaveFile)*4);
    wp->tempSaveFile = __memAlloc(HEAP_MAIN, NAND_TEMP_SAVE_SIZE);
    wp->banner = __memAlloc(HEAP_MAIN, sizeof(*wp->banner));

    // Create banner
    switch (gp->language)
    {
        case 0:
            // Some broken japanese strings?
            NANDInitBanner(wp->banner, 0x10,
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

    // Setup banner icons
    tpl = __memAlloc(0, CXGetUncompressedSize(&lz_saveImagesTpl));
    CXUncompressLZ(&lz_saveImagesTpl, tpl);
    TPLBind(tpl);
    memcpy(wp->banner->bannerTexture, tpl->imageTable[0].image->data, sizeof(wp->banner->bannerTexture));
    memcpy(wp->banner->iconTextures[0], tpl->imageTable[1].image->data, sizeof(wp->banner->iconTextures[0]));
    memcpy(wp->banner->iconTextures[1], tpl->imageTable[1].image->data, sizeof(wp->banner->iconTextures[1]));
    memcpy(wp->banner->iconTextures[2], tpl->imageTable[2].image->data, sizeof(wp->banner->iconTextures[2]));
    NAND_SET_ICON_SPEED(wp->banner, 0, 3);
    NAND_SET_ICON_SPEED(wp->banner, 1, 3);
    NAND_SET_ICON_SPEED(wp->banner, 2, 2);
    NAND_SET_ICON_SPEED(wp->banner, 3, 0);
    __memFree(0, tpl);
    wp->bannerSize = sizeof(NANDBanner) - (5 * sizeof(wp->banner->iconTextures[0]));

    // Init save files
    for (i = 0; i < 4; i++)
        nandClearSave(i);
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
        return true;
    else if (wp->flag & NAND_FLAG_Waiting)
        return true;
    else
        return false;
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
    // The function actually deletes all saves
    (void) saveId;

    // "Already running"
    assert(0x16d, !flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");

    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_DELETE_SAVE;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0; // deleting save 0 makes all saves considered deleted
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
    save->checksum = nandCalcChecksum(save);
    save->checksumNOT = ~save->checksum;
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

    evtSetValue(NULL, GSWF(401), (s32) ((mp->miscFlags >> 21) & 1));
    evtSetValue(NULL, GSWF(402), (s32) ((mp->miscFlags >> 19) & 1));

    s32 temp = (guideGetWork()->flag0 & 0x80);
    evtSetValue(NULL, GSWF(403), temp == 0);

    evtSetValue(NULL, GSWF(404), (s32) ((mp->dispFlags >> 23) & 1));
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
    save->checksum = nandCalcChecksum(save);
    save->checksumNOT = ~save->checksum;
}


void nandLoadSave(s32 saveId)
{
    SpmarioGlobals *_gp;
    s32 lastSaveLoaded;
    s32 language;
    s32 fps;
    SaveFile *save;

    // Get save file
    save = &wp->saves[saveId];

    // Backup globals not read from save
    language = gp->language;
    fps = gp->fps;
    lastSaveLoaded = gp->lastSaveLoaded;

    memcpy(gp, &save->spmarioGlobals, sizeof(save->spmarioGlobals));
    memcpy(pouchGetPtr(), &save->pouch, sizeof(save->pouch));
    memcpy(npcGetWorkPtr()->unknown_0x728, &save->unknown_0x21b0, sizeof(save->unknown_0x21b0));
    gp->discIsEjected = 0;
    _gp = gp; // Sign of inlining?
    _gp->lastSaveLoadTime = OSGetTime();
    _gp->gameSpeed = 1.0f;

    // Restore globals not read from save
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
    // Hack to stop this function being inlined
    (void) 0;

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
                nand_check();
                break;

            case 1:
                wp->task = 0;
                wp->flag &= ~NAND_FLAG_Exec;
                break;
        }

        wp->stage++;
    }
}

void nandWriteBannerMain()
{
    if (wp->flag & NAND_FLAG_Waiting)
        return;

    if ((wp->code != NAND_CODE_OK) && (wp->code != NAND_CODE_EXISTS))
    {
        wp->task = 0;
        wp->flag &= ~NAND_FLAG_Exec;
        wp->flag |= 4;
        return;
    }

    switch (wp->stage)
    {
        case 0:
            nand_get_home_dir(wp->homedir);
            break;

        case 1:
            nand_change_dir(wp->homedir);
            break;

        case 2:
            nand_create("banner.bin");
            break;

        case 3:
            nand_open("banner.bin", &wp->fileInfo, NAND_MODE_WRITE);
            break;

        case 4:
            nand_write(&wp->fileInfo, wp->banner, wp->bannerSize);
            break;

        case 5:
            nand_close(&wp->fileInfo);
            break;

        case 6:
            wp->task = NANDMGR_TASK_NONE;
            wp->flag &= ~NAND_FLAG_Exec;
            break;
    }

    wp->stage += 1;
}

void nandWriteAllSavesMain()
{
    char filename[NAND_PATH_LENGTH];

    if (wp->flag & NAND_FLAG_Waiting)
        return;

    if (wp->code != NAND_CODE_OK)
    {
        if (wp->code == NAND_CODE_EXISTS)
        {
            wp->code = 0;
            wp->stage = 6;
        }
        else
        {
            wp->task = 0;
            wp->flag &= ~NAND_FLAG_Exec;
            wp->flag |= 4;
            return;
        }
    }

    switch (wp->stage)
    {
        case 0:
            nand_get_home_dir(wp->homedir);
            break;

        case 1:
            nand_change_dir(wp->homedir);
            break;

        case 2:
            sprintf(filename, "wiimario%02d", wp->saveId);
            nand_create(filename);
            break;

        case 3:
            sprintf(filename, "wiimario%02d", wp->saveId);
            nand_open(filename, &wp->fileInfo, NAND_MODE_WRITE);
            break;

        case 4:
            nandClearSave(wp->saveId);
            memset(wp->tempSaveFile, 0, NAND_TEMP_SAVE_SIZE);
            memcpy(wp->tempSaveFile, wp->saves + wp->saveId, sizeof(SaveFile));
            nand_write(&wp->fileInfo, wp->tempSaveFile, sizeof(SaveFile));
            break;

        case 5:
            nand_close(&wp->fileInfo);
            break;

        case 6:
            wp->saveId += 1;
            if (wp->saveId < 4)
            {
                wp->stage = 2;
                return;
            }
            wp->task = NANDMGR_TASK_NONE;
            wp->flag &= ~NAND_FLAG_Exec;
    }

    wp->stage += 1;
}

void nandWriteSaveMain()
{
    char filename[NAND_PATH_LENGTH];

    if (wp->flag & NAND_FLAG_Waiting)
        return;

    if (wp->code != NAND_CODE_OK)
    {
        wp->task = 0;
        wp->flag &= ~NAND_FLAG_Exec;
        wp->flag |= 4;
        return;
    }

    switch (wp->stage)
    {
        case 0:
            nand_get_home_dir(wp->homedir);
            break;

        case 1:
            nand_change_dir(wp->homedir);
            break;

        case 2:
            sprintf(filename, "wiimario%02d", wp->saveId);
            nand_open(filename, &wp->fileInfo, NAND_MODE_WRITE);
            break;

        case 3:
            memset(wp->tempSaveFile, 0, NAND_TEMP_SAVE_SIZE);
            memcpy(wp->tempSaveFile, wp->saves + wp->saveId, sizeof(SaveFile));
            nand_write(&wp->fileInfo, wp->tempSaveFile, sizeof(SaveFile));
            break;

        case 4:
            nand_close(&wp->fileInfo);
            break;

        case 5:
            wp->task = 0;
            wp->flag &= ~NAND_FLAG_Exec;
            break;
    }

    wp->stage += 1;
}

void nandWriteBannerLoadAllSavesMain()
{
    char filename[NAND_PATH_LENGTH];
    SaveFile * save;
    s32 i;

    if (wp->flag & NAND_FLAG_Waiting)
        return;

    if (wp->code != NAND_CODE_OK)
    {
        if ((wp->saveId < 4 && wp->stage >= 7) &&
            (wp->code == -5 || wp->code == -0xF))
        {
                wp->flag |= 4;
        }
        else
        {
            wp->task = 0;
            wp->flag &= ~NAND_FLAG_Exec;
            wp->flag |= 4;
            return;
        }
    }

    switch (wp->stage)
    {
        case 0:
            nand_get_home_dir(wp->homedir);
            break;

        case 1:
            nand_change_dir(wp->homedir);
            break;

        case 2:
            nand_open("banner.bin", &wp->fileInfo, NAND_MODE_READ | NAND_MODE_WRITE);
            wp->tempBanner = __memAlloc(0, wp->bannerSize);
            break;

        case 3:
            nand_read(&wp->fileInfo, wp->tempBanner, wp->bannerSize);
            break;

        case 4:
            if (memcmp(wp->tempBanner, wp->banner, wp->bannerSize) != 0)
            {
                nand_seek(&wp->fileInfo, 0, 0);
            }
            else
            {
                wp->stage = 6;
                return;
            }
            break;

        case 5:
            nand_write(&wp->fileInfo, wp->banner, wp->bannerSize);
            break;

        case 6:
            __memFree(0, wp->tempBanner);
            nand_close(&wp->fileInfo);
            break;

        case 7:
            sprintf(filename, "wiimario%02d", wp->saveId);
            nand_open(filename, &wp->fileInfo, NAND_MODE_READ);
            break;

        case 8:
            memset(wp->tempSaveFile, 0, NAND_TEMP_SAVE_SIZE);
            nand_read(&wp->fileInfo, wp->tempSaveFile, NAND_TEMP_SAVE_SIZE);
            break;

        case 9:
            nand_close(&wp->fileInfo);
            break;

        case 10:
            memcpy(wp->saves + wp->saveId, wp->tempSaveFile, sizeof(SaveFile));
            wp->saveId += 1;
            if (wp->saveId < 4)
            {
                wp->stage = 7;
                return;
            }

            save = wp->saves;
            for (i = 0; i < 4; save++, i++)
            {
                if (save->checksum != nandCalcChecksum(save))
                {
                    save->flags &= ~1;
                    save->flags |= SAVE_FLAG_CORRUPT;
                }
            }

            wp->task = NANDMGR_TASK_NONE;
            wp->flag &= ~NAND_FLAG_Exec;
            break;
    }

    wp->stage += 1;
}

void nandDeleteSaveMain()
{
    char filename[NAND_PATH_LENGTH];

    if (wp->flag & NAND_FLAG_Waiting)
        return;

    if (wp->code != NAND_CODE_OK)
    {
        wp->task = 0;
        wp->flag &= 0xFFFFFFFE;
        wp->flag |= 4;
        return;
    }

    switch (wp->stage)
    {
        case 0:
            nand_get_home_dir(wp->homedir);
            break;

        case 1:
            nand_change_dir(wp->homedir);
            break;

        case 2:
            sprintf(filename, "wiimario%02d", wp->saveId);
            nand_delete(filename);
            break;

        case 3:
            wp->task = NANDMGR_TASK_NONE;
            wp->flag &= ~NAND_FLAG_Exec;
            break;
        }

    wp->stage += 1;
}

static void genericCallback(s32 result, NANDCommandBlock * commandBlock)
{
    (void) commandBlock;

    if (result > 0)
        result = 0;

    wp->flag &= ~NAND_FLAG_Waiting;
    wp->code = result;
}

static void checkCallback(s32 result, NANDCommandBlock * commandBlock)
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

static void nand_check()
{
    s32 tries;

    wp->flag |= NAND_FLAG_Waiting;
    tries = 0;
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

}

static void nand_get_home_dir(char * homedir)
{
    s32 tries;
    s32 code;

    wp->flag |= NAND_FLAG_Waiting;
    tries = 0;
    while (code = NANDGetHomeDir(homedir), code == NAND_CODE_BUSY)
    {
        if (++tries > NAND_ATTEMPTS_MAX)
        {
            code = -128;
            break;
        }
    }
    wp->code = code;
    wp->flag &= ~NAND_FLAG_Waiting;
}

static void nand_change_dir(const char * dir)
{
    s32 tries;

    wp->flag |= NAND_FLAG_Waiting;
    tries = 0;
    while (NANDChangeDirAsync(dir, genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
    {
        if (++tries > NAND_ATTEMPTS_MAX)
        {
            wp->code = -128;
            wp->flag &= ~NAND_FLAG_Waiting;
            break;
        }
    }
}

static void nand_create(const char * path)
{
    s32 tries;

    wp->flag |= NAND_FLAG_Waiting;
    tries = 0;
    while (NANDCreateAsync(path, NAND_PERMISSION_READ_WRITE, 0, genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
    {
        if (++tries > NAND_ATTEMPTS_MAX)
        {
            wp->code = -128;
            wp->flag &= ~NAND_FLAG_Waiting;
            break;
        }
    }
}

static void nand_delete(const char * path)
{
    s32 tries;

    wp->flag |= NAND_FLAG_Waiting;
    tries = 0;
    while (NANDDeleteAsync(path, genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
    {
        if (++tries > NAND_ATTEMPTS_MAX)
        {
            wp->code = -128;
            wp->flag &= ~NAND_FLAG_Waiting;
            break;
        }
    }
}

static void nand_open(const char * path, NANDFileInfo * fileInfo, u8 mode)
{
    s32 tries;

    wp->flag |= NAND_FLAG_Waiting;
    tries = 0;
    while (NANDSafeOpenAsync(path, fileInfo, mode, wp->openingBuffer, wp->openingBufferSize, genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
    {
        if (++tries > NAND_ATTEMPTS_MAX)
        {
            wp->code = -128;
            wp->flag &= ~NAND_FLAG_Waiting;
            break;
        }
    }
}

static void nand_seek(NANDFileInfo * fileInfo, u32 offset, u8 mode)
{
    s32 tries;

    wp->flag |= NAND_FLAG_Waiting;
    tries = 0;
    while (NANDSeekAsync(fileInfo, offset, mode, genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
    {
        if (++tries > 0x64)
        {
            wp->code = -128;
            wp->flag &= ~NAND_FLAG_Waiting;
            break;
        }
    }
}

static void nand_read(NANDFileInfo * fileInfo, void * data, u32 length)
{
    s32 tries;

    wp->flag |= NAND_FLAG_Waiting;
    tries = 0;
    while (NANDReadAsync(fileInfo, data, length, genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
    {
        if (++tries > NAND_ATTEMPTS_MAX)
        {
            wp->code = -128;
            wp->flag &= ~NAND_FLAG_Waiting;
            break;
        }
    }
}

static void nand_write(NANDFileInfo * fileInfo, void * data, u32 length)
{
    s32 tries;

    wp->flag |= NAND_FLAG_Waiting;
    tries = 0;
    while (NANDWriteAsync(fileInfo, data, length, genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
    {
        if (++tries > NAND_ATTEMPTS_MAX)
        {
            wp->code = -128;
            wp->flag &= ~NAND_FLAG_Waiting;
            break;
        }
    }
}

static void nand_close(NANDFileInfo * fileInfo)
{
    s32 tries;

    wp->flag |= NAND_FLAG_Waiting;
    tries = 0;
    while (NANDSafeCloseAsync(fileInfo, genericCallback, &wp->commandBlock) == NAND_CODE_BUSY)
    {
        if (++tries > NAND_ATTEMPTS_MAX)
        {
            wp->code = -128;
            wp->flag &= ~NAND_FLAG_Waiting;
            break;
        }
    }    
}
