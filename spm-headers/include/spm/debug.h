#pragma once

#include <common.h>

CPP_WRAPPER(spm::debug)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x1c - 0x0];
} DebugWork;
SIZE_ASSERT(DebugWork, 0x1c)

void debugInit();
void debugReInit();
// OSPanic

DECOMP_STATIC(DebugWork debug_work)
DECOMP_STATIC(DebugWork debug_wp)

CPP_WRAPPER_END()
