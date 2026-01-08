#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/evtmgr.h>
#include <spm/npcdrv.h>

CPP_WRAPPER(spm::npc_shadoo)

extern "C"{

EVT_UNKNOWN_USER_FUNC(func_802154fc)
EVT_UNKNOWN_USER_FUNC(func_80215514)
EVT_UNKNOWN_USER_FUNC(func_80215540)
EVT_UNKNOWN_USER_FUNC(func_80215f44)
EVT_UNKNOWN_USER_FUNC(func_80224804)
EVT_UNKNOWN_USER_FUNC(func_80224874)
EVT_UNKNOWN_USER_FUNC(func_80225380)


}
CPP_WRAPPER_END()
