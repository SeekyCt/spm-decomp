#pragma once

#include <common.h>

CPP_WRAPPER(wii::os)

typedef struct
{
    u8 unknown_0x0[0x18 - 0x0];
} OSMutex;
SIZE_ASSERT(OSMutex, 0x18)

void OSInitMutex(OSMutex * mutex);
UNKNOWN_FUNCTION(OSLockMutex);
UNKNOWN_FUNCTION(OSUnlockMutex);
UNKNOWN_FUNCTION(__OSUnlockAllMutex);

CPP_WRAPPER_END()
