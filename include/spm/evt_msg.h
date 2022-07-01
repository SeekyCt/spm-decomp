#pragma once

#include <common.h>
#include <evt_cmd.h>

void evt_msg_init();

// evt_msg_print(u32 flags, const char * msg, s32, const char * speaker)
EVT_DECLARE_USER_FUNC(evt_msg_print, 4)

// evt_msg_continue()
EVT_DECLARE_USER_FUNC(evt_msg_continue, 0)
