#include <common.h>
#include <animdrv.h>
#include <bgdrv.h>
#include <dispdrv.h>
#include <evtmgr.h>
#include <evtmgr_cmd.h>
#include <evt_door.h>
#include <hitdrv.h>
#include <hud.h>
#include <itemdrv.h>
#include <lzss10.h>
#include <lz_texts.h>
#include <mapdrv.h>
#include <mario.h>
#include <mario_pouch.h>
#include <mobjdrv.h>
#include <memory.h>
#include <npcdrv.h>
#include <parse.h>
#include <seqdrv.h>
#include <seq_title.h>
#include <somewhere.h>
#include <spmario.h>
#include <stdio.h>
#include <string.h>
#include <system.h>
#include <rel/dan.h>

#define CHECK_ALL_MASK(num, mask) (((num) & (mask)) == (mask))
#define CHECK_ANY_MASK(num, mask) (((num) & (mask)) != 0)

static DanWork * wp; // 80f65888
static const char * danMapParts[DAN_PARTS_COUNT]; // 80e4fef8
static DokanDesc danDokanDescs[8]; // 80e4ff38
static MapDoorDesc danMapDoorDescs[2]; // 80e50038
static int danFlipsideLockItems[2]; // 80e50078
static int danFlopsideLockItems[2]; // 80e50080
// dan_lock_interact_evt 80e50088
// dan_lock_open_evt 80e50160
// dan_enemy_room_init_evt 80e50198
static DokanDesc danChestRoomDokanDesc; // 80e50560
static MapDoorDesc danChestRoomMapDoorDescs[2]; // 80e50580
// dan_exit_pipe_sign_interact_evt 80e505c0
// dan_chest_open_evt 80e505f8
// rotenShopItems 80e50730
// rotenShopDef 80e50848
// rotenTribeAnimDefs 80e50860
// dan_chest_room_init_evt 80e508a0
// dan_30_map_door_desc 80e510e8
// dan_30_dokan_desc 80e51108
// dan_70_dokan_desc 80e51128
// dan_30_init_evt 80e51148
// dan_70_init_evt 80e51378
// dashellTribeAnimDefs 80e515a8
// dan_30_chest_interact_evt 80e515d0
// dan_30_chest_open_evt 80e516e8
// wracktailTribeAnimDefs 80e522f0
// dan_wracktail_main_evt 80e52338
// dan_start_wracktail_evt 80e53820
// dan_70_mario_chest_open_evt 80e53900
// dan_70_dark_mario_chest_open_evt 80e53988
// dan_70_peach_chest_open_evt 80e53a10
// dan_70_dark_peach_chest_open_evt 80e53a98
// dan_70_bowser_chest_open_evt 80e53b20
// dan_70_dark_bowser_chest_open_evt 80e53ba8
// dan_70_luigi_chest_open_evt 80e53c30
// dan_70_dark_luigi_chest_open_evt 80e53cb8
// dan_shadoo_main_evt 80e53d40
// dan_shadoo_fight_evt 80e54588
// dan_shadoo_defeat_evt 80e54bc8
// dan_70_reward_appear_evt 80e54ccc
// dan_start_shadoo_evt 80e55848

int evt_dan_read_data(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;

    // Allocate work on first run
    // (check is a relD leftover, this only runs once on retail anyway)
    if (isFirstCall)
    {
        wp = __memAlloc(1, sizeof(*wp));
        memset(wp, 0, sizeof(*wp));
        wp->dungeons = __memAlloc(1, sizeof(DanDungeon[DUNGEON_MAX]));
        memset(wp->dungeons, 0, sizeof(DanDungeon[DUNGEON_MAX]));
    }
    
    // Prepare pit text to be read
    u32 size = lzss10ParseHeader(pitText).decompSize;
    void * decompPitText = __memAlloc(0, size);
    lzss10Decompress(pitText, decompPitText);
    parseInit(decompPitText, size);

    // Add all dungeon entries to work
    while (parsePush("<Dungeon>"))
    {
        // Read no (dungeon id)
        int no = 0;
        int i = 0;
        parseTagGet1("<no>", PARSE_VALUE_TYPE_INT, &no);
        assertf(144, no >= 0 && no < DUNGEON_MAX, "‚È‚ñ‚©”Ô†‚ª‚¨‚©‚µ‚¢ [%d]", no);

        // Read item id (chest contents in chest rooms, null & unused elsewhere)
        char itemName[64];
        parseTagGet1("<item>", PARSE_VALUE_TYPE_STRING, itemName);
        wp->dungeons[no].item = itemTypeNameToId(itemName);

        // Read map (bitflags for parts of the map to enable and disable in enemy rooms, 0 & unused elsewhere)
        parseTagGet1("<map>", PARSE_VALUE_TYPE_INT, &wp->dungeons[no].map);

        // Read doors
        while (parsePush("<door>"))
        {
            parseTagGet1("<enter>", PARSE_VALUE_TYPE_INT, &wp->dungeons[no].doors[i].enter);
            parseTagGet1("<exit>", PARSE_VALUE_TYPE_INT, &wp->dungeons[no].doors[i].exit);
            parsePopNext();
            i++;
        }
        wp->dungeons[no].doorCount = i;

        // Read enemies
        i = 0;
        while (parsePush("<enemy>"))
        {
            parseTagGet1("<name>", PARSE_VALUE_TYPE_INT, &wp->dungeons[no].enemies[i].name);
            parseTagGet1("<num>", PARSE_VALUE_TYPE_INT, &wp->dungeons[no].enemies[i].num);
            parseTagGet1("<pos>", PARSE_VALUE_TYPE_INT, &wp->dungeons[no].enemies[i].pos);
            parsePopNext();
            i++;
        }
        wp->dungeons[no].enemyCount = i;

        // Move to next dungeon
        parsePopNext();
    }

    // Free pit text
    parsePop();
    __memFree(0, decompPitText);

    return EVT_CONTINUE;
}

int evt_dan_handle_map_parts(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon
    int no = evtGetValue(entry, entry->pCurData[0]);
    DanDungeon * dungeon = wp->dungeons + no;

    // Turn off all parts by default
    mapGrpFlagOn(false, "parts", MAPOBJ_FLAG_HIDE);
    hitGrpFlagOn(false, "A2_parts", HITOBJ_FLAG_DISABLE);
    mapGrpFlag4On(false, "block", 0x20);

    // Enable parts from map mask
    for (u32 i = 0; i < DAN_PARTS_COUNT; i++)
    {
        if (dungeon->map & (1 << i))
        {
            char a2Part[256];
            mapGrpFlagOff(false, danMapParts[i], MAPOBJ_FLAG_HIDE);
            sprintf(a2Part, "A2_%s", danMapParts[i]);
            hitGrpFlagOff(false, a2Part, HITOBJ_FLAG_DISABLE);
        }
    }

    // Enable merged parts where possible
    if (CHECK_ALL_MASK(dungeon->map, 0xC))
    {
        mapGrpFlagOff(false, "parts_12_a", MAPOBJ_FLAG_HIDE);
        mapGrpFlagOn(false, "parts_12_b", MAPOBJ_FLAG_HIDE);
        mapGrpFlagOn(false, "parts_12_c", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2_parts_12_a", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_12_b", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_12_c", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ALL_MASK(dungeon->map, 0xC0))
    {
        mapGrpFlagOff(false, "parts_09_a", MAPOBJ_FLAG_HIDE);
        mapGrpFlagOn(false, "parts_09_b", MAPOBJ_FLAG_HIDE);
        mapGrpFlagOn(false, "parts_09_c", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2_parts_09_a", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_09_b", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_09_c", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ALL_MASK(dungeon->map, 0x300))
    {
        mapGrpFlagOff(false, "parts_11_a", MAPOBJ_FLAG_HIDE);
        mapGrpFlagOn(false, "parts_11_b", MAPOBJ_FLAG_HIDE);
        mapGrpFlagOn(false, "parts_11_c", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2_parts_11_a", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_11_b", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_11_c", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ALL_MASK(dungeon->map, 0x3000))
    {
        mapGrpFlagOff(false, "parts_10_a", MAPOBJ_FLAG_HIDE);
        mapGrpFlagOn(false, "parts_10_b", MAPOBJ_FLAG_HIDE);
        mapGrpFlagOn(false, "parts_10_c", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2_parts_10_a", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_10_b", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_10_c", HITOBJ_FLAG_DISABLE);
    }

    return EVT_CONTINUE;
}

int evt_dan_handle_dokans(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon
    int no = evtGetValue(entry, entry->pCurData[0]);
    DanDungeon * dungeon = wp->dungeons + no;

    // Turn off all pipes by default
    mapGrpFlagOn(false, "dokan", MAPOBJ_FLAG_HIDE);
    hitGrpFlagOn(false, "A2D_dokan", HITOBJ_FLAG_DISABLE);
    hitGrpFlagOn(false, "A3D_dokan", HITOBJ_FLAG_DISABLE);

    // Turn on enabled pipes
    if (CHECK_ANY_MASK(dungeon->map, 0x10000)) {
        mapGrpFlagOff(false, "dokan_01", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_01", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_01", HITOBJ_FLAG_DISABLE);
        mapGrpFlagOff(false, "dokan_02", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_02", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_02", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x20000)) {
        mapGrpFlagOff(false, "dokan_03", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_03", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_03", HITOBJ_FLAG_DISABLE);
        mapGrpFlagOff(false, "dokan_04", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_04", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_04", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x40000)) {
        mapGrpFlagOff(false, "dokan_05", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_05", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_05", HITOBJ_FLAG_DISABLE);
        mapGrpFlagOff(false, "dokan_06", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_06", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_06", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x80000)) {
        mapGrpFlagOff(false, "dokan_07", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_07", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_07", HITOBJ_FLAG_DISABLE);
        mapGrpFlagOff(false, "dokan_08", MAPOBJ_FLAG_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_08", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_08", HITOBJ_FLAG_DISABLE);
    }

    return EVT_CONTINUE;
}

int evt_dan_handle_doors(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon and room
    EvtScriptCode * args = entry->pCurData;
    int no = evtGetValue(entry, args[0]);
    int room = evtGetValue(entry, args[1]);
    DanDungeon * dungeon = wp->dungeons + no;

    // Hide all doors by default
    mapGrpFlagOn(false, "doa", MAPOBJ_FLAG_HIDE);

    // Determine which door definition to use
    // (room is an internal name from relD debug prints)
    if (room > dungeon->doorCount)
        room = 1;
    if (room == 0)
        room = (rand() % dungeon->doorCount) + 1;
    room -= 1;

    // Store door ids
    wp->doorInfo.enter = dungeon->doors[room].enter;
    wp->doorInfo.exit = dungeon->doors[room].exit;

    char str[64];

    // Show enter door & make tangible
    sprintf(str, "doa_%02d", wp->doorInfo.enter);
    mapGrpFlagOff(false, str, MAPOBJ_FLAG_HIDE);
    sprintf(str, "A2_doa_%02d", wp->doorInfo.enter);
    hitGrpFlagOff(false, str, HITOBJ_FLAG_DISABLE);
    sprintf(str, "A3_doa_%02d", wp->doorInfo.enter);
    hitGrpFlagOff(false, str, HITOBJ_FLAG_DISABLE);

    // Show exit door & make tangible
    sprintf(str, "doa_%02d", wp->doorInfo.exit);
    mapGrpFlagOff(0, str, 1);
    sprintf(str, "A2_doa_%02d", wp->doorInfo.exit);
    hitGrpFlagOff(0, str, 1);
    sprintf(str, "A3_doa_%02d", wp->doorInfo.exit);
    hitGrpFlagOff(0, str, 1);

    // Generate names for enter DoorDesc
    sprintf(wp->enterDoorName_l, "doa%d_l", wp->doorInfo.enter);
    sprintf(wp->enterDoorName_r, "doa%d_r", wp->doorInfo.enter);
    sprintf(wp->enterDoorHitName2d, "A2_doa_%02d", wp->doorInfo.enter);
    sprintf(wp->enterDoorHitName3d, "A3_doa_%02d", wp->doorInfo.enter);
    sprintf(wp->prevMapName, "");
    sprintf(wp->enterDoor_desc0x18, "");

    // Generate names for exit DoorDesc
    sprintf(wp->exitDoorName_l, "doa%d_l", wp->doorInfo.exit);
    sprintf(wp->exitDoorName_r, "doa%d_r", wp->doorInfo.exit);
    sprintf(wp->exitDoorHitName2d, "A2_doa_%02d", wp->doorInfo.exit);
    sprintf(wp->exitDoorHitName3d, "A3_doa_%02d", wp->doorInfo.exit);
    sprintf(wp->nextMapName, getNextDanMapname(no + 1));
    sprintf(wp->exitDoor_desc0x18, "");

    // Fill in enter DoorDesc
    danMapDoorDescs[0].name_l = wp->enterDoorName_l;
    danMapDoorDescs[0].name_r = wp->enterDoorName_r;
    danMapDoorDescs[0].hitName2d = wp->enterDoorHitName2d;
    danMapDoorDescs[0].hitName3d = wp->enterDoorHitName3d;
    danMapDoorDescs[0].destMapName = wp->prevMapName;
    danMapDoorDescs[0].unknown_0x18 = wp->enterDoor_desc0x18;

    // Fill in exit DoorDesc
    danMapDoorDescs[1].name_l = wp->exitDoorName_l;
    danMapDoorDescs[1].name_r = wp->exitDoorName_r;
    danMapDoorDescs[1].hitName2d = wp->exitDoorHitName2d;
    danMapDoorDescs[1].hitName3d = wp->exitDoorHitName3d;
    danMapDoorDescs[1].destMapName = wp->nextMapName;
    danMapDoorDescs[1].unknown_0x18 = wp->exitDoor_desc0x18;

    // Output door name
    sprintf(wp->enterDoorName, "doa_%02d", wp->doorInfo.enter);
    sprintf(wp->exitDoorName, "doa_%02d", wp->doorInfo.exit);
    strcpy(gp->doorName, wp->enterDoorName_l);
    evtSetValue(entry, args[2], (s32) wp->enterDoorName_l);
    evtSetValue(entry, args[3], (s32) wp->exitDoorName_l);

    // Unknown
    evtSetValue(entry, GSWF(23), 0);

    // Output lock position
    Vec3 doorPos;
    hitObjGetPos(wp->exitDoorHitName2d, &doorPos);
    evtSetFloat(entry, args[4], doorPos.x);
    evtSetFloat(entry, args[5], doorPos.y - 40.0f);
    evtSetFloat(entry, args[6], doorPos.z);

    return EVT_CONTINUE;
}

int evt_dan_get_door_names(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    EvtScriptCode * args = entry->pCurData;
    evtSetValue(entry, args[0], (s32) wp->enterDoorName);
    evtSetValue(entry, args[1], (s32) wp->exitDoorName);

    return EVT_CONTINUE;
}

int evt_dan_get_exit_door_name_l(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    EvtScriptCode * args = entry->pCurData;
    evtSetValue(entry, args[0], (s32) wp->exitDoorName_l);

    return EVT_CONTINUE;
}

int evt_dan_get_enemy_info(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon and enemy index
    EvtScriptCode * args = entry->pCurData;    
    int no = evtGetValue(entry, args[0]);
    int enemyIdx = evtGetValue(entry, args[1]);
    DanDungeon * dungeon = wp->dungeons + no;

    if ((enemyIdx < 0) || (enemyIdx >= 16))
    {
        // Return 0 for invalid enemies
        evtSetValue(entry, args[2], 0);
        evtSetValue(entry, args[3], 0);

        return EVT_CONTINUE;
    }
    else
    {
        // Get enemy
        DanEnemy * enemy = dungeon->enemies + enemyIdx;

        if (enemy->num > 0)
        {
            // Find template with correct tribe id
            int tribeId = enemy->name - 1;
            int i;
            NPCEnemyTemplate * curTemplate = npcEnemyTemplates;
            for (i = 0; i < NPCTEMPLATE_MAX; i++, curTemplate++)
            {
                if (((curTemplate->unknown_0x8 & 1) == 0) && (curTemplate->tribeId == tribeId))
                    break;
            }
            assertf(628, i < NPCTEMPLATE_MAX, "‚Ý‚Â‚©‚è‚Ü‚¹‚ñ‚Å‚µ‚½[%d]", tribeId);

            // Return template id and num
            evtSetValue(entry, args[2], i);
            evtSetValue(entry, args[3], enemy->num);
        }
        else
        {
            // Return 0 for empty enemy slots
            evtSetValue(entry, args[2], 0);
            evtSetValue(entry, args[3], 0);
        }

        return EVT_CONTINUE;
    }
}

inline void danPushSpawnTable(int doorId)
{
    wp->spawnTable[wp->spawnTableCount++] = doorId;
}

int evt_dan_make_spawn_table(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon
    int no = evtGetValue(entry, entry->pCurData[0]);
    DanDungeon * dungeon = wp->dungeons + no;

    // Build spawn table with all available doors
    wp->spawnTableCount = 0;
    if (CHECK_ANY_MASK(dungeon->map, 0x400))
        danPushSpawnTable(1);
    danPushSpawnTable(2);
    danPushSpawnTable(3);
    if (CHECK_ANY_MASK(dungeon->map, 0x1000))
    {
        danPushSpawnTable(4);
        danPushSpawnTable(5);
    }
    danPushSpawnTable(6);
    danPushSpawnTable(7);
    if (CHECK_ANY_MASK(dungeon->map, 0x8000))
        danPushSpawnTable(8);
    if (CHECK_ANY_MASK(dungeon->map, 0x40))
    {
        danPushSpawnTable(10);
        danPushSpawnTable(11);
    }
    danPushSpawnTable(12);
    danPushSpawnTable(13);
    if (CHECK_ANY_MASK(dungeon->map, 0x100))
    {
        danPushSpawnTable(14);
        danPushSpawnTable(15);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x1))
        danPushSpawnTable(17);
    danPushSpawnTable(18);
    danPushSpawnTable(19);
    if (CHECK_ANY_MASK(dungeon->map, 0x4))
    {
        danPushSpawnTable(20);
        danPushSpawnTable(21);
    }
    danPushSpawnTable(22);
    danPushSpawnTable(23);
    if (CHECK_ANY_MASK(dungeon->map, 0x20))
        danPushSpawnTable(24);
    danPushSpawnTable(25);
    danPushSpawnTable(26);
    danPushSpawnTable(27);
    danPushSpawnTable(28);
    danPushSpawnTable(29);
    danPushSpawnTable(30);
    danPushSpawnTable(31);
    danPushSpawnTable(32);

    // Randomise spawn table
    for (int i = 0; i < 100; i++)
    {
        int idx1 = rand() % wp->spawnTableCount;
        int idx2 = rand() % wp->spawnTableCount;
        int temp = wp->spawnTable[idx1];
        wp->spawnTable[idx1] = wp->spawnTable[idx2];
        wp->spawnTable[idx2] = temp;
    }

    return EVT_CONTINUE;
}

int evt_dan_get_enemy_spawn_pos(EvtEntry * entry, bool isInitialCall)
{
    (void) isInitialCall;

    // Get enemy number, dungeon, and enemy
    EvtScriptCode * args = entry->pCurData;
    int enemyNum = evtGetValue(entry, args[0]);
    int no = evtGetValue(entry, args[1]);
    int enemyIdx = evtGetValue(entry, args[2]);
    DanDungeon * dungeon = wp->dungeons + no;
    DanEnemy * enemy = dungeon->enemies + enemyIdx;

    // Find the (enemyNum % wp->spawnTableCount)th available door
    int targetPos = enemyNum % wp->spawnTableCount;
    int j = 0;
    char doorName[64];
    if (enemy->pos != 0)
    {
        sprintf(doorName, "A2_doa_%02d", enemy->pos);
    }
    else
    {
        int i;
        for (i = 0; i < wp->spawnTableCount; i++)
        {
            if ((wp->doorInfo.enter != wp->spawnTable[i]) && (wp->doorInfo.exit != wp->spawnTable[i]))
            {
                if (j == targetPos)
                    break;
                j++;
            }
        }
        sprintf(doorName, "A2_doa_%02d", wp->spawnTable[i]);
    }

    // Get the position of the door to spawn the enemy at
    Vec3 doorPos;
    hitObjGetPos(doorName, &doorPos);
    doorPos.z = 0.0f;

    // Unknown, outputs coords with some changes if needed
    float f1, f2, f3, f4, f5, f6, f7;
    f4 = 1000.0f;
    if (hitCheckFilter(doorPos.x, doorPos.y, 0.0f, 0.0f, -1.0f, 0.0f, NULL, &f1, &f2, &f3, &f4, &f5, &f6, &f7))
    {
        evtSetFloat(entry, args[3], f1);
        evtSetFloat(entry, args[4], f2);
        evtSetFloat(entry, args[5], f3);
    }
    else
    {
        evtSetFloat(entry, args[3], doorPos.x);
        evtSetFloat(entry, args[4], doorPos.y);
        evtSetFloat(entry, args[5], doorPos.z);
    }

    return EVT_CONTINUE;
}

int evt_dan_decide_key_enemy(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get the item id of the key
    int itemId = evtGetValue(entry, entry->pCurData[0]);

    // Make a list of all available enemies
    NPCWork * npcWp = npcGetWorkPtr();
    NPCEntry * curNpc = npcWp->entries;
    int enemyCount = 0;
    NPCEntry * enemies[80];
    for (int i = 0; i < npcWp->num; curNpc++, i++)
    {
        if (CHECK_ANY_MASK(curNpc->flags_8, 0x1) && !CHECK_ANY_MASK(curNpc->flags_8, 0x40000))
            enemies[enemyCount++] = curNpc;
    }

    // Allocate key
    enemies[rand() % enemyCount]->dropItemId = itemId;

    return EVT_CONTINUE;
}

void danCountdownDone()
{
    // Kill the player
    seqSetSeq(SEQ_GAMEOVER, NULL, NULL);
}

int evt_dan_start_countdown(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    // Start the 5 minute timer
    hudStartCountdown(300, danCountdownDone);

    return EVT_CONTINUE;
}

bool danCheckKeyInMapBbox()
{
    // Get the item list
    ItemWork * itemWp = itemGetWork();
    ItemEntry * item = itemWp->entries;

    // Get the range of valid coordinates
    Vec3 min, max;
    hitGetMapEntryBbox(0, &min, &max);
    
    // Check whether any item is the key within valid coordinates
    int itemCount = itemWp->num;
    int i;
    for (i = 0; i < itemCount; i++, item++)
    {
        if (
            CHECK_ANY_MASK(item->flags, 0x1) &&
            ((item->type == DAN_KEY) || (item->type == URA_DAN_KEY)) &&
            (min.x <= item->position.x) && (max.x >= item->position.x) &&
            (min.y <= item->position.y) && (max.y >= item->position.y) &&
            (min.z <= item->position.z) && (max.z >= item->position.z)
        )
            break;
    }

    if (i < itemCount)
        return true;
    else
        return false;
}

bool danCheckKeyEnemyInMapBbox()
{
    // Get the enemy list
    NPCWork * npcWp = npcGetWorkPtr();
    NPCEntry * npc = npcWp->entries;

    // Get the range of valid coordinates
    Vec3 min, max;
    hitGetMapEntryBbox(0, &min, &max);

    // Maybe a typo?
    min.x -= 5.0f;
    min.y -= 5.0f;
    min.z -= 5.0f;
    min.x += 5.0f;
    min.y += 5.0f;
    min.z += 5.0f;

    // Check whether any NPC is an enemy with the key within valid coordinates
    int npcCount = npcWp->num;
    int i;
    for (i = 0; i < npcCount; i++, npc++)
    {
        if (CHECK_ANY_MASK(npc->flags_8, 0x1))
        {
            Vec3 tempMin = {min.x, min.y, min.z};
            tempMin.y -= npc->unknown_0x3ac;
            if (
                (tempMin.x <= npc->position.x) && (max.x >= npc->position.x) &&
                (tempMin.y <= npc->position.y) && (max.y >= npc->position.y) &&
                (tempMin.z <= npc->position.z) && (max.z >= npc->position.z)
            )
                break;
        }
    }

    if (i < npcCount)
        return true;
    else
        return false;
}

int evt_dan_handle_key_failsafe(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Check whether the key exists anywhere
    if (
        !danCheckKeyEnemyInMapBbox() && !danCheckKeyInMapBbox() &&
        !pouchCheckHaveItem(DAN_KEY) && !pouchCheckHaveItem(URA_DAN_KEY) &&
        !itemCheckForId(DAN_KEY) && !itemCheckForId(URA_DAN_KEY)
    )
    {
        // Spawn the key at the lock if not
        MOBJEntry * lock = mobjNameToPtr("lock_00");
        int keyId = DAN_KEY;
        if (evtGetValue(entry, GSW(1)) >= 100)
            keyId = URA_DAN_KEY;
        
        itemEntry(lock->pos.x, lock->pos.y, 0.0f, NULL, keyId, 1, NULL, 0);
        func_800cd554(lock->pos.x, lock->pos.y, 0.0f, 0.0f, -1.0f, 0.0f, 4, 8);
        func_800b426c(lock->pos.x, lock->pos.y, 0.0f, 1, 0);

        return EVT_CONTINUE;
    }
    else
    {
        return EVT_BLOCK_WEAK;
    }
}

int evt_dan_handle_chest_room_dokans_and_doors(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon number
    int no = evtGetValue(entry, entry->pCurData[0]);
    
    // Update destination of exit door
    danChestRoomMapDoorDescs[1].destMapName = getNextDanMapname(no + 1);

    // Set the entering door name
    strcpy(gp->doorName, "doa1_l");
    
    // Set exit pipe destination
    if (no < 100)
    {
        // Flipside pit
        danChestRoomDokanDesc.destMapName = "mac_05";
        danChestRoomDokanDesc.unknown_0x1c = "dokan_1";
    }
    else
    {
        // Flopside pit
        danChestRoomDokanDesc.destMapName = "mac_15";
        danChestRoomDokanDesc.unknown_0x1c = "dokan_1";
    }

    return EVT_CONTINUE;
}

int evt_dan_get_chest_room_item(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon number
    EvtScriptCode * args = entry->pCurData;
    int no = evtGetValue(entry, args[0]);
    
    // Return the item in this room's chest
    evtSetValue(entry, args[1], wp->dungeons[no - 1].item);

    return EVT_CONTINUE;
}

int evt_dan_boss_room_set_door_name(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    // Set the entering door name
    strcpy(gp->doorName, "doa1_l");

    return EVT_CONTINUE;
}

void func_80c839cc(const char * param_1, bool param_2)
{
    if (param_2)
    {
        func_8004f5c8(param_1);
        func_8008f2a4(param_1);
        func_801c37e4(param_1);
        func_801c3694("zun", param_1);
    }
    else
    {
        func_8004f5f0(param_1);
        func_8008f324(param_1);
        func_801c3848(param_1);
    }
}

// Unfinished, just for float & string pools
void wracktailDispCb(void * param, int animGroupIdx, int param_3)
{
    NPCEntry * npc = (NPCEntry *) param;

    (void) animGroupIdx;
    (void) param_3;

    __dummy_string("antena_naka");
    __dummy_float(190.0f);
    __dummy_float(46.0f);
    __dummy_float(255.0f);
    __dummy_float(61.0f);
    __dummy_float(304.0f);
}

int evt_dan_set_wracktail_disp_cb(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    // Set wracktail's disp callback to wracktailDispCb
    NPCEntry * npc = npcNameToPtr("zun");
    animPoseSetDispCallback(npc->m_Anim.m_nPoseId, wracktailDispCb, npc);

    return EVT_CONTINUE;
}

// Unfinished, just for float pool
int func_80c83c48(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    __dummy_float(0.0f);

    return EVT_CONTINUE;
}

// Unfinished, just for float pool
void screenBlinkDisp(s8 cameraId, void * param)
{
    (void) cameraId;
    (void) param;

    __dummy_float(240.0f);
    __dummy_float(-240.0f);
}

int evt_dan_screen_blink(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    // Chedule screenBlinkDisp to run this frame
    dispEntry(11, 4, 1100.0f, screenBlinkDisp, NULL);

    return EVT_BLOCK_WEAK;
}

const char * func_80c83f6c(const char * param_1)
{
    MarioWork * mp = marioGetPtr();
    if (mp->character == PLAYER_PEACH && strcmp(param_1, "S_1") == 0)
        return "S_1B";
    else
        return param_1;
}

static DanWork * wp = NULL;

static const char * danMapParts[DAN_PARTS_COUNT] = {
    "parts_05",   // 0x1
    "parts_06",   // 0x2
    "parts_12_b", // 0x4
    "parts_12_c", // 0x8
    "parts_07",   // 0x10
    "parts_08",   // 0x20
    "parts_09_b", // 0x40
    "parts_09_c", // 0x60
    "parts_11_b", // 0x80
    "parts_11_c", // 0x100
    "parts_01",   // 0x200
    "parts_02",   // 0x400
    "parts_10_b", // 0x800
    "parts_10_c", // 0x1000
    "parts_03",   // 0x2000
    "parts_04"    // 0x4000
};

static DokanDesc danDokanDescs[8] = {
    {2, 0x100, 0, "dokan_1", NULL, "A2D_dokan_1", "A3D_dokan_1", NULL, "dokan_2"},
    {3, 0x100, 0, "dokan_2", NULL, "A2D_dokan_2", "A3D_dokan_2", NULL, "dokan_1"},
    {2, 0x100, 0, "dokan_3", NULL, "A2D_dokan_3", "A3D_dokan_3", NULL, "dokan_4"},
    {3, 0x100, 0, "dokan_4", NULL, "A2D_dokan_4", "A3D_dokan_4", NULL, "dokan_3"},

    {2, 0x100, 0, "dokan_5", NULL, "A2D_dokan_5", "A3D_dokan_5", NULL, "dokan_6"},
    {3, 0x100, 0, "dokan_6", NULL, "A2D_dokan_6", "A3D_dokan_6", NULL, "dokan_5"},
    {2, 0x100, 0, "dokan_7", NULL, "A2D_dokan_7", "A3D_dokan_7", NULL, "dokan_8"},
    {3, 0x100, 0, "dokan_8", NULL, "A2D_dokan_8", "A3D_dokan_8", NULL, "dokan_7"},
};

static MapDoorDesc danMapDoorDescs[2] = {
    // enter
    {0x102004, NULL, NULL, NULL, NULL, "", "", 16},
    // exit
    {0x102004, NULL, NULL, NULL, NULL, "", "", 16}
};

static int danFlipsideLockItems[2] = {DAN_KEY, -1};
static int danFlopsideLockItems[2] = {URA_DAN_KEY, -1};

// dan_lock_interact_evt
// dan_lock_open_evt
// dan_enemy_room_init_evt

static DokanDesc danChestRoomDokanDesc = {
    2, 0x8000, 0, "dokan_1", "dan", "A2D_dokan_1", "A3D_dokan_1", "mac_05", "dokan_1"
};

static MapDoorDesc danChestRoomMapDoorDescs[2] = {
    // enter
    {0x180004, "doa1_l", "doa1_r", "A2D_doa_01", "A3D_doa_01", "dan_01", NULL, 16},
    // exit
    {0x4, "doa2_l", "doa2_r", "A2D_doa_02", "A3D_doa_02", "dan_01", NULL, 16}
};

// dan_exit_pipe_sign_interact_evt
// dan_chest_open_evt
// rotenShopItems
// rotenShopDef
// rotenTribeAnimDefs
// dan_chest_room_init_evt
// dan_30_map_door_desc
// dan_30_dokan_desc
// dan_70_dokan_desc
// dan_30_init_evt
// dan_70_init_evt
// dashellTribeAnimDefs
// dan_30_chest_interact_evt
// dan_30_chest_open_evt
// wracktailTribeAnimDefs
// dan_wracktail_main_evt
// dan_start_wracktail_evt
// dan_70_mario_chest_open_evt
// dan_70_dark_mario_chest_open_evt
// dan_70_peach_chest_open_evt
// dan_70_dark_peach_chest_open_evt
// dan_70_bowser_chest_open_evt
// dan_70_dark_bowser_chest_open_evt
// dan_70_luigi_chest_open_evt
// dan_70_dark_luigi_chest_open_evt
// dan_shadoo_main_evt
// dan_shadoo_fight_evt
// dan_shadoo_defeat_evt
// dan_70_reward_appear_evt
// dan_start_shadoo_evt
