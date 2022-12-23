#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_msg)

void evt_msg_init();

UNKNOWN_FUNCTION(_evt_msg_print);

// evt_msg_print(u32 flags, const char * msg, s32, const char * speaker)
EVT_DECLARE_USER_FUNC(evt_msg_print, 4)

UNKNOWN_FUNCTION(evt_msg_print_insert);
UNKNOWN_FUNCTION(evt_msg_print_add);
UNKNOWN_FUNCTION(evt_msg_print_add_insert);

// evt_msg_continue()
EVT_DECLARE_USER_FUNC(evt_msg_continue, 0)

UNKNOWN_FUNCTION(evt_msg_repeat);
UNKNOWN_FUNCTION(evt_msg_close);
UNKNOWN_FUNCTION(evt_msg_select);
UNKNOWN_FUNCTION(evt_msg_toge);
UNKNOWN_FUNCTION(evt_msg_togelr);
UNKNOWN_FUNCTION(evt_msg_pri);
UNKNOWN_FUNCTION(evtMsgFmtStr);
UNKNOWN_FUNCTION(evt_msg_fmt_str);
UNKNOWN_FUNCTION(evtMsgFmtInt);
UNKNOWN_FUNCTION(evt_msg_fmt_int);
UNKNOWN_FUNCTION(func_800fdc6c);

CPP_WRAPPER_END()
