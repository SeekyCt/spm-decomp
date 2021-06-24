/*
    Common definitions
*/
#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

#define NULL ((void*)0)

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long s64;
typedef int s32;
typedef short s16;
typedef char s8;

typedef float f32;
typedef double f64;

typedef u32 size_t;

typedef u16 wchar_t;

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

// Used in un-implemented functions to make string/float pools match when asm is affected
void __dummy_string(const char *); void __dummy_float(float);

#endif
