#pragma once

#include <common.h>
#include <spm/seqdrv.h>

CPP_WRAPPER(spm::seq_load)

USING(spm::seqdrv::SeqWork)

UNKNOWN_FUNCTION(initNewSaveFile);
void seq_loadInit(SeqWork * work);
void seq_loadMain(SeqWork * work);
void seq_loadExit(SeqWork * work);

CPP_WRAPPER_END()
