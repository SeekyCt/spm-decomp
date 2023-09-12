#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_img)

EVT_DECLARE_USER_FUNC(evt_img_entry, 1)

UNKNOWN_FUNCTION(evt_img_set_position);
EVT_DECLARE_USER_FUNC(evt_img_set_paper, 2)
EVT_DECLARE_USER_FUNC(evt_img_set_paper_anim, 2)
EVT_DECLARE_USER_FUNC(evt_img_alloc_capture, 8)
EVT_DECLARE_USER_FUNC(evt_img_free_capture, 2)
UNKNOWN_FUNCTION(evt_img_clear_virtual_space);
EVT_DECLARE_USER_FUNC(evt_img_release, 1)
EVT_DECLARE_USER_FUNC(evt_img_wait_animend, 1)

UNKNOWN_FUNCTION(func_800ec998);
UNKNOWN_FUNCTION(func_800eca64);
UNKNOWN_FUNCTION(func_800ecae0);
UNKNOWN_FUNCTION(func_800ecb5c);
UNKNOWN_FUNCTION(func_800ecbd8);

CPP_WRAPPER_END()
