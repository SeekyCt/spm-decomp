#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::an2_08)

typedef struct RpgNPC {
/* 0x00 */ s32 flags;
/* 0x04 */ s32 stunTime;
/* 0x08 */ s32 maxHp;
/* 0x0C */ s32 attackStrength;
/* 0x10 */ s32 killDisappearTimer;
/* 0x14 */ s32 killXp;
} RpgNPC;
SIZE_ASSERT(RpgNPC, 0x18)

typedef struct RpgMenuOption {
/* 0x00 */ const char * name;
/* 0x04 */ s32 index;
} RpgMenuOption;
SIZE_ASSERT(RpgMenuOption, 0x8)

typedef struct RpgMenu {
/* 0x00 */ u16 flags;
/* 0x02 */ u16 __pad_02;
/* 0x04 */ f32 unk_04;
/* 0x08 */ f32 unk_08;
/* 0x0c */ f32 unk_0c;
/* 0x10 */ f32 unk_10;
/* 0x14 */ RpgMenuOption options[16];
/* 0x94 */ s32 selected;
/* 0x98 */ int numOptions;
} RpgMenu;
SIZE_ASSERT(RpgMenu, 0x9c)

typedef struct RpgWork {
/* 0x00 */ s32 flags;
/* 0x04 */ s32 num;
/* 0x08 */ RpgMenu* menus;
/* 0x0C */ RpgNPC rpgNpcInfo[3];
/* 0x54 */ s32 statusEffects;
/* 0x58 */ s32 statusEffectsTimer[15]; // The amount of time in turns until a status effect wears off
/* 0x94 */ s32 totalScore;
} RpgWork; //sizeof 0x98
SIZE_ASSERT(RpgWork, 0x98)

DECOMP_STATIC(RpgWork *rpgdrv_wp)
DECOMP_STATIC(const char * lbl_80def2c8[4])

EVT_DECLARE(begin_rpg_parent_evt)
EVT_DECLARE(final_rpg_child_evt)
EVT_DECLARE(underchomp_attack_1_evt)
EVT_DECLARE(underchomp_attack_2_evt)
EVT_DECLARE(underchomp_attack_3_evt)
EVT_DECLARE(rpg_check_win_or_continue_evt)
EVT_DECLARE(rpg_attacking_evt)
EVT_DECLARE(rpg_use_pixls_evt)
EVT_DECLARE(rpg_use_items_evt)
EVT_DECLARE(rpg_run_away_evt)
EVT_DECLARE(rpg_snd_miss_evt)
EVT_DECLARE(rpg_snd_hit_evt)

UNKNOWN_FUNCTION(func_80c6c908)
s32 rpgHandleMenu(s32 type, RpgMenuOption* options);
u32 func_80c6cccc(double param_1,double param_2,double param_3,double param_4, s32 type, u16 param_6);
UNKNOWN_FUNCTION(func_80c6ce24)
void rpg_screen_draw();

EVT_DECLARE_USER_FUNC(evt_rpg_npctribe_handle, 0)
EVT_DECLARE_USER_FUNC(evt_rpg_change_menu_flag, 0)
EVT_DECLARE_USER_FUNC(evt_rpg_choice_handler, 5)
EVT_DECLARE_USER_FUNC(evt_rpg_char_get, 1)
EVT_DECLARE_USER_FUNC(evt_rpg_underchomp_name_grab, 2)
EVT_DECLARE_USER_FUNC(evt_rpg_calc_damage_to_enemy, 3)
EVT_DECLARE_USER_FUNC(evt_rpg_enemy_take_damage, 4)
EVT_DECLARE_USER_FUNC(evt_rpg_calc_mario_damage, 2)
EVT_DECLARE_USER_FUNC(evt_rpg_mario_take_damage, 3)
EVT_DECLARE_USER_FUNC(evt_rpg_point_calculation, 2)
EVT_DECLARE_USER_FUNC(evt_rpg_enemy_death_check, 2)
EVT_DECLARE_USER_FUNC(evt_rpg_effect_check, 1)
EVT_DECLARE_USER_FUNC(evt_rpg_status_remove, 3)
EVT_DECLARE_USER_FUNC(evt_rpg_menu_effects_handler, 2)
EVT_DECLARE_USER_FUNC(evt_rpg_wakeup_check, 2)
EVT_DECLARE_USER_FUNC(evt_rpg_effects_handle, 2)
EVT_DECLARE_USER_FUNC(evt_rpg_calc_item_stats, 4)
EVT_DECLARE_USER_FUNC(evt_rpg_add_xp, 1)
EVT_DECLARE_USER_FUNC(evt_rpg_point_handling, 1)
EVT_DECLARE_USER_FUNC(evt_rpg_get_item_msg, 2)
EVT_DECLARE_USER_FUNC(evt_an2_08_draw_face, 2)
UNKNOWN_FUNCTION(func_80c72d5c)
UNKNOWN_FUNCTION(func_80c72e10)

CPP_WRAPPER_END()
