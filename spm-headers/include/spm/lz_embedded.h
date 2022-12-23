/*
    lz-compressed assets embedded in the dol
*/

#pragma once

#include <common.h>

CPP_WRAPPER(spm::lz_embedded)

// Identical copy of wiimario_snd.dat
// Unused in favour of the one on the disc
extern u8 wiimario_snd_dat[];

// Identical copy of ag2tg.bin
// One on the disc is unused in favour of this
extern u8 ag2tg[];

// Cooking recipe definitions
// parse.c xml format
extern u8 cookingText[];

// Treasure map definitions
// parse.c xml format 
extern u8 treasureMapText[];

// Pit of 100 Trials (Flipside & Flopside) floor definitions
// parse.c xml format
extern u8 pitText[];

CPP_WRAPPER_END()
