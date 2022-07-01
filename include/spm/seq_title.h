#pragma once

#include <common.h>
#include <spm/seqdrv.h>

void seq_titleInit(SeqWork * work);
void seq_titleMain(SeqWork * work);
void seq_titleExit(SeqWork * work);

// more

// No idea why this is in this file of all places, but it is
const char * getNextDanMapname(s32 dungeonNo);
