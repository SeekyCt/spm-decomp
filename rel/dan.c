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

static DanWork * wp = NULL;
static const char * danMapParts[DAN_PARTS_COUNT];
static MapDoorDesc danMapDoorDescs[2];
static DokanDesc danChestRoomDokanDesc;
static MapDoorDesc danChestRoomMapDoorDescs[2];

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
        assertf_alt(144, no >= 0 && no < DUNGEON_MAX, "‚È‚ñ‚©”Ô†‚ª‚¨‚©‚µ‚¢ [%d]", no);

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

    return 2;
}

int evt_dan_handle_map_parts(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon
    int no = evtGetValue(entry, entry->pCurData[0]);
    DanDungeon * dungeon = wp->dungeons + no;

    // Turn off all parts by default
    mapGrpFlagOn(false, "parts", 1);
    hitGrpFlagOn(false, "A2_parts", 1);
    mapGrpFlag4On(false, "block", 0x20);

    // Disable parts from map mask
    for (u32 i = 0; i < DAN_PARTS_COUNT; i++)
    {
        if (dungeon->map & (1 << i))
        {
            char a2Part[256];
            mapGrpFlagOff(0, danMapParts[i], 1);
            sprintf(a2Part, "A2_%s", danMapParts[i]);
            hitGrpFlagOff(0, a2Part, 1);
        }
    }

    // Handle special parts
    if (CHECK_ALL_MASK(dungeon->map, 0xC))
    {
        mapGrpFlagOff(0, "parts_12_a", 1);
        mapGrpFlagOn(0, "parts_12_b", 1);
        mapGrpFlagOn(0, "parts_12_c", 1);
        hitGrpFlagOff(0, "A2_parts_12_a", 1);
        hitGrpFlagOn(0, "A2_parts_12_b", 1);
        hitGrpFlagOn(0, "A2_parts_12_c", 1);
    }
    if (CHECK_ALL_MASK(dungeon->map, 0xc0))
    {
        mapGrpFlagOff(0, "parts_09_a", 1);
        mapGrpFlagOn(0, "parts_09_b", 1);
        mapGrpFlagOn(0, "parts_09_c", 1);
        hitGrpFlagOff(0, "A2_parts_09_a", 1);
        hitGrpFlagOn(0, "A2_parts_09_b", 1);
        hitGrpFlagOn(0, "A2_parts_09_c", 1);
    }
    if (CHECK_ALL_MASK(dungeon->map, 0x300))
    {
        mapGrpFlagOff(0, "parts_11_a", 1);
        mapGrpFlagOn(0, "parts_11_b", 1);
        mapGrpFlagOn(0, "parts_11_c", 1);
        hitGrpFlagOff(0, "A2_parts_11_a", 1);
        hitGrpFlagOn(0, "A2_parts_11_b", 1);
        hitGrpFlagOn(0, "A2_parts_11_c", 1);
    }
    if (CHECK_ALL_MASK(dungeon->map, 0x3000))
    {
        mapGrpFlagOff(0, "parts_10_a", 1);
        mapGrpFlagOn(0, "parts_10_b", 1);
        mapGrpFlagOn(0, "parts_10_c", 1);
        hitGrpFlagOff(0, "A2_parts_10_a", 1);
        hitGrpFlagOn(0, "A2_parts_10_b", 1);
        hitGrpFlagOn(0, "A2_parts_10_c", 1);
    }

    return 2;
}

int evt_dan_handle_dokans(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    // Get dungeon
    int no = evtGetValue(entry, entry->pCurData[0]);
    DanDungeon * dungeon = wp->dungeons + no;

    // Turn off all pipes by default
    mapGrpFlagOn(0, "dokan", 1);
    hitGrpFlagOn(0, "A2D_dokan", 1);
    hitGrpFlagOn(0, "A3D_dokan", 1);

    // Turn on enabled pipes
    if (CHECK_ANY_MASK(dungeon->map, 0x10000)) {
        mapGrpFlagOff(0, "dokan_01", 1);
        hitGrpFlagOff(0, "A2D_dokan_01", 1);
        hitGrpFlagOff(0, "A3D_dokan_01", 1);
        mapGrpFlagOff(0, "dokan_02", 1);
        hitGrpFlagOff(0, "A2D_dokan_02", 1);
        hitGrpFlagOff(0, "A3D_dokan_02", 1);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x20000)) {
        mapGrpFlagOff(0, "dokan_03", 1);
        hitGrpFlagOff(0, "A2D_dokan_03", 1);
        hitGrpFlagOff(0, "A3D_dokan_03", 1);
        mapGrpFlagOff(0, "dokan_04", 1);
        hitGrpFlagOff(0, "A2D_dokan_04", 1);
        hitGrpFlagOff(0, "A3D_dokan_04", 1);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x40000)) {
        mapGrpFlagOff(0, "dokan_05", 1);
        hitGrpFlagOff(0, "A2D_dokan_05", 1);
        hitGrpFlagOff(0, "A3D_dokan_05", 1);
        mapGrpFlagOff(0, "dokan_06", 1);
        hitGrpFlagOff(0, "A2D_dokan_06", 1);
        hitGrpFlagOff(0, "A3D_dokan_06", 1);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x80000)) {
        mapGrpFlagOff(0, "dokan_07", 1);
        hitGrpFlagOff(0, "A2D_dokan_07", 1);
        hitGrpFlagOff(0, "A3D_dokan_07", 1);
        mapGrpFlagOff(0, "dokan_08", 1);
        hitGrpFlagOff(0, "A2D_dokan_08", 1);
        hitGrpFlagOff(0, "A3D_dokan_08", 1);
    }

    return 2;
}

int rand();

int evt_dan_handle_doors(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    EvtScriptCode * args = entry->pCurData;
    int no = evtGetValue(entry, args[0]);
    int room = evtGetValue(entry, args[1]);
    DanDungeon * dungeon = wp->dungeons + no;

    mapGrpFlagOn(0, "doa", 1);

    if (room > dungeon->doorCount)
        room = 1;
    if (room == 0)
        room = (rand() % dungeon->doorCount) + 1;
    room -= 1;

    wp->doorInfo.enter = dungeon->doors[room].enter;
    wp->doorInfo.exit = dungeon->doors[room].exit;

    char str[64];

    sprintf(str, "doa_%02d", wp->doorInfo.enter);
    mapGrpFlagOff(0, str, 1);
    sprintf(str, "A2_doa_%02d", wp->doorInfo.enter);
    hitGrpFlagOff(0, str, 1);
    sprintf(str, "A3_doa_%02d", wp->doorInfo.enter);
    hitGrpFlagOff(0, str, 1);

    sprintf(str, "doa_%02d", wp->doorInfo.exit);
    mapGrpFlagOff(0, str, 1);
    sprintf(str, "A2_doa_%02d", wp->doorInfo.exit);
    hitGrpFlagOff(0, str, 1);
    sprintf(str, "A3_doa_%02d", wp->doorInfo.exit);
    hitGrpFlagOff(0, str, 1);

    sprintf(wp->enterDoorName_l, "doa%d_l", wp->doorInfo.enter);
    sprintf(wp->enterDoorName_r, "doa%d_r", wp->doorInfo.enter);
    sprintf(wp->enterDoorName_A2, "A2_doa_%02d", wp->doorInfo.enter);
    sprintf(wp->enterDoorName_A3, "A3_doa_%02d", wp->doorInfo.enter);
    sprintf(wp->prevMapName, "");
    sprintf(wp->enterDoor_desc0x18, "");

    sprintf(wp->exitDoorName_l, "doa%d_l", wp->doorInfo.exit);
    sprintf(wp->exitDoorName_r, "doa%d_r", wp->doorInfo.exit);
    sprintf(wp->exitDoorName_A2, "A2_doa_%02d", wp->doorInfo.exit);
    sprintf(wp->exitDoorName_A3, "A3_doa_%02d", wp->doorInfo.exit);
    sprintf(wp->nextMapName, getNextDanMapname(no + 1));
    sprintf(wp->exitDoor_desc0x18,"");

    danMapDoorDescs[0].name_l = wp->enterDoorName_l;
    danMapDoorDescs[0].name_r = wp->enterDoorName_r;
    danMapDoorDescs[0].name_A2 = wp->enterDoorName_A2;
    danMapDoorDescs[0].name_A3 = wp->enterDoorName_A3;
    danMapDoorDescs[0].destMapName = wp->prevMapName;
    danMapDoorDescs[0].unknown_0x18 = wp->enterDoor_desc0x18;

    danMapDoorDescs[1].name_l = wp->exitDoorName_l;
    danMapDoorDescs[1].name_r = wp->exitDoorName_r;
    danMapDoorDescs[1].name_A2 = wp->exitDoorName_A2;
    danMapDoorDescs[1].name_A3 = wp->exitDoorName_A3;
    danMapDoorDescs[1].destMapName = wp->nextMapName;
    danMapDoorDescs[1].unknown_0x18 = wp->exitDoor_desc0x18;

    sprintf(wp->enterDoorName, "doa_%02d", wp->doorInfo.enter);
    sprintf(wp->exitDoorName, "doa_%02d", wp->doorInfo.exit);
    strcpy(gp->doorName, wp->enterDoorName_l);
    evtSetValue(entry, args[2], (s32) wp->enterDoorName_l);
    evtSetValue(entry, args[3], (s32) wp->exitDoorName_l);
    evtSetValue(entry, GSWF(23), 0);

    Vec3 doorPos;
    hitObjGetPos(wp->exitDoorName_A2, &doorPos);
    evtSetFloat(entry, args[4], doorPos.x);
    evtSetFloat(entry, args[5], doorPos.y - 40.0f);
    evtSetFloat(entry, args[6], doorPos.z);
    return 2;
}

int evt_dan_get_door_names(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    EvtScriptCode * args = entry->pCurData;
    evtSetValue(entry, args[0], (s32) wp->enterDoorName);
    evtSetValue(entry, args[1], (s32) wp->exitDoorName);

    return 2;
}

int evt_dan_get_exit_door_name_l(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    EvtScriptCode * args = entry->pCurData;
    evtSetValue(entry, args[0], (s32) wp->exitDoorName_l);

    return 2;
}

int evt_dan_get_enemy_info(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    EvtScriptCode * args = entry->pCurData;
    
    int no = evtGetValue(entry, args[0]);
    int enemyIdx = evtGetValue(entry, args[1]);
    
    DanDungeon * dungeon = wp->dungeons + no;

    if ((enemyIdx < 0) || (enemyIdx >= 16))
    {
        evtSetValue(entry, args[2], 0);
        evtSetValue(entry, args[3], 0);

        return 2;
    }
    else
    {
        DanEnemy * enemy = dungeon->enemies + enemyIdx;

        if (enemy->num > 0)
        {
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
            evtSetValue(entry, args[2], 0);
            evtSetValue(entry, args[3], 0);
        }

        return 2;
    }
}

inline void unkInline(int n)
{
    wp->unknown_0x10[wp->unknown_0xc++] = n;
}

int func_80c82d38(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    int no = evtGetValue(entry, entry->pCurData[0]);

    DanDungeon * dungeon = wp->dungeons + no;

    wp->unknown_0xc = 0;

    if (CHECK_ANY_MASK(dungeon->map, 0x400))
        unkInline(1);

    unkInline(2);
    unkInline(3);

    if (CHECK_ANY_MASK(dungeon->map, 0x1000))
    {
        unkInline(4);
        unkInline(5);
    }

    unkInline(6);
    unkInline(7);

    if (CHECK_ANY_MASK(dungeon->map, 0x8000))
        unkInline(8);

    if (CHECK_ANY_MASK(dungeon->map, 0x40))
    {
        unkInline(10);
        unkInline(11);
    }

    unkInline(12);
    unkInline(13);

    if (CHECK_ANY_MASK(dungeon->map, 0x100))
    {
        unkInline(14);
        unkInline(15);
    }
    if (CHECK_ANY_MASK(dungeon->map, 0x1))
        unkInline(17);
    
    unkInline(18);
    unkInline(19);

    if (CHECK_ANY_MASK(dungeon->map, 0x4))
    {
        unkInline(20);
        unkInline(21);
    }

    unkInline(22);
    unkInline(23);

    if (CHECK_ANY_MASK(dungeon->map, 0x20))
        unkInline(24);

    unkInline(25);
    unkInline(26);
    unkInline(27);
    unkInline(28);
    unkInline(29);
    unkInline(30);
    unkInline(31);
    unkInline(32);

    for (int i = 0; i < 100; i++)
    {
        int idx1 = rand() % wp->unknown_0xc;
        int idx2 = rand() % wp->unknown_0xc;
        int temp = wp->unknown_0x10[idx1];
        wp->unknown_0x10[idx1] = wp->unknown_0x10[idx2];
        wp->unknown_0x10[idx2] = temp;
    }

    return 2;
}

int evt_dan_get_enemy_spawn_pos(EvtEntry * entry, bool isInitialCall)
{
    (void) isInitialCall;

    EvtScriptCode * args = entry->pCurData;

    int enemyNum = evtGetValue(entry, args[0]);
    int no = evtGetValue(entry, args[1]);
    int enemyIdx = evtGetValue(entry, args[2]);
    DanDungeon * dungeon = wp->dungeons + no;
    DanEnemy * enemy = dungeon->enemies + enemyIdx;

    int k = enemyNum % wp->unknown_0xc;
    int j = 0;

    char doorName[64];
    if (enemy->pos != 0)
    {
        sprintf(doorName, "A2_doa_%02d", enemy->pos);
    }
    else
    {
        int i;
        for (i = 0; i < wp->unknown_0xc; i++)
        {
            if ((wp->doorInfo.enter != wp->unknown_0x10[i]) && (wp->doorInfo.exit != wp->unknown_0x10[i]))
            {
                if (j == k)
                    break;
                j++;
            }
        }
        sprintf(doorName, "A2_doa_%02d", wp->unknown_0x10[i]);
    }

    Vec3 doorPos;
    hitObjGetPos(doorName, &doorPos);
    doorPos.z = 0.0f;

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

    return 2;
}

int evt_dan_decide_key_enemy(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    int itemId = evtGetValue(entry, entry->pCurData[0]);
    NPCWork * npcWp = npcGetWorkPtr();

    NPCEntry * curNpc = npcWp->entries;
    int enemyCount = 0;
    NPCEntry * enemies[80];
    for (int i = 0; i < npcWp->num; curNpc++, i++)
    {
        if (CHECK_ANY_MASK(curNpc->flags_8, 0x1) && !CHECK_ANY_MASK(curNpc->flags_8, 0x40000))
            enemies[enemyCount++] = curNpc;
    }

    enemies[rand() % enemyCount]->dropItemId = itemId;

    return 2;
}

void danCountdownDone()
{
    seqSetSeq(SEQ_GAMEOVER, NULL, NULL);
}

int evt_dan_start_countdown(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    hudStartCountdown(300, danCountdownDone);

    return 2;
}

bool danCheckKeyInMapBbox()
{
    ItemWork * itemWp = itemGetWork();
    ItemEntry * item = itemWp->entries;

    Vec3 min, max;
    hitGetMapEntryBbox(0, &min, &max);
    
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
    NPCWork * npcWp = npcGetWorkPtr();
    NPCEntry * npc = npcWp->entries;

    Vec3 min, max;
    hitGetMapEntryBbox(0, &min, &max);

    min.x -= 5.0f;
    min.y -= 5.0f;
    min.z -= 5.0f;
    min.x += 5.0f;
    min.y += 5.0f;
    min.z += 5.0f;

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

    if (
        !danCheckKeyEnemyInMapBbox() && !danCheckKeyInMapBbox() &&
        !pouchCheckHaveItem(DAN_KEY) && !pouchCheckHaveItem(URA_DAN_KEY) &&
        !itemCheckForId(DAN_KEY) && !itemCheckForId(URA_DAN_KEY)
    )
    {
        MOBJEntry * lock = mobjNameToPtr("lock_00");
        int keyId = DAN_KEY;
        if (evtGetValue(entry, GSW(1)) >= 100)
            keyId = URA_DAN_KEY;
        
        itemEntry(lock->pos.x, lock->pos.y, 0.0f, NULL, keyId, 1, NULL, 0);
        func_800cd554(lock->pos.x, lock->pos.y, 0.0f, 0.0f, -1.0f, 0.0f, 4, 8);
        func_800b426c(lock->pos.x, lock->pos.y, 0.0f, 1, 0);
        return 2;
    }
    else
    {
        return 0;
    }
}

int evt_dan_handle_chest_room_dokans_and_doors(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    int no = evtGetValue(entry, entry->pCurData[0]);
    
    danChestRoomMapDoorDescs[1].destMapName = getNextDanMapname(no + 1);
    
    strcpy(gp->doorName, "doa1_l");
    
    if (no < 100)
    {
        danChestRoomDokanDesc.destMapName = "mac_05";
        danChestRoomDokanDesc.unknown_0x1c = "dokan_1";
    }
    else
    {
        danChestRoomDokanDesc.destMapName = "mac_15";
        danChestRoomDokanDesc.unknown_0x1c = "dokan_1";
    }

    return 2;
}

int evt_dan_get_chest_room_item(EvtEntry * entry, bool isFirstCall)
{
    (void) isFirstCall;

    EvtScriptCode * args = entry->pCurData;

    int no = evtGetValue(entry, args[0]);
    
    evtSetValue(entry, args[1], wp->dungeons[no - 1].item);

    return 2;
}

int evt_dan_boss_room_set_door_name(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    strcpy(gp->doorName, "doa1_l");

    return 2;
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

    NPCEntry * npc = npcNameToPtr("zun");
    animPoseSetDispCallback(npc->m_Anim.m_nPoseId, wracktailDispCb, npc);

    return 2;
}

// Unfinished, just for float pool
int func_80c83c48(EvtEntry * entry, bool isFirstCall)
{
    (void) entry;
    (void) isFirstCall;

    __dummy_float(0.0f);

    return 2;   
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

    dispEntry(11, 4, 1100.0f, screenBlinkDisp, NULL);

    return 0;
}

const char * func_80c83f6c(const char * param_1)
{
    MarioWork * mp = marioGetPtr();
    if (mp->character == PLAYER_PEACH && strcmp(param_1, "S_1") == 0)
        return "S_1B";
    else
        return param_1;
}

static const char * danMapParts[DAN_PARTS_COUNT] = {
    "parts_05",
    "parts_06",
    "parts_12_b",
    "parts_12_c",
    "parts_07",
    "parts_08",
    "parts_09_b",
    "parts_09_c",
    "parts_11_b",
    "parts_11_c",
    "parts_01",
    "parts_02",
    "parts_10_b",
    "parts_10_c",
    "parts_03",
    "parts_04"
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
