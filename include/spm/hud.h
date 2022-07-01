#pragma once

#include <common.h>

void hudInit();
void hudMain();

typedef void (CountdownDoneCb)();

void hudStartCountdown(u32 length, CountdownDoneCb * cb);
void func_8019af88();
void hudTurnOffFlipTimeBox(s32 idx);
