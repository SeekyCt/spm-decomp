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

void nandInit(void)
{
    TPLHeader *tpl;
    s32 i;

    // Init work
    memset(wp, 0, sizeof(*wp));
    wp->openingBufferSize = 0x4000;
    wp->openingBuffer = __memAlloc(HEAP_MAIN, wp->openingBufferSize);
    wp->saves = __memAlloc(HEAP_MAIN, sizeof(SaveFile)*4);
    wp->tempSaveFile = __memAlloc(HEAP_MAIN, sizeof(SaveFile) + 8); // alignment?
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

asm void nandLoadSave(s32 saveId)
{
    #include "asm/8023efe0.s"
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

#include "jumptable/804e5044.inc"
asm void nandWriteBannerMain()
{
    #include "asm/8023f200.s"
}

#include "jumptable/804e5060.inc"
asm void nandWriteAllSavesMain()
{
    #include "asm/8023f570.s"
}

asm void nandWriteSaveMain()
{
    #include "asm/8023fa24.s"
}

#include "jumptable/804e507c.inc"
asm void nandWriteBannerLoadAllSavesMain()
{
    #include "asm/8023fd64.s"
}

asm void nandDeleteSaveMain()
{
    #include "asm/80240414.s"
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
