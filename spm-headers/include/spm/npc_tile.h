#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/npcdrv.h>
#include <spm/evtmgr.h>

CPP_WRAPPER(spm::npc_tile)

USING(spm::npcdrv::NPCEntry)
USING(spm::evtmgr::EvtEntry)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x14 - 0x0];
/* 0x14 */ f32 xRotationLimiter;
/* 0x18 */ f32 yRotationLimiter;
/* 0x1C */ f32 zRotationLimiter;
/* 0x00 */ u8 unknown_0x20[0x5c - 0x20];
/* 0x5C */ f32 rotationSpeedScale;
/* 0x60 */ f32 rotationSpeed1;
/* 0x64 */ f32 rotationSpeed2;
/* 0x00 */ u8 unknown_0x68[0xf0 - 0x68];
} TileoidWork;
SIZE_ASSERT(TileoidWork, 0xf0)

void npc_tile_init_work(EvtEntry * evtEntry);

s32 npc_tile_setup_work(EvtEntry * evtEntry);

EVT_DECLARE(npc_tile_common_onspawn_evt)

CPP_WRAPPER_END()
