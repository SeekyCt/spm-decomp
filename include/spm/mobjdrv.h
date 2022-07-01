#pragma once

#include <common.h>
#include <wii/types.h>

typedef struct
{
    u8 unknown_0x0[0x18 - 0x0];
    Vec3 pos;
    u8 unknown_0x24[0x2a8 - 0x24];
} MOBJEntry;

void mobjInit();

// ...

void mobjMain();

// ...

MOBJEntry * mobjNameToPtr(const char * name);
