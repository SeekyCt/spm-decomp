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
#include <nw4r/snd.hpp>
#include <msl/string.h>

extern "C" {

// .bss
SpmarioGlobals spmario;
static OSThread DVDCheckThread;
static u8 dvdCheckStack[0x1000];
static u32 lbl_80528370[4];
static OSTime lbl_80528380[4];

// Render modes for each supported TV format, paired as 4:3 and 16:9.
#define SPM_SAMPLE_PATTERN \
    {{6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, \
     {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}}
#define SPM_VFILTER {8, 8, 10, 12, 10, 8, 8}
#define SPM_RENDER_MODE(tvMode, xfbHeight, xOrigin, yOrigin, viWidth, viHeight, xfbMode) \
    {tvMode, 608, 480, xfbHeight, xOrigin, yOrigin, viWidth, viHeight, xfbMode, 0, 0, \
     SPM_SAMPLE_PATTERN, SPM_VFILTER, {0, 0, 0}}

static GXRenderModeObj renderMode0 = SPM_RENDER_MODE(0, 480, 30, 0, 660, 480, 1);
static GXRenderModeObj renderMode1 = SPM_RENDER_MODE(0, 480, 17, 0, 686, 480, 1);
static GXRenderModeObj renderMode2 = SPM_RENDER_MODE(2, 480, 30, 0, 660, 480, 0);
static GXRenderModeObj renderMode3 = SPM_RENDER_MODE(2, 480, 17, 0, 686, 480, 0);
static GXRenderModeObj renderMode4 = SPM_RENDER_MODE(4, 568, 30, 6, 660, 568, 1);
static GXRenderModeObj renderMode5 = SPM_RENDER_MODE(4, 568, 19, 6, 682, 568, 1);
static GXRenderModeObj renderMode6 = SPM_RENDER_MODE(20, 480, 30, 0, 660, 480, 1);
static GXRenderModeObj renderMode7 = SPM_RENDER_MODE(20, 480, 17, 0, 686, 480, 1);
static GXRenderModeObj renderMode8 = SPM_RENDER_MODE(22, 480, 30, 0, 660, 480, 0);
static GXRenderModeObj renderMode9 = SPM_RENDER_MODE(22, 480, 17, 0, 686, 480, 0);
static GXRenderModeObj renderMode10 = SPM_RENDER_MODE(8, 480, 30, 0, 660, 480, 1);
static GXRenderModeObj renderMode11 = SPM_RENDER_MODE(8, 480, 17, 0, 686, 480, 1);
static GXRenderModeObj renderMode12 = SPM_RENDER_MODE(10, 480, 30, 0, 660, 480, 0);
static GXRenderModeObj renderMode13 = SPM_RENDER_MODE(10, 480, 17, 0, 686, 480, 0);

#undef SPM_RENDER_MODE
#undef SPM_VFILTER
#undef SPM_SAMPLE_PATTERN

// .sdata
SpmarioGlobals * gp = &spmario;

// .sbss
static bool unusedZero;
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

void spmarioInit()
{
    bool widescreen = SCGetAspectRatio() == 1;

    VIInit();

#define INIT_DEMO(wideMode, standardMode) \
    do \
    { \
        DemoUseMEMHeap = 0; \
        GXRenderModeObj * renderMode = &renderMode##wideMode; \
        if (!widescreen) \
            renderMode = &renderMode##standardMode; \
        DEMOInit(renderMode); \
    } while (0)

    if (((u32) OSGetResetCode() >> 31) == 0)
    {
        if (SCGetProgressiveMode() == 1 && VIGetDTVStatus())
        {
            if (VIGetTvFormat() == 0)
            {
                if (VIGetScanMode() == 2)
                {
                    INIT_DEMO(3, 2);
                }
                else
                {
                    VISetVSyncTimingTest();
                    INIT_DEMO(3, 2);

                    for (s32 i = 0; i < 100; i++)
                        VIWaitForRetrace();
                }

                resetCode = 1;
            }
            else if (VIGetTvFormat() == 1 || VIGetTvFormat() == 5)
            {
                if (VIGetScanMode() == 2)
                {
                    INIT_DEMO(9, 8);
                }
                else
                {
                    VISetVSyncTimingTest();
                    INIT_DEMO(9, 8);

                    for (s32 i = 0; i < 100; i++)
                        VIWaitForRetrace();
                }

                resetCode = 3;
            }
            else
            {
                if (VIGetScanMode() == 2)
                {
                    INIT_DEMO(13, 12);
                }
                else
                {
                    VISetVSyncTimingTest();
                    INIT_DEMO(13, 12);

                    for (s32 i = 0; i < 100; i++)
                        VIWaitForRetrace();
                }

                resetCode = 6;
            }
        }
        else if (VIGetTvFormat() == 0)
        {
            INIT_DEMO(1, 0);
            resetCode = 2;
        }
        else if (VIGetTvFormat() == 1)
        {
            INIT_DEMO(5, 4);
            resetCode = 5;
        }
        else if (VIGetTvFormat() == 5)
        {
            INIT_DEMO(7, 6);
            resetCode = 4;
        }
        else
        {
            INIT_DEMO(11, 10);
            resetCode = 7;
        }
    }
    else
    {
        resetCode = OSGetResetCode() & 0x3fffffff;

        switch (resetCode)
        {
            case 1:
                INIT_DEMO(3, 2);
                break;
            case 2:
                INIT_DEMO(1, 0);
                break;
            case 3:
                INIT_DEMO(9, 8);
                break;
            case 4:
                INIT_DEMO(7, 6);
                break;
            case 5:
                INIT_DEMO(5, 4);
                break;
            case 6:
                INIT_DEMO(13, 12);
                break;
            case 7:
                INIT_DEMO(11, 10);
                break;
        }
    }

#undef INIT_DEMO

    asm
    {
        li r3, 4
        oris r3, r3, 4
        mtspr GQR2, r3
        li r3, 5
        oris r3, r3, 5
        mtspr GQR3, r3
        li r3, 6
        oris r3, r3, 6
        mtspr GQR4, r3
        li r3, 7
        oris r3, r3, 7
        mtspr GQR5, r3
    }

    doResetButtonRestart = false;
    OSSetResetCallback(resetButtonRestart);
    doShutdown = false;
    OSSetPowerCallback(shutdownConsole);
    doHomeButtonRestart = false;
    doReturnToMenu = false;

    sysInitMutex();
    sysRandInit();
    unusedZero = false;
    startTick = (s32) OSGetTick();
    wpadInit();

    GXColor clearColour = {0, 0, 0, 0};
    GXSetCopyClear(&clearColour, 0x00ffffff);

    memset(gp, 0, sizeof(*gp));
    gp->framebufferWidth = 608;
    gp->framebufferHeight = 480;
    gp->timePlayed = 0;
    gp->frameTimeDelta = 0;
    gp->lastFrameTime = OSGetTime();
    gp->time = 0;
    *(OSTime *) &gp->unknown_0x128[8] = 0;
    *(OSTime *) &gp->unknown_0x128[0] = 0;
    gp->systemTime = 0;
    gp->gameTime = 0;

    if (VIGetTvFormat() == 0)
        gp->fps = 60;
    else if (VIGetTvFormat() == 5)
        gp->fps = 60;
    else
        gp->fps = 50;

    switch (SCGetLanguage())
    {
        case SC_LANGUAGE_JP:
            gp->language = 2;
            break;
        case SC_LANGUAGE_EN:
            gp->language = 2;
            break;
        case SC_LANGUAGE_GE:
            gp->language = 3;
            break;
        case SC_LANGUAGE_FR:
            gp->language = 4;
            break;
        case SC_LANGUAGE_SP:
            gp->language = 5;
            break;
        case SC_LANGUAGE_IT:
            gp->language = 6;
            break;
        case SC_LANGUAGE_DU:
            gp->language = 2;
            break;
    }

    if (gp->language == 0)
        strcpy(gp->saveName, "デフォルト");
    else
        strcpy(gp->saveName, "default");

    gp->systemLevelFlags = 0;
    gp->gameSpeed = 1.0f;

    VIRetraceCallback p = VISetPostRetraceCallback(viPostCallback);
    SPM_ASSERT(745, !p, "すでになんか登録されてます\n\0\0\0\0");

    romFontInit();
    pMainThread = OSGetCurrentThread();
    OSCreateThread(&DVDCheckThread, (ThreadFunc *) wiiDvdCheckThread, NULL,
                   &dvdCheckStack[sizeof(dvdCheckStack)], sizeof(dvdCheckStack), 8, OS_THREAD_DETACHED);
    OSResumeThread(&DVDCheckThread);

    DVDSetAutoFatalMessaging(true);
    DVDMgrInit();
    debugInit();
    spsndInit();
    fileInit();
    smartInit();
    homebuttonInit();
    sptextureInit();
    dispInit();
    camInit();
    fontmgrInit();
    iconInit();
    windowInit();
    mapInit();
    hitInit();
    itemInit();
    npcInit();
    fadeInit();
    bgInit();
    shadowInit();
    evtmgrInit();
    animInit();
    msgInit();
    mobjInit();
    effInit();
    marioInit();
    pouchInit();
    caseInit();
    frameInit();
    imgInit();
    lightInit();
    offscreenInit();
    extInit();
    swInit();
    nandInit();
    envInit();
    winMgrInit();
    hudInit();
    pausewinInit();
    acInit();
    relInit();
    seqInit_SPMARIO();
    seqSetSeq(0, NULL, NULL);
}

void spmarioMain()
{
    wpadMain();

    if (!homebuttonMain())
    {
        seqMain();
        nandMain();
        sptextureMain();
        msgdrvMain();
        bgMain();
        shadowMain();
        animMain();
        camMain();
        windowMain();
        mapMain();
        hitMain();
        evtmgrMain();
        hitRecalcMatrices();
        mapDisp();
        marioMain();
        pouchMain();
        mobjMain();
        fadeMain();
        frameMain();
        imgMain();
        lightMain();
        offscreenMain();
        itemMain();
        npcMain();
        caseMain();
        iconMain();
        extMain();
        envMain();
        winMgrMain();
        hudMain();
        pausewinMain();
        acMain();
        effMain();
        relMain();
    }

    spsndMain();

    if (DVDGetDriveStatus() == -1 && doResetButtonRestart)
    {
        doResetButtonRestart = false;
        OSSetResetCallback(resetButtonRestart);
    }

    if (doShutdown && DVDGetDriveStatus() == -1 && doShutdown)
        doShutdown = false;

    if (doResetButtonRestart || doHomeButtonRestart)
    {
        VISetBlack(true);
        VIFlush();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        wpadAllRumbleOff(1);
        spsndExit();
        DVDMgrDelete();

        if (OSGetCurrentThread() != &DVDCheckThread)
            OSCancelThread(&DVDCheckThread);

        while (nandIsExec())
        {
            nandMain();
            VIWaitForRetrace();
        }

        OSRestart(resetCode);
        while (true) {}
    }

    if (doShutdown)
    {
        VISetBlack(true);
        VIFlush();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        wpadAllRumbleOff(0);
        spsndExit();
        DVDMgrDelete();

        if (OSGetCurrentThread() != &DVDCheckThread)
            OSCancelThread(&DVDCheckThread);

        while (nandIsExec())
        {
            nandMain();
            VIWaitForRetrace();
        }

        OSShutdownSystem();
        while (true) {}
    }

    if (doReturnToMenu)
    {
        VISetBlack(true);
        VIFlush();
        VIWaitForRetrace();
        VIWaitForRetrace();
        VIWaitForRetrace();
        wpadAllRumbleOff(1);
        spsndExit();
        DVDMgrDelete();

        if (OSGetCurrentThread() != &DVDCheckThread)
            OSCancelThread(&DVDCheckThread);

        while (nandIsExec())
        {
            nandMain();
            VIWaitForRetrace();
        }

        OSReturnToMenu();
        while (true) {}
    }
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

#pragma warn_implicitconv off
static void viPostCallback(u32 retraceCount)
{
    (void) retraceCount;

    OSTime now = OSGetTime();
    gp->frameTimeDelta = now - gp->lastFrameTime;
    gp->frameTimeDelta = (OSTime) (gp->gameSpeed * (f32) gp->frameTimeDelta);

    if (!gp->discIsEjected && !func_80242198())
    {
        gp->timePlayed += gp->frameTimeDelta;

        OSTime maxTime = (OSTime) OSClockSpeed * 359999999;
        if (gp->timePlayed > maxTime)
            gp->timePlayed = maxTime;

        if (!(gp->systemLevelFlags & SPMARIO_FLAG_2))
            gp->systemTime += gp->frameTimeDelta;

        if (!(gp->systemLevelFlags & SPMARIO_FLAG_1))
            gp->gameTime += gp->frameTimeDelta;

        gp->time += gp->frameTimeDelta;
    }

    gp->lastFrameTime = now;
    gp->frameCounter++;
    wiiRumbleCheck();
}
#pragma warn_implicitconv on

static void wiiDvdCheckThread(void * param)
{
    (void) param;

    bool widescreen;
    bool discErrorActive = false;
    widescreen = SCGetAspectRatio() == 1;

    while (true)
    {
        s32 messageId = 0;
        switch ((u32) DVDGetDriveStatus())
        {
            case 1:
                break;

            case 4:
                gp->discIsEjected = true;
                messageId = 1;
                break;

            case 6:
                gp->discIsEjected = true;
                messageId = 1;
                break;

            case 11:
                messageId = 2;
                gp->discIsEjected = true;
                break;

            case 12:
                messageId = 4;
                gp->discIsEjected = true;
                break;

            default:
                gp->discIsEjected = false;
                break;
        }

        if (gp->discIsEjected && nandIsExec())
            gp->discIsEjected = false;

        if (!discErrorActive)
        {
            if (gp->discIsEjected)
            {
                OSSuspendThread(pMainThread);
                void * soundManager = func_802f4d0c();
                func_802f54a0(soundManager, 0.0f, 100);
                discErrorActive = true;

                for (s32 controller = 0; controller < 4; controller++)
                    WPADControlMotor(controller, 0);
            }
        }
        else if (!gp->discIsEjected)
        {
            OSResumeThread(pMainThread);
            void * soundManager = func_802f4d0c();
            func_802f54a0(soundManager, 1.0f, 100);
            discErrorActive = false;
        }

        if (gp->discIsEjected)
        {
            DEMOBeforeRender();

            if (messageId != 0)
            {
                const char * message = romFontGetMessage(messageId);
                s32 width = (s32) (f32) romFontGetWidth(message);
                Mtx44 projection;

                if (widescreen)
                {
                    C_MTXOrtho(projection, 228.0f, -228.0f, -416.0f, 416.0f, 1.0f, 1000.0f);
                    GXSetProjection(projection, GX_ORTHOGRAPHIC);
                }
                else
                {
                    C_MTXOrtho(projection, 228.0f, -228.0f, -304.0f, 304.0f, 1.0f, 1000.0f);
                    GXSetProjection(projection, GX_ORTHOGRAPHIC);
                }

                GXColor white = {0xff, 0xff, 0xff, 0xff};
                romFontPrintGX(-width / 2.0f, 60.0f, 1.0f, &white, message);
            }

            DEMODoneRender();
            wpadMain();

            if (DVDGetDriveStatus() == -1 && doResetButtonRestart)
            {
                doResetButtonRestart = false;
                OSSetResetCallback(resetButtonRestart);
            }

            if (doShutdown && DVDGetDriveStatus() == -1 && doShutdown)
                doShutdown = false;

            if (doResetButtonRestart || doHomeButtonRestart)
            {
                VISetBlack(true);
                VIFlush();
                VIWaitForRetrace();
                VIWaitForRetrace();
                VIWaitForRetrace();
                wpadAllRumbleOff(1);
                spsndExit();
                DVDMgrDelete();

                if (OSGetCurrentThread() != &DVDCheckThread)
                    OSCancelThread(&DVDCheckThread);

                while (nandIsExec())
                {
                    nandMain();
                    VIWaitForRetrace();
                }

                OSReturnToMenu();
                while (true) {}
            }

            if (doShutdown)
            {
                VISetBlack(true);
                VIFlush();
                VIWaitForRetrace();
                VIWaitForRetrace();
                VIWaitForRetrace();
                wpadAllRumbleOff(0);
                spsndExit();
                DVDMgrDelete();

                if (OSGetCurrentThread() != &DVDCheckThread)
                    OSCancelThread(&DVDCheckThread);

                while (nandIsExec())
                {
                    nandMain();
                    VIWaitForRetrace();
                }

                OSShutdownSystem();
                while (true) {}
            }

            if (doReturnToMenu)
            {
                VISetBlack(true);
                VIFlush();
                VIWaitForRetrace();
                VIWaitForRetrace();
                VIWaitForRetrace();
                wpadAllRumbleOff(1);
                spsndExit();
                DVDMgrDelete();

                if (OSGetCurrentThread() != &DVDCheckThread)
                    OSCancelThread(&DVDCheckThread);

                while (nandIsExec())
                {
                    nandMain();
                    VIWaitForRetrace();
                }

                OSReturnToMenu();
                while (true) {}
            }
        }

        VIWaitForRetrace();
    }
}

static void wiiRumbleCheck()
{
    u32 disableRumble[4] = {};
    OSTime now = OSGetTime();

    for (s32 controller = 0; controller < 4; controller++)
    {
        if (!gp->rumbleCooldown[controller])
        {
            u8 rumbleEnabled = (u8) wpadGetRumbleStatus(controller);
            if (rumbleEnabled == true)
            {
                if (!gp->rumbleStarted[controller])
                {
                    gp->rumbleStarted[controller] = true;
                    gp->rumbleStartTime[controller] = now;
                }

                if (OSTicksToMilliseconds((OSTime) ((s32) now - (s32) gp->rumbleStartTime[controller])) > 30000)
                {
                    if (!gp->rumbleCooldownStarted[controller])
                    {
                        gp->rumbleCooldownStarted[controller] = true;
                        gp->rumbleCooldown[controller] = true;
                        gp->rumbleCooldownStartTime[controller] = now;
                    }
                }

                lbl_80528370[controller] = false;
            }
            else if (gp->rumbleStarted[controller])
            {
                if (!lbl_80528370[controller])
                {
                    lbl_80528370[controller] = true;
                    lbl_80528380[controller] = now;
                }

                if (lbl_80528370[controller]
                    && OSTicksToMilliseconds((OSTime) ((s32) now - (s32) lbl_80528380[controller])) > 100)
                {
                    lbl_80528370[controller] = false;
                    gp->rumbleStarted[controller] = false;
                }
            }
        }
        else
        {
            if (OSTicksToMilliseconds((OSTime) ((s32) now - (s32) gp->rumbleCooldownStartTime[controller])) > 30000)
            {
                gp->rumbleCooldownStarted[controller] = false;
                gp->rumbleCooldown[controller] = false;
                gp->rumbleStarted[controller] = false;
            }

            disableRumble[controller] = true;
        }

        if (gp->discIsEjected)
            disableRumble[controller] = true;
        if (func_80242198())
            disableRumble[controller] = true;
        if ((u32) _spmarioSystemLevel > 1)
            disableRumble[controller] = true;

        if (disableRumble[controller])
            gp->disableRumble[controller] = true;
        else
            gp->disableRumble[controller] = false;
    }
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

}
