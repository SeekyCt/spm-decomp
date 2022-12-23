/*
    Functions used to spawn map objects with the user_func instruction in evt scripts,
    and the functions the objects use internally
*/

#pragma once

#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_mobj)

/*
    Saves the game
*/
EVT_DECLARE(mobj_save_blk_sysevt)

UNKNOWN_FUNCTION(evt_mobj_entry);
UNKNOWN_FUNCTION(evt_mobj_delete);
EVT_UNKNOWN_USER_FUNC(evt_mobj_exec_cancel)

// evt_mobj_hit_onoff(bool on, const char * name)
EVT_DECLARE_USER_FUNC(evt_mobj_hit_onoff, 2)

UNKNOWN_FUNCTION(func_800f3714);
UNKNOWN_FUNCTION(func_800f379c);

// evt_mobj_flag_onoff(bool on, bool flag4, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mobj_flag_onoff, 4)

UNKNOWN_FUNCTION(func_800f38d8);
UNKNOWN_FUNCTION(func_800f3980);

// evt_mobj_get_position(const char * name, f32& x, f32& y, f32& z)
EVT_DECLARE_USER_FUNC(evt_mobj_get_position, 4)

// evt_mobj_set_position(const char * name, f32 x, f32 y, f32 z)
EVT_DECLARE_USER_FUNC(evt_mobj_set_position, 4)

UNKNOWN_FUNCTION(evt_mobj_set_scale);
UNKNOWN_FUNCTION(evt_mobj_set_rotation);
UNKNOWN_FUNCTION(evt_mobj_set_anim);

// evt_mobj_wait_animation_end(const char * name, UNUSED)
EVT_DECLARE_USER_FUNC(evt_mobj_wait_animation_end, 2)

UNKNOWN_FUNCTION(evt_mobj_set_interact_script);
UNKNOWN_FUNCTION(evt_mobj_set_item);
UNKNOWN_FUNCTION(func_800f3fa8);
UNKNOWN_FUNCTION(func_800f4018);
UNKNOWN_FUNCTION(evt_mobj_check);
UNKNOWN_FUNCTION(func_800f429c);
UNKNOWN_FUNCTION(mobj_sui);
UNKNOWN_FUNCTION(mobj_sui_15);
UNKNOWN_FUNCTION(evt_mobj_sui);
UNKNOWN_FUNCTION(mobj_jump_kumo);
UNKNOWN_FUNCTION(mobj_big_jump_kumo);
UNKNOWN_FUNCTION(evt_mobj_jump_kumo);
UNKNOWN_FUNCTION(mobj_jumpstand);
UNKNOWN_FUNCTION(evt_mobj_jumpstand);
UNKNOWN_FUNCTION(mobj_hunmer_blk);
UNKNOWN_FUNCTION(evt_mobj_hunmer_blk);
UNKNOWN_FUNCTION(mobj_zyo);
UNKNOWN_FUNCTION(func_800f5a48);

// evt_mobj_zyo(const char * name, s32 lockItemId, f32 x, f32 y, f32 z, f32 yRotation,
//              EvtScriptCode * interactScript, EvtScriptCode * openScript, EvtVar collectedVar)
EVT_DECLARE_USER_FUNC(evt_mobj_zyo, 9)

UNKNOWN_FUNCTION(mobj_thako);

// evt_mobj_thako(s32 type, const char * name, f32 x, f32 y, f32 z, EvtScriptCode * interactScript,
//                EvtScriptCode * openScript, UNK arg8, UNK arg9)
EVT_DECLARE_USER_FUNC(evt_mobj_thako, 9)

UNKNOWN_FUNCTION(mobj_kan);

/*
    Spawns a sign mobj
    
    rotation:
        0 for facing 2d camera
        1 for perpendicular to 2d camera
*/

// evt_mobj_kan(bool rotation, const char * name, f32 x, f32 y, f32 z,
//              EvtScriptCode * interactScript, UNK arg7)
EVT_DECLARE_USER_FUNC(evt_mobj_kan, 7)

UNKNOWN_FUNCTION(mobj_arrow);

// evt_mobj_arrow(const char * name, f32 x, f32 y, f32 z, UNK arg5)
EVT_DECLARE_USER_FUNC(evt_mobj_arrow, 5)

/*
    Gets the item id for a mobj_blk to drop

    If using a fixed item, returns that
    If using a chance table, returns a random id from that
*/
UNKNOWN_FUNCTION(mobj_blk_decideItem);

UNKNOWN_FUNCTION(mobj_blk);
UNKNOWN_FUNCTION(func_800f71d8);
UNKNOWN_FUNCTION(func_800f7680);
UNKNOWN_FUNCTION(func_800f7ac0);
UNKNOWN_FUNCTION(func_800f7bac);
UNKNOWN_FUNCTION(func_800f7d7c);
UNKNOWN_FUNCTION(func_800f8030);
UNKNOWN_FUNCTION(func_800f8214);
UNKNOWN_FUNCTION(func_800f821c);
UNKNOWN_FUNCTION(func_800f8408);
UNKNOWN_FUNCTION(func_800f8a08);

/*
    Spawns a block mobj (question, brick & variants)

    types:
        0 = brick block
        1 = question block
        the rest unknown

    6th param can be 2 things:
        s32 itemId
        or
        ChanceEntry * chanceTable
        Format is an array of the following struct, terminated by one with an id of 0
        struct ChanceEntry
        {
            s32 itemId;
            s32 chance;
        }
*/
// evt_mobj_blk(s32 type, char * instanceName, f32 x, f32 y, f32 z,
//              s32 itemId / ChanceEntry * chanceTable, EvtScript * hitScript, bool disableHitting)
EVT_DECLARE_USER_FUNC(evt_mobj_blk, 8)

UNKNOWN_FUNCTION(mobj_eria_block);
UNKNOWN_FUNCTION(evt_mobj_eria_block);
UNKNOWN_FUNCTION(func_800f97f0);
UNKNOWN_FUNCTION(func_800f9b14);
UNKNOWN_FUNCTION(mobj_fire_block);
UNKNOWN_FUNCTION(evt_mobj_fire_block);
UNKNOWN_FUNCTION(mobj_hip_kui);
UNKNOWN_FUNCTION(evt_mobj_hip_kui);
UNKNOWN_FUNCTION(mobj_kaze);
UNKNOWN_FUNCTION(func_800fab28);
UNKNOWN_FUNCTION(func_800fab88);
UNKNOWN_FUNCTION(func_800fac0c);
UNKNOWN_FUNCTION(evt_mobj_kaze);
UNKNOWN_FUNCTION(mobj_save_blk);

/*
    Spawns a save block mobj
*/
// evt_mobj_save_blk(char * instanceName, f32 x, f32 y, f32 z, EvtScriptCode * interactScript)
EVT_DECLARE_USER_FUNC(evt_mobj_save_blk, 5)

CPP_WRAPPER_END()
