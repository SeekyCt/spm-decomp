#pragma once

#include <common.h>
#include <wii/os/OSTime.h>

CPP_WRAPPER(wii::os)

USING(wii::os::OSTime)

extern volatile OSTime __OSLastInterruptTime;
extern volatile s16 __OSLastInterrupt;
extern volatile u32 __OSLastInterruptSrr0;

u32 OSDisableInterrupts();
UNKNOWN_FUNCTION(OSEnableInterrupts);
void OSRestoreInterrupts(u32 interruptMask);
UNKNOWN_FUNCTION(__OSSetInterruptHandler);
UNKNOWN_FUNCTION(__OSGetInterruptHandler);
UNKNOWN_FUNCTION(__OSInterruptInit);
UNKNOWN_FUNCTION(SetInterruptMask);
UNKNOWN_FUNCTION(__OSMaskInterrupts);
UNKNOWN_FUNCTION(__OSUnmaskInterrupts);
UNKNOWN_FUNCTION(__OSDispatchInterrupt);
UNKNOWN_FUNCTION(ExternalInterruptHandler);

CPP_WRAPPER_END()
