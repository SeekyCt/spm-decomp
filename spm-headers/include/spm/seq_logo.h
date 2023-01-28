/*
    seq_logo is the sequencethat displays the first 2 screens of the game
*/

#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/seqdrv.h>
#include <wii/tpl.h>

CPP_WRAPPER(spm::seq_logo)

USING(spm::evtmgr::EvtEntry)
USING(spm::seqdrv::SeqWork)
USING(wii::tpl::TPLHeader)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x10 - 0x0];
/* 0x10 */ TPLHeader * healthAndSafetyTPL;
/* 0x14 */ TPLHeader * holdSidewaysTPL;
/* 0x18 */ EvtEntry * nandCheckEvtEntry;
/* 0x1C */ s32 nandCheckEvtEntryId;
} SeqLogoWork;
SIZE_ASSERT(SeqLogoWork, 0x20)
DECOMP_STATIC(SeqLogoWork seq_logo_work)
DECOMP_STATIC(SeqLogoWork * seq_logo_wp)

void seq_logoInit(SeqWork * work);
void seq_logoMain(SeqWork * work);
void seq_logoExit(SeqWork * work);
UNKNOWN_FUNCTION(seq_logoDisp);

CPP_WRAPPER_END()
