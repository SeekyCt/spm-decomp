#ifndef SEQDEF_H
#define SEQDEF_H

struct _SeqWork;
typedef void (SeqFunc)(struct _SeqWork *);

#ifdef GAME_SPM
    #define SEQ_COUNT 6
#else
    #define SEQ_COUNT 8
#endif

typedef struct {
    SeqFunc * init;
    SeqFunc * main;
    SeqFunc * exit;
} SeqDef;
/*
  00000298 0000ac 802ff6d8  1 .data 	seqdef.o 
  UNUSED   000008 ........ negone_one$374 seqdef.o 
  UNUSED   000008 ........ one_negone$373 seqdef.o 
  UNUSED   000008 ........ negone_one$360 seqdef.o 
  UNUSED   000008 ........ one_negone$359 seqdef.o 
  00000298 00006c 802ff6d8  4 seq_data 	seqdef.o 
*/
SeqDef seq_data[SEQ_COUNT];

#endif