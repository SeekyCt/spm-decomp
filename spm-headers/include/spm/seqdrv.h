/*
    seqdrv functions control the main loops of the game. One sequence is active at
    a time: its init function runs first, then its main function is ran every
    frame, and its exit function runs when changing to another sequence.
*/

#pragma once

#include <common.h>
#include <spm/seqdef.h>

CPP_WRAPPER(spm::seqdrv)

USING(spm::seqdef::SeqFunc)

enum SeqdrvSeq
{
/* 0x0 */ SEQ_LOGO,
/* 0x1 */ SEQ_TITLE,
/* 0x2 */ SEQ_GAME,
/* 0x3 */ SEQ_MAPCHANGE,
/* 0x4 */ SEQ_GAMEOVER,
/* 0x5 */ SEQ_LOAD
};

typedef struct _SeqWork
{
/* 0x00 */ s32 seq; // enum above, or -1 if none
/* 0x04 */ s32 stage; // number used by the seq_ functions to track their progress
/* 0x08 */ const char * p0; // parameter for seq function, only used in MapChange (map name)
/* 0x0C */ const char * p1; // parameter for seq function, only used in MapChange (door name)
/* 0x10 */ u8 unknown_0x10[0x20 - 0x10];
/* 0x20 */ SeqFunc * afterFunc; // ran after every call to the main SeqFunc if not null
} SeqWork;
SIZE_ASSERT(SeqWork, 0x24)

/*
    Initialises data used by seqdrv functions
*/
void seqInit_SPMARIO();

/*
    Calls all init, main & exit functions based on now_seq & next_seq
*/
void seqMain();

/*
    Sets the current sequence and its prameters
*/
void seqSetSeq(s32 seqNum, const char * p0, const char * p1);

/*
    Returns now_seq or 0 if equal to -1
*/
s32 seqGetSeq();

CPP_WRAPPER_END()
