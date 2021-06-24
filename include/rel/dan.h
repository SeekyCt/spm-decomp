#ifndef DAN_H
#define DAN_H

#define DUNGEON_MAX 200
#define DAN_PARTS_COUNT 16

typedef struct {
    s32 enter;
    s32 exit;
} DanDoor; // total size 0x8

typedef struct {
    s32 name;
    s32 num;
    s32 pos;
} DanEnemy; // total size 0xc;

typedef struct {
    u32 map; // flags for things to enable
    s32 item; // only used in chest rooms
    s32 doorCount;
    s32 enemyCount;
    DanDoor doors[20];
    DanEnemy enemies[16];
} DanDungeon; // total size 0x170

typedef struct {
    DanDungeon * dungeons; // DUNGEON_MAX
    DanDoor doorInfo;
    int unknown_0xc;
    int unknown_0x10[1];
    u8 unknown_0x14[0x110 - 0x14];
    char enterDoorName_l[64];
    char enterDoorName_r[64];
    char enterDoorName_A2[64];
    char enterDoorName_A3[64];
    char prevMapName[64];
    char enterDoor_desc0x18[64];
    char exitDoorName_l[64];
    char exitDoorName_r[64];
    char exitDoorName_A2[64];
    char exitDoorName_A3[64];
    char nextMapName[64];
    char exitDoor_desc0x18[64];
    u8 unknown_0x410[0x510 - 0x410];
    char enterDoorName[64];
    char exitDoorName[64];
} DanWork; // total size 0x590

// evt_dan_read_data()
int evt_dan_read_data(EvtEntry * entry, bool isFirstCall); // 80c81eb0

// evt_dan_handle_map_parts(int no)
int evt_dan_handle_map_parts(EvtEntry * entry, bool isFirstCall); // 80c82188

// evt_dan_handle_dokans(int no)
int evt_dan_handle_dokans(EvtEntry * entry, bool isFirstCall); // 80c82448

// evt_dan_handle_doors(int no, int room, &char * enterDoor, &char * exitDoor, &float lockX, &float lockY, &float lockZ)
int evt_dan_handle_doors(EvtEntry * entry, bool isFirstCall); // 80c8269c

// evt_dan_get_door_names(&char * enterName, &char * exitName)
int evt_dan_get_door_names(EvtEntry * entry, bool isFirstCall); // 80c82abc

// evt_dan_get_exit_door_name_l(&char * name)
int evt_dan_get_exit_door_name_l(EvtEntry * entry, bool isFirstCall); // 80c82b24

// evt_dan_get_enemy_info(int no, int enemyIdx, &int templateId, &int num)
int evt_dan_get_enemy_info(EvtEntry * entry, bool isFirstCall); // 80c82b5c

// func_80c82d38(int no)
int func_80c82d38(EvtEntry * entry, bool isFirstCall); // 80c82d38

// evt_dan_get_enemy_spawn_pos(int num, int no, int enemyIdx, &float x, &float y, &float z)
int evt_dan_get_enemy_spawn_pos(EvtEntry * entry, bool isInitialCall); // 80c83240

// evt_dan_decide_key_enemy(int itemId)
int evt_dan_decide_key_enemy(EvtEntry * entry, bool isFirstCall); // 80c8343c

void danCountdownDone(); // 80c834e8

// evt_dan_start_countdown()
int evt_dan_start_countdown(EvtEntry * entry, bool isFirstCall); // 80c834f8

bool danCheckKeyInMapBbox(); // 80c83528

bool danCheckKeyEnemyInMapBbox(); // 80c83618

// evt_dan_handle_key_failsafe()
int evt_dan_handle_key_failsafe(EvtEntry * entry, bool isFirstCall); // 80c83744

// evt_dan_handle_chest_room_dokans_and_doors(int no)
int evt_dan_handle_chest_room_dokans_and_doors(EvtEntry * entry, bool isFirstCall); // 80c8387c

// evt_dan_get_chest_room_item(&int itemId)
int evt_dan_get_chest_room_item(EvtEntry * entry, bool isFirstCall); // 80c83928

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
