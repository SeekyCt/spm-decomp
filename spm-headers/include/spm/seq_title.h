#pragma once

#include <common.h>
#include <spm/seqdrv.h>

CPP_WRAPPER(spm::seq_title)

USING(spm::seqdrv::SeqWork)

void seq_titleInit(SeqWork * work);
void seq_titleMain(SeqWork * work);
void seq_titleExit(SeqWork * work);
UNKNOWN_FUNCTION(func_8017b49c);
UNKNOWN_FUNCTION(seqTitleInitLayout);
UNKNOWN_FUNCTION(func_8017b8ac);

// No idea why this is in this file of all places, but it is
const char * getNextDanMapname(s32 dungeonNo);

CPP_WRAPPER_END()
