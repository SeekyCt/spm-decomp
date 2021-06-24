#ifndef WPADMGR_H
#define WPADMGR_H

#include <common.h>
#include <kpad.h>

// Controller stuff used to be in system.c in TTYD & no asserts in this code so name is unnoficial

#define WPAD_FLAG_ENABLE_CURSOR 2

#define WPAD_STATUS_COUNT 0x10

typedef struct {
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

WpadWork * wpadGetWork(); // 8023697c
void wpadInit(); // 80236998
void wpadAllRumbleOff(); // 80236ab8
void wpadMain(); // 80236af8
void wpadCursorOn(); // 80237618
void wpadCursorOff(); // 8023762c
bool wpadGetCursorStatus(); // 80237640
void wpadRumbleOn(s32 controller); // 80237650
void wpadRumbleOff(s32 controller); // 80237664
bool wpadGetRumbleStatus(s32 controller); // 80237678
u32 wpadGetButtonsHeld(s32 controller); // 80237688
u32 wpadGetButtonsReleased(s32 controller); // 8023769c
u32 wpadGetButtonsPressed(s32 controller); // 802376b0
u32 wpadGetButtonsHeldRepeat(s32 controller); // 802376c4
u32 unknown_802376e4(s32 controller); // 802376e4
void unknown_80237710(s32 controller); // 80237710
// 80237750

#endif
