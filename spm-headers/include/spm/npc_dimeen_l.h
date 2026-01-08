#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/npcdrv.h>

CPP_WRAPPER(spm::npc_dimeen_l)

USING(spm::npcdrv::NPCDefense)

extern NPCDefense superDimentioHeadDefenses;
extern NPCDefense superDimentioBodyDefenses;

EVT_DECLARE(dimen_unk_fight_script_1)
EVT_DECLARE(dimen_unk_fight_script_2)
EVT_DECLARE(dimen_unk_fight_script_3)

// npc_dimeen_l_box_deal_damage(s32 unused, s32 flags, s32 damage, f32 posX, f32 posY, f32 posZ)
EVT_DECLARE_USER_FUNC(npc_dimeen_l_box_deal_damage, 6)

EVT_UNKNOWN_USER_FUNC(npc_dimeen_l_determine_move_pos)

EVT_UNKNOWN_USER_FUNC(func_801e5fd0)

CPP_WRAPPER_END()
