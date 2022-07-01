#pragma once

#include <common.h>
#include <evt_cmd.h>

EVT_UNKNOWN_USER_FUNC(func_800d4de4)
EVT_UNKNOWN_USER_FUNC(func_800d74a0)
EVT_UNKNOWN_USER_FUNC(func_800d7930)

EVT_UNKNOWN_USER_FUNC(evt_sub_intpl_msec_init)
EVT_UNKNOWN_USER_FUNC(evt_sub_intpl_msec_get_value)

// evt_sub_random(s32 max, s32& ret)
EVT_DECLARE_USER_FUNC(evt_sub_random, 2)

// evt_sub_animgroup_async(const char * name)
EVT_DECLARE_USER_FUNC(evt_sub_animgroup_async, 1)
