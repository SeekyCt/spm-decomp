#pragma once

#include <common.h>

CPP_WRAPPER(wii::exi)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x20 - 0x0];
/* 0x20 */ s32 unknown_0x20;
/* 0x24 */ u8 unknown_0x24[0x40 - 0x24];
} EXIEcb;
SIZE_ASSERT(EXIEcb, 0x40)

extern EXIEcb Ecb[3];

UNKNOWN_FUNCTION(SetExiInterruptMask);
s32 EXIImm(s32 chan, void * data, s32 len, s32 mode, void * completionCb);
UNKNOWN_FUNCTION(EXIImmEx);
UNKNOWN_FUNCTION(EXIDma);
s32 EXISync(s32 chan);
UNKNOWN_FUNCTION(EXISetExiCallback);
s32 __EXIProbe(s32 chan);
UNKNOWN_FUNCTION(EXIAttach);
UNKNOWN_FUNCTION(EXIDetach);
s32 EXISelect(s32 chan, s32 dev, s32 freq);
s32 EXIDeselect(s32 chan);
UNKNOWN_FUNCTION(EXIInterruptHandler);
UNKNOWN_FUNCTION(TCInterruptHandler);
UNKNOWN_FUNCTION(EXTInterruptHandler);
UNKNOWN_FUNCTION(EXIInit);
s32 EXILock(s32 chan, s32 dev, void * unlockCb);
s32 EXIUnlock(s32 chan);
UNKNOWN_FUNCTION(UnlockedHandler);
s32 EXIGetID(s32 chan, s32 dev, u32 * outId);
UNKNOWN_FUNCTION(ProbeBarnacle);
UNKNOWN_FUNCTION(__OSEnableBarnacle);
UNKNOWN_FUNCTION(EXIWriteReg);

// Re-implemented in game_lib_ext:
// TODO: should that merge into this project?
s32 EXIProbe(s32 chan);

CPP_WRAPPER_END()
