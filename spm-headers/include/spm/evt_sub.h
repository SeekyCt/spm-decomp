#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_sub)

UNKNOWN_FUNCTION(func_800d378c);
EVT_DECLARE_USER_FUNC(evt_sub_intpl_msec_init, 4)
EVT_DECLARE_USER_FUNC(evt_sub_intpl_msec_get_value, 0)
UNKNOWN_FUNCTION(evt_sub_intpl_msec_get_value_para);
UNKNOWN_FUNCTION(evt_sub_spline_init);
UNKNOWN_FUNCTION(evt_sub_spline_get_value);
UNKNOWN_FUNCTION(evt_sub_spline_get_value_manual);
UNKNOWN_FUNCTION(evt_sub_spline_free);
UNKNOWN_FUNCTION(evt_sub_get_sincos);
UNKNOWN_FUNCTION(evt_sub_rumble_onoff);

// Gives a random number from 0 to max (inclusive)
// evt_sub_random(s32 max, s32& ret)
EVT_DECLARE_USER_FUNC(evt_sub_random, 2)

UNKNOWN_FUNCTION(evt_sub_get_stopwatch);
UNKNOWN_FUNCTION(func_800d41a8);
UNKNOWN_FUNCTION(evt_sub_get_dist);
UNKNOWN_FUNCTION(evt_sub_get_dir);
UNKNOWN_FUNCTION(evt_sub_get_system_flag);
UNKNOWN_FUNCTION(func_800d4460);
UNKNOWN_FUNCTION(evt_key_get_button);
UNKNOWN_FUNCTION(evt_key_get_buttonrep);
UNKNOWN_FUNCTION(evt_key_get_buttontrg);
UNKNOWN_FUNCTION(func_800d45ac);
UNKNOWN_FUNCTION(func_800d45dc);
UNKNOWN_FUNCTION(func_800d46a4);

//evt_sub_set_game_speed(float newSpeed)
EVT_DECLARE_USER_FUNC(evt_sub_set_game_speed, 1)

UNKNOWN_FUNCTION(evt_sub_get_mapname);


// Returns the door/bero name
// evt_sub_get_entername(&char* ret)
EVT_DECLARE_USER_FUNC(evt_sub_get_entername, 1)

UNKNOWN_FUNCTION(evt_sub_set_entername);
UNKNOWN_FUNCTION(func_800d47e4);
EVT_DECLARE_USER_FUNC(evt_sub_get_language, 1)

// evt_sub_animgroup_async(const char * name)
EVT_DECLARE_USER_FUNC(evt_sub_animgroup_async, 1)

UNKNOWN_FUNCTION(evt_sub_file_async);
UNKNOWN_FUNCTION(evt_sub_load_mapdata_bin);
UNKNOWN_FUNCTION(evt_sub_get_fps);
UNKNOWN_FUNCTION(evt_sub_fmt_str_int);
UNKNOWN_FUNCTION(func_800d4b4c);
UNKNOWN_FUNCTION(func_800d4b60);
UNKNOWN_FUNCTION(evt_hud_configure);
UNKNOWN_FUNCTION(func_800d4db0);
EVT_DECLARE_USER_FUNC(func_800d4de4, 2)
UNKNOWN_FUNCTION(func_800d4e48);
UNKNOWN_FUNCTION(func_800d5004);
UNKNOWN_FUNCTION(func_800d52a8);
UNKNOWN_FUNCTION(func_800d5588);
UNKNOWN_FUNCTION(func_800d59ac);
UNKNOWN_FUNCTION(func_800d59f0);
UNKNOWN_FUNCTION(func_800d5a24);
UNKNOWN_FUNCTION(func_800d5a60);
UNKNOWN_FUNCTION(func_800d5a94);
UNKNOWN_FUNCTION(func_800d5acc);
UNKNOWN_FUNCTION(func_800d5e80);
UNKNOWN_FUNCTION(func_800d6148);
UNKNOWN_FUNCTION(func_800d61ac);
UNKNOWN_FUNCTION(func_800d6230);
UNKNOWN_FUNCTION(func_800d6298);
UNKNOWN_FUNCTION(func_800d6308);
UNKNOWN_FUNCTION(func_800d6644);
UNKNOWN_FUNCTION(func_800d6674);
EVT_DECLARE_USER_FUNC(func_800d74a0, 2)
UNKNOWN_FUNCTION(func_800d776c);
UNKNOWN_FUNCTION(func_800d7858);
UNKNOWN_FUNCTION(evt_sub_get_save_name);
UNKNOWN_FUNCTION(evt_zero_vector);
EVT_UNKNOWN_USER_FUNC(func_800d7930)
UNKNOWN_FUNCTION(func_800d7b9c);
UNKNOWN_FUNCTION(func_800d7e70);
UNKNOWN_FUNCTION(func_800d815c);
UNKNOWN_FUNCTION(func_800d8498);
UNKNOWN_FUNCTION(func_800d8700);

CPP_WRAPPER_END()
