/*
    Revolution SDK WPAD library
*/

#ifndef WPAD_H
#define WPAD_H

#include <common.h>

#define WPAD_BTN_LEFT 0x1
#define WPAD_BTN_RIGHT 0x2
#define WPAD_BTN_DOWN 0x4
#define WPAD_BTN_UP 0x8
#define WPAD_BTN_PLUS 0x10
#define WPAD_BTN_2 0x100
#define WPAD_BTN_1 0x200
#define WPAD_BTN_B 0x400
#define WPAD_BTN_A 0x800
#define WPAD_BTN_MINUS 0x1000
#define WPAD_BTN_HOME 0x8000
#define WPAD_BTN_REPEAT 0x80000000

typedef void * (WPADAlloc)(u32 size);
typedef bool (WPADFree)(void * ptr);

void WPADSetAutoSleepTime(u8 minutes); // 802d090c
void WPADControlMotor(s32 controller, u32 onOff); // 802d0e70
bool WPADIsDpdEnabled(s32 controller); // 802d3250
void WPADRegisterAllocator(WPADAlloc * alloc, WPADFree * free); // 802dc3a0

#endif