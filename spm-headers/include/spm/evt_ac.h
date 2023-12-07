#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_ac)

// evt_ac_entry(const char * name, s32 type)
EVT_DECLARE_USER_FUNC(evt_ac_entry, 2)

//evt_ac_return_results(const char * name, s32& ret)
EVT_DECLARE_USER_FUNC(evt_ac_return_results, 2)

//evt_ac_delete(const char * name)
EVT_DECLARE_USER_FUNC(evt_ac_delete, 1)

CPP_WRAPPER_END()
