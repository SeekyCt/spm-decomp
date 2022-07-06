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

#define DEG_TO_RAD(a) (a * 0.01745329238474369f)

#define STATIC_ASSERT(cond, msg) typedef char static_assertion_##msg[(cond)?1:-1]
#define SIZE_ASSERT(type, size) STATIC_ASSERT(sizeof(type) == size, type##_size)
