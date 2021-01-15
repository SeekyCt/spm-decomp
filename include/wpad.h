/*
    Revolution SDK WPAD library
*/

#ifndef WPAD_H
#define WPAD_H

#include <common.h>

typedef void * (WPADAlloc)(u32 size);
typedef bool (WPADFree)(void * ptr);

void WPADSetAutoSleepTime(u8 minutes); // 802d090c
void WPADControlMotor(s32 controller, u32 onOff); // 802d0e70
void WPADRegisterAllocator(WPADAlloc * alloc, WPADFree * free); // 802dc3a0

#endif