#pragma once

#include <common.h>

typedef struct
{
    u8 r, g, b, a;
} RGBA;

typedef struct
{
    f32 x, y;
} Vec2;

typedef struct
{
    f32 x, y, z;
} Vec3;

typedef f32 Mtx34[3][4];
typedef f32 Mtx44[4][4];
