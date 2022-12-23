#include <common.h>
#include <spm/acdrv.h>
#include <spm/animdrv.h>
#include <spm/bgdrv.h>
#include <spm/camdrv.h>
#include <spm/casedrv.h>
#include <spm/debug.h>
#include <wii/DEMOInit.h>
#include <spm/dispdrv.h>
#include <spm/dvdmgr.h>
#include <spm/evtmgr.h>
#include <spm/effdrv.h>
#include <spm/envdrv.h>
#include <spm/extdrv.h>
#include <spm/fadedrv.h>
#include <spm/filemgr.h>
#include <spm/fontmgr.h>
#include <spm/framedrv.h>
#include <spm/hitdrv.h>
#include <spm/homebuttondrv.h>
#include <spm/hud.h>
#include <spm/icondrv.h>
#include <spm/imgdrv.h>
#include <spm/itemdrv.h>
#include <spm/lightdrv.h>
#include <spm/mapdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/memory.h>
#include <spm/mobjdrv.h>
#include <spm/msgdrv.h>
#include <spm/nandmgr.h>
#include <spm/npcdrv.h>
#include <spm/offscreendrv.h>
#include <spm/pausewin.h>
#include <spm/seqdrv.h>
#include <spm/relmgr.h>
#include <spm/romfont.h>
#include <spm/shadowdrv.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/sptexture.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <spm/windowdrv.h>
#include <spm/winmgr.h>
#include <spm/wpadmgr.h>
#include <wii/dvd.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <wii/os.h>
#include <wii/sc.h>
#include <wii/vi.h>
#include <wii/wpad.h>
#include <nw4r/snd.h>
#include <msl/string.h>

// This file has other data mixed in with the floats, so currently can't use literals in asm

// .rodata
static const f64 lbl_803387f0 = 4.503601774854144E15; // casting float
#include "orderstrings/803387f8_80338834.inc"

// .bss
SpmarioGlobals spmario;
static OSThread DVDCheckThread;
static u8 dvdCheckStack[0x1000];
static u32 lbl_80528370[4];
static u32 lbl_80528380[4][2];

// CW seemingly will only order it this way if a bss pool generates
// Should be removable once a referencing function is decompiled
ORDER_BSS_DATA
{
    ORDER_BSS(spmario)
    ORDER_BSS(DVDCheckThread)
    ORDER_BSS(dvdCheckStack)
}

// .sdata
SpmarioGlobals * gp = &spmario;

// .sbss
static OSThread * pMainThread;
s32 _spmarioSystemLevel;
static s32 resetCode;
static bool doReturnToMenu;
static bool doHomeButtonRestart;
static bool doShutdown;
static bool doResetButtonRestart;
static s32 startTick;
static s32 unusedZero;

// .sdata2
#include "orderfloatsm/805b3d20_805b3d24.inc"
const GXColor lbl_805b3d24 = {0xff, 0xff, 0xff, 0xff};
#include "orderfloatsm/805b3d28_805b3d50.inc"

// .data
// TODO: static GXRenderModeObjs
extern GXRenderModeObj lbl_8042a550;
extern GXRenderModeObj lbl_8042a58c;
extern GXRenderModeObj lbl_8042a5c8;
extern GXRenderModeObj lbl_8042a604;
extern GXRenderModeObj lbl_8042a640;
extern GXRenderModeObj lbl_8042a67c;
extern GXRenderModeObj lbl_8042a6b8;
extern GXRenderModeObj lbl_8042a6f4;
extern GXRenderModeObj lbl_8042a730;
extern GXRenderModeObj lbl_8042a76c;
extern GXRenderModeObj lbl_8042a7a8;
extern GXRenderModeObj lbl_8042a7e4;
extern GXRenderModeObj lbl_8042a820;
extern GXRenderModeObj lbl_8042a85c;

static void viPostCallback(u32 retraceCount);
static void wiiDvdCheckThread(void * param);
static void wiiRumbleCheck();

#include "jumptable/8042a898.inc"
#include "jumptable/8042a8b4.inc"
asm void spmarioInit()
{
    #include "asm/801a7ddc.s"
}

asm void spmarioMain()
{
    #include "asm/801a84d8.s"
}

void spmarioDisp()
{
    camDraw();
    g_bFirstSmartAlloc = false;
}

void spmarioSystemLevel(s32 level)
{
    switch (level)
    {
        case 0:
            _spmarioSystemLevel = level;
            gp->systemLevelFlags &= ~3;
            evtStartAll(0xff);
            break;

        case 1:
            _spmarioSystemLevel = level;
            gp->systemLevelFlags |= 1;
            evtStopAll(7);
            break;

        case 2:
            _spmarioSystemLevel = level;
            gp->systemLevelFlags |= 3;
            evtStopAll(0xff);
            break;
    }
}

s32 spmarioGetSystemLevel()
{
    return _spmarioSystemLevel;
}

asm void viPostCallback(u32 retraceCount)
{
    #include "asm/801a87f4.s"
}

#include "jumptable/8042a8d4.inc"
asm void wiiDvdCheckThread(void * param)
{
    #include "asm/801a898c.s"
}

asm void wiiRumbleCheck()
{
    #include "asm/801a8db4.s"
}

void resetButtonRestart()
{
    doResetButtonRestart = true;
}

void shutdownConsole()
{
    doShutdown = true;
}

void homeButtonRestart()
{
    doHomeButtonRestart = true;
}

void returnToWiiMenu()
{
    doReturnToMenu = true;
}
