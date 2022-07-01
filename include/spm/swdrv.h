/*
    Functions handling "Saved Work", which are variables used in evt
    scripts that are saved when closing the game
    Set, Get & Clear functions handle bools
    ByteGet and ByteSet functions handle s8 (or s32 for gsw0)
    LSW(F) functions are prefixed by an underscore, the rest are GSW(F)
*/

#pragma once

#include <common.h>

typedef struct
{
    const char * mapName;
    int num;
} AssignTblEntry;

typedef struct
{
    s32 coinId;
    s32 gameCoinId;
} SwWork;

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
void _swByteSet(s32 id, s8 num);

/*
    Returns a local saved work value
    Note: return is technically just s8 but calls used wrong casts when it was set to that
*/
s32 _swByteGet(s32 id);

// New to SPM so no symbols:
UNKNOWN_FUNCTION(func_80038204);
UNKNOWN_FUNCTION(func_800383a0);
UNKNOWN_FUNCTION(func_80038478);
UNKNOWN_FUNCTION(func_80038550);
UNKNOWN_FUNCTION(func_8003863c);
UNKNOWN_FUNCTION(func_8003864c);
UNKNOWN_FUNCTION(func_8003865c);
UNKNOWN_FUNCTION(func_8003875c);
UNKNOWN_FUNCTION(func_800387d8);
