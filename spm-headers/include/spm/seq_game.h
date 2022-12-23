#pragma once

#include <common.h>
#include <spm/seqdrv.h>

CPP_WRAPPER(spm::seq_game)

USING(spm::seqdrv::SeqWork)

UNKNOWN_FUNCTION(seqGamePreloadScriptsSpawnedNpcs);
UNKNOWN_FUNCTION(seqGameReadConnectedMapNames);
UNKNOWN_FUNCTION(seqGamePreloadConnectedMaps);
UNKNOWN_FUNCTION(loadMapdataBin);
void seq_gameInit(SeqWork * work);
void seq_gameMain(SeqWork * work);
void seq_gameExit(SeqWork * work);

CPP_WRAPPER_END()
