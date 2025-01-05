#pragma once

#include <common.h>

#define NULL 0

typedef s32 ptrdiff_t;
typedef __typeof__(sizeof(0)) size_t;

#define offsetof(type, member) ((size_t)&((type *)0)->member)

#ifndef __cplusplus
    typedef u16 wchar_t;
#endif
