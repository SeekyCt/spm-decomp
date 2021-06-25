/*
  Functions to execute and control the current 'sequence' of the game
*/

#ifndef SEQDRV_H
#define SEQDRV_H

#include <common.h>
#include <seqdef.h>

/*
  Functions:
  ==========
  seqInit_SPMARIO     yes (aka seqInit_MARIOSTORY)
  seqMain             yes (only matching spm 1:1, logic matching for ttyd)
  seqSetSeq           yes
  seqGetSeq           yes
  seqGetPrevSeq       yes (ttyd exclusive)
  seqGetNextSeq       yes (ttyd exclusive)
  seqCheckSeq         yes (ttyd exclusive)

  Other:
  ======
  Data matching except SeqWork missing fields (but those are not referenced in seqdrv.c code)
*/

enum SeqdrvSeq
{
    SEQ_LOGO = 0,
    SEQ_TITLE,
    SEQ_GAME,
    SEQ_MAPCHANGE,
    SEQ_GAMEOVER,
    SEQ_LOAD
};

typedef struct _SeqWork {
  s32 seq;
  s32 stage;
  char * p0;
  char * p1;
  u8 unknown_0x10[0x20 - 0x10];
#ifdef GAME_SPM
  SeqFunc * afterFunc;
#endif
} SeqWork;

#ifdef GAME_SPM
void seqInit_SPMARIO(); // 8017bf2c
#else
void seqInit_MARIOSTORY();
#endif
void seqMain(); // 8017bf6c
void seqSetSeq(s32 seq, char * p0, char * p1); // 8017c074
s32 seqGetSeq(); // 8017c084
#ifndef GAME_SPM
s32 seqGetPrevSeq();
s32 seqGetNextSeq();
bool seqCheckSeq();
#endif

#endif