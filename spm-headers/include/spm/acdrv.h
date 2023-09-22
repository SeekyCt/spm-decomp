/*
    acdrv handles action commands for item usage (and possibly other things?)
*/

#pragma once

#include <common.h>
#include <spm/memory.h>
#include <spm/pausewin.h>

CPP_WRAPPER(spm::acdrv)

USING(spm::memory::SmartAllocation)
USING(spm::pausewin::PausewinEntry)

struct _AcEntry;
typedef void (AcFunc)(struct _AcEntry * entry);
typedef void (AcDispFunc)(s8 camId, struct _AcEntry * entry);

// Require wii remote pointed at screen
#define AC_FLAG_POINTER 4
// Action command paused
#define AC_FLAG_PAUSED 2
// Entry allocated for an action command
#define AC_FLAG_IN_USE 1

typedef struct _AcEntry
{
/* 0x00 */ u16 flags;
/* 0x02 */ u8 unknown_0x2[0x4 - 0x2];
/* 0x04 */ s32 type;
/* 0x08 */ u8 unknown_0x8[0x10 - 0x8];
/* 0x10 */ const char * name;
/* 0x14 */ s32 pausewinEntryId;
/* 0x18 */ s64 startTime;
/* 0x20 */ u8 unknown_0x20[0x28 - 0x20];
/* 0x28 */ s32 state;
/* 0x2C */ AcFunc * initFunc;
/* 0x30 */ AcFunc * mainFunc;
/* 0x34 */ AcFunc * deleteFUnc;
/* 0x38 */ AcDispFunc * dispFunc;
/* 0x3C */ u8 unknown_0x3c[0x40 - 0x3c];
} AcEntry;
SIZE_ASSERT(AcEntry, 0x40)

typedef struct
{
/* 0x0 */ s32 num; // 4
/* 0x4 */ AcEntry * entries; // array of num length
} AcWork;
SIZE_ASSERT(AcWork, 0x8)

DECOMP_STATIC(AcWork acdrv_work)
DECOMP_STATIC(AcWork * acdrv_wp)

typedef struct
{
/* 0x00 */ AcFunc * initFunc;
/* 0x04 */ AcFunc * mainFunc;
/* 0x08 */ AcFunc * deleteFunc;
/* 0x0C */ AcDispFunc * dispFunc;
/* 0x10 */ const char * msgName;
} AcDef;
SIZE_ASSERT(AcDef, 0x14)

DECOMP_STATIC(AcDef acdrv_acDefs[19])
DECOMP_STATIC(SmartAllocation * acdrv_lp)

/*
    Allocates entries
*/
void acInit();

/*
    Clears entries
*/
void acReInit();

UNKNOWN_FUNCTION(func_8003ebb8);
UNKNOWN_FUNCTION(func_8003ec24);

/*
    Updates entries based on their state, calls their mainFuncs and queues their dispFuncs
*/
void acMain();

/*
    Creates an acEntry based on the acDef of the type specified
    Types:
        0 - kirakira otoshi
        1 - pow block
        2 - barrier frame
        3 - koura de pon
        4 - kachikachi koura
        5 - biribiri kinoko
        6 - obake kinoko
        7 - nemure yoikoyo
        8 - stop watch
        9 - nemuri kaihuku
        10 - honoo sakuretsu
        11 - koori kaihuku
        12 - honoo sakuretsu 2
        13 - honoo sakuretsu 3
        14 - honoo sakuretsu 4
        15 - honoo sakuretsu 5
        16 - nomikomi kaihuku
        17 - jiwajiwa kinoko
        18 - koura de pon 2
*/
AcEntry * acEntry(s32 type);

/*
  Returns a value between 12 and 7 depending on how well the player did in the minigame
*/
s32 acReturnResults(const char * name);

/*
    Deletes an entry, calling its deleteFunc and deleting its pausewin entry
*/
void acDelete(AcEntry * entry);

/*
    Halts/unhalts all entries
*/
void acPauseAll();
void acUnpauseAll();

UNKNOWN_FUNCTION(func_8003f6bc);

/*
    Returns whether any entries are in use
*/
bool acIsRunning();

/*
    Gets an entry by name
*/
AcEntry * acNameToPtr(const char * name);

/*
    Displays the message for the pausewin entry of an ac entry
*/
void acMsgDisp(PausewinEntry * entry);

UNKNOWN_FUNCTION(func_8003f934);
UNKNOWN_FUNCTION(func_8004077c);
UNKNOWN_FUNCTION(func_80040dac);
UNKNOWN_FUNCTION(func_80040eb4);

CPP_WRAPPER_END()
