#include <common.h>
#include <evt_cmd.h>
#include <spm/animdrv.h>
#include <spm/bgdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/eff_small_star.h>
#include <spm/eff_spm_confetti.h>
#include <spm/eff_zunbaba.h>
#include <spm/evt_cam.h>
#include <spm/evt_eff.h>
#include <spm/evt_fairy.h>
#include <spm/evt_frame.h>
#include <spm/evt_guide.h>
#include <spm/evt_hit.h>
#include <spm/evt_img.h>
#include <spm/evt_item.h>
#include <spm/evt_map.h>
#include <spm/evt_mario.h>
#include <spm/evt_msg.h>
#include <spm/evt_mobj.h>
#include <spm/evt_npc.h>
#include <spm/evt_offscreen.h>
#include <spm/evt_paper.h>
#include <spm/evt_pouch.h>
#include <spm/evt_shop.h>
#include <spm/evt_snd.h>
#include <spm/evt_sub.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_door.h>
#include <spm/hitdrv.h>
#include <spm/hud.h>
#include <spm/itemdrv.h>
#include <spm/item_data.h>
#include <spm/lz_embedded.h>
#include <spm/mapdrv.h>
#include <spm/mario.h>
#include <spm/mario_pouch.h>
#include <spm/mobjdrv.h>
#include <spm/memory.h>
#include <spm/npcdrv.h>
#include <spm/parse.h>
#include <spm/seqdrv.h>
#include <spm/seq_title.h>
#include <spm/spmario.h>
#include <spm/system.h>
#include <spm/rel/dan.h>
#include <spm/rel/machi.h>
#include <wii/cx.h>
#include <wii/gx.h>
#include <msl/stdio.h>
#include <msl/string.h>

#define CHECK_ALL_MASK(num, mask) (((num) & (mask)) == (mask))
#define CHECK_ANY_MASK(num, mask) (((num) & (mask)) != 0)

// TODO: static scripts
extern DanWork * wp;
extern const char * mapParts[DAN_PARTS_COUNT];
extern DokanDesc dokanDescs[8];
extern MapDoorDesc mapDoorDescs[2];
extern s32 flipsideLockItems[2];
extern s32 flopsideLockItems[2];
EVT_DECLARE(dan_lock_interact_evt)
EVT_DECLARE(dan_lock_open_evt)
EVT_DECLARE(dan_enemy_room_init_evt)
extern DokanDesc chestRoomDokanDesc;
extern MapDoorDesc chestRoomMapDoorDescs[2];
EVT_DECLARE(dan_exit_pipe_sign_interact_evt)
EVT_DECLARE(dan_chest_open_evt)
extern s32 rotenShopItems[23 * 3 + 1];
extern EvtShopDef rotenShopDef;
extern NPCTribeAnimDef rotenTribeAnimDefs[8];
EVT_DECLARE(dan_chest_room_init_evt)
extern MapDoorDesc wracktail_map_door_desc;
extern DokanDesc wracktail_dokan_desc;
extern DokanDesc shadoo_dokan_desc;
EVT_DECLARE(dan_30_init_evt)
EVT_DECLARE(dan_70_init_evt)
extern NPCTribeAnimDef dashellTribeAnimDefs[5];
EVT_DECLARE(dan_30_chest_interact_evt)
EVT_DECLARE(dan_30_chest_open_evt)
extern NPCTribeAnimDef wracktailTribeAnimDefs[9];
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
extern const GXColor shadooBlinkColour;
extern const f64 lbl_80cf0018;

s32 evt_dan_read_data(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;

    // Allocate work on first run
    // (check is a relD leftover, this only runs once on retail anyway)
    if (isFirstCall)
    {
        wp = (DanWork *) __memAlloc(1, sizeof(*wp));
        memset(wp, 0, sizeof(*wp));
        wp->dungeons = (DanDungeon *) __memAlloc(1, sizeof(DanDungeon[DUNGEON_MAX]));
        memset(wp->dungeons, 0, sizeof(DanDungeon[DUNGEON_MAX]));
    }
    
    // Prepare pit text to be read
    u32 size = CXGetCompressionHeader(pitText).decompSize;
    char * decompPitText = (char *) __memAlloc(0, size);
    CXUncompressLZ(pitText, decompPitText);
    parseInit(decompPitText, size);

    // Add all dungeon entries to work
    while (parsePush("<Dungeon>"))
    {
        // Read no (dungeon id)
        s32 no = 0;
        s32 i = 0;
        parseTagGet1("<no>", PARSE_VALUE_TYPE_INT, &no);
        assertf(144, no >= 0 && no < DUNGEON_MAX, "なんか番号がおかしい [%d]", no);

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

    return EVT_RET_CONTINUE;
}

s32 evt_dan_handle_map_parts(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon
    s32 no = evtGetValue(entry, entry->pCurData[0]);
    DanDungeon * dungeon = wp->dungeons + no;

    // Turn off all parts by default
    mapGrpFlagOn(false, "parts", MAPOBJ_FLAG0_HIDE);
    hitGrpFlagOn(false, "A2_parts", HITOBJ_FLAG_DISABLE);
    mapGrpFlag4On(false, "block", 0x20);

    // Enable parts from map mask
    for (u32 i = 0; i < DAN_PARTS_COUNT; i++)
    {
        if (dungeon->map & (1 << i))
        {
            char a2Part[256];
            mapGrpFlagOff(false, mapParts[i], MAPOBJ_FLAG0_HIDE);
            sprintf(a2Part, "A2_%s", mapParts[i]);
            hitGrpFlagOff(false, a2Part, HITOBJ_FLAG_DISABLE);
        }
    }

    // Enable merged parts where possible
    if (CHECK_ALL_MASK(dungeon->map, 0xC))
    {
        mapGrpFlagOff(false, "parts_12_a", MAPOBJ_FLAG0_HIDE);
        mapGrpFlagOn(false, "parts_12_b", MAPOBJ_FLAG0_HIDE);
        mapGrpFlagOn(false, "parts_12_c", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2_parts_12_a", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_12_b", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_12_c", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ALL_MASK(dungeon->map, 0xC0))
    {
        mapGrpFlagOff(false, "parts_09_a", MAPOBJ_FLAG0_HIDE);
        mapGrpFlagOn(false, "parts_09_b", MAPOBJ_FLAG0_HIDE);
        mapGrpFlagOn(false, "parts_09_c", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2_parts_09_a", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_09_b", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_09_c", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ALL_MASK(dungeon->map, 0x300))
    {
        mapGrpFlagOff(false, "parts_11_a", MAPOBJ_FLAG0_HIDE);
        mapGrpFlagOn(false, "parts_11_b", MAPOBJ_FLAG0_HIDE);
        mapGrpFlagOn(false, "parts_11_c", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2_parts_11_a", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_11_b", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_11_c", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ALL_MASK(dungeon->map, 0x3000))
    {
        mapGrpFlagOff(false, "parts_10_a", MAPOBJ_FLAG0_HIDE);
        mapGrpFlagOn(false, "parts_10_b", MAPOBJ_FLAG0_HIDE);
        mapGrpFlagOn(false, "parts_10_c", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2_parts_10_a", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_10_b", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOn(false, "A2_parts_10_c", HITOBJ_FLAG_DISABLE);
    }

    return EVT_RET_CONTINUE;
}

s32 evt_dan_handle_dokans(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon
    s32 no = evtGetValue(entry, entry->pCurData[0]);
    DanDungeon * dungeon = wp->dungeons + no;

    // Turn off all pipes by default
    mapGrpFlagOn(false, "dokan", MAPOBJ_FLAG0_HIDE);
    hitGrpFlagOn(false, "A2D_dokan", HITOBJ_FLAG_DISABLE);
    hitGrpFlagOn(false, "A3D_dokan", HITOBJ_FLAG_DISABLE);

    // Turn on enabled pipes
    if (CHECK_ANY_MASK(dungeon->map, 0x10000)) {
        mapGrpFlagOff(false, "dokan_01", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_01", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_01", HITOBJ_FLAG_DISABLE);
        mapGrpFlagOff(false, "dokan_02", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_02", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_02", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x20000)) {
        mapGrpFlagOff(false, "dokan_03", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_03", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_03", HITOBJ_FLAG_DISABLE);
        mapGrpFlagOff(false, "dokan_04", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_04", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_04", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x40000)) {
        mapGrpFlagOff(false, "dokan_05", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_05", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_05", HITOBJ_FLAG_DISABLE);
        mapGrpFlagOff(false, "dokan_06", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_06", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_06", HITOBJ_FLAG_DISABLE);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x80000)) {
        mapGrpFlagOff(false, "dokan_07", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_07", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_07", HITOBJ_FLAG_DISABLE);
        mapGrpFlagOff(false, "dokan_08", MAPOBJ_FLAG0_HIDE);
        hitGrpFlagOff(false, "A2D_dokan_08", HITOBJ_FLAG_DISABLE);
        hitGrpFlagOff(false, "A3D_dokan_08", HITOBJ_FLAG_DISABLE);
    }

    return EVT_RET_CONTINUE;
}

s32 evt_dan_handle_doors(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon and room
    EvtScriptCode * args = entry->pCurData;
    s32 no = evtGetValue(entry, args[0]);
    s32 room = evtGetValue(entry, args[1]);
    DanDungeon * dungeon = wp->dungeons + no;

    // Hide all doors by default
    mapGrpFlagOn(false, "doa", MAPOBJ_FLAG0_HIDE);

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
    mapGrpFlagOff(false, str, MAPOBJ_FLAG0_HIDE);
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
    sprintf(wp->nextMapName, getDanMapName(no + 1));
    sprintf(wp->exitDoor_desc0x18, "");

    // Fill in enter DoorDesc
    mapDoorDescs[0].name_l = wp->enterDoorName_l;
    mapDoorDescs[0].name_r = wp->enterDoorName_r;
    mapDoorDescs[0].hitName2d = wp->enterDoorHitName2d;
    mapDoorDescs[0].hitName3d = wp->enterDoorHitName3d;
    mapDoorDescs[0].destMapName = wp->prevMapName;
    mapDoorDescs[0].unknown_0x18 = wp->enterDoor_desc0x18;

    // Fill in exit DoorDesc
    mapDoorDescs[1].name_l = wp->exitDoorName_l;
    mapDoorDescs[1].name_r = wp->exitDoorName_r;
    mapDoorDescs[1].hitName2d = wp->exitDoorHitName2d;
    mapDoorDescs[1].hitName3d = wp->exitDoorHitName3d;
    mapDoorDescs[1].destMapName = wp->nextMapName;
    mapDoorDescs[1].unknown_0x18 = wp->exitDoor_desc0x18;

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

    return EVT_RET_CONTINUE;
}

s32 evt_dan_get_door_names(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    EvtScriptCode * args = entry->pCurData;
    evtSetValue(entry, args[0], (s32) wp->enterDoorName);
    evtSetValue(entry, args[1], (s32) wp->exitDoorName);

    return EVT_RET_CONTINUE;
}

s32 evt_dan_get_exit_door_name_l(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    EvtScriptCode * args = entry->pCurData;
    evtSetValue(entry, args[0], (s32) wp->exitDoorName_l);

    return EVT_RET_CONTINUE;
}

s32 evt_dan_get_enemy_info(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon and enemy index
    EvtScriptCode * args = entry->pCurData;    
    s32 no = evtGetValue(entry, args[0]);
    s32 enemyIdx = evtGetValue(entry, args[1]);
    DanDungeon * dungeon = wp->dungeons + no;

    if ((enemyIdx < 0) || (enemyIdx >= 16))
    {
        // Return 0 for invalid enemies
        evtSetValue(entry, args[2], 0);
        evtSetValue(entry, args[3], 0);

        return EVT_RET_CONTINUE;
    }
    else
    {
        // Get enemy
        DanEnemy * enemy = dungeon->enemies + enemyIdx;

        if (enemy->num > 0)
        {
            // Find template with correct tribe id
            s32 tribeId = enemy->name - 1;
            s32 i;
            NPCEnemyTemplate * curTemplate = npcEnemyTemplates;
            for (i = 0; i < NPCTEMPLATE_MAX; i++, curTemplate++)
            {
                if (((curTemplate->unknown_0x8 & 1) == 0) && (curTemplate->tribeId == tribeId))
                    break;
            }
            assertf(628, i < NPCTEMPLATE_MAX, "みつかりませんでした[%d]", tribeId);

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

        return EVT_RET_CONTINUE;
    }
}

inline void danPushSpawnTable(s32 doorId)
{
    wp->spawnTable[wp->spawnTableCount++] = doorId;
}

s32 evt_dan_make_spawn_table(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon
    s32 no = evtGetValue(entry, entry->pCurData[0]);
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
    for (s32 i = 0; i < 100; i++)
    {
        s32 idx1 = rand() % wp->spawnTableCount;
        s32 idx2 = rand() % wp->spawnTableCount;
        s32 temp = wp->spawnTable[idx1];
        wp->spawnTable[idx1] = wp->spawnTable[idx2];
        wp->spawnTable[idx2] = temp;
    }

    return EVT_RET_CONTINUE;
}

s32 evt_dan_get_enemy_spawn_pos(EvtEntry * entry, bool isInitialCall)
{
    (void) isInitialCall;

    // Get enemy number, dungeon, and enemy
    EvtScriptCode * args = entry->pCurData;
    s32 enemyNum = evtGetValue(entry, args[0]);
    s32 no = evtGetValue(entry, args[1]);
    s32 enemyIdx = evtGetValue(entry, args[2]);
    DanDungeon * dungeon = wp->dungeons + no;
    DanEnemy * enemy = dungeon->enemies + enemyIdx;

    // Find the (enemyNum % wp->spawnTableCount)th available door
    s32 targetPos = enemyNum % wp->spawnTableCount;
    s32 j = 0;
    char doorName[64];
    if (enemy->pos != 0)
    {
        sprintf(doorName, "A2_doa_%02d", enemy->pos);
    }
    else
    {
        s32 i;
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
    f32 f1, f2, f3, f4, f5, f6, f7;
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

    return EVT_RET_CONTINUE;
}

s32 evt_dan_decide_key_enemy(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get the item id of the key
    s32 itemId = evtGetValue(entry, entry->pCurData[0]);

    // Make a list of all available enemies
    NPCWork * npcWp = npcGetWorkPtr();
    NPCEntry * curNpc = npcWp->entries;
    s32 enemyCount = 0;
    NPCEntry * enemies[80];
    for (s32 i = 0; i < npcWp->num; curNpc++, i++)
    {
        if (CHECK_ANY_MASK(curNpc->flag8, 0x1) && !CHECK_ANY_MASK(curNpc->flag8, 0x40000))
            enemies[enemyCount++] = curNpc;
    }

    // Allocate key
    enemies[rand() % enemyCount]->dropItemId = itemId;

    return EVT_RET_CONTINUE;
}

void danCountdownDone()
{
    // Kill the player
    seqSetSeq(SEQ_GAMEOVER, NULL, NULL);
}

s32 evt_dan_start_countdown(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    // Start the 5 minute timer
    hudStartCountdown(300, danCountdownDone);

    return EVT_RET_CONTINUE;
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
    s32 itemCount = itemWp->num;
    s32 i;
    for (i = 0; i < itemCount; i++, item++)
    {
        if (
            CHECK_ANY_MASK(item->flags, 0x1) &&
            ((item->type == ITEM_ID_KEY_DAN_KEY) || (item->type == ITEM_ID_KEY_URA_DAN_KEY)) &&
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

bool danCheckEnemyInMapBbox()
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

    // Check whether any NPC is within valid coordinates
    s32 npcCount = npcWp->num;
    s32 i;
    for (i = 0; i < npcCount; i++, npc++)
    {
        if (CHECK_ANY_MASK(npc->flag8, 0x1))
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

s32 evt_dan_handle_key_failsafe(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Check whether the key exists anywhere
    if (
        !danCheckEnemyInMapBbox() && !danCheckKeyInMapBbox() &&
        !pouchCheckHaveItem(ITEM_ID_KEY_DAN_KEY) && !pouchCheckHaveItem(ITEM_ID_KEY_URA_DAN_KEY) &&
        !itemCheckForId(ITEM_ID_KEY_DAN_KEY) && !itemCheckForId(ITEM_ID_KEY_URA_DAN_KEY)
    )
    {
        // Spawn the key at the lock if not
        MobjEntry * lock = mobjNameToPtr("lock_00");
        s32 keyId = ITEM_ID_KEY_DAN_KEY;
        if (evtGetValue(entry, GSW(1)) >= 100)
            keyId = ITEM_ID_KEY_URA_DAN_KEY;
        
        itemEntry(NULL, keyId, 1, lock->pos.x, lock->pos.y, 0.0f, NULL, 0);
        effSmallStar(lock->pos.x, lock->pos.y, 0.0f, 0.0f, -1.0f, 0.0f, 4, 8);
        effSpmConfetti(lock->pos.x, lock->pos.y, 0.0f, 1, 0);

        return EVT_RET_CONTINUE;
    }
    else
    {
        return EVT_RET_BLOCK_WEAK;
    }
}

s32 evt_dan_handle_chest_room_dokans_and_doors(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon number
    s32 no = evtGetValue(entry, entry->pCurData[0]);
    
    // Update destination of exit door
    chestRoomMapDoorDescs[1].destMapName = getDanMapName(no + 1);

    // Set the entering door name
    strcpy(gp->doorName, "doa1_l");
    
    // Set exit pipe destination
    if (no < 100)
    {
        // Flipside pit
        chestRoomDokanDesc.destMapName = "mac_05";
        chestRoomDokanDesc.unknown_0x1c = "dokan_1";
    }
    else
    {
        // Flopside pit
        chestRoomDokanDesc.destMapName = "mac_15";
        chestRoomDokanDesc.unknown_0x1c = "dokan_1";
    }

    return EVT_RET_CONTINUE;
}

s32 evt_dan_get_chest_room_item(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon number
    EvtScriptCode * args = entry->pCurData;
    int no = evtGetValue(entry, args[0]); // must be int to match
    
    // Return the item in this room's chest
    evtSetValue(entry, args[1], wp->dungeons[no - 1].item);

    return EVT_RET_CONTINUE;
}

s32 evt_dan_boss_room_set_door_name(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    // Set the entering door name
    strcpy(gp->doorName, "doa1_l");

    return EVT_RET_CONTINUE;
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

// NOT_DECOMPILED wracktailDispCb

s32 evt_dan_set_wracktail_disp_cb(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    // Set wracktail's disp callback to wracktailDispCb
    NPCEntry * npc = npcNameToPtr("zun");
    animPoseSetDispCallback(npc->m_Anim.m_nPoseId, wracktailDispCb, npc);

    return EVT_RET_CONTINUE;
}

// NOT_DECOMPILED func_80c83c48

void screenBlinkDisp(s32 cameraId, void * param)
{
    (void) cameraId;
    (void) param;
}

s32 evt_dan_screen_blink(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    // Chedule screenBlinkDisp to run this frame
    dispEntry(11, 4, 1100.0f, screenBlinkDisp, NULL);

    return EVT_RET_BLOCK_WEAK;
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

static const char * mapParts[DAN_PARTS_COUNT] = {
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

static DokanDesc dokanDescs[8] = {
    {2, 0x100, 0, "dokan_1", NULL, "A2D_dokan_1", "A3D_dokan_1", NULL, "dokan_2"},
    {3, 0x100, 0, "dokan_2", NULL, "A2D_dokan_2", "A3D_dokan_2", NULL, "dokan_1"},
    {2, 0x100, 0, "dokan_3", NULL, "A2D_dokan_3", "A3D_dokan_3", NULL, "dokan_4"},
    {3, 0x100, 0, "dokan_4", NULL, "A2D_dokan_4", "A3D_dokan_4", NULL, "dokan_3"},

    {2, 0x100, 0, "dokan_5", NULL, "A2D_dokan_5", "A3D_dokan_5", NULL, "dokan_6"},
    {3, 0x100, 0, "dokan_6", NULL, "A2D_dokan_6", "A3D_dokan_6", NULL, "dokan_5"},
    {2, 0x100, 0, "dokan_7", NULL, "A2D_dokan_7", "A3D_dokan_7", NULL, "dokan_8"},
    {3, 0x100, 0, "dokan_8", NULL, "A2D_dokan_8", "A3D_dokan_8", NULL, "dokan_7"},
};

static MapDoorDesc mapDoorDescs[2] = {
    // enter
    {0x102004, NULL, NULL, NULL, NULL, "", "", 16},
    // exit
    {0x2004, NULL, NULL, NULL, NULL, "", "", 16}
};

static s32 flipsideLockItems[2] = {ITEM_ID_KEY_DAN_KEY, -1};
static s32 flopsideLockItems[2] = {ITEM_ID_KEY_URA_DAN_KEY, -1};

EVT_BEGIN(dan_lock_interact_evt)
    USER_FUNC(evt_mario_key_off, 0)
    IF_SMALL(GSW(1), 100)
        USER_FUNC(evt_pouch_check_have_item, 48, LW(0))
        IF_NOT_EQUAL(LW(0), 0)
            USER_FUNC(func_800d7930, 0, PTR(&flipsideLockItems), LW(0), 0)
            IF_NOT_EQUAL(LW(0), 48)
                USER_FUNC(evt_mobj_exec_cancel, PTR("me"))
            END_IF()
        END_IF()
    ELSE()
        USER_FUNC(evt_pouch_check_have_item, 44, LW(0))
        IF_NOT_EQUAL(LW(0), 0)
            USER_FUNC(func_800d7930, 0, PTR(&flopsideLockItems), LW(0), 0)
            IF_NOT_EQUAL(LW(0), 44)
                USER_FUNC(evt_mobj_exec_cancel, PTR("me"))
            END_IF()
        END_IF()
    END_IF()
    USER_FUNC(evt_mario_key_on)
    RETURN()
EVT_END()

EVT_BEGIN(dan_lock_open_evt)
    USER_FUNC(evt_dan_get_exit_door_name_l, LW(0))
    USER_FUNC(evt_door_enable_disable_map_door_desc, 1, LW(0))
    SET(LW(0), LW(0))
    RUN_CHILD_EVT(PTR(lbl_80417e10))
    RETURN()
EVT_END()

EVT_BEGIN(dan_enemy_room_init_evt)
    SET(LW(0), GSW(1))
    USER_FUNC(evt_dan_read_data)
    USER_FUNC(evt_dan_handle_map_parts, LW(0))
    USER_FUNC(evt_dan_handle_dokans, LW(0))
    USER_FUNC(evt_door_set_dokan_descs, PTR(&dokanDescs), 8)
    SET(LW(1), 0)
    USER_FUNC(evt_dan_handle_doors, LW(0), LW(1), LW(10), LW(11), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
    USER_FUNC(evt_door_set_map_door_descs, PTR(&mapDoorDescs), 2)
    USER_FUNC(evt_door_enable_disable_map_door_desc, 0, LW(10))
    USER_FUNC(evt_door_enable_disable_map_door_desc, 0, LW(11))
    IF_SMALL(GSW(1), 100)
        USER_FUNC(evt_mobj_zyo, PTR("lock_00"), 48, LW(2), LW(3), LW(4), 0, PTR(&dan_lock_interact_evt), PTR(&dan_lock_open_evt), 0)
    ELSE()
        USER_FUNC(evt_mobj_zyo, PTR("lock_00"), 44, LW(2), LW(3), LW(4), 0, PTR(&dan_lock_interact_evt), PTR(&dan_lock_open_evt), 0)
    END_IF()
    USER_FUNC(evt_dan_make_spawn_table, LW(0))
    SET(LW(10), 0)
    SET(LW(9), 0)
    DO(16)
        USER_FUNC(evt_dan_get_enemy_info, LW(0), LW(10), LW(11), LW(12))
        IF_LARGE(LW(12), 0)
            DO(LW(12))
                USER_FUNC(evt_dan_get_enemy_spawn_pos, LW(9), LW(0), LW(10), LW(13), LW(14), LW(15))
                ADD(LW(9), 1)
                USER_FUNC(evt_npc_entry_from_template, 0, LW(11), LW(13), LW(14), LW(15), 0, EVT_NULLPTR)
            WHILE()
        END_IF()
        ADD(LW(10), 1)
    WHILE()
    IF_SMALL(GSW(1), 100)
        USER_FUNC(evt_dan_decide_key_enemy, 48)
    ELSE()
        USER_FUNC(evt_dan_decide_key_enemy, 44)
    END_IF()
    USER_FUNC(func_80107cfc)
    USER_FUNC(evt_hitobj_attr_onoff, 1, 1, PTR("A2"), 1073741824)
    USER_FUNC(evt_hitobj_attr_onoff, 1, 1, PTR("A3"), 536870912)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 0, PTR("S"), 2)
    USER_FUNC(evt_mapobj_flag4_onoff, 1, 1, PTR("S"), 16)
    RUN_CHILD_EVT(PTR(&door_init_evt))
    ADD(GSW(1), 1)
    USER_FUNC(evt_snd_bgmon, 0, PTR("BGM_MAP_100F"))
    USER_FUNC(evt_snd_set_sfx_reverb_mode, 0)
    INLINE_EVT()
        USER_FUNC(evt_door_wait_flag, 256)
        USER_FUNC(func_800d74a0, 1, 6)
    END_INLINE()
    USER_FUNC(evt_dan_start_countdown)
    USER_FUNC(evt_mapobj_trans, PTR("Tetugoushi"), 0, -1000, 0)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("Tetugoushi"), 1)
    INLINE_EVT()
        USER_FUNC(evt_door_wait_flag, 256)
        USER_FUNC(func_80107d20)
        USER_FUNC(evt_sub_intpl_msec_init, 11, 255, 0, 1000)
        DO(0)
            USER_FUNC(evt_sub_intpl_msec_get_value)
            USER_FUNC(evt_dan_get_door_names, LW(2), LW(3))
            USER_FUNC(evt_mapobj_color, 1, LW(2), 255, 255, 255, LW(0))
            WAIT_FRM(1)
            IF_EQUAL(LW(1), 0)
                DO_BREAK()
            END_IF()
        WHILE()
        USER_FUNC(evt_mapobj_flag_onoff, 1, 1, LW(2), 1)
    END_INLINE()
    INLINE_EVT()
        USER_FUNC(evt_dan_handle_key_failsafe)
    END_INLINE()
    USER_FUNC(func_800d4de4, 1, 0)
    RETURN()
EVT_END()

static DokanDesc chestRoomDokanDesc = {
    2, 0x8000, 0, "dokan_1", "dan", "A2D_dokan_1", "A3D_dokan_1", "mac_05", "dokan_1"
};

static MapDoorDesc chestRoomMapDoorDescs[2] = {
    // enter
    {0x180004, "doa1_l", "doa1_r", "A2_doa_01", "A3_doa_01", "dan_01", NULL, 16},
    // exit
    {0x4, "doa2_l", "doa2_r", "A2_doa_02", "A3_doa_02", "dan_01", NULL, 16}
};

EVT_BEGIN(dan_exit_pipe_sign_interact_evt)
    USER_FUNC(evt_mario_key_off, 0)
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_002"), 0, 0)
    USER_FUNC(evt_mario_key_on)
    RETURN()
EVT_END()

EVT_BEGIN(dan_chest_open_evt)
    SET(LW(0), GSW(1))
    USER_FUNC(evt_dan_get_chest_room_item, LW(0), LW(10))
    USER_FUNC(evt_mobj_wait_animation_end, PTR("box"))
    USER_FUNC(evt_mobj_get_position, PTR("box"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item_entry, PTR("item"), LW(10), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item_flag_onoff, 1, PTR("item"), 2)
    USER_FUNC(func_800ece50, PTR("item"), LW(7), LW(8), LW(9))
    USER_FUNC(evt_sub_intpl_msec_init, 11, 0, 40, 1000)
    DO(0)
        USER_FUNC(evt_sub_intpl_msec_get_value)
        ADDF(LW(0), LW(8))
        USER_FUNC(func_800ecda0, PTR("item"), LW(7), LW(0), LW(9))
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
            DO_BREAK()
        END_IF()
    WHILE()
    WAIT_MSEC(500)
    USER_FUNC(evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(evt_item_wait_collected, PTR("item"))
    RETURN()
EVT_END()

static s32 rotenShopItems[] =
{
    65, -1, 0,
    66, -1, 0,
    67, -1, 0,
    68, -1, 0,
    69, -1, 0,
    70, -1, 0,
    73, -1, 0,
    74, -1, 0,
    75, -1, 0,
    76, -1, 0,
    77, -1, 0,
    78, -1, 0,
    79, -1, 0,
    80, -1, 0,
    81, -1, 0,
    82, -1, 0,
    85, -1, 0,
    83, -1, 0,
    84, -1, 0,
    86, -1, 0,
    71, -1, 0,
    72, -1, 0,
    87, -1, 0,
    -1
};

static EvtShopDef rotenShopDef =
{
    14, "roten", rotenShopItems, NULL, NULL
};

static NPCTribeAnimDef danRotenTribeAnimDefs[] =
{
    {0, "S_1"},
    {3, "T_1"},
    {1, "W_1"},
    {0x19, "I_1A"},
    {0x1a, "I_1B"},
    {0x1b, "I_1C"},
    {0x1c, "O_1"},
    {-1, "S_1"},
};

EVT_BEGIN(dan_chest_room_init_evt)
    SET(LW(0), GSW(1))
    USER_FUNC(evt_dan_read_data)
    USER_FUNC(evt_door_set_dokan_descs, PTR(&chestRoomDokanDesc), 1)
    USER_FUNC(evt_dan_handle_chest_room_dokans_and_doors, LW(0))
    USER_FUNC(evt_door_set_map_door_descs, PTR(&chestRoomMapDoorDescs), 2)
    USER_FUNC(evt_door_enable_disable_map_door_desc, 0, PTR("doa1_l"))
    SWITCH(GSW(1))
        CASE_EQUAL(9)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(433))
        CASE_EQUAL(19)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(434))
        CASE_EQUAL(29)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(435))
        CASE_EQUAL(39)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(436))
        CASE_EQUAL(49)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(437))
        CASE_EQUAL(59)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(438))
        CASE_EQUAL(69)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(439))
        CASE_EQUAL(79)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(440))
        CASE_EQUAL(89)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(441))
        CASE_EQUAL(109)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(442))
        CASE_EQUAL(119)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(443))
        CASE_EQUAL(129)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(444))
        CASE_EQUAL(139)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(445))
        CASE_EQUAL(149)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(446))
        CASE_EQUAL(159)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(447))
        CASE_EQUAL(169)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(448))
        CASE_EQUAL(179)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(449))
        CASE_EQUAL(189)
            USER_FUNC(evt_mobj_thako, 1, PTR("box"), 75, 25, FLOAT(-87.5), 0, PTR(&dan_chest_open_evt), 0, GSWF(450))
    END_SWITCH()
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("box"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("box"), 65536)
    USER_FUNC(evt_mobj_kan, 0, PTR("_kanban"), -280, 0, -158, 0, 0)
    USER_FUNC(evt_mobj_kan, 0, PTR("kanban"), -280, 0, FLOAT(-149.8994140625), PTR(&dan_exit_pipe_sign_interact_evt), 0)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("_kanban"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("kanban"), 16448)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("_kanban"), 65536)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("kanban"), 65536)
    USER_FUNC(evt_mobj_arrow, PTR("_arrow"), -75, 0, -158, 0)
    USER_FUNC(evt_mobj_arrow, PTR("arrow"), -75, 0, -150, 0)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("_arrow"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("arrow"), 16448)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("_arrow"), 65536)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("arrow"), 65536)
    USER_FUNC(evt_hitobj_attr_onoff, 1, 1, PTR("A2"), 1073741824)
    USER_FUNC(evt_hitobj_attr_onoff, 1, 1, PTR("A3"), 536870912)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 0, PTR("S"), 2)
    USER_FUNC(evt_mapobj_flag4_onoff, 1, 1, PTR("S"), 16)
    RUN_CHILD_EVT(PTR(&door_init_evt))
    ADD(GSW(1), 1)
    USER_FUNC(evt_sub_random, 100, LW(0))
    IF_SMALL(LW(0), 30)
        USER_FUNC(evt_npc_entry, PTR("roten"), PTR("n_machi_roten"), 0)
        USER_FUNC(func_801059d0, PTR("roten"), -1)
        USER_FUNC(evt_npc_set_property, PTR("roten"), 14, PTR(&danRotenTribeAnimDefs))
        USER_FUNC(evt_npc_set_anim, PTR("roten"), 0, 1)
        USER_FUNC(evt_npc_flag8_onoff, PTR("roten"), 1, 71303172)
        USER_FUNC(evt_npc_animflag_onoff, PTR("roten"), 1, 32)
        USER_FUNC(evt_npc_add_flip_part, PTR("roten"))
        USER_FUNC(func_80104694, PTR("roten"), 1)
        USER_FUNC(evt_npc_set_position, PTR("roten"), 398, 0, 0)
        USER_FUNC(func_80108194, PTR("roten"), 0)
        USER_FUNC(evt_npc_set_property, PTR("roten"), 11, 40)
        USER_FUNC(evt_npc_set_property, PTR("roten"), 10, 60)
        USER_FUNC(evt_npc_modify_part, PTR("roten"), -1, 11, 40)
        USER_FUNC(evt_npc_modify_part, PTR("roten"), -1, 10, 60)
        USER_FUNC(evt_shop_set_defs, PTR(&rotenShopDef), 1)
    END_IF()
    USER_FUNC(evt_snd_bgmon, 0, PTR("BGM_MAP_100F"))
    USER_FUNC(evt_snd_set_sfx_reverb_mode, 0)
    INLINE_EVT()
        USER_FUNC(evt_door_wait_flag, 256)
        USER_FUNC(func_800d74a0, 1, 6)
    END_INLINE()
    USER_FUNC(evt_mapobj_trans, PTR("Tetugoushi"), 0, -1000, 0)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("Tetugoushi"), 1)
    INLINE_EVT()
        USER_FUNC(evt_door_wait_flag, 256)
        USER_FUNC(evt_sub_intpl_msec_init, 11, 255, 0, 1000)
        DO(0)
            USER_FUNC(evt_sub_intpl_msec_get_value)
            USER_FUNC(evt_mapobj_color, 1, PTR("doa_01"), 255, 255, 255, LW(0))
            WAIT_FRM(1)
            IF_EQUAL(LW(1), 0)
                DO_BREAK()
            END_IF()
        WHILE()
        USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("doa_01"), 1)
    END_INLINE()
    USER_FUNC(func_800d4de4, 1, 0)
    RETURN()
EVT_END()

static MapDoorDesc dan_30_map_door_desc =
{
    0x100004, "doa1_l", "doa1_r", "A2_doa_01", "A3_doa_01", "dan_01", NULL, 0x10
};

static DokanDesc dan_30_dokan_desc =
{
    0, 0, 0, "dokan", "dan_30", "A2D_dokan_1", "A3D_dokan_1", "mac_05", "dokan_1"
};

static DokanDesc dan_70_dokan_desc =
{
    0, 0, 0, "dokan", "dan_70", "A2D_dokan_1", "A3D_dokan_1", "mac_15", "dokan_1"
};

EVT_BEGIN(dan_30_init_evt)
    USER_FUNC(evt_door_set_map_door_descs, PTR(&dan_30_map_door_desc), 1)
    USER_FUNC(evt_door_enable_disable_map_door_desc, 0, PTR("doa1_l"))
    USER_FUNC(evt_dan_boss_room_set_door_name)
    USER_FUNC(evt_hitobj_attr_onoff, 1, 1, PTR("A2D"), 1073741824)
    USER_FUNC(evt_hitobj_attr_onoff, 1, 1, PTR("A3D"), 536870912)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 0, PTR("S"), 2)
    USER_FUNC(evt_mapobj_flag4_onoff, 1, 1, PTR("S"), 16)
    USER_FUNC(evt_door_set_dokan_descs, PTR(&dan_30_dokan_desc), 1)
    RUN_CHILD_EVT(PTR(&door_init_evt))
    USER_FUNC(evt_snd_bgmon, 0, PTR("BGM_MAP_STG1_SABAKU1"))
    USER_FUNC(evt_snd_set_sfx_reverb_mode, 0)
    ADD(GSW(1), 1)
    INLINE_EVT()
        USER_FUNC(evt_door_wait_flag, 256)
        USER_FUNC(func_800d74a0, 1, 6)
    END_INLINE()
    IF_EQUAL(GSWF(409), 0)
        USER_FUNC(evt_door_enable_disable_dokan_desc, 0, PTR("dokan"))
        USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("dokan"), 1)
        USER_FUNC(evt_hitobj_onoff, PTR("A2D_dokan"), 1, 0)
        USER_FUNC(evt_hitobj_onoff, PTR("A3D_dokan"), 1, 0)
        RUN_CHILD_EVT(PTR(&dan_start_wracktail_evt))
    END_IF()
    USER_FUNC(evt_mapobj_trans, PTR("Tetugoushi"), 0, -1000, 0)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("Tetugoushi"), 1)
    INLINE_EVT()
        USER_FUNC(evt_door_wait_flag, 256)
        USER_FUNC(evt_sub_intpl_msec_init, 11, 255, 0, 1000)
        DO(0)
            USER_FUNC(evt_sub_intpl_msec_get_value)
            USER_FUNC(evt_mapobj_color, 1, PTR("doa_01"), 255, 255, 255, LW(0))
            WAIT_FRM(1)
            IF_EQUAL(LW(1), 0)
                DO_BREAK()
            END_IF()
        WHILE()
        USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("doa_01"), 1)
    END_INLINE()
    USER_FUNC(func_800d4de4, 1, 0)
    RETURN()
EVT_END()

EVT_BEGIN(dan_70_init_evt)
    USER_FUNC(evt_door_set_map_door_descs, PTR(&dan_30_map_door_desc), 1)
    USER_FUNC(evt_door_enable_disable_map_door_desc, 0, PTR("doa1_l"))
    USER_FUNC(evt_dan_boss_room_set_door_name)
    USER_FUNC(evt_hitobj_attr_onoff, 1, 1, PTR("A2"), 1073741824)
    USER_FUNC(evt_hitobj_attr_onoff, 1, 1, PTR("A3"), 536870912)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 0, PTR("S"), 2)
    USER_FUNC(evt_mapobj_flag4_onoff, 1, 1, PTR("S"), 16)
    USER_FUNC(evt_door_set_dokan_descs, PTR(&dan_70_dokan_desc), 1)
    RUN_CHILD_EVT(PTR(&door_init_evt))
    USER_FUNC(evt_snd_bgmoff_f_d, 0, 2000)
    USER_FUNC(evt_snd_set_sfx_reverb_mode, 1)
    ADD(GSW(1), 1)
    INLINE_EVT()
        USER_FUNC(evt_door_wait_flag, 256)
        USER_FUNC(func_800d74a0, 1, 6)
    END_INLINE()
    IF_SMALL(GSW(24), 2)
        USER_FUNC(evt_door_enable_disable_dokan_desc, 0, PTR("dokan"))
        USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("dokan"), 1)
        USER_FUNC(evt_hitobj_onoff, PTR("A2D_dokan"), 1, 0)
        USER_FUNC(evt_hitobj_onoff, PTR("A3D_dokan"), 1, 0)
        RUN_EVT(PTR(&dan_start_shadoo_evt))
    END_IF()
    USER_FUNC(evt_mapobj_trans, PTR("Tetugoushi"), 0, -1000, 0)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("Tetugoushi"), 1)
    INLINE_EVT()
        USER_FUNC(evt_door_wait_flag, 256)
        USER_FUNC(evt_sub_intpl_msec_init, 11, 255, 0, 1000)
        DO(0)
            USER_FUNC(evt_sub_intpl_msec_get_value)
            USER_FUNC(evt_mapobj_color, 1, PTR("doa_01"), 255, 255, 255, LW(0))
            WAIT_FRM(1)
            IF_EQUAL(LW(1), 0)
                DO_BREAK()
            END_IF()
        WHILE()
        USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("doa_01"), 1)
    END_INLINE()
    USER_FUNC(func_800d4de4, 1, 0)
    RETURN()
EVT_END()

static NPCTribeAnimDef dashellTribeAnimDefs[5] =
{
    {0, "S_1"},
    {1, "W_1"},
    {2, "R_1"},
    {3, "T_1"},
    {-1, "Z_1"},
};

EVT_BEGIN(dan_30_chest_interact_evt)
    USER_FUNC(evt_snd_bgmoff_f_d, 0, 1000)
    USER_FUNC(evt_mario_fairy_reset)
    USER_FUNC(evt_cam_look_at_door, 1, 0)
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, 600, 60, 384, 600, 60, -16, 500, 11)
    USER_FUNC(evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_mario_get_pos, LW(3), LW(4), LW(5))
    IF_SMALL(LW(0), LW(3))
        SET(LW(1), LW(0))
        ADD(LW(1), 65)
        USER_FUNC(func_800f09c4, LW(1), LW(2), FLOAT(80.0), 0, LW(0), LW(2))
    ELSE()
        SET(LW(1), LW(0))
        ADD(LW(1), -65)
        USER_FUNC(func_800f09c4, LW(1), LW(2), FLOAT(80.0), 0, LW(0), LW(2))
    END_IF()
    WAIT_MSEC(500)
    RETURN()
EVT_END()

EVT_BEGIN(dan_30_chest_open_evt)
    USER_FUNC(evt_mobj_wait_animation_end, PTR("box"))
    USER_FUNC(evt_mario_fairy_reset)
    USER_FUNC(evt_cam_look_at_door, 1, 0)
    USER_FUNC(evt_sub_animgroup_async, PTR("FRY_dash"))
    USER_FUNC(evt_npc_entry, PTR("fairy"), PTR("FRY_dash"), 0)
    USER_FUNC(evt_npc_set_position, PTR("fairy"), 0, -1000, 0)
    USER_FUNC(evt_npc_set_property, PTR("fairy"), 14, PTR(&dashellTribeAnimDefs))
    USER_FUNC(evt_npc_set_anim, PTR("fairy"), 0, 1)
    USER_FUNC(evt_npc_flag8_onoff, PTR("fairy"), 1, 205651972)
    USER_FUNC(evt_npc_animflag_onoff, PTR("fairy"), 1, 32)
    USER_FUNC(evt_npc_add_flip_part, PTR("fairy"))
    USER_FUNC(evt_mobj_get_position, PTR("box"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_npc_set_position, PTR("fairy"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_mario_get_pos, LW(3), LW(4), LW(5))
    IF_LARGE(LW(0), LW(3))
        USER_FUNC(func_801059d0, PTR("fairy"), -1)
    ELSE()
        USER_FUNC(func_801059d0, PTR("fairy"), 1)
    END_IF()
    USER_FUNC(func_80105b94, PTR("fairy"))
    USER_FUNC(func_800d2294, 0, 1000)
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008"), 0, 0)
    USER_FUNC(evt_snd_bgmon, 1, PTR("BGM_EVT_FAIRLIN_APPEAR1"))
    WAIT_MSEC(250)
    USER_FUNC(evt_snd_sfxon_3d_player_character, PTR("SFX_P_V_MARIO_GIMON1"), PTR("SFX_P_V_PEACH_ATTACK4"), PTR("SFX_P_V_KOOPA_DAMEGE1"), PTR("SFX_P_V_LUIGI_BIKKURI1"))
    USER_FUNC(evt_mario_set_pose, PTR("E_5"), 0)
    USER_FUNC(evt_mario_wait_anim)
    USER_FUNC(evt_mario_set_pose, PTR("T_7"), 0)
    USER_FUNC(evt_mario_flag8_onoff, 1, 2)
    USER_FUNC(evt_npc_get_position, PTR("fairy"), LW(2), LW(3), LW(4))
    USER_FUNC(evt_sub_intpl_msec_init, 11, 0, 7500, 2000)
    DO(0)
        USER_FUNC(evt_sub_intpl_msec_get_value)
        DIVF(LW(0), FLOAT(100.0))
        ADDF(LW(0), LW(3))
        USER_FUNC(evt_npc_set_position, PTR("fairy"), LW(2), LW(0), LW(4))
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
            DO_BREAK()
        END_IF()
    WHILE()
    USER_FUNC(evt_mario_flag8_onoff, 0, 2)
    INLINE_EVT()
        WAIT_MSEC(500)
        USER_FUNC(evt_mobj_get_position, PTR("box"), LW(0), LW(1), LW(2))
        USER_FUNC(evt_mario_get_pos, LW(3), LW(4), LW(5))
        IF_SMALL(LW(0), LW(3))
            ADD(LW(0), 65)
        ELSE()
            ADD(LW(0), -65)
        END_IF()
        USER_FUNC(func_800f09c4, LW(0), LW(5), FLOAT(80.0), 1, PTR("fairy"))
        USER_FUNC(evt_mario_set_pose, PTR("T_11"), 0)
        USER_FUNC(evt_mario_flag8_onoff, 1, 2)
    END_INLINE()
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, 600, 95, 284, 600, 95, -16, 1000, 11)
    WAIT_MSEC(1000)
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_009"), 0, PTR("fairy"))
    USER_FUNC(evt_cam3d_evt_zoom_in, -1, 600, 70, 434, 600, 70, -16, 800, 11)
    USER_FUNC(evt_mario_flag8_onoff, 0, 2)
    USER_FUNC(evt_mario_set_pose, PTR("T_7"), 0)
    USER_FUNC(evt_mario_flag8_onoff, 1, 2)
    USER_FUNC(evt_mobj_get_position, PTR("box"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_mario_get_pos, LW(3), LW(4), LW(5))
    IF_LARGE(LW(0), LW(3))
        ADD(LW(0), 55)
    ELSE()
        SUB(LW(0), 55)
    END_IF()
    USER_FUNC(evt_npc_get_position, PTR("fairy"), LW(3), LW(4), LW(5))
    USER_FUNC(evt_npc_jump_to, PTR("fairy"), LW(0), 15, LW(5), 20, 800)
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_010"), 0, PTR("fairy"))
    WAIT_MSEC(300)
    USER_FUNC(evt_npc_get_position, PTR("fairy"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_mario_get_pos, LW(3), LW(4), LW(5))
    IF_SMALL(LW(0), LW(3))
        USER_FUNC(evt_cam3d_evt_zoom_in, 1, 545, 55, 334, 545, 55, -16, 500, 11)
    ELSE()
        USER_FUNC(evt_cam3d_evt_zoom_in, 1, 655, 55, 334, 655, 55, -16, 500, 11)
    END_IF()
    WAIT_MSEC(500)
    INLINE_EVT_ID(LW(15))
        DO(2)
            USER_FUNC(evt_npc_get_position, PTR("fairy"), LW(3), LW(4), LW(5))
            USER_FUNC(evt_npc_jump_to, PTR("fairy"), LW(3), LW(4), LW(2), 20, 300)
            WAIT_FRM(1)
        WHILE()
    END_INLINE()
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_011"), 0, PTR("fairy"))
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, 600, 70, 434, 600, 70, -16, 500, 11)
    WAIT_MSEC(500)
    DO(0)
        CHK_EVT(LW(15), LW(0))
        IF_EQUAL(LW(0), 0)
            DO_BREAK()
        END_IF()
        WAIT_FRM(1)
    WHILE()
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_012"), 0, PTR("fairy"))
    USER_FUNC(evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(evt_npc_glide_to, PTR("fairy"), LW(0), LW(1), LW(2), 800, FLOAT(0.0), FLOAT(25.0), 1, 0, 0)
    USER_FUNC(evt_npc_set_position, PTR("fairy"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(evt_mobj_get_position, PTR("box"), LW(10), LW(11), LW(12))
    IF_LARGE(LW(10), LW(0))
        USER_FUNC(evt_cam3d_evt_zoom_in, 1, 535, 85, 534, 535, 85, -16, 3000, 11)
    ELSE()
        USER_FUNC(evt_cam3d_evt_zoom_in, 1, 665, 85, 534, 665, 85, -16, 3000, 11)
    END_IF()
    USER_FUNC(evt_mario_flag8_onoff, 0, 2)
    USER_FUNC(evt_mario_set_pose, PTR("T_11"), 0)
    USER_FUNC(evt_snd_sfxon_3d_player, PTR("SFX_EVT_FAILIN_ROUND1"))
    USER_FUNC(evt_npc_get_axis_movement_unit, PTR("fairy"), LW(9))
    USER_FUNC(evt_sub_intpl_msec_init, 4, 0, 1000, 3000)
    DO(0)
        USER_FUNC(evt_sub_intpl_msec_get_value)
        DIVF(LW(0), FLOAT(1000.0))
        USER_FUNC(func_80c4d444, PTR("fairy"), LW(9), LW(0))
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
            DO_BREAK()
        END_IF()
    WHILE()
    USER_FUNC(evt_mario_set_pose, PTR("I_2"), 0)
    INLINE_EVT()
        USER_FUNC(func_800d2268, 1)
        USER_FUNC(evt_snd_bgmon, 2, PTR("BGM_FF_FAIRLIN_GET1"))
        USER_FUNC(func_800d35d8, 0, 0, 250)
        USER_FUNC(func_800d3144, 0, 250)
        USER_FUNC(evt_snd_get_bgm_wait_time, 2, LW(0))
        WAIT_MSEC(LW(0))
        USER_FUNC(evt_snd_bgmoff_f_d, 2, 1000)
        USER_FUNC(func_800d3644, 0, 1000)
        USER_FUNC(func_800d31a0, 1000)
        USER_FUNC(func_800d22d8, 1, 1000)
    END_INLINE()
    INLINE_EVT()
        USER_FUNC(func_800e8518, 0, -1, 0, 300)
    END_INLINE()
    USER_FUNC(evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(func_800efce0, LW(3))
    ADDF(LW(1), LW(3))
    ADDF(LW(1), FLOAT(15.0))
    ADDF(LW(2), FLOAT(-10.0))
    USER_FUNC(evt_eff, PTR("eff"), PTR("spm_get"), 1, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_npc_set_camid, PTR("fairy"), 9)
    USER_FUNC(func_800f240c, 9)
    WAIT_MSEC(1500)
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_013"), 0, 0)
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_014"), 0, PTR("fairy"))
    USER_FUNC(evt_pouch_add_item, 231)
    USER_FUNC(evt_pouch_set_pixl_selected, 231)
    USER_FUNC(evt_fairy_reset)
    USER_FUNC(evt_npc_get_position, PTR("fairy"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_fairy_set_pos, 0, LW(0), LW(1), LW(2))
    USER_FUNC(evt_npc_set_position, PTR("fairy"), 0, -1000, 0)
    USER_FUNC(evt_eff_softdelete, PTR("eff"))
    USER_FUNC(evt_mario_set_pose, PTR("S_1"), 0)
    WAIT_MSEC(2000)
    INLINE_EVT()
        USER_FUNC(evt_snd_bgmoff_f_d, 1, 1000)
        WAIT_MSEC(1000)
        USER_FUNC(evt_snd_bgmon_f_d, 0, PTR("BGM_MAP_STG1_SABAKU1"), 1000)
    END_INLINE()
    USER_FUNC(func_800f23e4)
    USER_FUNC(evt_mario_get_pos, LW(0), LW(1), LW(2))
    USER_FUNC(evt_mario_set_pos, LW(0), LW(1), 0)
    USER_FUNC(evt_cam_zoom_to_coords, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(evt_mario_key_off, 0)
    USER_FUNC(evt_mario_fairy_reset)
    USER_FUNC(evt_cam_look_at_door, 1, 0)
    USER_FUNC(evt_mapobj_get_position, PTR("dokan"), LW(0), LW(1), LW(2))
    USER_FUNC(func_800e01f8)
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, LW(0), EVT_NULLPTR, EVT_NULLPTR, LW(0), EVT_NULLPTR, EVT_NULLPTR, 500, 11)
    WAIT_MSEC(500)
    WAIT_MSEC(1000)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 0, PTR("dokan"), 1)
    USER_FUNC(evt_hitobj_onoff, PTR("A2D_dokan"), 1, 1)
    USER_FUNC(evt_hitobj_onoff, PTR("A3D_dokan"), 1, 1)
    USER_FUNC(evt_hit_bind_mapobj, PTR("A2D_dokan"), PTR("dokan"))
    USER_FUNC(evt_hit_bind_mapobj, PTR("A3D_dokan"), PTR("dokan"))
    USER_FUNC(evt_mapobj_get_position, PTR("dokan"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_MOBJ_DOKAN_BORN1"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_sub_intpl_msec_init, 11, -3000, 0, 1000)
    DO(0)
        USER_FUNC(evt_sub_intpl_msec_get_value)
        DIVF(LW(0), FLOAT(100.0))
        USER_FUNC(evt_mapobj_trans, PTR("dokan"), 0, LW(0), 0)
        USER_FUNC(evt_hit_bind_update, PTR("A2D_dokan"))
        USER_FUNC(evt_hit_bind_update, PTR("A3D_dokan"))
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
            DO_BREAK()
        END_IF()
    WHILE()
    WAIT_MSEC(1000)
    USER_FUNC(evt_door_enable_disable_dokan_desc, 1, PTR("dokan"))
    USER_FUNC(evt_cam_zoom_to_coords, 0, 11)
    USER_FUNC(evt_mario_key_on)
    RETURN()
EVT_END()

static NPCTribeAnimDef wracktailTribeAnimDefs[] =
{
    {0, "zun_all_S_1B"},
    {0x1a, "zun_all_S_2B"},
    {0x19, "zun_all_Z_1"},
    {0x1c, "zun_all_I_1"},
    {0x1d, "zun_all_M_1"},
    {0x1e, "zun_all_D_1"},
    {0x1f, "zun_all_H_1"},
    {3, "zun_all_T_2"},
    {-1, "zun_all_Z_1"},
};

EVT_BEGIN(dan_wracktail_main_evt)
    DO(0)
        USER_FUNC(evt_mario_get_pos, LW(0), LW(1), LW(2))
        IF_LARGE(LW(0), 0)
            DO_BREAK()
        END_IF()
        WAIT_FRM(1)
    WHILE()
    USER_FUNC(evt_mario_key_off, 0)
    USER_FUNC(evt_snd_bgmoff_f_d, 0, 1000)
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_002_001"), 0, 0)
    USER_FUNC(func_800e6250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(evt_mario_set_pose, PTR("E_5"), 0)
    USER_FUNC(evt_mario_flag8_onoff, 1, 2)
    USER_FUNC(evt_mario_wait_anim)
    USER_FUNC(evt_mario_flag8_onoff, 0, 2)
    USER_FUNC(evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(evt_mario_direction_face, 90, 200)
    USER_FUNC(evt_mario_fairy_reset)
    USER_FUNC(evt_cam_look_at_door, 1, 0)
    USER_FUNC(evt_mario_direction_face, 90, 200)
    INLINE_EVT()
        USER_FUNC(evt_snd_sfxon, PTR("SFX_EVT_ZUNBABA_QUAKE1"))
        USER_FUNC(evt_snd_get_last_sfx_id, LW(0))
        USER_FUNC(evt_cam_shake, 5, FLOAT(0.0), FLOAT(1.19921875), FLOAT(0.0), 6000, 0)
        USER_FUNC(evt_snd_sfxoff, LW(0))
    END_INLINE()
    WAIT_MSEC(500)
    USER_FUNC(evt_snd_bgmon, 0, PTR("BGM_EVT_STG1_BOSS_APPEAR1"))
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, 450, 300, 2000, 450, 300, 0, 4000, 11)
    WAIT_MSEC(4000)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 1, PTR("doa_01"), 1)
    WAIT_MSEC(1000)
    USER_FUNC(func_8010c4a4, PTR("ofs"))
    USER_FUNC(evt_npc_set_position, PTR("zun"), 900, -1350, 0)
    USER_FUNC(func_800fef30, PTR("zun"), FLOAT(5.0), FLOAT(5.0), FLOAT(5.0))
    USER_FUNC(func_800e88d0, PTR("frm_1"), PTR("OFF_rot"), PTR("Z_1"), 206, 40, 264, 328)
    USER_FUNC(func_800e8ab4, PTR("frm_1"), 255, 255, 255, 255)
    USER_FUNC(func_800e8854, 1, PTR("frm_1"))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_LINE_DRAW1"), 450, 400, 0)
    USER_FUNC(func_800e89d4, PTR("frm_1"), PTR("ofs"), PTR(func_80c839cc))
    WAIT_MSEC(500)
    USER_FUNC(func_800e88d0, PTR("frm_2"), PTR("OFF_rot"), PTR("Z_1"), 414, 148, 128, 104)
    USER_FUNC(func_800e8ab4, PTR("frm_2"), 255, 255, 255, 255)
    USER_FUNC(func_800e8854, 1, PTR("frm_2"))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_LINE_DRAW1"), 450, 400, 0)
    USER_FUNC(func_800e89d4, PTR("frm_2"), PTR("ofs"), PTR(func_80c839cc))
    WAIT_MSEC(500)
    USER_FUNC(func_800e88d0, PTR("frm_3"), PTR("OFF_rot"), PTR("Z_1"), 477, 188, 109, 111)
    USER_FUNC(func_800e8ab4, PTR("frm_3"), 255, 255, 255, 255)
    USER_FUNC(func_800e8854, 1, PTR("frm_3"))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_LINE_DRAW1"), 450, 400, 0)
    USER_FUNC(func_800e89d4, PTR("frm_3"), PTR("ofs"), PTR(func_80c839cc))
    WAIT_MSEC(500)
    USER_FUNC(func_800e88d0, PTR("frm_4"), PTR("OFF_rot"), PTR("Z_1"), 477, 264, 94, 92)
    USER_FUNC(func_800e8ab4, PTR("frm_4"), 255, 255, 255, 255)
    USER_FUNC(func_800e8854, 1, PTR("frm_4"))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_LINE_DRAW1"), 450, 400, 0)
    USER_FUNC(func_800e89d4, PTR("frm_4"), PTR("ofs"), PTR(func_80c839cc))
    WAIT_MSEC(500)
    USER_FUNC(func_800e88d0, PTR("frm_5"), PTR("OFF_rot"), PTR("Z_1"), 467, 349, 107, 100)
    USER_FUNC(func_800e8ab4, PTR("frm_5"), 255, 255, 255, 255)
    USER_FUNC(func_800e8854, 1, PTR("frm_5"))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_LINE_DRAW1"), 450, 400, 0)
    USER_FUNC(func_800e89d4, PTR("frm_5"), PTR("ofs"), PTR(func_80c839cc))
    WAIT_MSEC(500)
    USER_FUNC(func_800e8854, 0, PTR("frm_1"))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_APPEAR1"), 450, 400, 0)
    USER_FUNC(func_800e8a58, PTR("frm_1"), PTR("A_1"))
    WAIT_MSEC(300)
    USER_FUNC(func_800e8854, 0, PTR("frm_2"))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_APPEAR1"), 450, 400, 0)
    USER_FUNC(func_800e8a58, PTR("frm_2"), PTR("A_1"))
    WAIT_MSEC(300)
    USER_FUNC(func_800e8854, 0, PTR("frm_3"))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_APPEAR1"), 450, 400, 0)
    USER_FUNC(func_800e8a58, PTR("frm_3"), PTR("A_1"))
    WAIT_MSEC(300)
    USER_FUNC(func_800e8854, 0, PTR("frm_4"))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_APPEAR1"), 450, 400, 0)
    USER_FUNC(func_800e8a58, PTR("frm_4"), PTR("A_1"))
    WAIT_MSEC(300)
    USER_FUNC(func_800e8854, 0, PTR("frm_5"))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_APPEAR1"), 450, 400, 0)
    USER_FUNC(func_800e8a58, PTR("frm_5"), PTR("A_1"))
    WAIT_MSEC(300)
    USER_FUNC(func_800e8824, PTR("frm_1"))
    USER_FUNC(func_800e8824, PTR("frm_2"))
    USER_FUNC(func_800e8824, PTR("frm_3"))
    USER_FUNC(func_800e8824, PTR("frm_4"))
    USER_FUNC(func_800e8824, PTR("frm_5"))
    USER_FUNC(func_8010c4d4, PTR("ofs"))
    USER_FUNC(evt_npc_animflag_onoff, PTR("zun"), 0, 128)
    USER_FUNC(func_80c83c48)
    USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
    SUB(LW(2), 100)
    USER_FUNC(evt_npc_set_position, PTR("zun"), LW(0), LW(1), LW(2))
    INLINE_EVT()
        USER_FUNC(evt_cam_shake, 5, FLOAT(1.19921875), FLOAT(1.19921875), FLOAT(0.0), 5000, 0)
    END_INLINE()
    INLINE_EVT()
        USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
        ADD(LW(0), -325)
        DO(20)
            USER_FUNC(evt_sub_random, 200, LW(1))
            ADD(LW(1), LW(0))
            USER_FUNC(evt_eff, 0, PTR("zunbaba"), 1, LW(1), 0, 200, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
            USER_FUNC(evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, 205, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MSEC(100)
            USER_FUNC(evt_eff, 0, PTR("zunbaba"), 2, LW(1), 0, 210, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
            USER_FUNC(evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, 215, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MSEC(100)
        WHILE()
    END_INLINE()
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_VOICE1"), 450, 400, 0)
    USER_FUNC(evt_npc_set_anim, PTR("zun"), 28, 1)
    USER_FUNC(func_80103410, PTR("zun"), 1)
    USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
    ADD(LW(2), 100)
    USER_FUNC(evt_npc_set_position, PTR("zun"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_npc_set_anim, PTR("zun"), 0, 1)
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, 450, 400, 1500, 450, 400, 0, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(evt_mario_set_pos, 0, 0, 0)
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_002_002"), 0, PTR("zun"))
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, 450, 400, 1250, 450, 400, 0, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_002_003"), 0, PTR("zun"))
    USER_FUNC(evt_snd_bgmoff_f_d, 0, 2000)
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, 750, 275, 2100, 750, 275, 250, 5000, 11)
    INLINE_EVT()
        USER_FUNC(evt_snd_sfxon, PTR("SFX_EVT_QUAKE1L"))
        USER_FUNC(evt_snd_get_last_sfx_id, LW(0))
        USER_FUNC(evt_cam_shake, 5, FLOAT(1.19921875), FLOAT(1.19921875), FLOAT(0.0), 5500, 0)
        USER_FUNC(evt_snd_sfxoff, LW(0))
    END_INLINE()
    INLINE_EVT()
        USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
        ADD(LW(0), -125)
        DO(28)
            USER_FUNC(evt_sub_random, 200, LW(1))
            ADD(LW(1), LW(0))
            USER_FUNC(evt_eff, 0, PTR("zunbaba"), 1, LW(1), 0, 200, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
            USER_FUNC(evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, 205, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MSEC(50)
            USER_FUNC(evt_eff, 0, PTR("zunbaba"), 2, LW(1), 0, 210, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
            USER_FUNC(evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, 215, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MSEC(50)
        WHILE()
        WAIT_MSEC(100)
        USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
        USER_FUNC(evt_eff, 0, PTR("kemuri_test"), 24, LW(0), 0, 200, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
    END_INLINE()
    INLINE_EVT()
        USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
        ADD(LW(0), -325)
        DO(28)
            USER_FUNC(evt_sub_random, 200, LW(1))
            ADD(LW(1), LW(0))
            USER_FUNC(evt_eff, 0, PTR("zunbaba"), 1, LW(1), 0, 200, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
            USER_FUNC(evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, 205, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MSEC(50)
            USER_FUNC(evt_eff, 0, PTR("zunbaba"), 2, LW(1), 0, 210, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
            USER_FUNC(evt_eff, 0, PTR("kemuri_test"), 23, LW(1), 0, 215, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MSEC(50)
        WHILE()
    END_INLINE()
    INLINE_EVT_ID(LW(10))
        WAIT_MSEC(1000)
        DO(0)
            USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
            SUBF(LW(1), FLOAT(4.0))
            USER_FUNC(evt_npc_set_position, PTR("zun"), LW(0), LW(1), LW(2))
            WAIT_FRM(1)
        WHILE()
    END_INLINE()
    USER_FUNC(evt_snd_bgmoff_f_d, 1, 3000)
    USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
    SUB(LW(2), 100)
    USER_FUNC(evt_npc_set_position, PTR("zun"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_EVT_ZUNBABA_VOICE1"), 450, 400, 0)
    USER_FUNC(evt_npc_set_anim, PTR("zun"), 28, 1)
    USER_FUNC(func_80103410, PTR("zun"), 1)
    DELETE_EVT(LW(10))
    USER_FUNC(evt_snd_bgmon, 0, PTR("BGM_BTL_BOSS_STG1"))
    INLINE_EVT()
        SET(LW(0), 680)
        USER_FUNC(evt_eff, 0, PTR("zunbaba"), 1, LW(0), 0, 210, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
        WAIT_MSEC(500)
        SET(LW(0), 680)
        USER_FUNC(evt_eff, 0, PTR("zunbaba"), 0, LW(0), 0, 210, FLOAT(1.0), 0, 0, 0, 0, 0, 0, 0)
        USER_FUNC(evt_eff, 0, PTR("kemuri_test"), 23, LW(0), 0, 215, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
        USER_FUNC(evt_eff, 0, PTR("kemuri_test"), 24, LW(0), 0, 215, FLOAT(2.0), 0, 0, 0, 0, 0, 0, 0)
    END_INLINE()
    USER_FUNC(evt_npc_animflag_onoff, PTR("zun"), 1, 128)
    USER_FUNC(func_800fef30, PTR("zun"), FLOAT(0.0), FLOAT(0.0), FLOAT(0.0))
    USER_FUNC(evt_npc_set_position, PTR("zun"), 0, -500, 0)
    USER_FUNC(evt_snd_bgmon, 0, PTR("BGM_BTL_BOSS_STG1"))
    USER_FUNC(evt_npc_get_position, PTR("zun"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_npc_tribe_agb_async, 420)
    USER_FUNC(evt_npc_tribe_agb_async, 419)
    USER_FUNC(evt_npc_tribe_agb_async, 421)
    USER_FUNC(evt_npc_tribe_agb_async, 44)
    USER_FUNC(evt_sub_animgroup_async, PTR("e_W_zun_hed"))
    USER_FUNC(evt_sub_animgroup_async, PTR("e_W_zun_nomal"))
    USER_FUNC(evt_sub_animgroup_async, PTR("e_W_zun_arm"))
    USER_FUNC(evt_sub_animgroup_async, PTR("e_W_zun_foot"))
    USER_FUNC(evt_sub_animgroup_async, PTR("e_W_zun_hane"))
    USER_FUNC(evt_sub_animgroup_async, PTR("e_W_zun_neck"))
    USER_FUNC(evt_sub_animgroup_async, PTR("e_W_zun_togetail"))
    USER_FUNC(evt_sub_animgroup_async, PTR("e_W_zun_tail"))
    USER_FUNC(evt_npc_entry_from_template, 0, 44, LW(0), 0, 0, 0, EVT_NULLPTR)
    WAIT_MSEC(3000)
    USER_FUNC(evt_cam_zoom_to_coords, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(evt_eff_softdelete, PTR("weak_p"))
    USER_FUNC(evt_npc_delete, PTR("zun"))
    USER_FUNC(evt_mario_key_on)
    SET(GSWF(389), 0)
    DO(0)
        IF_EQUAL(GSWF(389), 1)
            DO_BREAK()
        END_IF()
        WAIT_FRM(1)
    WHILE()
    SET(GSWF(409), 1)
    USER_FUNC(evt_pouch_check_have_item, 231, LW(0))
    IF_NOT_EQUAL(LW(0), 0)
        GOTO(10)
    END_IF()
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, 600, 70, 500, 600, 70, 0, 500, 11)
    WAIT_MSEC(1000)
    INLINE_EVT()
        USER_FUNC(evt_snd_bgmoff, 0)
        USER_FUNC(evt_snd_bgmon, 2, PTR("BGM_FF_CORRECT1"))
        USER_FUNC(func_800d35d8, 0, 0, 250)
        USER_FUNC(func_800d3144, 0, 250)
        USER_FUNC(evt_snd_get_bgm_wait_time, 2, LW(0))
        WAIT_MSEC(LW(0))
        USER_FUNC(evt_snd_bgmoff_f_d, 2, 1000)
        USER_FUNC(func_800d3644, 0, 1000)
        USER_FUNC(func_800d31a0, 1000)
        USER_FUNC(evt_snd_bgmon_f_d, 0, PTR("BGM_MAP_STG1_SABAKU1"), 1000)
    END_INLINE()
    USER_FUNC(evt_paper_entry, PTR("OFF_d_seal_lower_right"))
    USER_FUNC(evt_img_entry, PTR("img"))
    USER_FUNC(evt_img_set_paper, PTR("img"), PTR("OFF_d_seal_lower_right"))
    USER_FUNC(evt_img_alloc_capture, PTR("img"), 0, 0, 1, 154, 180, 300, 300)
    USER_FUNC(evt_img_set_paper_anim, PTR("img"), PTR("Z_1"))
    USER_FUNC(evt_img_free_capture, PTR("img"), 1)
    WAIT_FRM(1)
    USER_FUNC(evt_mobj_thako, 1, PTR("box"), 600, 0, FLOAT(-195.0), PTR(&dan_30_chest_interact_evt), PTR(&dan_30_chest_open_evt), 0, 0)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("box"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("box"), 65536)
    USER_FUNC(evt_snd_sfxon, PTR("SFX_MAP_TURN_PICTURE1"))
    USER_FUNC(evt_img_set_paper_anim, PTR("img"), PTR("A_1"))
    USER_FUNC(evt_img_wait_animend, PTR("img"))
    USER_FUNC(evt_img_free_capture, PTR("img"), 0)
    USER_FUNC(evt_img_release, PTR("img"))
    USER_FUNC(evt_paper_delete, PTR("OFF_d_seal_lower_right"))
    WAIT_MSEC(500)
    USER_FUNC(evt_cam_zoom_to_coords, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(evt_mario_key_on)
    RETURN()
    LBL(10)
    USER_FUNC(func_800f1684)
    USER_FUNC(evt_mario_fairy_reset)
    USER_FUNC(evt_cam_look_at_door, 1, 0)
    USER_FUNC(evt_mapobj_get_position, PTR("dokan"), LW(0), LW(1), LW(2))
    USER_FUNC(func_800e01f8)
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, LW(0), EVT_NULLPTR, EVT_NULLPTR, LW(0), EVT_NULLPTR, EVT_NULLPTR, 500, 11)
    WAIT_MSEC(500)
    WAIT_MSEC(1000)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 0, PTR("dokan"), 1)
    USER_FUNC(evt_hitobj_onoff, PTR("A2D_dokan"), 1, 1)
    USER_FUNC(evt_hitobj_onoff, PTR("A3D_dokan"), 1, 1)
    USER_FUNC(evt_hit_bind_mapobj, PTR("A2D_dokan"), PTR("dokan"))
    USER_FUNC(evt_hit_bind_mapobj, PTR("A3D_dokan"), PTR("dokan"))
    USER_FUNC(evt_mapobj_get_position, PTR("dokan"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_MOBJ_DOKAN_BORN1"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_sub_intpl_msec_init, 11, -3000, 0, 1000)
    DO(0)
        USER_FUNC(evt_sub_intpl_msec_get_value)
        DIVF(LW(0), FLOAT(100.0))
        USER_FUNC(evt_mapobj_trans, PTR("dokan"), 0, LW(0), 0)
        USER_FUNC(evt_hit_bind_update, PTR("A2D_dokan"))
        USER_FUNC(evt_hit_bind_update, PTR("A3D_dokan"))
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
            DO_BREAK()
        END_IF()
    WHILE()
    WAIT_MSEC(1000)
    USER_FUNC(evt_door_enable_disable_dokan_desc, 1, PTR("dokan"))
    USER_FUNC(evt_cam_zoom_to_coords, 0, 11)
    USER_FUNC(evt_mario_key_on)
    RETURN()
EVT_END()

EVT_BEGIN(dan_start_wracktail_evt)
    USER_FUNC(evt_npc_entry, PTR("zun"), PTR("e_W_zun_all"), 0)
    USER_FUNC(evt_npc_set_position, PTR("zun"), 0, -1000, 0)
    USER_FUNC(evt_npc_set_property, PTR("zun"), 14, PTR(&wracktailTribeAnimDefs))
    USER_FUNC(evt_npc_set_anim, PTR("zun"), 25, 1)
    USER_FUNC(evt_npc_flag8_onoff, PTR("zun"), 1, 205520900)
    USER_FUNC(func_800fef30, PTR("zun"), FLOAT(0.0), FLOAT(0.0), FLOAT(0.0))
    USER_FUNC(evt_npc_flag8_onoff, PTR("zun"), 1, 65536)
    USER_FUNC(evt_npc_animflag_onoff, PTR("zun"), 1, 64)
    USER_FUNC(evt_npc_animflag_onoff, PTR("zun"), 1, 128)
    USER_FUNC(evt_npc_add_flip_part, PTR("zun"))
    USER_FUNC(evt_dan_set_wracktail_disp_cb)
    RUN_EVT(PTR(&dan_wracktail_main_evt))
    RETURN()
EVT_END()

// Must come after "e_W_zun_all"
static const GXColor shadooBlinkColour = {0x00, 0x00, 0x00, 0xff};
const f64 lbl_80cf0018 = 4.503601774854144E15; // TODO: this is just a literal, but wasn't placed in the right location as a dummy

EVT_BEGIN(dan_70_mario_chest_open_evt)
    USER_FUNC(evt_mobj_wait_animation_end, PTR("me"))
    USER_FUNC(evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item_entry, PTR("item"), 522, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(evt_item_wait_collected, PTR("item"))
    SET(GF(0), 1)
    RETURN()
EVT_END()

EVT_BEGIN(dan_70_dark_mario_chest_open_evt)
    USER_FUNC(evt_mobj_wait_animation_end, PTR("me"))
    USER_FUNC(evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item_entry, PTR("item"), 461, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(evt_item_wait_collected, PTR("item"))
    SET(GF(1), 1)
    RETURN()
EVT_END()

EVT_BEGIN(dan_70_peach_chest_open_evt)
    USER_FUNC(evt_mobj_wait_animation_end, PTR("me"))
    USER_FUNC(evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item_entry, PTR("item"), 523, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(evt_item_wait_collected, PTR("item"))
    SET(GF(2), 1)
    RETURN()
EVT_END()

EVT_BEGIN(dan_70_dark_peach_chest_open_evt)
    USER_FUNC(evt_mobj_wait_animation_end, PTR("me"))
    USER_FUNC(evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item_entry, PTR("item"), 462, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(evt_item_wait_collected, PTR("item"))
    SET(GF(3), 1)
    RETURN()
EVT_END()

EVT_BEGIN(dan_70_bowser_chest_open_evt)
    USER_FUNC(evt_mobj_wait_animation_end, PTR("me"))
    USER_FUNC(evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item_entry, PTR("item"), 458, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(evt_item_wait_collected, PTR("item"))
    SET(GF(4), 1)
    RETURN()
EVT_END()

EVT_BEGIN(dan_70_dark_bowser_chest_open_evt)
    USER_FUNC(evt_mobj_wait_animation_end, PTR("me"))
    USER_FUNC(evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item_entry, PTR("item"), 459, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(evt_item_wait_collected, PTR("item"))
    SET(GF(5), 1)
    RETURN()
EVT_END()

EVT_BEGIN(dan_70_luigi_chest_open_evt)
    USER_FUNC(evt_mobj_wait_animation_end, PTR("me"))
    USER_FUNC(evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item_entry, PTR("item"), 521, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(evt_item_wait_collected, PTR("item"))
    SET(GF(6), 1)
    RETURN()
EVT_END()

EVT_BEGIN(dan_70_dark_luigi_chest_open_evt)
    USER_FUNC(evt_mobj_wait_animation_end, PTR("me"))
    USER_FUNC(evt_mobj_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_item_entry, PTR("item"), 460, 0, LW(0), LW(1), LW(2), 0, 0, 0, 0)
    USER_FUNC(evt_item_flag_onoff, 1, PTR("item"), 8)
    USER_FUNC(evt_item_wait_collected, PTR("item"))
    SET(GF(7), 1)
    RETURN()
EVT_END()


EVT_BEGIN(dan_shadoo_main_evt)
    IF_LARGE_EQUAL(GSW(24), 2)
        RETURN()
    END_IF()
    DO(0)
        USER_FUNC(evt_mario_get_pos, LW(0), LW(1), LW(2))
        IF_LARGE(LW(0), -50)
            DO_BREAK()
        END_IF()
        WAIT_FRM(1)
    WHILE()
    SWITCH(GSW(24))
        CASE_EQUAL(0)
            USER_FUNC(evt_mario_key_off, 0)
            USER_FUNC(evt_mario_fairy_reset)
            USER_FUNC(evt_cam_look_at_door, 1, 0)
            USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_00"), 0, 0)
            USER_FUNC(evt_mario_get_character, LW(0))
            IF_EQUAL(LW(0), 1)
                USER_FUNC(evt_mario_set_anim_change_handler, PTR(func_80c83f6c))
                USER_FUNC(evt_mario_set_pose, PTR("S_1B"), 0)
            END_IF()
            DO(2)
                USER_FUNC(evt_mario_direction_face, 270, 200)
                WAIT_MSEC(300)
                USER_FUNC(evt_mario_direction_face, 90, 200)
                WAIT_MSEC(300)
            WHILE()
            WAIT_MSEC(300)
            USER_FUNC(evt_mario_get_character, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                    USER_FUNC(func_800eaa88, LW(0))
                    IF_EQUAL(LW(0), 1)
                        USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_01"), 0, PTR("__guide__"))
                    END_IF()
                CASE_EQUAL(1)
                    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_02"), 0, PTR("__mario__"))
                CASE_EQUAL(2)
                    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_03"), 0, PTR("__mario__"))
                CASE_EQUAL(3)
                    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_04"), 0, PTR("__mario__"))
            END_SWITCH()
            WAIT_MSEC(300)
            SETF(GW(1), FLOAT(0.0))
            INLINE_EVT_ID(LW(9))
                USER_FUNC(evt_dan_screen_blink)
            END_INLINE()
            USER_FUNC(evt_sub_intpl_msec_init, 0, 0, 2400, 150)
            DO(0)
                USER_FUNC(evt_sub_intpl_msec_get_value)
                DIVF(LW(0), FLOAT(10.0))
                SETF(GW(1), LW(0))
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
            USER_FUNC(evt_snd_sfxon, PTR("SFX_EVT_DAN_CAMERA1"))
            USER_FUNC(evt_sub_intpl_msec_init, 0, 2400, 0, 150)
            DO(0)
                USER_FUNC(evt_sub_intpl_msec_get_value)
                DIVF(LW(0), FLOAT(10.0))
                SETF(GW(1), LW(0))
                WAIT_FRM(1)
                IF_EQUAL(LW(1), 0)
                    DO_BREAK()
                END_IF()
            WHILE()
            DELETE_EVT(LW(9))
            WAIT_MSEC(500)
            INLINE_EVT()
                WAIT_MSEC(200)
                USER_FUNC(evt_mario_set_pose, PTR("T_11"), 0)
            END_INLINE()
            USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_05"), 0, 0)
            USER_FUNC(evt_cam3d_evt_zoom_in, 1, 350, 70, 434, 350, 70, -16, 500, 11)
            WAIT_MSEC(500)
            USER_FUNC(evt_paper_entry, PTR("OFF_d_seal_lower_right"))
            USER_FUNC(evt_img_entry, PTR("img"))
            USER_FUNC(evt_img_set_paper, PTR("img"), PTR("OFF_d_seal_lower_right"))
            USER_FUNC(evt_img_alloc_capture, PTR("img"), 0, 0, 1, 154, 180, 300, 300)
            USER_FUNC(evt_img_set_paper_anim, PTR("img"), PTR("Z_1"))
            USER_FUNC(evt_img_free_capture, PTR("img"), 1)
            WAIT_FRM(1)
            USER_FUNC(evt_mapobj_flag_onoff, 1, 0, PTR("dokan"), 1)
            USER_FUNC(evt_hitobj_onoff, PTR("A2D_dokan"), 1, 1)
            USER_FUNC(evt_hitobj_onoff, PTR("A3D_dokan"), 1, 1)
            USER_FUNC(evt_door_enable_disable_dokan_desc, 1, PTR("dokan"))
            USER_FUNC(evt_snd_sfxon, PTR("SFX_MAP_TURN_PICTURE1"))
            USER_FUNC(evt_img_set_paper_anim, PTR("img"), PTR("A_1"))
            USER_FUNC(evt_img_wait_animend, PTR("img"))
            USER_FUNC(evt_img_free_capture, PTR("img"), 0)
            USER_FUNC(evt_img_release, PTR("img"))
            USER_FUNC(evt_paper_delete, PTR("OFF_d_seal_lower_right"))
            WAIT_MSEC(500)
            USER_FUNC(evt_mario_set_pose, PTR("S_1"), 0)
            USER_FUNC(evt_cam_zoom_to_coords, 0, 11)
            WAIT_MSEC(300)
            USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_05_01"), 0, 0)
            SET(GSW(24), 1)
            USER_FUNC(evt_mario_set_anim_change_handler, 0)
            USER_FUNC(evt_mario_key_on)
        CASE_EQUAL(1)
            USER_FUNC(evt_mario_key_off, 0)
            USER_FUNC(evt_mario_fairy_reset)
            USER_FUNC(evt_cam_look_at_door, 1, 0)
            INLINE_EVT()
                WAIT_MSEC(200)
                USER_FUNC(evt_mario_set_pose, PTR("T_11"), 0)
            END_INLINE()
            USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_17"), 0, 0)
            USER_FUNC(evt_mario_set_pose, PTR("E_3"), 0)
            USER_FUNC(evt_snd_sfxon, PTR("SFX_EVT_QUAKE1L"))
            USER_FUNC(evt_snd_get_last_sfx_id, LW(0))
            USER_FUNC(evt_cam_shake, 5, FLOAT(1.5), FLOAT(1.5), FLOAT(0.0), 2000, 0)
            USER_FUNC(evt_snd_sfxoff, LW(0))
            USER_FUNC(evt_mario_get_character, LW(1))
            IF_EQUAL(LW(1), 1)
                USER_FUNC(evt_mario_set_pose, PTR("S_1B"), 0)
            ELSE()
                USER_FUNC(evt_mario_set_pose, PTR("S_1"), 0)
            END_IF()
            USER_FUNC(evt_cam3d_evt_zoom_in, 1, 50, 150, 334, 50, 150, -16, 500, 11)
            WAIT_MSEC(500)
            WAIT_MSEC(1000)
            USER_FUNC(evt_snd_bgmon, 0, PTR("BGM_BTL_BOSS_MIDDLE1"))
            USER_FUNC(evt_npc_tribe_agb_async, 286)
            USER_FUNC(evt_npc_entry_from_template, 0, 286, 0, -100, 0, LW(10), EVT_NULLPTR)
            USER_FUNC(evt_npc_set_anim, LW(10), 0, 1)
            USER_FUNC(func_80107c38, LW(10), 0)
            USER_FUNC(evt_npc_flag8_onoff, LW(10), 0, 8)
            USER_FUNC(evt_npc_flag8_onoff, LW(10), 1, 65536)
            USER_FUNC(evt_npc_flip_to, LW(10), 1)
            USER_FUNC(evt_npc_finish_flip_instant, LW(10))
            USER_FUNC(evt_npc_set_position, LW(10), 50, 110, 20)
            USER_FUNC(func_800ff8f8, LW(10), 50, 110, 20)
            USER_FUNC(evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
            USER_FUNC(evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
            USER_FUNC(evt_npc_flip, LW(10))
            USER_FUNC(evt_npc_wait_flip_finished, LW(10))
            WAIT_MSEC(1000)
            USER_FUNC(evt_npc_flag8_onoff, LW(10), 1, 8)
            WAIT_MSEC(500)
            USER_FUNC(evt_snd_sfxon_npc, PTR("SFX_P_MARIO_LAND1"), LW(10))
            USER_FUNC(evt_cam3d_evt_zoom_in, 1, 0, 75, 484, 0, 75, -16, 800, 11)
            WAIT_MSEC(800)
            USER_FUNC(evt_snd_sfxon_3d_player_character, PTR("SFX_P_V_MARIO_BIKKURI1"), PTR("SFX_P_V_PEACH_ATTACK4"), PTR("SFX_P_V_KOOPA_BIKKURI1"), PTR("SFX_P_V_LUIGI_BIKKURI1"))
            USER_FUNC(evt_mario_set_pose, PTR("E_5"), 0)
            USER_FUNC(evt_mario_wait_anim)
            USER_FUNC(evt_mario_get_character, LW(0))
            IF_EQUAL(LW(0), 1)
                USER_FUNC(evt_mario_set_pose, PTR("S_1B"), 0)
            ELSE()
                USER_FUNC(evt_mario_set_pose, PTR("S_1"), 0)
            END_IF()
            WAIT_MSEC(300)
            USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_18"), 0, LW(10))
            USER_FUNC(func_80102bf8, LW(10))
            USER_FUNC(evt_cam_zoom_to_coords, 500, 11)
            WAIT_MSEC(500)
            USER_FUNC(evt_mario_set_anim_change_handler, 0)
            USER_FUNC(evt_mario_key_on)
            RUN_CHILD_EVT(PTR(&dan_shadoo_fight_evt))
        CASE_ETC()
    END_SWITCH()
    RETURN()
EVT_END()

EVT_BEGIN(dan_shadoo_fight_evt)
    DO(0)
        USER_FUNC(func_801086fc, LW(0))
        IF_EQUAL(LW(0), 0)
            DO_BREAK()
        END_IF()
        USER_FUNC(evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
        WAIT_FRM(1)
    WHILE()
    USER_FUNC(evt_mario_key_off, 0)
    USER_FUNC(evt_npc_tribe_agb_async, 285)
    USER_FUNC(evt_npc_entry_from_template, 0, 285, 0, -100, 0, LW(10), EVT_NULLPTR)
    USER_FUNC(evt_npc_set_anim, LW(10), 0, 1)
    USER_FUNC(func_80107c38, LW(10), 0)
    USER_FUNC(evt_npc_flag8_onoff, LW(10), 0, 8)
    USER_FUNC(evt_npc_flag8_onoff, LW(10), 1, 65536)
    USER_FUNC(evt_npc_flip_to, LW(10), 1)
    USER_FUNC(evt_npc_finish_flip_instant, LW(10))
    IF_SMALL(LW(11), -480)
        SET(LW(11), -480)
    END_IF()
    IF_LARGE(LW(11), 480)
        SET(LW(11), 480)
    END_IF()
    IF_SMALL(LW(12), 0)
        SET(LW(12), 0)
    END_IF()
    IF_SMALL(LW(13), -140)
        SET(LW(13), -140)
    END_IF()
    IF_LARGE(LW(13), 140)
        SET(LW(13), 140)
    END_IF()
    USER_FUNC(evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
    USER_FUNC(func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
    USER_FUNC(evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
    USER_FUNC(evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
    USER_FUNC(evt_npc_flip, LW(10))
    USER_FUNC(evt_npc_wait_flip_finished, LW(10))
    USER_FUNC(evt_npc_flag8_onoff, LW(10), 1, 8)
    USER_FUNC(func_80102bf8, LW(10))
    USER_FUNC(evt_mario_key_on)
    DO(0)
        USER_FUNC(func_801086fc, LW(0))
        IF_EQUAL(LW(0), 0)
            DO_BREAK()
        END_IF()
        USER_FUNC(evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
        WAIT_FRM(1)
    WHILE()
    USER_FUNC(evt_mario_key_off, 0)
    USER_FUNC(evt_npc_tribe_agb_async, 288)
    USER_FUNC(evt_npc_entry_from_template, 0, 288, 0, -100, 0, LW(10), EVT_NULLPTR)
    USER_FUNC(evt_npc_set_anim, LW(10), 0, 1)
    USER_FUNC(func_80107c38, LW(10), 0)
    USER_FUNC(evt_npc_flag8_onoff, LW(10), 0, 8)
    USER_FUNC(evt_npc_flag8_onoff, LW(10), 1, 65536)
    USER_FUNC(evt_npc_flip_to, LW(10), 1)
    USER_FUNC(evt_npc_finish_flip_instant, LW(10))
    IF_SMALL(LW(11), -480)
        SET(LW(11), -480)
    END_IF()
    IF_LARGE(LW(11), 480)
        SET(LW(11), 480)
    END_IF()
    IF_SMALL(LW(12), 0)
        SET(LW(12), 0)
    END_IF()
    IF_SMALL(LW(13), -140)
        SET(LW(13), -140)
    END_IF()
    IF_LARGE(LW(13), 140)
        SET(LW(13), 140)
    END_IF()
    USER_FUNC(evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
    USER_FUNC(func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
    USER_FUNC(evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
    USER_FUNC(evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
    USER_FUNC(evt_npc_flip, LW(10))
    USER_FUNC(evt_npc_wait_flip_finished, LW(10))
    USER_FUNC(evt_npc_flag8_onoff, LW(10), 1, 8)
    USER_FUNC(func_80102bf8, LW(10))
    USER_FUNC(evt_mario_key_on)
    DO(0)
        USER_FUNC(func_801086fc, LW(0))
        IF_EQUAL(LW(0), 0)
            DO_BREAK()
        END_IF()
        USER_FUNC(evt_npc_get_position, LW(10), LW(11), LW(12), LW(13))
        WAIT_FRM(1)
    WHILE()
    USER_FUNC(evt_mario_key_off, 0)
    USER_FUNC(evt_npc_tribe_agb_async, 287)
    USER_FUNC(evt_npc_entry_from_template, 0, 287, 0, -100, 0, LW(10), EVT_NULLPTR)
    USER_FUNC(evt_npc_set_anim, LW(10), 0, 1)
    USER_FUNC(func_80107c38, LW(10), 0)
    USER_FUNC(evt_npc_flag8_onoff, LW(10), 0, 8)
    USER_FUNC(evt_npc_flag8_onoff, LW(10), 1, 65536)
    USER_FUNC(evt_npc_flip_to, LW(10), 1)
    USER_FUNC(evt_npc_finish_flip_instant, LW(10))
    IF_SMALL(LW(11), -480)
        SET(LW(11), -480)
    END_IF()
    IF_LARGE(LW(11), 480)
        SET(LW(11), 480)
    END_IF()
    IF_SMALL(LW(12), 0)
        SET(LW(12), 0)
    END_IF()
    IF_SMALL(LW(13), -140)
        SET(LW(13), -140)
    END_IF()
    IF_LARGE(LW(13), 140)
        SET(LW(13), 140)
    END_IF()
    USER_FUNC(evt_npc_set_position, LW(10), LW(11), LW(12), LW(13))
    USER_FUNC(func_800ff8f8, LW(10), LW(11), LW(12), LW(13))
    USER_FUNC(evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_DRAW1"), LW(10))
    USER_FUNC(evt_snd_sfxon_npc, PTR("SFX_EVT_100_PC_LINE_TURN1"), LW(10))
    USER_FUNC(evt_npc_flip, LW(10))
    USER_FUNC(evt_npc_wait_flip_finished, LW(10))
    USER_FUNC(evt_npc_flag8_onoff, LW(10), 1, 8)
    USER_FUNC(func_80102bf8, LW(10))
    USER_FUNC(evt_npc_set_unitwork, LW(10), 8, PTR(&dan_shadoo_defeat_evt))
    USER_FUNC(evt_mario_key_on)
    RETURN()
EVT_END()

EVT_BEGIN(dan_shadoo_defeat_evt)
    USER_FUNC(evt_mario_key_off, 0)
    USER_FUNC(evt_mario_fairy_reset)
    USER_FUNC(evt_cam_look_at_door, 1, 0)
    USER_FUNC(func_801049ec, PTR("me"))
    USER_FUNC(func_80108194, PTR("me"), 0)
    USER_FUNC(evt_npc_set_anim, PTR("me"), 4, 1)
    USER_FUNC(evt_npc_set_property, PTR("me"), 14, 0)
    USER_FUNC(evt_mario_face_npc, PTR("me"))
    USER_FUNC(evt_npc_get_position, PTR("me"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, LW(0), 60, 384, LW(0), 60, -16, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(evt_npc_animflag_onoff, PTR("me"), 1, 536870912)
    USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_19"), 0, PTR("me"))
    RUN_EVT(PTR(&dan_70_reward_appear_evt))
    RUN_CHILD_EVT(PTR(lbl_80439f10))
    RETURN()
EVT_END()

EVT_BEGIN(dan_70_reward_appear_evt)
    WAIT_MSEC(500)
    USER_FUNC(evt_mario_get_character, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
            USER_FUNC(func_800eaa88, LW(0))
            IF_EQUAL(LW(0), 1)
                USER_FUNC(evt_mario_get_pos, LW(0), LW(1), LW(2))
                USER_FUNC(evt_cam3d_evt_zoom_in, 1, LW(0), 60, 384, LW(0), 60, -16, 500, 11)
                WAIT_MSEC(1000)
                USER_FUNC(evt_mario_set_pose, PTR("T_11"), 0)
                USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_20"), 0, PTR("__guide__"))
                USER_FUNC(evt_mario_set_pose, PTR("S_1"), 0)
            END_IF()
        CASE_EQUAL(1)
            USER_FUNC(evt_mario_get_pos, LW(0), LW(1), LW(2))
            USER_FUNC(evt_cam3d_evt_zoom_in, 1, LW(0), 60, 384, LW(0), 60, -16, 500, 11)
            WAIT_MSEC(1000)
            USER_FUNC(evt_mario_set_pose, PTR("T_11"), 0)
            USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_21"), 0, PTR("__mario__"))
            USER_FUNC(evt_mario_set_pose, PTR("S_1"), 0)
        CASE_EQUAL(2)
            USER_FUNC(evt_mario_get_pos, LW(0), LW(1), LW(2))
            USER_FUNC(evt_cam3d_evt_zoom_in, 1, LW(0), 60, 384, LW(0), 60, -16, 500, 11)
            WAIT_MSEC(1000)
            USER_FUNC(evt_mario_set_pose, PTR("T_11"), 0)
            USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_22"), 0, PTR("__mario__"))
            USER_FUNC(evt_mario_set_pose, PTR("S_1"), 0)
        CASE_EQUAL(3)
            USER_FUNC(evt_mario_get_pos, LW(0), LW(1), LW(2))
            USER_FUNC(evt_cam3d_evt_zoom_in, 1, LW(0), 60, 384, LW(0), 60, -16, 500, 11)
            WAIT_MSEC(1000)
            USER_FUNC(evt_mario_set_pose, PTR("T_11"), 0)
            USER_FUNC(evt_msg_print, 0, PTR("mac_dungeon_008_23"), 0, PTR("__mario__"))
            USER_FUNC(evt_mario_set_pose, PTR("S_1"), 0)
    END_SWITCH()
    USER_FUNC(evt_snd_bgmoff_f_d, 0, 2000)
    USER_FUNC(evt_mario_set_pose, PTR("S_1"), 0)
    USER_FUNC(evt_mario_face_coords, 200, 0)
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, 0, 160, 1034, 0, 160, -16, 500, 11)
    WAIT_MSEC(500)
    INLINE_EVT()
        USER_FUNC(evt_snd_bgmon, 2, PTR("BGM_FF_CORRECT1"))
        USER_FUNC(evt_snd_get_bgm_wait_time, 2, LW(0))
        WAIT_MSEC(LW(0))
        USER_FUNC(evt_snd_bgmoff_f_d, 2, 1000)
        USER_FUNC(evt_snd_bgmon_f_d, 0, PTR("BGM_MAP_100F"), 1000)
    END_INLINE()
    USER_FUNC(evt_mobj_thako, 0, PTR("b1"), FLOAT(-262.5), -100, -75, 0, PTR(&dan_70_mario_chest_open_evt), 0, 0)
    USER_FUNC(evt_mobj_thako, 0, PTR("b2"), FLOAT(-187.5), -100, -75, 0, PTR(&dan_70_dark_mario_chest_open_evt), 0, 0)
    USER_FUNC(evt_mobj_thako, 0, PTR("b3"), FLOAT(-112.5), -100, -75, 0, PTR(&dan_70_peach_chest_open_evt), 0, 0)
    USER_FUNC(evt_mobj_thako, 0, PTR("b4"), FLOAT(-37.5), -100, -75, 0, PTR(&dan_70_dark_peach_chest_open_evt), 0, 0)
    USER_FUNC(evt_mobj_thako, 0, PTR("b5"), FLOAT(37.5), -100, -75, 0, PTR(&dan_70_bowser_chest_open_evt), 0, 0)
    USER_FUNC(evt_mobj_thako, 0, PTR("b6"), FLOAT(112.5), -100, -75, 0, PTR(&dan_70_dark_bowser_chest_open_evt), 0, 0)
    USER_FUNC(evt_mobj_thako, 0, PTR("b7"), FLOAT(187.5), -100, -75, 0, PTR(&dan_70_luigi_chest_open_evt), 0, 0)
    USER_FUNC(evt_mobj_thako, 0, PTR("b8"), FLOAT(262.5), -100, -75, 0, PTR(&dan_70_dark_luigi_chest_open_evt), 0, 0)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("b1"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("b1"), 65536)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("b2"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("b2"), 65536)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("b3"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("b3"), 65536)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("b4"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("b4"), 65536)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("b5"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("b5"), 65536)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("b6"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("b6"), 65536)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("b7"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("b7"), 65536)
    USER_FUNC(evt_mobj_flag_onoff, 1, 0, PTR("b8"), 64)
    USER_FUNC(evt_mobj_flag_onoff, 1, 1, PTR("b8"), 65536)
    USER_FUNC(evt_sub_intpl_msec_init, 11, 1000, 0, 2000)
    DO(0)
        USER_FUNC(evt_sub_intpl_msec_get_value)
        DIVF(LW(0), FLOAT(1000.0))
        SETF(LW(2), LW(0))
        MULF(LW(2), FLOAT(-100.0))
        SETF(LW(3), LW(0))
        MULF(LW(3), FLOAT(1440.0))
        USER_FUNC(evt_mobj_get_position, PTR("b1"), LW(5), LW(6), LW(7))
        USER_FUNC(evt_mobj_set_position, PTR("b1"), LW(5), LW(2), LW(7))
        USER_FUNC(evt_mobj_get_position, PTR("b2"), LW(5), LW(6), LW(7))
        USER_FUNC(evt_mobj_set_position, PTR("b2"), LW(5), LW(2), LW(7))
        USER_FUNC(evt_mobj_get_position, PTR("b3"), LW(5), LW(6), LW(7))
        USER_FUNC(evt_mobj_set_position, PTR("b3"), LW(5), LW(2), LW(7))
        USER_FUNC(evt_mobj_get_position, PTR("b4"), LW(5), LW(6), LW(7))
        USER_FUNC(evt_mobj_set_position, PTR("b4"), LW(5), LW(2), LW(7))
        USER_FUNC(evt_mobj_get_position, PTR("b5"), LW(5), LW(6), LW(7))
        USER_FUNC(evt_mobj_set_position, PTR("b5"), LW(5), LW(2), LW(7))
        USER_FUNC(evt_mobj_get_position, PTR("b6"), LW(5), LW(6), LW(7))
        USER_FUNC(evt_mobj_set_position, PTR("b6"), LW(5), LW(2), LW(7))
        USER_FUNC(evt_mobj_get_position, PTR("b7"), LW(5), LW(6), LW(7))
        USER_FUNC(evt_mobj_set_position, PTR("b7"), LW(5), LW(2), LW(7))
        USER_FUNC(evt_mobj_get_position, PTR("b8"), LW(5), LW(6), LW(7))
        USER_FUNC(evt_mobj_set_position, PTR("b8"), LW(5), LW(2), LW(7))
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
            DO_BREAK()
        END_IF()
    WHILE()
    USER_FUNC(evt_mobj_hit_onoff, 1, PTR("b1"))
    USER_FUNC(evt_mobj_hit_onoff, 1, PTR("b2"))
    USER_FUNC(evt_mobj_hit_onoff, 1, PTR("b3"))
    USER_FUNC(evt_mobj_hit_onoff, 1, PTR("b4"))
    USER_FUNC(evt_mobj_hit_onoff, 1, PTR("b5"))
    USER_FUNC(evt_mobj_hit_onoff, 1, PTR("b6"))
    USER_FUNC(evt_mobj_hit_onoff, 1, PTR("b7"))
    USER_FUNC(evt_mobj_hit_onoff, 1, PTR("b8"))
    USER_FUNC(evt_mario_set_anim_change_handler, 0)
    USER_FUNC(evt_cam_zoom_to_coords, 500, 11)
    USER_FUNC(evt_mario_key_on)
    SET(GF(0), 0)
    SET(GF(1), 0)
    SET(GF(2), 0)
    SET(GF(3), 0)
    SET(GF(4), 0)
    SET(GF(5), 0)
    SET(GF(6), 0)
    SET(GF(7), 0)
    DO(0)
        IF_EQUAL(GF(0), 1)
            IF_EQUAL(GF(1), 1)
                IF_EQUAL(GF(2), 1)
                    IF_EQUAL(GF(3), 1)
                        IF_EQUAL(GF(4), 1)
                            IF_EQUAL(GF(5), 1)
                                IF_EQUAL(GF(6), 1)
                                    IF_EQUAL(GF(7), 1)
                                        DO_BREAK()
                                    END_IF()
                                END_IF()
                            END_IF()
                        END_IF()
                    END_IF()
                END_IF()
            END_IF()
        END_IF()
        WAIT_FRM(1)
    WHILE()
    LBL(0)
    USER_FUNC(evt_mario_key_off, 0)
    USER_FUNC(evt_mario_fairy_reset)
    USER_FUNC(evt_cam_look_at_door, 1, 0)
    USER_FUNC(evt_mapobj_get_position, PTR("dokan"), LW(0), LW(1), LW(2))
    USER_FUNC(func_800e01f8)
    USER_FUNC(evt_cam3d_evt_zoom_in, 1, LW(0), EVT_NULLPTR, EVT_NULLPTR, LW(0), EVT_NULLPTR, EVT_NULLPTR, 500, 11)
    WAIT_MSEC(500)
    WAIT_MSEC(1000)
    USER_FUNC(evt_mapobj_flag_onoff, 1, 0, PTR("dokan"), 1)
    USER_FUNC(evt_hitobj_onoff, PTR("A2D_dokan"), 1, 1)
    USER_FUNC(evt_hitobj_onoff, PTR("A3D_dokan"), 1, 1)
    USER_FUNC(evt_hit_bind_mapobj, PTR("A2D_dokan"), PTR("dokan"))
    USER_FUNC(evt_hit_bind_mapobj, PTR("A3D_dokan"), PTR("dokan"))
    USER_FUNC(evt_mapobj_get_position, PTR("dokan"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_snd_sfxon_3d, PTR("SFX_MOBJ_DOKAN_BORN1"), LW(0), LW(1), LW(2))
    USER_FUNC(evt_sub_intpl_msec_init, 11, -3000, 0, 1000)
    DO(0)
        USER_FUNC(evt_sub_intpl_msec_get_value)
        DIVF(LW(0), FLOAT(100.0))
        USER_FUNC(evt_mapobj_trans, PTR("dokan"), 0, LW(0), 0)
        USER_FUNC(evt_hit_bind_update, PTR("A2D_dokan"))
        USER_FUNC(evt_hit_bind_update, PTR("A3D_dokan"))
        WAIT_FRM(1)
        IF_EQUAL(LW(1), 0)
            DO_BREAK()
        END_IF()
    WHILE()
    WAIT_MSEC(1000)
    USER_FUNC(evt_cam_zoom_to_coords, 500, 11)
    WAIT_MSEC(500)
    USER_FUNC(evt_door_enable_disable_dokan_desc, 1, PTR("dokan"))
    USER_FUNC(evt_mario_key_on)
    SET(GSW(24), 2)
    RETURN()
EVT_END()

EVT_BEGIN(dan_start_shadoo_evt)
    RUN_EVT(PTR(&dan_shadoo_main_evt))
    RETURN()
EVT_END()
