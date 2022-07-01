/*
  Functions to execute and control the current 'sequence' of the game
*/

#pragma once

#include <common.h>
#include <spm/seqdef.h>

enum SeqdrvSeq
{
    SEQ_LOGO = 0,
    SEQ_TITLE,
    SEQ_GAME,
    SEQ_MAPCHANGE,
    SEQ_GAMEOVER,
    SEQ_LOAD
};

typedef struct _SeqWork
{
  s32 seq;
  s32 stage;
  char * p0;
  char * p1;
  u8 unknown_0x10[0x20 - 0x10];
  SeqFunc * afterFunc;
} SeqWork;

void seqInit_SPMARIO();
void seqMain();
void seqSetSeq(s32 seq, char * p0, char * p1);
s32 seqGetSeq();
