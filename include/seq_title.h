#ifndef SEQ_TITLE_H
#define SEQ_TITLE_H

#include <common.h>
#include <seqdrv.h>

void seq_titleInit(SeqWork * work); // 8017b130
void seq_titleMain(SeqWork * work); // 8017b250
void seq_titleExit(SeqWork * work); // 8017b410

// more

// No idea why this is in this file of all places, but it is
const char * getNextDanMapname(int dungeonNo); // 8017bc4c

#endif