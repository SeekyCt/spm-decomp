#pragma once

#include <common.h>

CPP_WRAPPER(wii::os)

typedef void (*PowerCallback)();

void OSSetResetCallback(PowerCallback * func);
void OSSetPowerCallback(PowerCallback * func);
UNKNOWN_FUNCTION(__OSInitSTM);
UNKNOWN_FUNCTION(__OSShutdownToSBY);
UNKNOWN_FUNCTION(__OSHotReset);
UNKNOWN_FUNCTION(__OSSetVIForceDimming);
UNKNOWN_FUNCTION(__OSUnregisterStateEvent);
UNKNOWN_FUNCTION(__OSVIDimReplyHandler);
UNKNOWN_FUNCTION(__OSDefaultResetCallback);
UNKNOWN_FUNCTION(__OSDefaultPowerCallback);
UNKNOWN_FUNCTION(__OSStateEventHandler);

CPP_WRAPPER_END()
