#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_map)

// evt_mapobj_trans(const char * name, s32 x, s32 y, s32 z)
EVT_DECLARE_USER_FUNC(evt_mapobj_trans, 4)

UNKNOWN_FUNCTION(evt_mapobj_rotate);
UNKNOWN_FUNCTION(func_800ed7f8);
UNKNOWN_FUNCTION(evt_map_set_fog);
UNKNOWN_FUNCTION(evt_map_fog_onoff);
UNKNOWN_FUNCTION(func_800ed9b0);
UNKNOWN_FUNCTION(func_800eda74);
UNKNOWN_FUNCTION(func_800edab4);

// evt_mapobj_color(s32 group, const char * name, u8 r, u8 g, u8 b, u8 a)
EVT_DECLARE_USER_FUNC(evt_mapobj_color, 6)

EVT_DECLARE_USER_FUNC(evt_map_playanim, 3)

UNKNOWN_FUNCTION(func_800edca8);

EVT_DECLARE_USER_FUNC(evt_map_checkanim, 3)

UNKNOWN_FUNCTION(func_800edd50);
UNKNOWN_FUNCTION(func_800eddb4);
UNKNOWN_FUNCTION(evt_map_set_playrate);
UNKNOWN_FUNCTION(func_800ede70);
UNKNOWN_FUNCTION(func_800edec8);

// evt_mapobj_flag_onoff(s32 group, bool on, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mapobj_flag_onoff, 4)

// evt_mapobj_flag4_onoff(s32 group, bool on, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mapobj_flag4_onoff, 4)

UNKNOWN_FUNCTION(func_800ee0b4);
UNKNOWN_FUNCTION(func_800ee13c);

// evt_mapobj_get_position(const char * name, f32& x, f32& y, f32& z)
EVT_DECLARE_USER_FUNC(evt_mapobj_get_position, 4)

UNKNOWN_FUNCTION(func_800ee290);
UNKNOWN_FUNCTION(func_800ee51c);

EVT_DECLARE_USER_FUNC(evt_mapdisp_onoff, 1)

UNKNOWN_FUNCTION(func_800ee59c);
UNKNOWN_FUNCTION(evt_mapobj_blendmode);
UNKNOWN_FUNCTION(func_800ee9f4);
UNKNOWN_FUNCTION(func_800eec8c);
UNKNOWN_FUNCTION(func_800eee68);
UNKNOWN_FUNCTION(func_800ef198);

CPP_WRAPPER_END()
