#pragma once

#include <common.h>

CPP_WRAPPER(wii::os)

typedef struct
{
/* 0x000 */ u32 gpr[32];
/* 0x080 */ u32 cr;
/* 0x084 */ u32 lr;
/* 0x088 */ u32 ctr;
/* 0x08C */ u32 xer;
/* 0x090 */ f64 fpr[32];
/* 0x190 */ u8 unknown_0x190[0x194 - 0x190];
/* 0x194 */ u32 fpscr;
/* 0x198 */ u32 srr0;
/* 0x19C */ u32 srr1;
/* 0x1A0 */ u16 mode;
/* 0x1A2 */ u16 state;
/* 0x1A4 */ u32 gqr[8];
/* 0x1C0 */ u8 unknown_0x1c4[0x1c8 - 0x1c4];
/* 0x1C8 */ f64 psf[32];
} OSContext;
SIZE_ASSERT(OSContext, 0x2c8)

UNKNOWN_FUNCTION(__OSLoadFPUContext);
UNKNOWN_FUNCTION(__OSSaveFPUContext);
UNKNOWN_FUNCTION(OSSaveFPUContext);
UNKNOWN_FUNCTION(OSSetCurrentContext);
UNKNOWN_FUNCTION(OSGetCurrentContext);
UNKNOWN_FUNCTION(OSSaveContext);
UNKNOWN_FUNCTION(OSLoadContext);
UNKNOWN_FUNCTION(OSGetStackPointer);
UNKNOWN_FUNCTION(OSSwitchFiber);
UNKNOWN_FUNCTION(OSSwitchFiberEx);
UNKNOWN_FUNCTION(OSClearContext);
UNKNOWN_FUNCTION(OSInitContext);
void OSDumpContext(OSContext * context);
UNKNOWN_FUNCTION(OSSwitchFPUContext);
UNKNOWN_FUNCTION(__OSContextInit);

CPP_WRAPPER_END()
