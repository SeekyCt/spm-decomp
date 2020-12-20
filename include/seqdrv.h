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

/*
  00004d58 000020 803ca978  1 .bss 	seqdrv.o 
  00004d58 000020 803ca978  4 seqWork 	seqdrv.o 
*/
extern SeqWork seqWork; // 80512360

/*
  00000080 00008c 80413da0  1 .sdata 	seqdrv.o 
  00000080 000004 80413da0  4 now_seq 	seqdrv.o 
  00000084 000004 80413da4  4 next_seq 	seqdrv.o 
  00000088 000004 80413da8  4 prev_seq 	seqdrv.o 
*/
extern int now_seq; // 805ae0b8
extern int next_seq; // 805ae0bc
extern int prev_seq; // 805ae0c0

/*
  00000080 000008 804151e0  1 .sbss 	seqdrv.o 
  00000080 000004 804151e0  4 next_p0 	seqdrv.o 
  00000084 000004 804151e4  4 next_p1 	seqdrv.o 
*/
extern char * next_p0; // 805ae918 - seqMapName
extern char * next_p1; // 805ae91c - seqDoorName

/*
  00027c6c 00016c 8002d24c  1 .text 	seqdrv.o 
  00027c6c 00001c 8002d24c  4 seqCheckSeq 	seqdrv.o 
  00027c88 000008 8002d268  4 seqGetNextSeq 	seqdrv.o 
  00027c90 000008 8002d270  4 seqGetPrevSeq 	seqdrv.o 
  00027c98 000008 8002d278  4 seqGetSeq 	seqdrv.o 
  00027ca0 000010 8002d280  4 seqSetSeq 	seqdrv.o 
  00027cb0 0000e8 8002d290  4 seqMain 	seqdrv.o 
  00027d98 000040 8002d378  4 seqInit_MARIOSTORY 	seqdrv.o 
*/
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