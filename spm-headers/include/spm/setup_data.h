/*
    setup_data defines the structure of the dat files in "./setup/"
    and handles the item-spawning side of them
*/

#include <common.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::setup_data)

USING(wii::mtx::Vec3)

// v0-5 unknown, v0-4 don't have support for items, v0 is unused
// Skawo has the size of the enemy structs for other versions and an unusual unknown field in v1
// documented in https://github.com/skawo/Super-Paper-Mario-Level-Editor-Randomizer/

typedef struct
{
/* 0x00 */ s32 instanceId; // ignored if 0
/* 0x04 */ u8 unknown_0x4[0x10 - 0x4];
/* 0x10 */ s32 unitWork[16];
/* 0x50 */ u8 unknown_0x50[0x5c - 0x50];
/* 0x5C */ f32 gravityRotation; // degrees anti-clockwise about the z-axis
} MiscSetupDataV6;
SIZE_ASSERT(MiscSetupDataV6, 0x60)

typedef struct
{
/* 0x00 */ Vec3 pos;
/* 0x0C */ s32 type; // nTemplateNo, index into npcEnemyTemplates
/* 0x10 */ MiscSetupDataV6 misc;
} SetupEnemyV6;
SIZE_ASSERT(SetupEnemyV6, 0x70)

typedef struct
{
/* 0x0000 */ u16 version;
/* 0x0002 */ // padding 0x2-3
/* 0x0004 */ SetupEnemyV6 enemies[100];
} SetupFileV6;
SIZE_ASSERT(SetupFileV6, 0x2bc4)

typedef struct
{
/* 0x0 */ u16 flags; // 0x10 and 0x1 required to spawn, others unused
/* 0x2 */ u16 type; // only 0 is supported, which is a coin
/* 0x4 */ Vec3 pos;
} SetupItem;
SIZE_ASSERT(SetupItem, 0x10)

typedef struct
{
/* 0x0000 */ SetupFileV6 main;
/* 0x2BC4 */ s32 itemCount;
/* 0x2BC8 */ s32 itemVersion; // 20051201
/* 0x2BCC */ SetupItem items[1]; // itemCount items
} SetupFileV6WithItems;
OFFSET_ASSERT(SetupFileV6WithItems, items, 0x2bcc)

typedef struct
{
/* 0x0 */ u16 id; // only 0 is used
/* 0x2 */ u16 itemTeplateId; // index into setupItemTemplates
} SetupItemTemplate;
SIZE_ASSERT(SetupItemTemplate, 0x4)

extern SetupItemTemplate setupItemTemplates[1]; // only contains id 0 - a coin

/*
    Takes a pointer to the item list of a setup file & its length and spawns them 
*/
void setupSpawnItems(s32 count, SetupItem * items);

/*
    Takes a pointer to a setup file and returns the number of items in it, the
    version of the item data and the pointer to the array of items
    
    Returns all 0s for setup files that aren't v5 or v6, or for v5 & v6 that
    don't have any item data in them
    (This reads uninitialised memory that happens to be 0 because of disc alignment)
*/
void setupReadItemInfo(void * file, s32 * count, s32 * version, SetupItem ** data);

CPP_WRAPPER_END()
