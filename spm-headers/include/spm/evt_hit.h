#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_hit)

UNKNOWN_FUNCTION(func_800eab1c);
UNKNOWN_FUNCTION(func_800eabb8);

// evt_hitobj_onoff(const char * name, s32 group, bool on)
EVT_DECLARE_USER_FUNC(evt_hitobj_onoff, 4)

UNKNOWN_FUNCTION(func_800ead20);
UNKNOWN_FUNCTION(func_800eadec);
UNKNOWN_FUNCTION(func_800eaed0);
UNKNOWN_FUNCTION(func_800eb15c);

// evt_hit_bind_mapobj(const char * hit_name, const char * map_name)
EVT_DECLARE_USER_FUNC(evt_hit_bind_mapobj, 2)

// evt_hit_bind_update(const char * hit_name)
EVT_DECLARE_USER_FUNC(evt_hit_bind_update, 1)

UNKNOWN_FUNCTION(func_800eb564);
UNKNOWN_FUNCTION(func_800eb5dc);
UNKNOWN_FUNCTION(func_800eb654);

// evt_hitobj_attr_onoff(s32 group, bool on, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_hitobj_attr_onoff, 4)

UNKNOWN_FUNCTION(func_800eb7f4);
UNKNOWN_FUNCTION(func_800eb8bc);
UNKNOWN_FUNCTION(func_800ebd74);

CPP_WRAPPER_END()
