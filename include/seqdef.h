/*
    Defines which functions should be called in each sequence for seqdrv
*/
#ifndef SEQDEF_H
#define SEQDEF_H

#include <common.h>

#define SEQ_COUNT 6

struct _SeqWork;
typedef void (SeqFunc)(struct _SeqWork *);

typedef struct
{
    SeqFunc * init;
    SeqFunc * main;
    SeqFunc * exit;
} SeqDef;
extern SeqDef seq_data[SEQ_COUNT]; // 804287a8

#endif