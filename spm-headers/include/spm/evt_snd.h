#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_snd)

// evt_snd_bgmon(s32 bgmPlayerId, const char * name)
EVT_DECLARE_USER_FUNC(evt_snd_bgmon, 2)

EVT_DECLARE_USER_FUNC(evt_snd_bgmon_f_d, 3)

// evt_snd_bgmoff(s32 bgmPlayerId)
EVT_DECLARE_USER_FUNC(evt_snd_bgmoff, 1)
EVT_DECLARE_USER_FUNC(evt_snd_bgmoff_f_d, 2)

EVT_UNKNOWN_USER_FUNC(func_800d2268)
EVT_UNKNOWN_USER_FUNC(func_800d2294)
EVT_UNKNOWN_USER_FUNC(func_800d22d8)
UNKNOWN_FUNCTION(func_800d231c);
UNKNOWN_FUNCTION(func_800d2388);
UNKNOWN_FUNCTION(func_800d23cc);
UNKNOWN_FUNCTION(func_800d2438);
UNKNOWN_FUNCTION(func_800d247c);

// evt_snd_get_bgm_wait_time(s32 bgmPlayerId, s32& ret)
EVT_DECLARE_USER_FUNC(evt_snd_get_bgm_wait_time, 2)

UNKNOWN_FUNCTION(evt_snd_get_bgm_name);

// evt_snd_sfxon(const char * name)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon, 1)


UNKNOWN_FUNCTION(func_800d2834);
UNKNOWN_FUNCTION(func_800d2894);

EVT_DECLARE_USER_FUNC(evt_snd_sfxon_character, 4)

// evt_snd_sfxon_3d(const char * name, f32 x, f32 y, f32 z)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_3d, 4)

UNKNOWN_FUNCTION(func_800d2a58);

// evt_snd_sfxon_npc(const char * sfxName, const char * npcName)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_npc, 2)

// evt_snd_sfxon_npc_delay(const char * sfxName, const char * npcName, s32 delayTime)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_npc_delay, 3)

// evt_snd_sfxon_3d_player(const char * name)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_3d_player, 1)

UNKNOWN_FUNCTION(func_800d2c58);

// evt_snd_sfxon_3d_player_character(const char * marioName, const char * peach,
//                                   const char * bowserName, const char * luigiName)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_3d_player_character, 4)

UNKNOWN_FUNCTION(func_800d2db8);
UNKNOWN_FUNCTION(func_800d2ed0);

// evt_snd_sfxoff(s32 id)
EVT_DECLARE_USER_FUNC(evt_snd_sfxoff, 1)

EVT_DECLARE_USER_FUNC(func_800d2fa4, 2)
UNKNOWN_FUNCTION(func_800d3000);

// evt_snd_get_last_sfx_id(s32& ret)
EVT_DECLARE_USER_FUNC(evt_snd_get_last_sfx_id, 1)
UNKNOWN_FUNCTION(func_800d3060);
UNKNOWN_FUNCTION(func_800d30e8);
EVT_UNKNOWN_USER_FUNC(func_800d3144)
EVT_UNKNOWN_USER_FUNC(func_800d31a0)
UNKNOWN_FUNCTION(func_800d31d0);
UNKNOWN_FUNCTION(func_800d3248);
EVT_DECLARE_USER_FUNC(evt_snd_sfx_wait, 1)
EVT_DECLARE_USER_FUNC(evt_snd_sfx_wait_name, 1)
UNKNOWN_FUNCTION(evt_snd_sfx_flag_on);
UNKNOWN_FUNCTION(evt_snd_sfx_flag_off);
UNKNOWN_FUNCTION(func_800d33dc);
UNKNOWN_FUNCTION(evt_snd_envon);
EVT_DECLARE_USER_FUNC(evt_snd_envon_f, 3)
EVT_DECLARE_USER_FUNC(evt_snd_string_call, 1)
UNKNOWN_FUNCTION(func_800d34e4);
UNKNOWN_FUNCTION(func_800d3528);
UNKNOWN_FUNCTION(func_800d3594);
EVT_UNKNOWN_USER_FUNC(func_800d35d8)
EVT_UNKNOWN_USER_FUNC(func_800d3644)
UNKNOWN_FUNCTION(func_800d3688);

// evt_snd_set_sfx_reverb_mode(s32 mode)
EVT_DECLARE_USER_FUNC(evt_snd_set_sfx_reverb_mode, 1)

UNKNOWN_FUNCTION(evt_snd_flag_on);
UNKNOWN_FUNCTION(evt_snd_flag_off);

CPP_WRAPPER_END()
