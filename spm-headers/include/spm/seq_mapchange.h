/*
    seq_mapChange is the sequence that loads a map
*/

#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/seqdrv.h>

CPP_WRAPPER(spm::seq_mapchange)

USING(spm::evtmgr::EvtEntry)
USING(spm::seqdrv::SeqWork)

typedef struct
{
/* 0x00 */ char areaName[32];
/* 0x20 */ char mapName[32];
/* 0x40 */ char beroName[32]; // door the map was entered with
/* 0x60 */ EvtEntry * initEvtEntry;
/* 0x64 */ s32 initEvtEntryId;
/* 0x68 */ u8 unknown_0x68[0x74 - 0x68];
} SeqMapChangeWork;
SIZE_ASSERT(SeqMapChangeWork, 0x74)

DECOMP_STATIC(SeqMapChangeWork * seq_mapchange_wp)
DECOMP_STATIC(SeqMapChangeWork seq_mapchange_work)

void seq_mapChangeInit(SeqWork * work);
void seq_mapChangeExit(SeqWork * work);
void seq_mapChangeMain(SeqWork * work);
void _unload(const char * curMapName, const char * nextMapName, const char * doorName);

CPP_WRAPPER_END()
