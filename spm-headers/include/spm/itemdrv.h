/*
    itemdrv handles in-world items
*/

#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/filemgr.h>
#include <spm/icondrv.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::itemdrv)

USING(spm::evtmgr::EvtScriptCode)
USING(spm::evtmgr::EvtVar)
USING(spm::filemgr::FileEntry)
USING(spm::icondrv::IconEntry)
USING(wii::mtx::Vec3)

#define ITEM_NAME_MAX 12

typedef struct
{
/* 0x00 */ u32 flags;
/* 0x04 */ char name[ITEM_NAME_MAX];
/* 0x10 */ s16 type;
/* 0x12 */ s16 behaviour;
/* 0x14 */ Vec3 position;
/* 0x20 */ u8 unknown_0x20[0x24 - 0x20];
/* 0x24 */ IconEntry * icon;
/* 0x28 */ s32 animPoseId;
/* 0x2C */ EvtVar switchNumber;
/* 0x30 */ EvtScriptCode * pickupScript;
/* 0x34 */ u8 unknown_0x34[0x88 - 0x34];
} ItemEntry;
SIZE_ASSERT(ItemEntry, 0x88)

typedef struct
{
/* 0x00 */ s32 num; // 0x100
/* 0x04 */ ItemEntry * entries; // array of num
/* 0x08 */ u8 unknown_0x8[0x64 - 0x8];
/* 0x64 */ s32 tutorialWindowId;
/* 0x68 */ u8 unknown_0x68[0x80 - 0x68];
} ItemWork;
SIZE_ASSERT(ItemWork, 0x80)

DECOMP_STATIC(ItemWork itemdrv_work)
DECOMP_STATIC(ItemWork * itemdrv_wp)

/*
    Returns a pointer to the ItemWork instance
*/
ItemWork * itemGetWork();

/*
    Initialise data used by item functions
*/
void itemInit();
void itemReInit();

UNKNOWN_FUNCTION(func_80077724);

/*
    Handles the effect on the player and sound effects of picking up an item
*/
void itemHandlePickupEffect(Vec3 * itemPos, s32 itemType);

UNKNOWN_FUNCTION(func_80077c48);
UNKNOWN_FUNCTION(func_80077c90);

/*
    Updates items
*/
void itemMain();

/*
    Spawns an item
    Switch number is the script variable (usually GSWF) indicating if it's already bene collected
*/
ItemEntry * itemEntry(const char * name, s32 type, s32 behaviour, f32 x, f32 y, f32 z,
                      EvtScriptCode * pickupScript, EvtVar switchNumber);

/*
    Despawns an item
*/
void itemDelete(const char * name);

/*
    Gets an ItemEntry by name
*/
ItemEntry * itemNameToPtr(const char * name);

UNKNOWN_FUNCTION(func_80079468);
UNKNOWN_FUNCTION(func_800795b8);
UNKNOWN_FUNCTION(func_80079814);
UNKNOWN_FUNCTION(func_80079df4);
UNKNOWN_FUNCTION(func_8007a2e0);
UNKNOWN_FUNCTION(func_8007a598);
UNKNOWN_FUNCTION(func_8007a758);

/*
    Handles most of the pickup behaviour for items (deletion, xp, switchNumber etc),
    and calls itemHandlePickup for the rest
*/
s32 itemCollect(s32 entryId, ItemEntry * entry);

UNKNOWN_FUNCTION(func_8007bc2c);

/*
    Converts an item type name to the id for that item type
    Type names are the name field from items.txt with the following prefixes
        - ITEM_ID_USE_ for items with ids in the range 65-119 (inclusive)
        - ITEM_ID_COOK_ for items with ids in the range 120-215 (inclusive)
        - ITEM_ID_CARD_ for items with ids in the range 282-537 (inclusive)
    (Items that don't fall within any of these ranges can not be used)
*/
s32 itemTypeNameToId(const char * typeNmae);

UNKNOWN_FUNCTION(func_8007be24);
UNKNOWN_FUNCTION(func_8007bee4);
UNKNOWN_FUNCTION(func_8007c3b8);
UNKNOWN_FUNCTION(func_8007c8ec);
UNKNOWN_FUNCTION(func_8007ce34);
UNKNOWN_FUNCTION(func_8007cfc0);
UNKNOWN_FUNCTION(func_8007d1f4);
UNKNOWN_FUNCTION(func_8007d454);
UNKNOWN_FUNCTION(func_8007d568);
UNKNOWN_FUNCTION(func_8007d684);

/*
    Sets the current animation of an item
*/
void itemSetAnim(char * itemName,char * animName);

UNKNOWN_FUNCTION(func_8007d868);

/*
    Checks for the existence of any items in the world with
    the specified item type id
*/
bool itemCheckForId(s32 typeId);

UNKNOWN_FUNCTION(func_8007d8fc);
UNKNOWN_FUNCTION(func_8007dc24);
UNKNOWN_FUNCTION(func_8007dd50);
UNKNOWN_FUNCTION(func_8007dd60);
UNKNOWN_FUNCTION(func_8007de50);
UNKNOWN_FUNCTION(func_8007e0e4);

CPP_WRAPPER_END()
