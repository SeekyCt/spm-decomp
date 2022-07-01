#pragma once

#include <common.h>
#include <evt_cmd.h>

// evt_npc_set_position(const char * instanceName, float x, float y, float z)
EVT_DECLARE_USER_FUNC(evt_npc_set_position, 4)

// evt_npc_entry(const char * instanceName, const char * animPoseName, s32 instanceId)
EVT_DECLARE_USER_FUNC(evt_npc_entry, 3)

EVT_DECLARE_USER_FUNC(func_80103054, 1)
EVT_DECLARE_USER_FUNC(evt_npc_set_anim, 3)
EVT_DECLARE_USER_FUNC(func_80104694, 2)

// evt_npc_flag8_onoff(const char * instanceName, bool on, u32 mask)
EVT_DECLARE_USER_FUNC(evt_npc_flag8_onoff, 3)

// evt_npc_animflag_onoff(const char * instanceName, bool on, u32 mask)
EVT_DECLARE_USER_FUNC(evt_npc_animflag_onoff, 3)

// evt_npc_set_property(const char * instanceName, s32 propertyId, s32 value)
EVT_DECLARE_USER_FUNC(evt_npc_set_property, 3)

// evt_npc_set_property(const char * instanceName, s32 partId, s32 propertyId, s32 value)
// partId -1 updates all parts
EVT_DECLARE_USER_FUNC(evt_npc_modify_part, 4)

EVT_DECLARE_USER_FUNC(func_801059d0, 2)

// evt_npc_entry_from_template(s32 instanceIdOverride, s32 templateId, s32 x, s32 y,
//                             s32 z, &s32 instanceId, &char * instanceName)
EVT_DECLARE_USER_FUNC(evt_npc_entry_from_template, 7)

EVT_DECLARE_USER_FUNC(func_80107cfc, 0)
EVT_DECLARE_USER_FUNC(func_80107d20, 0)
EVT_DECLARE_USER_FUNC(func_80108194, 2)

// evt_npc_teleport_effect(bool appearDisappear, const char * instanceName)
// Found by JohnP55
EVT_DECLARE_USER_FUNC(evt_npc_teleport_effect, 2)

EVT_UNKNOWN_USER_FUNC(func_80105b94)

// evt_npc_get_position(const char * name, float& x, float& y, float& z)
EVT_DECLARE_USER_FUNC(evt_npc_get_position, 4)

// evt_npc_jump_to(const char * name, float destX, float destY, float destZ, float height, s32 length)
EVT_DECLARE_USER_FUNC(evt_npc_jump_to, 6)

EVT_UNKNOWN_USER_FUNC(evt_npc_glide_to)

// evt_npc_get_axis_movement_unit(const char * name, float& ret)
EVT_DECLARE_USER_FUNC(evt_npc_get_axis_movement_unit, 2)

// evt_npc_set_camid(const char * name, s32 camId)
EVT_DECLARE_USER_FUNC(evt_npc_set_camid, 2)

EVT_UNKNOWN_USER_FUNC(func_800fef30)
EVT_UNKNOWN_USER_FUNC(func_80103410)

// evt_npc_tribe_agb_async(s32 tribeId)
EVT_DECLARE_USER_FUNC(evt_npc_tribe_agb_async, 1)

// evt_npc_delete(const char * name)
EVT_DECLARE_USER_FUNC(evt_npc_delete, 1)

EVT_UNKNOWN_USER_FUNC(func_80107c38)
EVT_UNKNOWN_USER_FUNC(func_80105708)
EVT_UNKNOWN_USER_FUNC(func_801055a4)
EVT_UNKNOWN_USER_FUNC(func_800ff8f8)
EVT_UNKNOWN_USER_FUNC(func_80105768)
EVT_UNKNOWN_USER_FUNC(func_80105550)
EVT_UNKNOWN_USER_FUNC(func_80102bf8)
EVT_UNKNOWN_USER_FUNC(func_801086fc)
EVT_UNKNOWN_USER_FUNC(func_801049ec)

// evt_npc_set_unitwork(const char * name, s32 idx, s32 val)
EVT_DECLARE_USER_FUNC(evt_npc_set_unitwork, 3)
