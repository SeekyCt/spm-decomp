#pragma once

#include <common.h>
#include <wii/os/OSContext.h>

CPP_WRAPPER(wii::os)

USING(wii::os::OSContext)

typedef struct _OSThread
{
    OSContext context;
    u8 unknown_0x2c8[0x2fc - 0x2c8];
    struct _OSThread * next;
    struct _OSThread * prev;
    u8 unknown_0x304[0x318 - 0x304];
} OSThread;
SIZE_ASSERT(OSThread, 0x318)

typedef struct
{
/* 0x0 */ u8 unknown_0x0[0x8 - 0x0];
} OSThreadQueue;
SIZE_ASSERT(OSThreadQueue, 0x8)

FIXED_ADDR(OSThread *, currentThread, 0x800000e4);

typedef void (*ThreadFunc)();

UNKNOWN_FUNCTION(DefaultSwitchThreadCallback);
UNKNOWN_FUNCTION(__OSThreadInit);
UNKNOWN_FUNCTION(OSInitThreadQueue);
OSThread * OSGetCurrentThread();
UNKNOWN_FUNCTION(OSDisableScheduler);
UNKNOWN_FUNCTION(OSEnableScheduler);
UNKNOWN_FUNCTION(UnsetRun);
UNKNOWN_FUNCTION(__OSGetEffectivePriority);
UNKNOWN_FUNCTION(SetEffectivePriority);
UNKNOWN_FUNCTION(__OSPromoteThread);
UNKNOWN_FUNCTION(SelectThread);
UNKNOWN_FUNCTION(__OSReschedule);
void OSYieldThread();
s32 OSCreateThread(OSThread * thread, ThreadFunc * func, void * funcParam, void * stackTop,
                   u32 stackSize, s32 priority, u16 param_7);
UNKNOWN_FUNCTION(OSExitThread);
void OSCancelThread(OSThread * thread);
UNKNOWN_FUNCTION(OSJoinThread);
s32 OSResumeThread(OSThread * thread);
s32 OSSuspendThread(OSThread * thread);
void OSSleepThread(OSThreadQueue * thread);
void OSWakeupThread(OSThreadQueue * thread);

CPP_WRAPPER_END()
