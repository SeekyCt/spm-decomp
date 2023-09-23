/*
    spmario contains the main functions and variables of the game
    In TTYD, this file was named mariost
*/

#pragma once

#include <common.h>
#include <spm/swdrv.h>
#include <wii/os.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::spmario)

USING(spm::swdrv::SwCoinEntry)
USING(wii::os::OSThread)
USING(wii::os::OSTime)
USING(wii::mtx::Vec3)

#define SPMARIO_FLAG_1 1
#define SPMARIO_FLAG_2 2
#define SPMARIO_FLAG_4 4
#define SPMARIO_FLAG_8 8

typedef struct
{
/* 0x0000 */ u32 flags;
/* 0x0004 */ s32 fps; // not read from save file
/* 0x0008 */ s32 language; // not read from save file
/* 0x000C */ u32 discIsEjected;
/* 0x0010 */ u32 systemLevelFlags;
/* 0x0014 */ u8 unknown_0x14[0x18 - 0x14];
/* 0x0018 */ u16 framebufferWidth;
/* 0x001A */ u16 framebufferHeight;
/* 0x001C */ f32 gameSpeed;
/* 0x0020 */ char saveName[32];
/* 0x0040 */ s32 lastSaveLoaded; // not read from save file
/* 0x0044 */ char mapName[32];
/* 0x0064 */ char doorName[32];
/* 0x0084 */ char gameOverMapName[32];
/* 0x00A4 */ char prevMapName[32];
/* 0x00C4 */ u8 unknown_0xc4[0xc8 - 0xc4];
/* 0x00C8 */ OSTime lastSaveUpdateTime; // last time save file in memory was written to
/* 0x00D0 */ Vec3 savePosition;
/* 0x00DC */ s32 saveFileId;
/* 0x00E0 */ u32 unknown_0xe0; // not read from save file
/* 0x00E4 */ u8 unknown_0xe4[0xf8 - 0xe4];
/* 0x00F8 */ OSTime time;
/* 0x0100 */ OSTime timePlayed;
/* 0x0108 */ u8 unknown_0x108[0x110 - 0x108];
/* 0x0110 */ OSTime lastSaveLoadTime;
/* 0x0118 */ u8 unknown_0x118[0x138 - 0x118];
/* 0x0138 */ OSTime lastButtonPressTime; // uncertain
/* 0x0140 */ s32 gsw0; // first GSW is a full word
/* 0x0144 */ u32 gswf[256]; // 8192 flags
/* 0x0544 */ s8 gsw[2048]; // index 0 used in favour of gsw0
/* 0x0D44 */ u32 lswf[16]; // 512 flags
/* 0x0D84 */ u8 lsw[1024];
/* 0x1184 */ SwCoinEntry coinEntries[32];
/* 0x1A84 */ u8 unknown_0x1a84[0x1b00 - 0x1a84];
/* 0x1B00 */ bool disableRumble[4]; // overrides to force off wpadmgr's enableRumble,
                                    // index controller id
/* 0x1B04 */ u8 unknown_0x1b04[0x1b08 - 0x1b04];
} SpmarioGlobals;
SIZE_ASSERT(SpmarioGlobals, 0x1b08)
extern SpmarioGlobals * gp;
extern SpmarioGlobals spmario;

DECOMP_STATIC(OSThread * spmario_pMainThread)
extern s32 _spmarioSystemLevel;
DECOMP_STATIC(s32 spmario_resetCode)
DECOMP_STATIC(s8 spmario_doReturnToMenu)
DECOMP_STATIC(s8 spmario_doHomeButtonRestart)
DECOMP_STATIC(s8 spmario_doShutdown)
DECOMP_STATIC(s8 spmario_doResetButtonRestart)
DECOMP_STATIC(s32 spmario_startTick)
DECOMP_STATIC(s32 spmario_unusedZero)

void spmarioInit();
void spmarioMain();
void spmarioDisp();
void spmarioSystemLevel(s32 level);
s32 spmarioGetSystemLevel();
DECOMP_STATIC(UNKNOWN_FUNCTION(spmario_viPostCallback))
DECOMP_STATIC(UNKNOWN_FUNCTION(spmario_wiiDvdCheckThread))
DECOMP_STATIC(UNKNOWN_FUNCTION(spmario_wiiRumbleCheck))
void resetButtonRestart();
void shutdownConsole();
void homeButtonRestart();
void returnToWiiMenu();

CPP_WRAPPER_END()
