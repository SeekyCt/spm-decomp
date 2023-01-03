/*
    wpadmgr handles controller inputs & vibration 

    The game reads up to 16 KPADStatuses for each controller but only ever seems to use the latest
*/

#pragma once

#include <common.h>
#include <wii/kpad.h>

CPP_WRAPPER(spm::wpadmgr)

USING(wii::mtx::Vec2)
USING(wii::kpad::KPADStatus)

// Controller stuff used to be in system.c in TTYD & no asserts in this code so name is unnoficial

#define WPAD_FLAG_ENABLE_CURSOR 2

#define WPAD_STATUS_COUNT 0x10

typedef struct
{
/* 0x0000 */ u32 flags;
/* 0x0004 */ u8 unknown_0x4[0x44 - 0x4];
/* 0x0044 */ bool enableRumble[4]; // index controller id, controller vibrates when true
/* 0x0000 */ bool rumblePrev[4]; // rumble enable flags from previous frame
/* 0x0000 */ Vec2 pointingPos[4]; // wii remote pointer positions
    // controller input states, major index controller id, minor index age (latest is 0)
/* 0x0000 */ KPADStatus statuses[4][WPAD_STATUS_COUNT];
/* 0x0000 */ s32 kpadReadRet[4]; // last return from KPADRead for each controller
/* 0x0000 */ KPADStatus pastStatuses[4][60];
/* 0x0000 */ u32 unknown_0x9d3c[4];
/* 0x0000 */ s32 unknown_0x9d4c[4];
/* 0x0000 */ u8 unknown_0x9d5c[0x9d60 - 0x9d5c];
/* 0x0000 */ s64 unknown_0x9d60[4];
/* 0x0000 */ u8 unknown_0x9d80[0x9dc8 - 0x9d80];
} WpadWork;
SIZE_ASSERT(WpadWork, 0x9dc8)

/*
    Returns the pointer to the WpadWork instance
*/
WpadWork * wpadGetWork();

/*
    Initialises data used by wpad functions and the KPAD library
*/
void wpadInit();

/*
    Stops vibration of all controllers
*/
void wpadAllRumbleOff();

/*
    Updates all controllers
*/
void wpadMain();

/*
    Enables/disables pointers updating on all controllers
*/
void wpadCursorOn();
void wpadCursorOff();

/*
    Returns whether cursors are enabled
*/
bool wpadGetCursorStatus();

/*
    Makes a controller start/stop vibrating
*/
void wpadRumbleOn(s32 controller);
void wpadRumbleOff(s32 controller);

/*
    Returns whether a controller is vibrating
    (ignores SpmarioGlobals override)
*/
bool wpadGetRumbleStatus(s32 controller);

/*
    Returns a bitmask of all buttons that are down on a controller 
*/
u32 wpadGetButtonsHeld(s32 controller);

/*
    Returns a bitmask of all buttons that have just been released/pressed on a controller
*/
u32 wpadGetButtonsReleased(s32 controller);
u32 wpadGetButtonsPressed(s32 controller);

/*
    Returns a bitmask of buttons held long enough to trigger the 'repeat' flag
*/
u32 wpadGetButtonsHeldRepeat(s32 controller);

u32 func_802376e4(s32 controller);
void func_80237710(s32 controller);
bool func_80237750();

CPP_WRAPPER_END()
