#pragma once

#include <common.h>

CPP_WRAPPER(wii::os)

UNKNOWN_FUNCTION(DCEnable);
UNKNOWN_FUNCTION(DCInvalidateRange);
void * DCFlushRange(void * ptr, size_t length);
UNKNOWN_FUNCTION(DCFlushRangeNoSync);
UNKNOWN_FUNCTION(DCZeroRange);
void ICInvalidateRange(void * ptr, u32 size);
UNKNOWN_FUNCTION(ICFlashInvalidate);
UNKNOWN_FUNCTION(ICEnable);
UNKNOWN_FUNCTION(LCDisable);
UNKNOWN_FUNCTION(DMAErrorHandler);
UNKNOWN_FUNCTION(__OSCacheInit);

CPP_WRAPPER_END()
