#pragma once

#include <evt_cmd.h>

// Script to save the game
EVT_DECLARE(mobj_save_blk_sysevt)

EVT_UNKNOWN_USER_FUNC(evt_mobj_exec_cancel)

// evt_mobj_flag_onoff(bool on, bool flag4, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mobj_flag_onoff, 4)

// evt_mobj_zyo(const char * name, s32 lockItemId, float x, float y, float z, float yRotation,
//              EvtScriptCode * interactScript, EvtScriptCode * openScript, EvtVar collectedVar)
EVT_DECLARE_USER_FUNC(evt_mobj_zyo, 9)

// evt_mobj_thako(s32 type, const char * name, float x, float y, float z, EvtScriptCode * interactScript,
//                EvtScriptCode * openScript, UNK arg8, UNK arg9)
EVT_DECLARE_USER_FUNC(evt_mobj_thako, 9)

// evt_mobj_kan(bool rotation, const char * name, float x, float y, float z, EvtScriptCode * interactScript,
//              UNK arg7)
EVT_DECLARE_USER_FUNC(evt_mobj_kan, 7)

// evt_mobj_arrow(const char * name, float x, float y, float z, UNK arg5)
EVT_DECLARE_USER_FUNC(evt_mobj_arrow, 5)

// evt_mobj_wait_animation_end(const char * name, UNUSED)
EVT_DECLARE_USER_FUNC(evt_mobj_wait_animation_end, 2)

// evt_mobj_get_position(const char * name, float& x, float& y, float& z)
EVT_DECLARE_USER_FUNC(evt_mobj_get_position, 4)

// evt_mobj_set_position(const char * name, float x, float y, float z)
EVT_DECLARE_USER_FUNC(evt_mobj_set_position, 4)

// evt_mobj_hit_onoff(bool on, const char * name)
EVT_DECLARE_USER_FUNC(evt_mobj_hit_onoff, 2)
