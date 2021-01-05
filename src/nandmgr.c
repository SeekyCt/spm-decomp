#include <common.h>
#include <evtmgr_cmd.h>
#include <lzss10.h>
#include <memory.h>
#include <nand.h>
#include <nandmgr.h>
#include <os.h>
#include <somewhere.h>
#include <string.h>
#include <system.h>
#include <tpl.h>

#define flag(value, mask) (value & mask) // needed for assert

static NandWork nandWork;
static NandWork * wp = &nandWork;

#include "files/saveImagesTpl.inc" // static const u8 saveImagesTpl[0x5058]

// Register usage not matching, might need nandClearSave inlined?
void nandInit() {
    memset(wp, 0, sizeof(*wp));
    wp->openingBufferSize = 0x4000;
    wp->openingBuffer = __memAlloc(0, wp->openingBufferSize);
    wp->saves = __memAlloc(0, sizeof(SaveFile) * 4);
    wp->unknown_0x14 = __memAlloc(0, 0x25c0);
    wp->banner = __memAlloc(0, sizeof(NANDBanner));
    switch (gp->language) {
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
    for (int i = 0; i < 4; i++) {
        SaveFile * curSave = &wp->saves[i];
        memset(curSave, 0, sizeof(*curSave));
        curSave->flags |= 1;
        curSave->checksum = 0;
        curSave->checksumNOT = 0xffffffff;
        u32 checksum = 0;
        for (int j = 0; j < sizeof(*curSave); j += 2) {
            checksum += ((u8 *)curSave)[j];
            checksum += ((u8 *)curSave)[j+1];
        }
        curSave->checksum = checksum;
        curSave->checksumNOT = ~checksum;
    }
}

void nandMain() {
    switch (wp->task) {
        case NANDMGR_TASK_NONE:
            return;
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
    }
}

bool nandIsExec() {
    if (wp->flag & NAND_FLAG_Exec)
        return 1;
    else
        return (bool) (wp->flag & NAND_FLAG_Waiting);
}

s32 nandGetCode() {
    return wp->code;
}

SaveFile * nandGetSaveFiles() {
    return wp->saves;
}

void nandCheck() {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_CHECK;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0;
}

void nandWriteBanner() {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_WRITE_BANNER;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0;
}

void nandWriteAllSaves() {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_WRITE_ALL_SAVES;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0;
}

void nandWriteSave(s32 saveId) {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_WRITE_SAVE;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = saveId;
}

void nandWriteBannerLoadAllSaves() {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_WRITE_BANNER_LOAD_ALL_SAVES;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0;
}

void nandDeleteSave(s32 saveId) {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_DELETE_SAVE;
    wp->code = 0;
    wp->stage = 0;
    wp->saveId = 0; // bug
    (void) saveId;
}

void nandCopySave(s32 sourceId, s32 destId) {
    memcpy(&wp->saves[destId], &wp->saves[sourceId], sizeof(SaveFile));
}

void nandClearSave(s32 saveId) {
    SaveFile * save = &wp->saves[saveId];
    memset(save, 0, sizeof(*save));
    save->flags |= 1;
    save->checksum = 0;
    save->checksumNOT = 0xffffffff;
    u32 checksum = 0;
    for (int i = 0; i < sizeof(*save); i += 2) {
        checksum += ((u8 *)save)[i];
        checksum += ((u8 *)save)[i+1]; // scheduling doesn't match with just one
    }
    save->checksum = checksum;
    save->checksumNOT = ~checksum;
}

void nandUpdateSave(s32 saveId) {
    SaveFile * save = &wp->saves[saveId];
    UnkStruct80510458 * r29 = unknown_0x80121f48();
    memset(save, 0, sizeof(SaveFile));
    SpmarioGlobals * _gp = gp; // a sign of inlining?
    _gp->lastSaveUpdateTime = OSGetTime();
    _gp->unknown_0xd0 = r29->unknown_0x5c;
    _gp->unknown_0xd4 = r29->unknown_0x60;
    _gp->unknown_0xd8 = r29->unknown_0x64;
    evtSetValue(NULL, GSWF(401), (s32) ((r29->unknown_0x4 >> 21) & 1));
    evtSetValue(NULL, GSWF(402), (s32) ((r29->unknown_0x4 >> 19) & 1));
    s32 temp = (*unknown_0x8011fa2c() & 0x80);
    evtSetValue(NULL, GSWF(403), temp == 0);
    evtSetValue(NULL, GSWF(404), (s32) ((r29->unknown_0x8 >> 23) & 1));
    evtSetValue(NULL, GSW(23), r29->unknown_0x38);
    gp->flags &= ~1;
    gp->flags &= ~2;
    gp->flags &= ~4;
    gp->flags &= ~8;
    gp->unknown_0xc = 0;
    gp->unknown_0x1c = 1.0f;
    memcpy(&save->spmarioGlobals, gp, sizeof(SpmarioGlobals));
    memcpy(&save->unknown_0x1b10, unknown_0x8014c088(), sizeof(save->unknown_0x1b10));
    memcpy(&save->unknown_0x21b0, unknown_0x801c9adc()->unknown_0x728, sizeof(save->unknown_0x21b0));
    save->flags &= ~3;
    save->checksum = 0;
    save->checksumNOT = 0xffffffff;
    u32 checksum = 0;
    for (int i = 0; i < sizeof(*save); i += 2) {
        checksum += ((u8 *)save)[i];
        checksum += ((u8 *)save)[i+1];
    }
    save->checksum = checksum;
    save->checksumNOT = ~checksum;
}

// Register usage not matching
void nandLoadSave(s32 saveId) {
    SaveFile * save = &wp->saves[saveId];
    s32 language = gp->language;
    u32 unk4 = gp->unknown_0x4;
    u32 unk40 = gp->unknown_0x40;
    memcpy(gp, &save->spmarioGlobals, sizeof(SpmarioGlobals));
    memcpy(unknown_0x8014c088(), &save->unknown_0x1b10, sizeof(save->unknown_0x1b10));
    memcpy(unknown_0x801c9adc()->unknown_0x728, &save->unknown_0x21b0, sizeof(save->unknown_0x21b0));
    gp->unknown_0xc = 0;
    SpmarioGlobals * _gp = gp; // probably another inline function
    _gp->lastSaveLoadTime = OSGetTime();
    _gp->unknown_0x1c = 1.0f;
    gp->unknown_0x40 = unk40;
    gp->language = language;
    gp->unknown_0x4 = unk4;
    gp->unknown_0xe0 = 0;
}

void nandDisableSaving() {
    wp->flag |= NAND_FLAG_NoSave;
}

void nandEnableSaving() {
    wp->flag &= ~NAND_FLAG_NoSave;
}

bool nandCheckSaving() {
    return (wp->flag & NAND_FLAG_NoSave) != 0;
}

void nandCheckMain() {
    if (wp->flag & NAND_FLAG_Waiting) return;

    if (wp->code != NAND_CODE_OK) {
        wp->task = 0;
        wp->flag &= ~NAND_FLAG_Exec;
        wp->flag |= 4;
    }
    else {
        switch (wp->stage) {
            case 0:
                wp->flag |= NAND_FLAG_Waiting;
                s32 tries = 0;
                wp->answer = 0;
                while (NANDCheckAsync(NAND_BLOCK_COUNT, NAND_INODE_COUNT,
                                      &wp->answer, &checkCallback, &wp->commandBlock) == NAND_CODE_BUSY) {
                    if (++tries > NAND_ATTEMPTS_MAX) {
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

// void nandWriteBannerMain();
// void nandWriteAllSavesMain();
// void nandWriteSaveMain();
// void nandWriteBannerLoadAllSavesMain();
// void nandDeleteSaveMain();

void genericCallback(s32 result, NANDCommandBlock * commandBlock) {
    (void) commandBlock;
    if (result > 0) {
        result = 0;
    }
    wp->flag &= ~NAND_FLAG_Waiting;
    wp->code = result;
}

void checkCallback(s32 result, NANDCommandBlock * commandBlock) {
    (void) commandBlock;
    wp->flag &= ~NAND_FLAG_Waiting;
    wp->code = result;
    if (wp->code != 0) return;
    if (wp->answer & 0xa) wp->code = -11;
    if (wp->answer & 5) wp->code = -9;
}

#undef flag