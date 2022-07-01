#pragma once

#include <common.h>
#include <wii/kpad.h>

// Controller stuff used to be in system.c in TTYD & no asserts in this code so name is unnoficial

#define WPAD_FLAG_ENABLE_CURSOR 2

#define WPAD_STATUS_COUNT 0x10

typedef struct
{
    u32 flags;
    u8 unknown_0x4[0x44 - 0x4];
    bool enableRumble[4];
    bool rumblePrev[4]; // rumble enable flags from previous frame
    Vec2 pointingPos[4]; // wii remote pointer positions
    KPADStatus statuses[4][WPAD_STATUS_COUNT];
    s32 kpadReadRet[4];
    KPADStatus pastStatuses[4][60];
    u32 unknown_0x9d3c[4];
    s32 unknown_0x9d4c[4];
    u8 unknown_0x9d5c[0x9d60 - 0x9d5c];
    s64 unknown_0x9d60[4];
    u8 unknown_0x9d80[0x9dc8 - 0x9d80];
} WpadWork;

WpadWork * wpadGetWork();
void wpadInit();
void wpadAllRumbleOff();
void wpadMain();
void wpadCursorOn();
void wpadCursorOff();
bool wpadGetCursorStatus();
void wpadRumbleOn(s32 controller);
void wpadRumbleOff(s32 controller);
bool wpadGetRumbleStatus(s32 controller);
u32 wpadGetButtonsHeld(s32 controller);
u32 wpadGetButtonsReleased(s32 controller);
u32 wpadGetButtonsPressed(s32 controller);
u32 wpadGetButtonsHeldRepeat(s32 controller);
u32 func_802376e4(s32 controller);
void func_80237710(s32 controller);
bool func_80237750();
