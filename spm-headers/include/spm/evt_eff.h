#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_eff)

EVT_DECLARE_USER_FUNC(evt_eff, 14)

// evt_eff_softdelete(const char * name)
EVT_DECLARE_USER_FUNC(evt_eff_softdelete, 1)

UNKNOWN_FUNCTION(func_800e617c);
UNKNOWN_FUNCTION(func_800e61b0);
UNKNOWN_FUNCTION(func_800e61e0);
UNKNOWN_FUNCTION(func_800e6210);
EVT_UNKNOWN_USER_FUNC(func_800e6250)

CPP_WRAPPER_END()
