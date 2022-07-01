/*
    Main functions and variables of the game
*/

#pragma once

#include <common.h>
#include <wii/os.h>
#include <wii/types.h>

typedef struct
{
    u32 flags;
    s32 fps;
    s32 language;
    u32 discIsEjected;
    u32 systemLevelFlags;
    u8 unknown_0x14[0x18 - 0x14];
    u16 framebufferWidth;
    u16 framebufferHeight;
    float gameSpeed;
    char saveName[32];
    s32 lastSaveLoaded;
    char mapName[32];
    char doorName[32];
    u8 unknown_0x84[0xc8 - 0x84];
    OSTime lastSaveUpdateTime; // last time save file in memory was written to
    Vec3 savePosition;
    s32 saveFileId;
    u32 unknown_0xe0;
    u8 unknown_0xe4[0xf8 - 0xe4];
    OSTime time;
    u8 unknown_0x100[0x110 - 0x100];
    OSTime lastSaveLoadTime;
    u8 unknown_0x118[0x138 - 0x118];
    OSTime lastButtonPressTime;
    s32 gsw0;
    u32 gswf[256]; // 8192 flags
    s8 gsw[2048];
    u32 lswf[16]; // 512 flags
    s8 lsw[1024];
    u8 unknown_0x1184[0x900]; // coin related?
    u8 unknown_0x1a84[0x1b08 - 0x1a84];
} SpmarioGlobals;
extern SpmarioGlobals * gp;
extern SpmarioGlobals spmario;

extern s32 _spmarioSystemLevel;

void spmarioInit();
void spmarioMain();
void spmarioDisp();
void spmarioSystemLevel(s32 level);
s32 spmarioGetSystemLevel();
// viPostCallback
// wiiDvdCheckThread
// wiiRumbleCheck
void resetButtonRestart();
void shutdownConsole();
void homeButtonRestart();
void returnToWiiMenu();
