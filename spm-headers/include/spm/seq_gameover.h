#pragma once

#include <common.h>
#include <spm/seqdrv.h>

CPP_WRAPPER(spm::seq_gameover)

USING(spm::seqdrv::SeqWork)

void seq_gameOverInit(SeqWork * work);
void seq_gameOverMain(SeqWork * work);
void seq_gameOverExit(SeqWork * work);

CPP_WRAPPER_END()
