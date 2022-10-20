#pragma once

#include <common.h>

#define ITEM_ID_KEY_START 16
#define ITEM_ID_KEY_MAX 65
#define ITEM_ID_USE_START 65
#define ITEM_ID_USE_MAX 216
#define ITEM_ID_COOK_START 120
#define ITEM_ID_COOK_MAX 216
#define ITEM_ID_CHAR_START 216
#define ITEM_ID_CHAR_MAX 220
#define ITEM_ID_FAIRY_START 220
#define ITEM_ID_FAIRY_MAX 232
#define ITEM_ID_MAP_START 234
#define ITEM_ID_MAP_MAX 282
#define ITEM_ID_CARD_START 282
#define ITEM_ID_CARD_MAX 538

typedef struct
{
    bool selectable;
    bool selected;
    s16 id;
} PouchCharOrPixlInfo;

typedef struct
{
    s32 characterId;
    s32 score;
    u8 unknown_0x8[0x18 - 0x8];
} PouchMinigameScore;

typedef struct
{
    u8 unknown_0x0[0x4 - 0x0]; // unused?
    s32 level;
    s32 attack;
    s32 hp;
    s32 maxHp;
    s32 flipTimer;
    s32 xp;
    s32 coins;
    s16 keyItem[32];
    s16 useItem[10];
    s16 shopItem[32];
    PouchCharOrPixlInfo characters[4];
    PouchCharOrPixlInfo pixls[16];
    s32 shopPoints;
    u32 shopPointRewardsCollected; // bit mask, index 1 << i
    s8 catchCards[256]; // value is count, index is id - first card id
    u8 unknown_0x20c[256];
    u32 ownedMaps[2]; // [id / 32] & 1 << (id % 32)
    u32 foundMaps[2]; // [id / 32] & 1 << (id % 32)
    u32 knownRecipes[3]; // [id / 32] & 1 << (id % 32)
    u32 knownCards[8]; // [id / 32] & 1 << (id % 32)
    s32 charmsRemaining;
    s32 killsBeforeNextCharm;
    u8 unknown_0x350[0x368 - 0x350];
    PouchMinigameScore minigameScores[4][5];
    u8 unknown_0x548[0x688 - 0x548];
    s32 arcadeTokens;
    s32 totalCoinsCollected;
    s32 maxJumpCombo;
    s32 maxStylishCombo;
    s32 enemiesDefeated;
    u8 unknown_0x69c[0x6a0 - 0x69c]; // unused?
} MarioPouchWork;

typedef struct
{
    s32 unknown_0x0;
    s32 unknown_0x4;
    s32 unknown_0x8;
    s32 unknown_0xc;
    s32 unknown_0x10;
    s32 unknown_0x14;
    u8 unknown_0x18[0x20 - 0x18];
} MarioPouchWork2;

MarioPouchWork * pouchGetPtr();
MarioPouchWork2 * pouch2GetPtr();
void pouchInit();
void pouchReInit();
void pouchMain();
void pouchResetFlip();
void pouchSetLevel(s32 level);
s32 pouchGetLevel();
s32 pouchGetNextLevelXp();
void pouchSetAttack(s32 attack);
s32 pouchGetAttack();
void pouchAddAttack(s32 increase);
void pouchSetHp(s32 hp);
s32 pouchGetHp();
void pouchAddHp(s32 increase);
void pouchSetMaxHp(s32 maxHp);
s32 pouchGetMaxHp();
s32 pouchGetXp();
void pouchAddXp(s32 increase);
void pouchSetCoin(s32 coins);
s32 pouchGetCoin();
void pouchAddTotalCoin(s32 increase); // inlined
void pouchAddCoin(s32 increase);
bool pouchAddItem(s16 itemId);
bool pouchCheckHaveItem(s16 itemId);
void pouchRemoveItem(s16 itemId);
void pouchRemoveItemIdx(s16 itemId, s32 idx);
bool pouchAddShopItem(s16 itemId);
void pouchRemoveShopItem(s16 itemId);
void pouchRemoveShopItemIdx(s16 itemId, s32 idx);
PouchCharOrPixlInfo * pouchGetCharInfo(s32 slot);
void pouchMakeCharSelectable(s16 itemId);
void pouchMakeCharNotSelectable(s16 itemId);
PouchCharOrPixlInfo * pouchGetPixlInfo(s32 slot);
void pouchMakePixlSelectable(s16 itemId);
void pouchMakePixlNotSelectable(s16 itemId);
bool pouchCheckPixlSelected(s16 itemId);
s16 pouchGetCurPixl();
void pouchSetPixlSelected(s16 itemId);
void pouchRegisterMapFound(s16 itemId);
void pouchRegisterRecipeKnown(s16 itemId);
bool pouchCheckRecipeKnown(s16 itemId);
s32 pouchGetCardCount(s16 itemId);
bool pouchCheckCardKnown(s16 itemId);
void pouchRegisterJumpCombo(s32 length);
void pouchRegisterStylishCombo(s32 length);
void pouchSetEnemiesDefeated(s32 count);
void pouchGetTotalCoinsCollected(s32 count);
s32 pouchGetMaxJumpCombo();
s32 pouchGetMaxStylishCombo();
s32 pouchGetEnemiesDefeated();
s32 pouchGetArcadeTokens();
void pouchSetArcadeTokens(s32 tokens);
s32 pouchCountUseItems();
s32 pouchCountKeyItems();
s32 pouchCountShopItems();
s32 pouchCountChars();
s32 pouchCountPixls();
s32 pouchCountPixlsNotSelected();
bool pouchCheckFreeUseItem();
s32 pouchCountMapsOwned();
s32 pouchCountRecipesKnown();
s32 pouchCountCardsKnown();
