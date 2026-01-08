#pragma once

#include <common.h>
#include <wii/gx.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::mario_fairy)

typedef struct
{
/* 0x0 */ u32 itemId;
/* 0x4 */ void * useFunc;
} FairyUseFuncDef;
SIZE_ASSERT(FairyUseFuncDef, 0x8)

DECOMP_STATIC(FairyUseFuncDef fairyUseFuncs[13])

s32 marioFairyUse();

CPP_WRAPPER_END()
