#ifndef SEQDRV_H
#define SEQDRV_H

#include <stdbool.h>

#include "seqdef.h"

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

typedef struct _SeqWork {
  int seq;
  int stage;
  char * p0;
  char * p1;
  char unknown_0x10[0x20 - 0x10];
#ifdef GAME_SPM
  SeqFunc * afterFunc;
#endif
} SeqWork;
extern SeqWork seqWork; // 80512360

extern int now_seq; // 805ae0b8
extern int next_seq; // 805ae0bc
extern int prev_seq; // 805ae0c0

extern char * next_p0; // 805ae918 - seqMapName
extern char * next_p1; // 805ae91c - seqDoorName

#ifdef GAME_SPM
void seqInit_SPMARIO(); // 8017bf2c
#else
void seqInit_MARIOSTORY();
#endif
void seqMain(); // 8017bf6c
void seqSetSeq(int seq, char * p0, char * p1); // 8017c074
int seqGetSeq(); // 8017c084
#ifndef GAME_SPM
int seqGetPrevSeq();
int seqGetNextSeq();
bool seqCheckSeq();
#endif

#endif