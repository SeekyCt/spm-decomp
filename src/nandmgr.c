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

// .data
extern char lbl_804e50a8[]; // TODO: wchar stringbase

// .bss
static NandWork work;

// .sdata
static NandWork * wp = &work;

#define flag(value, mask) (value & mask) // needed for assert

static void genericCallback(s32 result, NANDCommandBlock * commandBlock);
static void checkCallback(s32 result, NANDCommandBlock * commandBlock);

static u32 nandCalcChecksum(SaveFile * save)
{
    u32 checksum = 0;
    for (int i = 0; i < sizeof(*save); i += 2)
    {
        checksum += ((u8 *)save)[i];
        checksum += ((u8 *)save)[i+1]; // doesn't match with just one
    }
    return checksum;
}

#include "jumptable/804e5008.inc"
asm void nandInit()
{
    #include "asm/8023e60c.s"
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
