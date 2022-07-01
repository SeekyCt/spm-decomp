#pragma once

// evt_hitobj_onoff(const char * name, bool group, bool on)
EVT_DECLARE_USER_FUNC(evt_hitobj_onoff, 4)

// evt_hit_bind_mapobj(const char * hit_name, const char * map_name)
EVT_DECLARE_USER_FUNC(evt_hit_bind_mapobj, 2)

// evt_hit_bind_update(const char * hit_name)
EVT_DECLARE_USER_FUNC(evt_hit_bind_update, 1)

// evt_hitobj_attr_onoff(bool group, bool on, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_hitobj_attr_onoff, 4)
