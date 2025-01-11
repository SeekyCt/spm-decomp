#pragma once

#include <common.h>

#ifndef __cplusplus

#ifdef M2C
    #define bool char
#else
    #define bool _Bool
#endif

#define true 1
#define false 0

#endif
