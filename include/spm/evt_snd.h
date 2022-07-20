#pragma once

#include <evt_cmd.h>

// evt_snd_bgmon(s32 param_1, const char * name)
EVT_DECLARE_USER_FUNC(evt_snd_bgmon, 2)

EVT_DECLARE_USER_FUNC(func_800d36f4, 1)

// evt_snd_set_sfx_reverb_mode(s32 mode)
EVT_DECLARE_USER_FUNC(evt_snd_set_sfx_reverb_mode, 1)

EVT_UNKNOWN_USER_FUNC(evt_snd_bgmoff_f_d)
EVT_UNKNOWN_USER_FUNC(func_800d2294)

// evt_snd_sfxon_3d_player_character(const char * marioName, const char * peach,
//                                   const char * bowserName, const char * luigiName)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_3d_player_character, 4)

// evt_snd_sfxon_3d_player(const char * name)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_3d_player, 1)

EVT_UNKNOWN_USER_FUNC(func_800d2268)
EVT_UNKNOWN_USER_FUNC(func_800d35d8)
EVT_UNKNOWN_USER_FUNC(func_800d3144)

// evt_snd_get_bgm_wait_time(s32 bgmPlayerId, s32& ret)
EVT_DECLARE_USER_FUNC(evt_snd_get_bgm_wait_time, 2)

EVT_UNKNOWN_USER_FUNC(func_800d3644)
EVT_UNKNOWN_USER_FUNC(func_800d31a0)
EVT_UNKNOWN_USER_FUNC(func_800d22d8)
EVT_UNKNOWN_USER_FUNC(evt_snd_bgmon_f_d)

// evt_snd_sfxon_3d(const char * name, f32 x, f32 y, f32 z)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_3d, 4)

// evt_snd_sfxon(const char * name)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon, 1)

// evt_snd_get_last_sfx_id(s32& ret)
EVT_DECLARE_USER_FUNC(evt_snd_get_last_sfx_id, 1)

// evt_snd_sfxoff(s32 id)
EVT_DECLARE_USER_FUNC(evt_snd_sfxoff, 1)

// evt_snd_bgmoff(s32 bgmPlayerId)
EVT_DECLARE_USER_FUNC(evt_snd_bgmoff, 1)

// evt_snd_sfxon_npc(const char * sfxName, const char * npcName)
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_npc, 2)
