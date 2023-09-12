#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_mario)

UNKNOWN_FUNCTION(evt_mario_flag0_onoff);
UNKNOWN_FUNCTION(evt_mario_flag4_onoff);

// evt_mario_flag8_onoff(bool onOff, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mario_flag8_onoff, 2)

UNKNOWN_FUNCTION(func_800ef53c);
UNKNOWN_FUNCTION(evt_mario_cont_onoff);



// evt_mario_key_on()
EVT_DECLARE_USER_FUNC(evt_mario_key_on, 0)

// evt_mario_key_off(int)
EVT_DECLARE_USER_FUNC(evt_mario_key_off, 1)

UNKNOWN_FUNCTION(evt_mario_key_off2);
UNKNOWN_FUNCTION(func_800ef814);
UNKNOWN_FUNCTION(func_800ef8c8);

// evt_mario_get_character(s32& ret)
EVT_DECLARE_USER_FUNC(evt_mario_get_character, 1)

EVT_DECLARE_USER_FUNC(evt_mario_set_character, 1)

// evt_mario_get_pos(f32 x, f32 y, f32 z)
EVT_DECLARE_USER_FUNC(evt_mario_set_pos, 3)

// evt_mario_get_pos(f32& x, f32& y, f32& z)
EVT_DECLARE_USER_FUNC(evt_mario_get_pos, 3)

UNKNOWN_FUNCTION(func_800efac4);
UNKNOWN_FUNCTION(func_800efb50);
UNKNOWN_FUNCTION(func_800efbdc);
UNKNOWN_FUNCTION(func_800efc54);
EVT_UNKNOWN_USER_FUNC(func_800efce0)

EVT_DECLARE_USER_FUNC(evt_mario_direction_reset, 0)

UNKNOWN_FUNCTION(func_800efd58);

EVT_DECLARE_USER_FUNC(evt_mario_direction_face, 2)

UNKNOWN_FUNCTION(func_800eff6c);

// evt_mario_face_npc(const char * name)
EVT_DECLARE_USER_FUNC(evt_mario_face_npc, 1)

// evt_mario_face_coords(float positionX, float PositionZ)
EVT_DECLARE_USER_FUNC(evt_mario_face_coords, 2)

UNKNOWN_FUNCTION(func_800f013c);
UNKNOWN_FUNCTION(func_800f0160);
UNKNOWN_FUNCTION(func_800f01ac);
UNKNOWN_FUNCTION(func_800f0210);
UNKNOWN_FUNCTION(evt_mario_face);
UNKNOWN_FUNCTION(evt_mario_face_free);
UNKNOWN_FUNCTION(func_800f0304);

EVT_DECLARE_USER_FUNC(evt_mario_pos_change, 3)

UNKNOWN_FUNCTION(func_800f05b0);
UNKNOWN_FUNCTION(func_800f074c);
EVT_UNKNOWN_USER_FUNC(func_800f09c4)
UNKNOWN_FUNCTION(func_800f0c28);
UNKNOWN_FUNCTION(func_800f0d58);
UNKNOWN_FUNCTION(func_800f119c);
EVT_UNKNOWN_USER_FUNC(func_800f1684)
UNKNOWN_FUNCTION(func_800f1778);
UNKNOWN_FUNCTION(func_800f1810);
UNKNOWN_FUNCTION(func_800f1858);

// evt_mario_set_pose(const char * name, s16 time)
EVT_DECLARE_USER_FUNC(evt_mario_set_pose, 2)

EVT_DECLARE_USER_FUNC(evt_mario_wait_anim, 0)
UNKNOWN_FUNCTION(func_800f1a08);
UNKNOWN_FUNCTION(func_800f1a4c);
UNKNOWN_FUNCTION(func_800f1abc);
UNKNOWN_FUNCTION(func_800f1b08);
UNKNOWN_FUNCTION(func_800f1ba8);
UNKNOWN_FUNCTION(func_800f1c1c);
UNKNOWN_FUNCTION(func_800f1c88);
UNKNOWN_FUNCTION(func_800f1d0c);
UNKNOWN_FUNCTION(func_800f1d80);
UNKNOWN_FUNCTION(func_800f1e30);
UNKNOWN_FUNCTION(func_800f1eb0);
UNKNOWN_FUNCTION(func_800f1f30);
UNKNOWN_FUNCTION(func_800f1f9c);
UNKNOWN_FUNCTION(func_800f2008);
UNKNOWN_FUNCTION(func_800f2074);
UNKNOWN_FUNCTION(func_800f2124);
UNKNOWN_FUNCTION(func_800f212c);
UNKNOWN_FUNCTION(func_800f2144);
UNKNOWN_FUNCTION(func_800f2310);
EVT_UNKNOWN_USER_FUNC(func_800f23e4)
EVT_UNKNOWN_USER_FUNC(func_800f240c)

EVT_DECLARE_USER_FUNC(evt_mario_fairy_reset, 0)

UNKNOWN_FUNCTION(evt_mario_swim_onoff);
UNKNOWN_FUNCTION(func_800f24d8);
UNKNOWN_FUNCTION(func_800f2544);
UNKNOWN_FUNCTION(evt_set_gravity);
UNKNOWN_FUNCTION(evt_get_gravity);
UNKNOWN_FUNCTION(func_800f262c);
UNKNOWN_FUNCTION(func_800f267c);
UNKNOWN_FUNCTION(func_800f26c0);
UNKNOWN_FUNCTION(func_800f27f4);
UNKNOWN_FUNCTION(func_800f2974);
UNKNOWN_FUNCTION(func_800f29c8);
UNKNOWN_FUNCTION(evt_mario_tamara_onoff);
UNKNOWN_FUNCTION(evt_mario_tamara_chg_mode);
UNKNOWN_FUNCTION(func_800f2c00);
UNKNOWN_FUNCTION(func_800f2c98);
UNKNOWN_FUNCTION(func_800f2cbc);
UNKNOWN_FUNCTION(func_800f2cfc);
UNKNOWN_FUNCTION(func_800f2d74);
UNKNOWN_FUNCTION(func_800f2df4);
UNKNOWN_FUNCTION(func_800f2e30);
UNKNOWN_FUNCTION(func_800f2e54);
UNKNOWN_FUNCTION(evt_mario_set_bottomless_cb);
UNKNOWN_FUNCTION(evt_mario_get_bottomless_cb);

// evt_mario_set_anim_change_handler(MarioAnimChangeHandler * handler)
EVT_DECLARE_USER_FUNC(evt_mario_set_anim_change_handler, 1)

UNKNOWN_FUNCTION(func_800f2fa8);
UNKNOWN_FUNCTION(func_800f2fec);
UNKNOWN_FUNCTION(func_800f3074);
UNKNOWN_FUNCTION(func_800f30bc);
UNKNOWN_FUNCTION(func_800f315c);
UNKNOWN_FUNCTION(evt_mario_set_pane_boundaries);
UNKNOWN_FUNCTION(evt_mario_get_pane_for_pos);
UNKNOWN_FUNCTION(evt_mario_set_pane);
UNKNOWN_FUNCTION(evt_mario_pane_change_func);
UNKNOWN_FUNCTION(evt_mario_get_pane_change_func);
UNKNOWN_FUNCTION(func_800f3310);
UNKNOWN_FUNCTION(func_800f3334);
UNKNOWN_FUNCTION(evt_mario_check_3d);
UNKNOWN_FUNCTION(func_800f33b0);
UNKNOWN_FUNCTION(evt_mario_calc_damage_to_enemy);

CPP_WRAPPER_END()
