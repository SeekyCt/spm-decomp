#ifndef DAN_H
#define DAN_H

#define DUNGEON_MAX 200
#define DAN_PARTS_COUNT 16

// A definition of a possible door setup in the room, read from pitText
typedef struct
{
    s32 enter; // index of the door to enter the room with
    s32 exit; // index of the door to exit the room with
} DanDoor; // total size 0x8

// A definition of enemies to spawn in the room, read from pitText
typedef struct
{
    s32 name; // tribe id + 1 of the enemy type
    s32 num; // how many of the enemy to spawn
    s32 pos; // where to spawn the enemy, 0 for random
} DanEnemy; // total size 0xc;

// A definition of one pit room, read from pitText
typedef struct
{
    u32 map; // flags for parts of the map to enable ( https://cdn.discordapp.com/attachments/498246688130203658/858790159141830716/pitparts.png )
    s32 item; // only used in chest rooms
    s32 doorCount; // amount of used entries in the doors array
    s32 enemyCount; // amount of used entries in the enemies array
    DanDoor doors[20]; // possible door setups in this room
    DanEnemy enemies[16]; // enemies to spawn in this room
} DanDungeon; // total size 0x170

typedef struct
{
    DanDungeon * dungeons; // array of size DUNGEON_MAX
    DanDoor doorInfo; // the door configuration that ended up being used in this room
    int spawnTableCount; // number of entries used in the spawm table
    int spawnTable[32]; // door ids enemies can spawn at
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
} DanWork; // total size 0x590

/*
    Init work, decompress & read pitText
*/
// evt_dan_read_data()
int evt_dan_read_data(EvtEntry * entry, bool isFirstCall); // 80c81eb0

/*
    Enable & disable parts of the map based on this dungeon's <map> flags
*/
// evt_dan_handle_map_parts(int no)
int evt_dan_handle_map_parts(EvtEntry * entry, bool isFirstCall); // 80c82188

/*
    Enable & disable parts of the map based on this dungeon's <map> flags
*/
// evt_dan_handle_dokans(int no)
int evt_dan_handle_dokans(EvtEntry * entry, bool isFirstCall); // 80c82448

/*
    Select which doors to use as the entrance and exit randomly based on this map's <door> entries
    Outputs the names of the enter & exit doors and the co-ordinates for the lock to spawn at
*/
// evt_dan_handle_doors(int no, int room, &char * enterDoor, &char * exitDoor, &float lockX, &float lockY, &float lockZ)
int evt_dan_handle_doors(EvtEntry * entry, bool isFirstCall); // 80c8269c

/*
    Get the enter and exit door names
*/
// evt_dan_get_door_names(&char * enterName, &char * exitName)
int evt_dan_get_door_names(EvtEntry * entry, bool isFirstCall); // 80c82abc

/*
    Get the _l name for the exit door
*/
// evt_dan_get_exit_door_name_l(&char * name)
int evt_dan_get_exit_door_name_l(EvtEntry * entry, bool isFirstCall); // 80c82b24

/*
    Get the template id and amount of the enemy to spawn for an index into this map's <enemy> entries
*/
// evt_dan_get_enemy_info(int no, int enemyIdx, &int templateId, &int num)
int evt_dan_get_enemy_info(EvtEntry * entry, bool isFirstCall); // 80c82b5c

/*
    Makes this dungeon's spawn table based on enabled map parts
*/
// evt_dan_make_spawn_table(int no)
int evt_dan_make_spawn_table(EvtEntry * entry, bool isFirstCall); // 80c82d38

/*
    Returns the spawn coordinates for an enemy which is within the group at <enemyIdx>
    num is a counter of how many enemies have spawned before this one (in any group)
*/
// evt_dan_get_enemy_spawn_pos(int num, int no, int enemyIdx, &float x, &float y, &float z)
int evt_dan_get_enemy_spawn_pos(EvtEntry * entry, bool isInitialCall); // 80c83240

/*
    Gives a key of the specified item id to a random enemy
*/
// evt_dan_decide_key_enemy(int itemId)
int evt_dan_decide_key_enemy(EvtEntry * entry, bool isFirstCall); // 80c8343c

/*
    Hud countdown callback, kills the player
*/
void danCountdownDone(); // 80c834e8

/*
    Starts the 5 minute countdown
*/
// evt_dan_start_countdown()
int evt_dan_start_countdown(EvtEntry * entry, bool isFirstCall); // 80c834f8

/*
    Returns whether a key exists on the ground within the map boundaries
*/
bool danCheckKeyInMapBbox(); // 80c83528

/*
    Returns whether an enemy holding the key exists within the map boundaries
*/
bool danCheckKeyEnemyInMapBbox(); // 80c83618

/*
    user_func that runs endlessly in a parallel script to spawn the key at the door
    if it becomes impossible to obtain
*/
// evt_dan_handle_key_failsafe()
int evt_dan_handle_key_failsafe(EvtEntry * entry, bool isFirstCall); // 80c83744

/*
    Sets the entering door name, exit pipe destination and door destination for a chest room
*/
// evt_dan_handle_chest_room_dokans_and_doors(int no)
int evt_dan_handle_chest_room_dokans_and_doors(EvtEntry * entry, bool isFirstCall); // 80c8387c

/*
    Returns the item that should be in the chest for the current chest room
*/
// evt_dan_get_chest_room_item(int no, &int itemId)
int evt_dan_get_chest_room_item(EvtEntry * entry, bool isFirstCall); // 80c83928

/*
    Sets the entering door name for a boss room
*/
// evt_dan_boss_room_set_door_name()
int evt_dan_boss_room_set_door_name(EvtEntry * entry, bool isFirstCall); // 80c83990

void func_80c839cc(const char * param_1, bool param_2); // 80c839cc
void wracktailDispCb(void * param, int animGroupIdx, int param_3); // 80c83a3c
// evt_dan_set_wracktail_disp_cb()
int evt_dan_set_wracktail_disp_cb(EvtEntry * entry, bool isFirstCall); // 80c83c04
// func_80c83c48()
int func_80c83c48(EvtEntry * entry, bool isFirstCall); // 80c83c48
void screenBlinkDisp(s8 cameraId, void * param); // 80c83c98
// evt_dan_screen_blink()
int evt_dan_screen_blink(EvtEntry * entry, bool isFirstCall); // 80c83f28
const char * func_80c83f6c(const char * param_1); // 80c83f6c

#endif
