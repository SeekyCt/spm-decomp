/*
    Revolution SDK OS Library
*/

#pragma once

#include <common.h>

#define OSClockSpeed ((*(u32 *) 0x800000F8) / 4)
#define OSTicksToMilliseconds(ticks) ((ticks) / (OSClockSpeed / 1000))
#define OSMillisecondsToTicks(ms) ((ms) * (OSClockSpeed / 1000))

typedef s64 OSTime;
typedef u32 OSTick;

OSTime OSGetTime();
OSTick OSGetTick();

void OSPanic(const char * filename, s32 line, const char * msg, ...);

void * OSGetMEM1ArenaHi();
void * OSGetMEM2ArenaHi();
void * OSGetArenaHi();
void * OSGetMEM1ArenaLo();
void * OSGetMEM2ArenaLo();
void * OSGetArenaLo();
void OSSetMEM1ArenaHi(void * arenaHi);
void OSSetMEM2ArenaHi(void * arenaHi);
void OSSetArenaHi(void * arenaHi);
void OSSetMEM1ArenaLo(void * arenaLo);
void OSSetMEM2ArenaLo(void * arenaLo);
void OSSetArenaLo(void * arenaLo);

void * DCFlushRange(void * ptr, size_t length);

typedef void (RelExportFunc)();

// Unions are offsets in file then pointers after OSLink
struct _RelHeader;
typedef struct _RelHeader
{
    u32 id;
    struct _RelHeader * next;
    struct _RelHeader * prev;
    u32 numSections;
    union
    {
        u32 sectionInfoOffset;
        void * sectionInfo;
    };
    union
    {
        u32 nameOffset;
        void * name;
    };
    u32 nameSize;
    u32 version;
    u32 bssSize;
    union
    {
        u32 relOffset;
        void * relocationTable;
    };
    union
    {
        u32 impOffset;
        void * impTable;
    };
    u32 impSize;
    u8 prologSection;
    u8 epilogSection;
    u8 unresolvedSection;
    u8 bssSection;
    union
    {
        u32 prologOffset;
        RelExportFunc * prolog;
    };
    union
    {
        u32 epilogOffset;
        RelExportFunc * epilog;
    };
    union
    {
        u32 unresolvedOffset;
        RelExportFunc * unresolved;
    };
    u32 align;
    u32 bssAlign;
    u32 fixSize;
} RelHeader;

bool OSLink(RelHeader * relFile, void * bss);

typedef struct
{
    u8 unknown_0x0[0x318 - 0x0];
} OSThread;
typedef void (*ThreadFunc)();

OSThread * OSGetCurrentThread();
s32 OSCreateThread(OSThread * thread, ThreadFunc * func, void * funcParam, void * stackTop, u32 stackSize, s32 priority, u16 param_7);
void OSCancelThread(OSThread * thread);
s32 OSResumeThread(OSThread * thread);
s32 OSSuspendThread(OSThread * thread);
void OSYieldThread();

typedef struct
{
    u8 unknown_0x0[0x18 - 0x0];
} OSMutex;

void OSInitMutex(OSMutex * mutex);

typedef void (*PowerCallback)();

void OSSetResetCallback(PowerCallback * func);
void OSSetPowerCallback(PowerCallback * func);
void OSShutdownSystem();
void OSRestart(s32 code);
void OSReturnToMenu();
s32 OSGetResetCode();
