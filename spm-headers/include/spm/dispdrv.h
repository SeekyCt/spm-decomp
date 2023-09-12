/*
    dispdrv is a wrapper for functions that can draw to the screen
*/

#pragma once

#include <common.h>

CPP_WRAPPER(spm::dispdrv)

#define DISP_ZOFFSET 100000.0
#define MAX_SCISSOR_NO 32

typedef void (DispCallback)(s32 cameraId, void * param);

typedef struct
{
/* 0x0 */ s8 cameraId;
/* 0x1 */ s8 renderMode;
/* 0x2 */ u8 unknown_0x2[0x4 - 0x2];
/* 0x4 */ f32 order;
/* 0x8 */ DispCallback * callback;
/* 0xC */ f32 * callbackParam;
} DispEntry;
SIZE_ASSERT(DispEntry, 0x10)

DECOMP_STATIC(DispEntry * dispdrv_pDispWork)
DECOMP_STATIC(DispEntry ** dispdrv_pSortWork)
DECOMP_STATIC(s32 dispdrv_entry_n)
DECOMP_STATIC(DispEntry * dispdrv_currentWorkPtr)

typedef struct
{
/* 0x0 */ u8 unknown_0x0[0x10 - 0x0];
} DispScissor;
SIZE_ASSERT(DispScissor, 0x10)

DECOMP_STATIC(DispScissor dispdrv_dispScissors[0x20])

/*
    Initialise data used by dispdrv functions
*/
void dispInit();
void dispReInit();

/*
    Register a function to be displayed this frame
*/
void dispEntry(s8 cameraId, s8 renderMode, f32 z, DispCallback * callback, void * callbackParam);

void dispSetCurScissor(s32 scissorNo);
void dispSetScissor(s32 id, u32 field_0, u32 field_4, u32 field_8, u32 field_c);

DECOMP_STATIC(bool dispdrv__sort(DispEntry ** lhs, DispEntry ** rhs))
void dispSort();

/*
    Runs every entry's callback
*/
void dispDraw(s32 cameraId);

f32 dispCalcZ(f32 param_1);

/*
    Returns the DispEntry currently being executed
*/
DispEntry * dispGetCurWork();

UNKNOWN_FUNCTION(func_8005b52c);

CPP_WRAPPER_END()
