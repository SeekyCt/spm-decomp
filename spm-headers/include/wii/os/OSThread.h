#pragma once

#include <common.h>
#include <wii/os/OSContext.h>

CPP_WRAPPER(wii::os)

USING(wii::os::OSContext)

struct _OSMutex;
struct _OSThread;

typedef struct
{
/* 0x0 */ struct _OSThread * next;
/* 0x4 */ struct _OSThread * prev;
} OSThreadLink;
SIZE_ASSERT(OSThreadLink, 0x8)

typedef struct
{
/* 0x0 */ struct _OSThread * head;
/* 0x4 */ struct _OSThread * tail;
} OSThreadQueue;
SIZE_ASSERT(OSThreadQueue, 0x8)

typedef struct _OSThread
{
/* 0x000 */ OSContext context;
/* 0x2C8 */ u16 state;
/* 0x2CA */ u16 attr;
/* 0x2CC */ s32 suspend;
/* 0x2D0 */ s32 priority;
/* 0x2D4 */ s32 base;
/* 0x2D8 */ void * val;
/* 0x2DC */ OSThreadQueue * queue;
/* 0x2E0 */ OSThreadLink link;
/* 0x2E8 */ OSThreadQueue queueJoin;
/* 0x2F0 */ struct _OSMutex * mutex;
/* 0x2F4 */ u8 unknown_0x2f4[0x2fc - 0x2f4];
/* 0x2FC */ OSThreadLink linkActive;
/* 0x304 */ u8 * stackBase;
/* 0x308 */ u32 * stackEnd;
/* 0x30C */ s32 error;
/* 0x310 */ void * specific[2];
} OSThread;
SIZE_ASSERT(OSThread, 0x318)

FIXED_ADDR(OSThread *, OS_CURRENT_THREAD, 0x800000e4);
FIXED_ADDR(OSThreadQueue, OS_THREAD_QUEUE, 0x800000dc);

typedef void * (ThreadFunc)(void *);

#define OS_THREAD_DETACHED 1

#define OS_THREAD_STATE_EXITED 0
#define OS_THREAD_STATE_READY 1
#define OS_THREAD_STATE_RUNNING 2
#define OS_THREAD_STATE_SLEEPING 4
#define OS_THREAD_STATE_MORIBUND 8

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
BOOL OSCreateThread(OSThread * thread, ThreadFunc * func, void * funcParam, void * stackTop,
                   u32 stackSize, s32 priority, u16 attr);
void OSExitThread(void * val);
void OSCancelThread(OSThread * thread);
BOOL OSJoinThread(OSThread * thread, void ** outVal);
s32 OSResumeThread(OSThread * thread);
s32 OSSuspendThread(OSThread * thread);
void OSSleepThread(OSThreadQueue * thread);
void OSWakeupThread(OSThreadQueue * thread);

CPP_WRAPPER_END()
