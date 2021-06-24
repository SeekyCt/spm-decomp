/*
    Functions handling "Saved Work", which are variables used in evt
    scripts that are saved when closing the game
    Set, Get & Clear functions handle bools
    ByteGet and ByteSet functions handle s8 (or s32 for gsw0)
    LSW(F) functions are prefixed by an underscore, the rest are GSW(F)
*/

#ifndef SWDRV_H
#define SWDRV_H

#include <common.h>

/*
    Zeros all of the evt variables in SpmarioGlobals (GSW, GSWF, LSW, LSWF),
    SpMarioGlobals.unknown_0x1184 and unknown_805adf40[0-1]
*/
void swInit(); // 80037eac

/*
    Zeros SpmarioGlobals LSW, LSWF & unknown_0x1184 and unknown_805adf40[0]
*/
void swReInit(); // 80037f48

/*
    Turns on a global saved work flag
*/
void swSet(s32 id); // 80037fac

/*
    Returns the value of a global saved work flag
*/
bool swGet(s32 id); // 80037fec

/*
    Turns off a global saved work flag
*/
void swClear(s32 id); // 80038034

// TTYD symbol map shows there's an swToggle but it went unused in both games

/*
    Sets a global saved work value
    Value cast to u8 unless setting GSW 0
*/
void swByteSet(s32 id, s32 num); // 80038074

/*
    Gets a global saved work value
    Return is u8 unless getting GSW 0
*/
s32 swByteGet(s32 id); // 800380f8

/*
    Turns on a local saved work flag
*/
void _swSet(s32 id); // 8003811c

/*
    Returns the value of a local saved work flag
*/
bool _swGet(s32 id); // 8003815c

/*
    Turns off a local saved work flag
*/
void _swClear(s32 id); // 800381a4

// TTYD symbol map shows there's an _swToggle but it went unused in both games

/*
    Sets a local saved work value
*/
void _swByteSet(s32 id, s8 num); // 800381e4

/*
    Returns a local saved work value
    Note: return is technically just s8 but calls used wrong casts when it was set to that
*/
s32 _swByteGet(s32 id); // 800381f4

// New to SPM so no symbols:
// 80038204
// 800383a0
// 80038478
// 80038550
// 8003863c
// 8003864c
// 8003865c
// 8003875c
// 800387d8

#endif