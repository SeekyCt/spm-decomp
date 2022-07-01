#pragma once

#include <common.h>
#include <wii/types.h>

typedef struct
{
    u32 flag;
    u8 unknown_0x4[0x54 - 0x4];
    Mtx34 viewMtx;
    u8 unknown_0x84[0x94 - 0x84];
    Mtx44 projMtx;
    u8 unknown_0xd4[0xf0 - 0xd4];
    f32 top;
    f32 bottom;
    f32 left;
    f32 right;
    u8 unknown_0x100[0x158 - 0x100];
    s32 isOrtho;
    Vec3 pos;
    Vec3 target;
    u8 unknown_0x174[0x18c - 0x174]; 
    f32 aspect;
    u8 unknown_0x190[0x2f0 - 0x174]; 
} CamEntry;

void camInit();

void camMain();
void camDraw();

bool camCheck3d(s32 camId);
CamEntry * camGetPtr(s32 camId);
CamEntry * camGetCurPtr();
