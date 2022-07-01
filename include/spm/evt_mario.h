#pragma once

#include <common.h>
#include <evt_cmd.h>

EVT_UNKNOWN_USER_FUNC(evt_mario_key_on)
EVT_UNKNOWN_USER_FUNC(evt_mario_key_off)

// evt_mario_get_pos(float& x, float& y, float& z)
EVT_DECLARE_USER_FUNC(evt_mario_get_pos, 3)

// evt_mario_get_pos(float x, float y, float z)
EVT_DECLARE_USER_FUNC(evt_mario_set_pos, 3)

EVT_UNKNOWN_USER_FUNC(func_800efce0)
EVT_UNKNOWN_USER_FUNC(func_800efd88)
EVT_UNKNOWN_USER_FUNC(func_800f1684)

// evt_mario_set_pose(const char * name, s16 time)
EVT_DECLARE_USER_FUNC(evt_mario_set_pose, 2)
EVT_DECLARE_USER_FUNC(evt_mario_wait_anim, 0)

EVT_UNKNOWN_USER_FUNC(func_800f23e4)
EVT_UNKNOWN_USER_FUNC(func_800f240c)
EVT_UNKNOWN_USER_FUNC(func_800f2450)

// evt_mario_flag8_onoff(bool onOff, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mario_flag8_onoff, 2)

EVT_UNKNOWN_USER_FUNC(func_800f09c4)

// evt_mario_get_character(s32& ret)
EVT_DECLARE_USER_FUNC(evt_mario_get_character, 1)

// evt_mario_set_anim_change_handler(MarioAnimChangeHandler * handler)
EVT_DECLARE_USER_FUNC(evt_mario_set_anim_change_handler, 1)

// evt_mario_face_npc(const char * name)
EVT_DECLARE_USER_FUNC(evt_mario_face_npc, 1)

EVT_UNKNOWN_USER_FUNC(func_800f0074)
