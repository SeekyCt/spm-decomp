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
void swSet(s32 num); // 80037fac

/*
    Returns the value of a global saved work flag
*/
bool swGet(s32 num); // 80037fec

/*
    Turns off a global saved work flag
*/
void swClear(s32 num); // 80038034

// TTYD symbol map shows there's an swToggle but it went unused in both games

/*
    Sets a global saved work value
    Value cast to s8 unless setting GSW 0
*/
void swByteSet(s32 num, s32 value);

/*
    Gets a global saved work value
    Return is s8 unless getting GSW 0
*/
s32 swByteGet(s32 num);

// _swSet
s32 _swGet(s32 num);
// _swClear
// TTYD symbol map shows there's an _swToggle but it went unused in both games
// _swToggle
// _swByteSet
s32 _swByteGet(s32 num);

#endif