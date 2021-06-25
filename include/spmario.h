/*
    Main functions and variables of the game
*/

#ifndef SPMARIO_H
#define SPMARIO_H

#include <common.h>

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
    int lastSaveLoaded;
    char mapName[32];
    char doorName[32];
    u8 unknown_0x84[0xc8 - 0x84];
    s64 lastSaveUpdateTime; // last time save file in memory was written to
    Vec3 savePosition;
    int saveFileId;
    u32 unknown_0xe0;
    u8 unknown_0xe4[0xf8 - 0xe4];
    s64 time;
    u8 unknown_0x100[0x110 - 0x100];
    s64 lastSaveLoadTime;
    u8 unknown_0x118[0x138 - 0x118];
    s64 lastButtonPressTime;
    s32 gsw0;
    u32 gswf[256]; // 8192 flags
    s8 gsw[2048];
    u32 lswf[16]; // 512 flags
    s8 lsw[1024];
    u8 unknown_0x1184[0x900]; // coin related?
    u8 unknown_0x1a84[0x1b08 - 0x1a84];
} SpmarioGlobals;
extern SpmarioGlobals * gp; // 805ae178
extern SpmarioGlobals spmario; // 80525550

extern s32 _spmarioSystemLevel; // 805ae9c8

void spmarioInit(); // 801a7ddc
void spmarioMain(); // 801a84d8
void spmarioDisp(); // 801a8748
void spmarioSystemLevel(s32 level); // 801a8770
s32 spmarioGetSystemLevel(); // 801a87ec
// viPostCallback // 801a87f4
// wiiDvdCheckThread // 801a898c
// wiiRumbleCheck // 801a8db4
void resetButtonRestart(); // 801a9068
void shutdownConsole(); // 801a9074
void homeButtonRestart(); // 801a9080
void returnToWiiMenu(); // 801a908c

#endif
