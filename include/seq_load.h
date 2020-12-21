#ifndef SEQ_LOAD_H
#define SEQ_LOAD_H

#include <common.h>
#include <seqdrv.h>

void seq_loadInit(SeqWork * work); // 8017c10c
void seq_loadMain(SeqWork * work); // 8017c13c
void seq_loadExit(SeqWork * work); // 8017c138

#endif