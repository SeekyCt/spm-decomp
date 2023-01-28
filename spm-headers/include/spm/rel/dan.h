#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/evt_door.h>
#include <spm/evt_shop.h>
#include <spm/npcdrv.h>

CPP_WRAPPER(spm::dan)

USING(spm::evt_door::DokanDesc)
USING(spm::evt_door::MapDoorDesc)
USING(spm::evt_shop::EvtShopDef)
USING(spm::npcdrv::NPCTribeAnimDef)

#define DUNGEON_MAX 200
#define DAN_PARTS_COUNT 16

// A definition of a possible door setup in the room, read from pitText
typedef struct
{
    s32 enter; // index of the door to enter the room with
    s32 exit; // index of the door to exit the room with
} DanDoor;
SIZE_ASSERT(DanDoor, 0x8)

// A definition of enemies to spawn in the room, read from pitText
typedef struct
{
    s32 name; // tribe id + 1 of the enemy type
    s32 num; // how many of the enemy to spawn
    s32 pos; // where to spawn the enemy, 0 for random
} DanEnemy;
SIZE_ASSERT(DanEnemy, 0xc)

// A definition of one pit room, read from pitText
typedef struct
{
        // flags for parts of the map to enable
        // ( https://cdn.discordapp.com/attachments/498246688130203658/858790159141830716/pitparts.png )
    u32 map;
    s32 item; // only used in chest rooms
    s32 doorCount; // amount of used entries in the doors array
    s32 enemyCount; // amount of used entries in the enemies array
    DanDoor doors[20]; // possible door setups in this room
    DanEnemy enemies[16]; // enemies to spawn in this room
} DanDungeon;
SIZE_ASSERT(DanDungeon, 0x170)

typedef struct
{
    DanDungeon * dungeons; // array of size DUNGEON_MAX
    DanDoor doorInfo; // the door configuration that ended up being used in this room
    s32 spawnTableCount; // number of entries used in the spawm table
    s32 spawnTable[32]; // door ids enemies can spawn at
    u8 unknown_0x90[0x110 - 0x90];
    char enterDoorName_l[64];
    char enterDoorName_r[64];
    char enterDoorHitName2d[64];
    char enterDoorHitName3d[64];
    char prevMapName[64];
    char enterDoor_desc0x18[64];
    char exitDoorName_l[64];
    char exitDoorName_r[64];
    char exitDoorHitName2d[64];
    char exitDoorHitName3d[64];
    char nextMapName[64];
    char exitDoor_desc0x18[64];
    u8 unknown_0x410[0x510 - 0x410];
    char enterDoorName[64];
    char exitDoorName[64];
} DanWork;
SIZE_ASSERT(DanWork, 0x590)

DECOMP_STATIC(DanWork * dan_wp)

DECOMP_STATIC(const char * dan_mapParts[16])
DECOMP_STATIC(DokanDesc dan_dokanDescs[8])
DECOMP_STATIC(MapDoorDesc dan_mapDoorDescs[2])
DECOMP_STATIC(s32 dan_flipsideLockItems[])
DECOMP_STATIC(s32 dan_flopsideLockItems[])

EVT_DECLARE(dan_lock_interact_evt)
EVT_DECLARE(dan_lock_open_evt)
EVT_DECLARE(dan_enemy_room_init_evt)

DECOMP_STATIC(DokanDesc dan_chestRoomDokanDesc)
DECOMP_STATIC(MapDoorDesc dan_chestRoomMapDoorDescs[2])

EVT_DECLARE(dan_exit_pipe_sign_interact_evt)
EVT_DECLARE(dan_chest_open_evt)

DECOMP_STATIC(s32 dan_rotenShopItems[])
DECOMP_STATIC(EvtShopDef dan_rotenShopDef)
DECOMP_STATIC(NPCTribeAnimDef dan_rotenTribeAnimDefs[])

EVT_DECLARE(dan_chest_room_init_evt)

DECOMP_STATIC(MapDoorDesc dan_wracktail_map_door_desc)
DECOMP_STATIC(DokanDesc dan_wracktail_dokan_desc)
DECOMP_STATIC(DokanDesc dan_shadoo_dokan_desc)

EVT_DECLARE(dan_30_init_evt)
EVT_DECLARE(dan_70_init_evt)

DECOMP_STATIC(NPCTribeAnimDef dan_dashellTribeAnimDefs[])

EVT_DECLARE(dan_30_chest_interact_evt)
EVT_DECLARE(dan_30_chest_open_evt)

DECOMP_STATIC(NPCTribeAnimDef dan_wracktailTribeAnimDefs[])

EVT_DECLARE(dan_wracktail_main_evt)
EVT_DECLARE(dan_start_wracktail_evt)
EVT_DECLARE(dan_70_mario_chest_open_evt)
EVT_DECLARE(dan_70_dark_mario_chest_open_evt)
EVT_DECLARE(dan_70_peach_chest_open_evt)
EVT_DECLARE(dan_70_dark_peach_chest_open_evt)
EVT_DECLARE(dan_70_bowser_chest_open_evt)
EVT_DECLARE(dan_70_dark_bowser_chest_open_evt)
EVT_DECLARE(dan_70_luigi_chest_open_evt)
EVT_DECLARE(dan_70_dark_luigi_chest_open_evt)
EVT_DECLARE(dan_shadoo_main_evt)
EVT_DECLARE(dan_shadoo_fight_evt)
EVT_DECLARE(dan_shadoo_defeat_evt)
EVT_DECLARE(dan_70_reward_appear_evt)
EVT_DECLARE(dan_start_shadoo_evt)

/*
    Init work, decompress & read pitText
*/
// evt_dan_read_data()
EVT_DECLARE_USER_FUNC(evt_dan_read_data, 0)

/*
    Enable & disable parts of the map based on this dungeon's <map> flags
*/
// evt_dan_handle_map_parts(s32 no)
EVT_DECLARE_USER_FUNC(evt_dan_handle_map_parts, 1)

/*
    Enable & disable parts of the map based on this dungeon's <map> flags
*/
// evt_dan_handle_dokans(s32 no)
EVT_DECLARE_USER_FUNC(evt_dan_handle_dokans, 1)

/*
    Select which doors to use as the entrance and exit randomly based on this map's <door> entries
    Outputs the names of the enter & exit doors and the co-ordinates for the lock to spawn at
*/
// evt_dan_handle_doors(s32 no, s32 room, &char * enterDoor, &char * exitDoor, &f32 lockX,
//                      &f32 lockY, &f32 lockZ)
EVT_DECLARE_USER_FUNC(evt_dan_handle_doors, 7)

/*
    Get the enter and exit door names
*/
// evt_dan_get_door_names(&char * enterName, &char * exitName)
EVT_DECLARE_USER_FUNC(evt_dan_get_door_names, 2)

/*
    Get the _l name for the exit door
*/
// evt_dan_get_exit_door_name_l(&char * name)
EVT_DECLARE_USER_FUNC(evt_dan_get_exit_door_name_l, 1)

/*
    Get the template id and amount of the enemy to spawn for an index into this map's <enemy> entries
*/
// evt_dan_get_enemy_info(s32 no, s32 enemyIdx, &s32 templateId, &s32 num)
EVT_DECLARE_USER_FUNC(evt_dan_get_enemy_info, 4)

/*
    Makes this dungeon's spawn table based on enabled map parts
*/
// evt_dan_make_spawn_table(s32 no)
EVT_DECLARE_USER_FUNC(evt_dan_make_spawn_table, 1)

/*
    Returns the spawn coordinates for an enemy which is within the group at <enemyIdx>
    num is a counter of how many enemies have spawned before this one (in any group)
*/
// evt_dan_get_enemy_spawn_pos(s32 num, s32 no, s32 enemyIdx, &f32 x, &f32 y, &f32 z)
EVT_DECLARE_USER_FUNC(evt_dan_get_enemy_spawn_pos, 6)

/*
    Gives a key of the specified item id to a random enemy
*/
// evt_dan_decide_key_enemy(s32 itemId)
EVT_DECLARE_USER_FUNC(evt_dan_decide_key_enemy, 1)

/*
    Hud countdown callback, kills the player
*/
void danCountdownDone();

/*
    Starts the 5 minute countdown
*/
// evt_dan_start_countdown()
EVT_DECLARE_USER_FUNC(evt_dan_start_countdown, 0)

/*
    Returns whether a key exists on the ground within the map boundaries
*/
bool danCheckKeyInMapBbox();

/*
    Returns whether any enemy exists within the map boundaries
*/
bool danCheckEnemyInMapBbox();

/*
    user_func that runs endlessly in a parallel script to spawn the key at the door
    if it becomes impossible to obtain
*/
// evt_dan_handle_key_failsafe()
EVT_DECLARE_USER_FUNC(evt_dan_handle_key_failsafe, 0)

/*
    Sets the entering door name, exit pipe destination and door destination for a chest room
*/
// evt_dan_handle_chest_room_dokans_and_doors(s32 no)
EVT_DECLARE_USER_FUNC(evt_dan_handle_chest_room_dokans_and_doors, 1)

/*
    Returns the item that should be in the chest for the current chest room
*/
// evt_dan_get_chest_room_item(s32 no, &s32 itemId)
EVT_DECLARE_USER_FUNC(evt_dan_get_chest_room_item, 2)

/*
    Sets the entering door name for a boss room
*/
// evt_dan_boss_room_set_door_name()
EVT_DECLARE_USER_FUNC(evt_dan_boss_room_set_door_name, 0)

void func_80c839cc(const char * param_1, bool param_2);
void wracktailDispCb(void * param, s32 animGroupIdx, s32 param_3);

// evt_dan_set_wracktail_disp_cb()
EVT_DECLARE_USER_FUNC(evt_dan_set_wracktail_disp_cb, 0)

// func_80c83c48()
EVT_DECLARE_USER_FUNC(func_80c83c48, 0)

void screenBlinkDisp(s32 cameraId, void * param);

// evt_dan_screen_blink()
EVT_DECLARE_USER_FUNC(evt_dan_screen_blink, 0)

const char * func_80c83f6c(const char * param_1);

CPP_WRAPPER_END()
