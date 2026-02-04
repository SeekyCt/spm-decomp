#pragma once

#include <common.h>

#define NULL 0

typedef s32 ptrdiff_t;

#ifdef M2C
    typedef u32 size_t;
#else
    typedef __typeof__(sizeof(0)) size_t;
#endif

#if defined __clang__ || defined __GNUC__
    #define offsetof(type, member) __builtin_offsetof(type, member)
#else
    #define offsetof(type, member) ((size_t)&((type *)0)->member)
#endif

#ifndef __cplusplus
    typedef u16 wchar_t;
#endif
