#pragma once

#include <common.h>
#include <wii/os/OSThread.h>

CPP_WRAPPER(wii::os)

USING(wii::os::OSThread)

typedef struct _OSMutex
{
    u8 unknown_0x0[0x8 - 0x0];
    OSThread * thread;
    s32 count;
    u8 unknown_0x10[0x10 - 0x8];
} OSMutex;
SIZE_ASSERT(OSMutex, 0x18)

void OSInitMutex(OSMutex * mutex);
void OSLockMutex(OSMutex * mutex);
void OSUnlockMutex(OSMutex * mutex);
UNKNOWN_FUNCTION(__OSUnlockAllMutex);

CPP_WRAPPER_END()
