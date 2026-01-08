/*
    Functions used to spawn map objects with the user_func instruction in evt scripts,
    and the functions the objects use internally
*/

#pragma once

#include <evt_cmd.h>
#include <spm/evtmgr.h>
#include <spm/mobjdrv.h>

CPP_WRAPPER(spm::evt_mobj)

USING(spm::evtmgr::EvtEntry)
USING(spm::mobjdrv::MobjEntry)

// evt_mobj_sui handles switches, specifically those with a distinctive yellow stand
enum MobjSuiType
{
    /* 0x00 */ MOBJ_SUI_BLUE,        // Blue Switch
    /* 0x01 */ MOBJ_SUI_RED,         // Red Switch
    /* 0x02 */ MOBJ_SUI_BLUE_BIG,    // Big Blue Switch, requires Thudley to hit
    /* 0x03 */ MOBJ_SUI_RED_BIG,     // Big Red Switch, requires Thudley to hit
    /* 0x04 */ MOBJ_SUI_BLUE_2D,     // 2D-only Blue Switch
    /* 0x05 */ MOBJ_SUI_RED_2D,      // 2D-only Red Switch
    /* 0x06 */ MOBJ_SUI_BLUE_3D,     // 3D-only Blue Switch
    /* 0x07 */ MOBJ_SUI_RED_3D,      // 3D-only Red Switch
    /* 0x08 */ MOBJ_SUI_ZUN,         // Unfinished; red switch with a spinning heart & blue hit effect, disappears on hit
    /* 0x09 */ MOBJ_SUI_RED_NO1,     // Red Switch with a 1 on the side in 3D
    /* 0x0A */ MOBJ_SUI_RED_NO2,     // Red Switch with a 2 on the side in 3D
    /* 0x0B */ MOBJ_SUI_RED_NO3,     // Red Switch with a 3 on the side in 3D
    /* 0x0C */ MOBJ_SUI_RED_NO4,     // Red Switch with a 4 on the side in 3D
    /* 0x0D */ MOBJ_SUI_BLUE_SMALL,  // Very small Blue Switch, player needs similar Z axis to hit
    /* 0x0E */ MOBJ_SUI_RED_SMALL,   // Very small Red Switch, player needs similar Z axis to hit
    /* 0x0F */ MOBJ_SUI_STG3_BLUE,   // 3-3 Switch in the light blue state, toggles to pink on hit
    /* 0x10 */ MOBJ_SUI_STG3_PINK,   // 3-3 Switch in the pink state, toggles to light blue on hit
    /* 0x11 */ MOBJ_SUI_STG3_BLUE_2D // 2D-only Blue Switch, like type 0x04 but with the tile texture
};

// evt_mobj_blk handles blocks
enum MobjBlkType
{
    /* 0x00 */ MOBJ_BLK_BRICK_BREAKABLE,
    /* 0x01 */ MOBJ_BLK_QUESTION,
    /* 0x02 */ MOBJ_BLK_BRICK_COIN,
    /* 0x03 */ MOBJ_BLK_BRICK_MULTICOIN,
    /* 0x04 */ MOBJ_BLK_USED,
    /* 0x05 */ MOBJ_BLK_LADDER,
    /* 0x06 */ MOBJ_BLK_BREAKABLE_TILE,
    /* 0x07 */ MOBJ_BLK_BRICK_RED,
    /* 0x08 */ MOBJ_BLK_BRICK_2D,
    /* 0x09 */ MOBJ_BLK_BRICK_3D,
    /* 0x0A */ MOBJ_BLK_QUESTION_2D,
    /* 0x0B */ MOBJ_BLK_QUESTION_3D,
    /* 0x0C */ MOBJ_BLK_SWITCH_BLUE_2D,
    /* 0x0D */ MOBJ_BLK_SWITCH_BLUE_3D,
    /* 0x0E */ MOBJ_BLK_SWITCH_RED_2D,
    /* 0x0F */ MOBJ_BLK_SWITCH_RED_3D,
    /* 0x10 */ MOBJ_BLK_FLIP_2D,
    /* 0x11 */ MOBJ_BLK_FLIP_3D,
    /* 0x12 */ MOBJ_BLK_SWITCH_RED_NO1,
    /* 0x13 */ MOBJ_BLK_SWITCH_RED_NO2,
    /* 0x14 */ MOBJ_BLK_SWITCH_RED_NO3,
    /* 0x15 */ MOBJ_BLK_SWITCH_RED_NO4,
    /* 0x16 */ MOBJ_BLK_THROWABLE,
    /* 0x17 */ MOBJ_BLK_COLOUR,
    /* 0x18 */ MOBJ_BLK_SWITCH_RED,
    /* 0x19 */ MOBJ_BLK_SWITCH_BLUE,
    /* 0x1A */ MOBJ_BLK_GRAVITY_VERTICAL,
    /* 0x1B */ MOBJ_BLK_GRAVITY_HORIZONTAL,
    /* 0x1C */ MOBJ_BLK_BLECK_SWITCH_1,
    /* 0x1D */ MOBJ_BLK_BLECK_SWITCH_2,
    /* 0x1E */ MOBJ_BLK_BLECK_SWITCH_3,
    /* 0x1F */ MOBJ_BLK_BLECK_SWITCH_4
};

// evt_mobj_hunmer_blk handles the Yellow Blocks that require Cudge to be broken
enum MobjHunmerBlkType
{
    /* 0x00 */ MOBJ_HUNMER_BLK_SMALL, // Unused
    /* 0x01 */ MOBJ_HUNMER_BLK,
    /* 0x02 */ MOBJ_HUNMER_BLK_STG5, // Big yellow block, front masked like Floro Caverns
    /* 0x03 */ MOBJ_HUNMER_BLK_MACHI // Big yellow block, front masked like a brick in Flipside Outskirts
};

// evt_mobj_thako handles chests
enum MobjThakoType
{
    /* 0x00 */ MOBJ_THAKO,
    /* 0x01 */ MOBJ_THAKO_BIG,
    /* 0x02 */ MOBJ_THAKO_DAN,
    /* 0x03 */ MOBJ_THAKO_DAN_BIG
};

/*
    Saves the game
*/
EVT_DECLARE(mobj_save_blk_sysevt)

UNKNOWN_FUNCTION(evt_mobj_entry)

// evt_mobj_delete(const char * name)
EVT_DECLARE_USER_FUNC(evt_mobj_delete, 1)

EVT_UNKNOWN_USER_FUNC(evt_mobj_exec_cancel)

// evt_mobj_hit_onoff(bool on, const char * name)
EVT_DECLARE_USER_FUNC(evt_mobj_hit_onoff, 2)

UNKNOWN_FUNCTION(func_800f3714)

// evt_mobj_get_joint_names(const char * name, const char * a2Name, const char * a3Name)
EVT_DECLARE_USER_FUNC(evt_mobj_get_joint_names, 3)

// evt_mobj_flag_onoff(bool on, bool flag4, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mobj_flag_onoff, 4)

UNKNOWN_FUNCTION(func_800f38d8)
UNKNOWN_FUNCTION(func_800f3980)

// evt_mobj_get_position(const char * name, f32& x, f32& y, f32& z)
EVT_DECLARE_USER_FUNC(evt_mobj_get_position, 4)

// evt_mobj_set_position(const char * name, f32 x, f32 y, f32 z)
EVT_DECLARE_USER_FUNC(evt_mobj_set_position, 4)

// evt_mobj_set_scale(const char * name, f32 xScale, f32 yScale, f32 zScale)
EVT_DECLARE_USER_FUNC(evt_mobj_set_scale, 4)

// evt_mobj_set_rotation(const char * name, f32 xRotation, f32 yRotation, f32 zRotation)
EVT_DECLARE_USER_FUNC(evt_mobj_set_rotation, 4)

// evt_mobj_set_anim(const char * mobjName, const char * animName)
EVT_DECLARE_USER_FUNC(evt_mobj_set_anim, 2)

// evt_mobj_wait_animation_end(const char * name, UNUSED)
EVT_DECLARE_USER_FUNC(evt_mobj_wait_animation_end, 2)

UNKNOWN_FUNCTION(evt_mobj_set_interact_script)
UNKNOWN_FUNCTION(evt_mobj_set_item)
UNKNOWN_FUNCTION(func_800f3fa8)
UNKNOWN_FUNCTION(func_800f4018)
UNKNOWN_FUNCTION(evt_mobj_check)
UNKNOWN_FUNCTION(func_800f429c)
s32 mobj_sui(MobjEntry * mobj);
s32 mobj_sui_stg3(MobjEntry * mobj);

// evt_mobj_sui(s32 type, const char * instanceName, f32 x, f32 y, f32 z,
//              EvtScriptCode * hitScript, EvtVar/bool inactive, void * stg3SuiNames)
// stg3SuiNames is an instanceName array exclusively used for sui types 15/16 (Pink/Blue) to toggle other switches with types 15/16
EVT_DECLARE_USER_FUNC(evt_mobj_sui, 8)

UNKNOWN_FUNCTION(mobj_jump_kumo)
UNKNOWN_FUNCTION(mobj_big_jump_kumo)
UNKNOWN_FUNCTION(evt_mobj_jump_kumo)
UNKNOWN_FUNCTION(mobj_jumpstand)

// evt_mobj_jumpstand(s32 type, const char * instanceName, f32 x, f32 y, f32 z,
//              EvtScriptCode * interactScript, EvtVar inactive
EVT_DECLARE_USER_FUNC(evt_mobj_jumpstand, 8)

UNKNOWN_FUNCTION(mobj_hunmer_blk)

// evt_mobj_hunmer_blk(s32 type, const char * instanceName, f32 x, f32 y, f32 z,
//              EvtScriptCode * interactScript, EvtVar inactive)
EVT_DECLARE_USER_FUNC(evt_mobj_hunmer_blk, 7)

UNKNOWN_FUNCTION(mobj_zyo)
UNKNOWN_FUNCTION(func_800f5a48)

// evt_mobj_zyo(const char * name, s32 lockItemId, f32 x, f32 y, f32 z, f32 yRotation,
//              EvtScriptCode * interactScript, EvtScriptCode * openScript, EvtVar inactive)
// The interactScript normally handles window selection logic (although this is NOT hardcoded), openScript handles what happens after the lock is opened.
EVT_DECLARE_USER_FUNC(evt_mobj_zyo, 9)

UNKNOWN_FUNCTION(mobj_thako)

// evt_mobj_thako(s32 type, const char * name, f32 x, f32 y, f32 z, EvtScriptCode * interactScript,
//              EvtScriptCode * openScript, s32 item, EvtVar/bool opened)
EVT_DECLARE_USER_FUNC(evt_mobj_thako, 9)

UNKNOWN_FUNCTION(mobj_kan)

/*
    Spawns a sign mobj

    rotation:
        0 for facing 2d camera
        1 for perpendicular to 2d camera
*/

// evt_mobj_kan(bool rotation, const char * name, f32 x, f32 y, f32 z,
//              EvtScriptCode * interactScript, EvtVar UNUSED)
EVT_DECLARE_USER_FUNC(evt_mobj_kan, 7)

UNKNOWN_FUNCTION(mobj_arrow)

// evt_mobj_arrow(const char * name, f32 x, f32 y, f32 z, UNK arg5)
EVT_DECLARE_USER_FUNC(evt_mobj_arrow, 5)

/*
    Gets the item id for a mobj_blk to drop

    If using a fixed item, returns that
    If using a chance table, returns a random id from that
*/
UNKNOWN_FUNCTION(mobj_blk_decideItem)

UNKNOWN_FUNCTION(mobj_blk)
UNKNOWN_FUNCTION(mobj_blk_multicoin)
UNKNOWN_FUNCTION(mobj_blk_breakable)
UNKNOWN_FUNCTION(mobj_blk_used)
UNKNOWN_FUNCTION(mobj_blk_ladder)
UNKNOWN_FUNCTION(mobj_blk_flip)
UNKNOWN_FUNCTION(mobj_blk_switch)
UNKNOWN_FUNCTION(mobj_blk_throwable)
UNKNOWN_FUNCTION(mobj_blk_colour)
UNKNOWN_FUNCTION(mobj_blk_gravity)
UNKNOWN_FUNCTION(mobj_blk_bleck_switch)

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
//              s32 itemId / ChanceEntry * chanceTable, EvtScript * hitScript, bool inactive)
EVT_DECLARE_USER_FUNC(evt_mobj_blk, 8)

UNKNOWN_FUNCTION(mobj_eria_block)

// evt_mobj_eria_block(const char * instanceName, f32 x, f32 y, f32 z,
//              EvtScriptCode * interactScript, bool inactive)
EVT_DECLARE_USER_FUNC(evt_mobj_eria_block, 6)

UNKNOWN_FUNCTION(func_800f97f0)
UNKNOWN_FUNCTION(func_800f9b14)
UNKNOWN_FUNCTION(mobj_fire_block)

// evt_mobj_fire_block(const char * instanceName, f32 x, f32 y, f32 z,
//              s32 numFire, f32 angle, f32 moveDir, UNUSED arg8, bool stationary
// arg8 is technically EvtScriptCode * interactScript, but it's unused and nonfunctional
EVT_DECLARE_USER_FUNC(evt_mobj_fire_block, 9)

UNKNOWN_FUNCTION(mobj_hip_kui)

// evt_mobj_hip_kui(s32 type, const char * instanceName, f32 x, f32 y, f32 z,
//              EvtScriptCode * descendScript, EvtScriptCode * ascendScript, bool state
// type 0 is a medium height post, 1 is tall. state is 1 when ground pounded, else 0
// descendScript activates when ground pounding a post from up --> down; ascendScript activates down --> up
EVT_DECLARE_USER_FUNC(evt_mobj_hip_kui, 8)

UNKNOWN_FUNCTION(mobj_kaze)
UNKNOWN_FUNCTION(mobjGetKazePosition)
UNKNOWN_FUNCTION(updateKazePosition)
UNKNOWN_FUNCTION(mobjDeleteKaze)
UNKNOWN_FUNCTION(evt_mobj_kaze)
UNKNOWN_FUNCTION(mobj_save_blk)

/*
    Spawns a save block mobj
*/
// evt_mobj_save_blk(char * instanceName, f32 x, f32 y, f32 z, EvtScriptCode * interactScript)
EVT_DECLARE_USER_FUNC(evt_mobj_save_blk, 5)

CPP_WRAPPER_END()
