/*
    Main functions and variables of the game
*/

#ifndef SPMARIO_H
#define SPMARIO_H

#include <common.h>

typedef struct {
    u8 unknown_0x0[0x4 - 0x0];
    u32 unknown_0x4;
    u32 language;
    u8 unknown_0xc[0x10 - 0xc];
    u32 systemLevelFlags;
    u8 unknown_0x14[0xf8 - 0x14];
    s64 time;
    u8 unknown_0x100[0x140 - 0x100];
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