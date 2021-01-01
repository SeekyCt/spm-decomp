#include <common.h>
#include <lzss10.h>
#include <memory.h>
#include <nand.h>
#include <nandmgr.h>
#include <string.h>
#include <system.h>
#include <tpl.h>

#define flag(value, mask) (value & mask) // needed for assert

static NandWork nandWork;
static NandWork * wp = &nandWork;

#include "files/saveImagesTpl.inc" // static const u8 saveImagesTpl[0x5058]

// Register usage not matching, might need nandmgrInitSave inlined?
void nandmgrInit() {
    memset(wp, 0, sizeof(*wp));
    wp->unknown_0x4_size = 0x4000;
    wp->unknown_0x4 = __memAlloc(0, wp->unknown_0x4_size);
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
    wp->unknown_0x1a0 = 0x96a0;
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

void nandmgrMain() {
    switch (wp->task) {
        case NANDMGR_TASK_NONE:
            return;
        case NANDMGR_TASK_1:
            nandmgrTask1();
            break;
        case NANDMGR_TASK_2:
            nandmgrTask2();
            break;
        case NANDMGR_TASK_3:
            nandmgrTask3();
            break;
        case NANDMGR_TASK_4:
            nandmgrTask4();
            break;
        case NANDMGR_TASK_5:
            nandmgrTask5();
            break;
        case NANDMGR_TASK_6:
            nandmgrTask6();
            break;
    }
}

bool nandmgrCheckFlag1Or2() {
    if (wp->flag & 1)
        return 1;
    else
        return (bool) (wp->flag & 2);
}

s32 nandmgrGet1b4() {
    return wp->unknown_0x1b4;
}

SaveFile * nandmgrGetSaveFiles() {
    return wp->saves;
}

void nandmgrStartTask1() {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_1;
    wp->unknown_0x1b4 = 0;
    wp->unknown_0x1b0 = 0;
    wp->unknown_0x1b8 = 0;
}

void nandmgrStartTask2() {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_2;
    wp->unknown_0x1b4 = 0;
    wp->unknown_0x1b0 = 0;
    wp->unknown_0x1b8 = 0;
}

void nandmgrStartTask3() {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_3;
    wp->unknown_0x1b4 = 0;
    wp->unknown_0x1b0 = 0;
    wp->unknown_0x1b8 = 0;
}


void nandmgrStartTask4(u32 unknown_1b8) {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_4;
    wp->unknown_0x1b4 = 0;
    wp->unknown_0x1b0 = 0;
    wp->unknown_0x1b8 = unknown_1b8;
}

void nandmgrStartTask5() {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_5;
    wp->unknown_0x1b4 = 0;
    wp->unknown_0x1b0 = 0;
    wp->unknown_0x1b8 = 0;
}

void nandmgrStartTask6() {
    // "Already running"
    assert(!flag(wp->flag, NAND_FLAG_Exec), "すでに実行中");
    wp->flag = NAND_FLAG_Exec;
    wp->task = NANDMGR_TASK_6;
    wp->unknown_0x1b4 = 0;
    wp->unknown_0x1b0 = 0;
    wp->unknown_0x1b8 = 0;
}

void nandmgrCopySave(s32 sourceId, s32 destId) {
    memcpy(&wp->saves[destId], &wp->saves[sourceId], sizeof(SaveFile));
}

void nandmgrInitSave(s32 saveId) {
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

#undef flag