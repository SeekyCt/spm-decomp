#pragma once

#include <common.h>
#include <evt_cmd.h>

// evt_pouch_check_have_item(s32 id, bool& ret)
EVT_DECLARE_USER_FUNC(evt_pouch_check_have_item, 2)

// evt_pouch_add_item(s32 id)
EVT_DECLARE_USER_FUNC(evt_pouch_add_item, 1)

// evt_pouch_set_pixl_selected(s32 id)
EVT_DECLARE_USER_FUNC(evt_pouch_set_pixl_selected, 1)
