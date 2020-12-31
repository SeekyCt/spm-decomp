#include <common.h>
#include <lzss10.h>
#include <memory.h>
#include <nand.h>
#include <nandmgr.h>
#include <string.h>
#include <tpl.h>

static NandWork nandWork;
static NandWork * wp = &nandWork;

#include "files/saveImagesTpl.inc" // static const u8 saveImagesTpl[0x5058]

// Register usage + scheduling in the loop not matching
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
        for (int j = 0; j < sizeof(*curSave); j++) {
            checksum += ((u8 *)curSave)[j];
        }
        curSave->checksum = checksum;
        curSave->checksumNOT = ~checksum;
    }
}
