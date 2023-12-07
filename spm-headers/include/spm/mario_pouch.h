/*
    mario_pouch stores information like xp, HP, items and unlocked pixls
    Note: unlocked characters and pixls are treat as items internally
*/

#pragma once

#include <common.h>
#include <wii/os.h>

CPP_WRAPPER(spm::mario_pouch)

#define POUCH_KEY_ITEM_MAX 32
#define POUCH_USE_ITEM_MAX 10
#define POUCH_SHOP_ITEM_MAX 32
#define POUCH_CHAR_ITEM_MAX 4
#define POUCH_FAIRY_ITEM_MAX 16

USING(wii::os::OSTime)
USING(wii::mtx::Vec3)

typedef struct
{
/* 0x0 */ bool selectable;
/* 0x1 */ bool selected;
/* 0x2 */ u16 itemType;
} PouchCharOrPixlInfo;
SIZE_ASSERT(PouchCharOrPixlInfo, 0x4)

typedef struct
{
/* 0x00 */ s32 characterId;
/* 0x04 */ s32 score;
/* 0x08 */ Unk32 unknown_0x8;
/* 0x0C */ Unk32 unknown_0xc;
/* 0x10 */ u8 unknown_0x10;
/* 0x11 */ u8 unknown_0x11[0x18 - 0x11];
} PouchMinigameScore;
SIZE_ASSERT(PouchMinigameScore, 0x18)

typedef struct
{
/* 0x00 */ Unk32 unknown_0x0;
/* 0x04 */ s32 unknown_0x4;
/* 0x08 */ OSTime unknown_0x8;
/* 0x10 */ Unk32 unknown_0x10;
/* 0x14 */ Unk32 unknown_0x14;
/* 0x18 */ u8 unknown_0x18;
/* 0x19 */ u8 unknown_0x19[0x20 - 0x19];
} PouchUnk;
SIZE_ASSERT(PouchUnk, 0x20)

/*
    This struct is copied in full to & from the save file
*/
typedef struct
{
/* 0x000 */ u8 unknown_0x0[0x4 - 0x0]; // unused?
/* 0x004 */ s32 level;
/* 0x008 */ s32 attack;
/* 0x00C */ s32 hp;
/* 0x010 */ s32 maxHp;
/* 0x014 */ s32 flipTimer;
/* 0x018 */ s32 xp;
/* 0x01C */ s32 coins;
/* 0x020 */ u16 keyItem[POUCH_KEY_ITEM_MAX]; // key items
/* 0x060 */ u16 useItem[POUCH_USE_ITEM_MAX]; // usable items
/* 0x074 */ u16 shopItem[POUCH_SHOP_ITEM_MAX]; // items stored by the player at a shop
/* 0x0B4 */ PouchCharOrPixlInfo characters[POUCH_CHAR_ITEM_MAX];
/* 0x0C4 */ PouchCharOrPixlInfo pixls[POUCH_FAIRY_ITEM_MAX];
/* 0x104 */ s32 shopPoints;
/* 0x108 */ u32 shopPointRewardsCollected; // bit mask, index 1 << i
/* 0x10C */ u8 catchCards[256]; // value is the amount currently owned, index is item id
                                // minus the first card item id (282)
/* 0x20C */ u8 unknown_0x20c[0x30c - 0x20c];

            // where x is the item id minus the first treasure map item id (234):
/* 0x30C */ u32 ownedMaps[2]; // maps currently in inventory, index [x / 32] & 1 << (x % 32)
/* 0x314 */ u32 foundMaps[2]; // maps with rewards claimed, index [x / 32] & 1 << (x % 32)

            // where x is the item id minus the first cooked item id (120):
/* 0x31C */ u32 knownRecipes[3]; // flags indicating if a recipe was ever cooked/collected,
                                 // index [x / 32] & 1 << (x % 32)

            // where x is the item id minus the first card item id (282):
/* 0x328 */ u32 knownCards[8]; // flags indicating if a card was ever collected,
                               // index [x / 32] & 1 << (x % 32)

/* 0x348 */ s32 charmsRemaining; // Merlee charms remaining from current purchase
/* 0x34C */ s32 killsBeforeNextCharm; // kills before the next Merlee charm triggers
/* 0x350 */ Vec3 unknown_0x350;
/* 0x35C */ Vec3 unknown_0x35c;
/* 0x368 */ PouchMinigameScore minigameScores[4][5];
/* 0x548 */ PouchUnk unknown_0x548[2][5];
/* 0x688 */ s32 arcadeTokens;
/* 0x68C */ s32 totalCoinsCollected; // all-time coins collected
/* 0x690 */ s32 maxJumpCombo;
/* 0x694 */ s32 maxStylishCombo;
/* 0x698 */ s32 enemiesDefeated; // all-time enemies defeated
/* 0x69C */ u8 unknown_0x69c[0x6a0 - 0x69c];
} MarioPouchWork;
SIZE_ASSERT(MarioPouchWork, 0x6a0)

typedef struct
{
/* 0x00 */ OSTime unknown_0x0;
/* 0x08 */ OSTime unknown_0x8;
/* 0x10 */ s32 unknown_0x10;
/* 0x14 */ s32 unknown_0x14;
/* 0x18 */ OSTime unknown_0x18;
} MarioPouchWork2;
SIZE_ASSERT(MarioPouchWork2, 0x20)

DECOMP_STATIC(MarioPouchWork mario_pouch_work)
DECOMP_STATIC(MarioPouchWork mario_pouch_work2)

/*
    Returns a pointer to the MarioPouchWork / MarioPouchWork2 instance
*/
MarioPouchWork * pouchGetPtr();
MarioPouchWork2 * pouch2GetPtr(); // inlined

/*
    Initialise data used by pouch functions
    Overwritten later once a save is loaded
*/
void pouchInit();
void pouch2Init(); // inlined
void pouchReInit();

/*
    Updates 3d flip timer
*/
void pouchMain();

void pouchResetFlip();

/*
    Set/get the player's level
*/
void pouchSetLevel(s32 level);
s32 pouchGetLevel();

/*
    Returns the xp required by the next level
*/
s32 pouchGetNextLevelXp();

/*
    Set/get/add to the player's attack power
*/
void pouchSetAttack(s32 attack);
s32 pouchGetAttack();
void pouchAddAttack(s32 increase);

/*
    Set/get/add to the player's HP
*/
void pouchSetHp(s32 hp);
s32 pouchGetHp();
void pouchAddHp(s32 increase);

/*
    Set/get the player's max HP
*/
void pouchSetMaxHp(s32 maxHp);
s32 pouchGetMaxHp();

/*
    Get/add to the player's xp
*/
void pouchSetXp(s32 xp); // inlined
s32 pouchGetXp();
void pouchAddXp(s32 increase);

/*
    Set/get/add to the player's coin count
*/
void pouchSetCoin(s32 coins);
s32 pouchGetCoin();
void pouchAddTotalCoin(s32 increase); // inlined
void pouchAddCoin(s32 increase); // increases totalCoinsCollected

/*
    Adds the specified item to its corresponding inventory (can't be used for shop items)
    Returns whether it was successfully added
*/
bool pouchAddItem(s32 itemId);

/*
    Checks whether the specified item id is in its corresponding inventory
    (can't be used for shop items)
*/
bool pouchCheckHaveItem(s16 itemId);

/*
    Removes the first item with the specified id in its corresponding inventory
    (can't be used for shop items)
*/
void pouchRemoveItem(s16 itemId);

/*
    Removes the with the specified id at the specified index in its corresponding inventory
    Only supports keyItem and useItem
    Asserts that the id of the item at the index is what was passed in
*/
void pouchRemoveItemIdx(s16 itemId, s32 idx);

/*
    Adds the specified item id to the player's shop stored inventory
    Returns whether it was successfully added
*/
bool pouchAddShopItem(s16 itemId);

/*
    Removes the first item with the specified id in the player's shop stored inventory
*/
void pouchRemoveShopItem(s16 itemId);

/*
    Removes the with the specified id at the specified index in the player's shop stored inventory
    Asserts that the id of the item at the index is what was passed in
*/
void pouchRemoveShopItemIdx(s16 itemId, s32 idx);

/*
    Get the information about a character slot
*/
PouchCharOrPixlInfo * pouchGetCharInfo(s32 slot);

/*
    Make a character selectable/not by id, if it exists in a slot
*/
void pouchMakeCharSelectable(s16 itemId);
void pouchMakeCharNotSelectable(s16 itemId);

/*
    Get the information about a pixl slot
*/
PouchCharOrPixlInfo * pouchGetPixlInfo(s32 slot);

/*
    Make a pixl selectable/not by id, if it exists in a slot
*/
void pouchMakePixlSelectable(s16 itemId);
void pouchMakePixlNotSelectable(s16 itemId);

/*
    Check if a pixl is selected by id
*/
bool pouchCheckPixlSelected(s16 itemId);

/*
    Returns the id of the currently selected pixl
*/
u16 pouchGetCurPixl();

/*
    Makes a pixl selected by id, if it exists in a slot
    Deselects all other pixls
*/
void pouchSetPixlSelected(s16 itemId);

/*
    Turns on the foundMaps/knownRecipes bitflag for the specified item id
*/
void pouchRegisterMapFound(s16 itemId);
void pouchRegisterRecipeKnown(s16 itemId);

/*
    Returns whether the specified recipe item id has ever been cooked/collected
*/
bool pouchCheckRecipeKnown(s16 itemId);

/*
    Returns the amount of the specified card in the inventory
*/
s32 pouchGetCardCount(s16 itemId);

/*
    Returns whether the specified card item id has ever been collected
*/
bool pouchCheckCardKnown(s16 itemId);

/*
    Takes a jump/stylish combo length and updates maxJumpCombo/maxStylishCombo if it's higher
*/
void pouchRegisterJumpCombo(s32 length);
void pouchRegisterStylishCombo(s32 length);

/*
    Updates all-time enemies defeated count
*/
void pouchSetEnemiesDefeated(s32 count);

/*
    Returns how many coins have ever been collected on this save
*/
s32 pouchGetTotalCoinsCollected();

/*
    Returns the longest jump/stylish combo length recorded
*/
s32 pouchGetMaxJumpCombo();
s32 pouchGetMaxStylishCombo();

/*
    Returns all-time enemies defeated count
*/
s32 pouchGetEnemiesDefeated();

/*
    Returns/sets the number of arcade tokens in the inventory
*/
s32 pouchGetArcadeTokens();
void pouchSetArcadeTokens(s32 count);

/*
    Returns the number of useItem/keyItem/shopItem/char/pixl slots that aren't empty
    [and aren't selected]
*/
s32 pouchCountUseItems();
s32 pouchCountKeyItems();
s32 pouchCountShopItems();
s32 pouchCountChars();
s32 pouchCountPixls();
s32 pouchCountPixlsNotSelected();

/*
    Returns whether any useItem slot is emtpy
*/
bool pouchCheckFreeUseItem();

/*
    Returns the number of treasure maps currently in the inventory
*/
s32 pouchCountMapsOwned();

/*
    Returns the number of recipes / cards ever collected
*/
s32 pouchCountRecipesKnown();
s32 pouchCountCardsKnown();

CPP_WRAPPER_END()
