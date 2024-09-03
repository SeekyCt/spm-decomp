/*
    WARNING: Not fully decompiled
    This file is currently not linked into the final dol
*/

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

extern "C" {

// .bss
SpmarioGlobals spmario;
static OSThread DVDCheckThread;
static u8 dvdCheckStack[0x1000];
static u32 lbl_80528370[4];
static u32 lbl_80528380[4][2];

// .sdata
SpmarioGlobals * gp = &spmario;

// .sbss
static s32 unusedZero;
static s32 startTick;
static bool doResetButtonRestart;
static bool doShutdown;
static bool doHomeButtonRestart;
static bool doReturnToMenu;
static s32 resetCode;
s32 _spmarioSystemLevel;
static OSThread * pMainThread;

static void viPostCallback(u32 retraceCount);
static void wiiDvdCheckThread(void * param);
static void wiiRumbleCheck();

// NOT_DECOMPILED spmarioInit

// NOT_DECOMPILED spmarioMain

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

// NOT_DECOMPILED viPostCallback

// NOT_DECOMPILED wiiDvdCheckThread

// NOT_DECOMPILED wiiRumbleCheck

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

}
