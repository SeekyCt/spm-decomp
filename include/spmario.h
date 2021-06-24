/*
    Main functions and variables of the game
*/

#ifndef SPMARIO_H
#define SPMARIO_H

#include <common.h>

typedef struct {
    u32 flags;
    u32 unknown_0x4;
    s32 language;
    u32 unknown_0xc;
    u32 systemLevelFlags;
    u8 unknown_0x14[0x18 - 0x14];
    u16 framebufferWidth;
    u16 framebufferHeight;
    float unknown_0x1c;
    char saveName[32]; // size needs checking
    u32 unknown_0x40;
    char mapName[32];
    char doorName[32];
    u8 unknown_0x84[0xc8 - 0x84];
    s64 lastSaveUpdateTime; // last time save file in memory was written to ()
    float unknown_0xd0;
    float unknown_0xd4;
    float unknown_0xd8;
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

// New to SPM so no symbols
// TODO: maybe static?
void unknown_801a9068();
void unknown_801a9074();
void unknown_801a9080();
void unknown_801a908c();

#endif
