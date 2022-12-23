#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_guide)

UNKNOWN_FUNCTION(evt_guide_set_pos);
UNKNOWN_FUNCTION(evt_guide_get_pos);
UNKNOWN_FUNCTION(func_800e9ce8);
UNKNOWN_FUNCTION(func_800e9da4);
UNKNOWN_FUNCTION(func_800e9ddc);
UNKNOWN_FUNCTION(func_800e9e14);
UNKNOWN_FUNCTION(func_800e9ff8);
UNKNOWN_FUNCTION(func_800ea05c);
UNKNOWN_FUNCTION(func_800ea0a8);
UNKNOWN_FUNCTION(func_800ea25c);
UNKNOWN_FUNCTION(func_800ea3ec);
UNKNOWN_FUNCTION(func_800ea584);
UNKNOWN_FUNCTION(func_800ea718);
UNKNOWN_FUNCTION(func_800ea748);
UNKNOWN_FUNCTION(evt_guide_enter_run_mode_1);
UNKNOWN_FUNCTION(evt_guide_enter_runmode_2);
UNKNOWN_FUNCTION(func_800ea7e8);
UNKNOWN_FUNCTION(func_800ea858);

// evt_guide_flag2_onoff(bool onoff, u32 flags)
EVT_DECLARE_USER_FUNC(evt_guide_flag2_onoff, 2)

UNKNOWN_FUNCTION(evt_guide_flag0_onoff);

// evt_guide_check_flag0(u32 flags, bool &ret)
EVT_DECLARE_USER_FUNC(evt_guide_check_flag0, 2)

UNKNOWN_FUNCTION(func_800ea9f4);
EVT_UNKNOWN_USER_FUNC(func_800eaa88)
UNKNOWN_FUNCTION(func_800eaadc);

CPP_WRAPPER_END()
