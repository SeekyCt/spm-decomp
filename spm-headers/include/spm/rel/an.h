#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::an)

EVT_DECLARE_USER_FUNC(evt_an_init_tpl, 0)
EVT_DECLARE_USER_FUNC(evt_an_darkness_onoff, 1)
EVT_DECLARE_USER_FUNC(evt_an_remove_npcs, 1)
// more

CPP_WRAPPER_END()
