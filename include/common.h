#pragma once

#include <ppcdis.h>

#ifdef __INTELLISENSE__ 
    #define asm
#endif

// Basic types

typedef unsigned long long u64;
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef long long s64;
typedef long s32;
typedef short s16;
typedef char s8;

typedef float f32;
typedef double f64;

typedef u32 size_t;

#define NULL 0

typedef s32 BOOL;

#ifndef __cplusplus

#define bool _Bool

#define true 1
#define false 0

typedef u16 wchar_t;

#endif

#define SDA2_BASE 0x805b7260
