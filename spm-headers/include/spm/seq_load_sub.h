#pragma once

#include <common.h>
#include <spm/seqdrv.h>
#include <spm/evtmgr.h>
#include <spm/filemgr.h>

#include <wii/gx.h>

CPP_WRAPPER(spm::seq_load_sub)

USING(spm::seqdrv::SeqWork)
USING(spm::evtmgr::EvtEntry)
USING(spm::filemgr::FileEntry)
USING(wii::gx::GXColor)

typedef struct {
    u32 flags;
    u32 state;
    FileEntry* fileselTpl;
    s32 selectedSave;
    u8 _10[0x2c-0x10];
    EvtEntry* saveUpdateEvt;
    s32 saveUpdateEvtId;
    u8 _34[0x48-0x34];
    GXColor bgGradientTop;
    GXColor bgGradientBottom;
    u8 _50[0x5c-0x50];
} SeqLoadSubWork;
SIZE_ASSERT(SeqLoadSubWork, 0x5c)

DECOMP_STATIC(SeqLoadSubWork* seq_load_sub_wp)

s32 loadMain();

CPP_WRAPPER_END()
