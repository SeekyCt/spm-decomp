/*
    Defines which functions should be called in each sequence for seqdrv
*/

#pragma once

#include <common.h>

CPP_WRAPPER(spm::seqdrv)

struct _SeqWork;

CPP_WRAPPER_END()

CPP_WRAPPER(spm::seqdef)

USING(spm::seqdrv::_SeqWork)

#define SEQ_COUNT 6

typedef void (SeqFunc)(struct _SeqWork *);

typedef struct
{
    SeqFunc * init;
    SeqFunc * main;
    SeqFunc * exit;
} SeqDef;
extern SeqDef seq_data[SEQ_COUNT];

CPP_WRAPPER_END()
