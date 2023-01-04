/*
    swdrv handles "Saved Work", which are variables used in evt
    scripts that are saved when closing the game
    Set, Get & Clear functions handle bools
    ByteGet and ByteSet functions handle s8 (or s32 for gsw0)
    LSW(F) functions are prefixed by an underscore, the rest are GSW(F)
*/

#pragma once

#include <common.h>

CPP_WRAPPER(spm::swdrv)

#define MAX_COIN_MAP 32
#define MAX_COIN_BIT 0x200

typedef struct
{
/* 0x00 */ char mapName[8];
/* 0x08 */ u32 coinFlags[16];
} SwCoinEntry;
SIZE_ASSERT(SwCoinEntry, 0x48)

typedef struct
{
/* 0x0 */ const char * mapName;
/* 0x4 */ s32 num;
} AssignTblEntry;
SIZE_ASSERT(AssignTblEntry, 0x8)

typedef struct
{
/* 0x0 */ s32 coinId;
/* 0x4 */ s32 gameCoinId;
} SwWork;
SIZE_ASSERT(SwWork, 0x8)

DECOMP_STATIC(s32 swdrv_gameCoinGswMap[])
DECOMP_STATIC(AssignTblEntry swdrv_assign_tbl[])
DECOMP_STATIC(SwWork swdrv_work)
DECOMP_STATIC(SwWork * swdrv_wp)

/*
    Zeros all of the evt variables in SpmarioGlobals (GSW, GSWF, LSW, LSWF),
    SpMarioGlobals.unknown_0x1184, coinId and gameCoinId
*/
void swInit();

/*
    Zeros SpmarioGlobals LSW, LSWF & unknown_0x1184 and coinId
*/
void swReInit();

/*
    Turns on a global saved work flag
*/
void swSet(s32 id);

/*
    Returns the value of a global saved work flag
*/
bool swGet(s32 id);

/*
    Turns off a global saved work flag
*/
void swClear(s32 id);

// TTYD symbol map shows there's an swToggle but it went unused in both games

/*
    Sets a global saved work value
    Value cast to u8 unless setting GSW 0
*/
void swByteSet(s32 id, s32 num);

/*
    Gets a global saved work value
    Return is u8 unless getting GSW 0
*/
s32 swByteGet(s32 id);

/*
    Turns on a local saved work flag
*/
void _swSet(s32 id);

/*
    Returns the value of a local saved work flag
*/
bool _swGet(s32 id);

/*
    Turns off a local saved work flag
*/
void _swClear(s32 id);

// TTYD symbol map shows there's an _swToggle but it went unused in both games

/*
    Sets a local saved work value
*/
void _swByteSet(s32 id, u8 num);

/*
    Returns a local saved work value
    Note: return is technically just u8 but calls used wrong casts when it was set to that
*/
s32 _swByteGet(s32 id);

s32 swGetCoinId();
void swCoinSet(s32 id);
void swCoinClear(s32 id);
bool swCoinGet(s32 id);
void swResetCoinId();
void swResetGameCoinId();
s32 swGetGameCoinId();
bool swGameCoinGet(s32 id);
void swGameCoinSet(s32 id);

CPP_WRAPPER_END()
