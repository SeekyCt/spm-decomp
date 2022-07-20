#pragma once

#include <evt_cmd.h>

// evt_mapobj_flag_onoff(bool group, bool on, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mapobj_flag_onoff, 4)

// evt_mapobj_flag4_onoff(bool group, bool on, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mapobj_flag4_onoff, 4)

// evt_mapobj_trans(const char * name, s32 x, s32 y, s32 z)
EVT_DECLARE_USER_FUNC(evt_mapobj_trans, 4)

// evt_mapobj_color(bool group, const char * name, u8 r, u8 g, u8 b, u8 a)
EVT_DECLARE_USER_FUNC(evt_mapobj_color, 6)

// evt_mapobj_get_position(const char * name, f32& x, f32& y, f32& z)
EVT_DECLARE_USER_FUNC(evt_mapobj_get_position, 4)
