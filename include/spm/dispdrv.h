#pragma once

#include <common.h>

typedef void (DispCallback)(s32 cameraId, void * param);

typedef struct {
    u8 unknown_0x0[0x10 - 0x0];
} DispEntry;

void dispInit();

void dispEntry(s8 cameraId, u8 renderMode, f32 z, DispCallback * cb, void * cbParam);

DispEntry * dispGetCurWork();
