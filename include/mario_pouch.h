#ifndef MARIO_POUCH_H
#define MARIO_POUCH_H

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
    int characterId;
    int score;
    u8 unknown_0x8[0x18 - 0x8];
} PouchMinigameScore;

typedef struct
{
    u8 unknown_0x0[0x4 - 0x0]; // unused?
    int level;
    int attack;
    int hp;
    int maxHp;
    int flipTimer;
    int xp;
    int coins;
    s16 keyItem[32];
    s16 useItem[10];
    s16 shopItem[32];
    PouchCharOrPixlInfo characters[4];
    PouchCharOrPixlInfo pixls[16];
    int shopPoints;
    u32 shopPointRewardsCollected; // bit mask, index 1 << i
    s8 catchCards[256]; // value is count, index is id - first card id
    u8 unknown_0x20c[256];
    u32 ownedMaps[2]; // [id / 32] & 1 << (id % 32)
    u32 foundMaps[2]; // [id / 32] & 1 << (id % 32)
    u32 knownRecipes[3]; // [id / 32] & 1 << (id % 32)
    u32 knownCards[8]; // [id / 32] & 1 << (id % 32)
    int charmsRemaining;
    int killsBeforeNextCharm;
    u8 unknown_0x350[0x368 - 0x350];
    PouchMinigameScore minigameScores[4][5];
    u8 unknown_0x548[0x688 - 0x548];
    int arcadeTokens;
    int totalCoinsCollected;
    int maxJumpCombo;
    int maxStylishCombo;
    int enemiesDefeated;
    u8 unknown_0x69c[0x6a0 - 0x69c]; // unused?
} MarioPouchWork;

MarioPouchWork * pouchGetPtr(); // 8014c088
void pouchInit(); // 8014c094
void pouchReInit(); // 8014c728
void pouchMain(); // 8014cd90
// func_8014d184
void pouchSetLevel(int level); // 8014d1bc
int pouchGetLevel(); // 8014d1f0
int pouchGetNextLevelXp(); // 8014d200
void pouchSetAttack(int attack); // 8014d394
int pouchGetAttack(); // 8014d3c8
void pouchAddAttack(int increase); // 8014d3d8
void pouchSetHp(int attack); // 8014d414
int pouchGetHp(); // 8014d458
void pouchAddHp(int increase); // 8014d468
void pouchSetMaxHp(int maxHp); // 8014d4b4
int pouchGetMaxHp(); // 8014d4e8
int pouchGetXp(); // 8014d4f8
void pouchAddXp(int increase); // 8014d508
void pouchSetCoin(int coins); // 8014d548
int pouchGetCoin(); // 8014d57c
void pouchAddTotalCoin(int increase); // inlined
void pouchAddCoin(int increase); // 8014d58c
bool pouchAddItem(short itemId); // 8014d5f8
bool pouchCheckHaveItem(int itemId); // 8014dbf4
void pouchRemoveItem(int itemId); // 8014df4c
// pouchRemoveItemIdx 8014e6d0
// pouchAddShopItem 8014ea38
// pouchRemoveShopItem 8014eb68
// pouchRemoveShopItemIdx 8014edb8
// pouchGetCharInfo 8014ef98
// pouchMakeCharSelectable 8014efb0
// pouchMakeCharNotSelectable 8014f018
// pouchGetPixlInfo 8014f080
// pouchMakePixlSelectable 8014f098
// pouchMakePixlNotSelectable 8014f154
// pouchCheckPixlSelected 8014f210
// pouchGetCurPixl 8014f31c
// pouchSetPixlSelected 8014f408
// pouchRegisterMapFound 8014f594
void pouchRegisterRecipeKnown(int itemId); // 8014f64c
// pouchCheckRecipeKnown 8014f704
// pouchGetCardCount 8014f7c8
// pouchCheckCardKnown 8014f84c
// pouchRegisterJumpCombo 8014f910
// pouchRegisterStylishCombo 8014f944
// pouchSetEnemiesDefeated 8014f978
// pouchGetTotalCoinsCollected 8014f99c
// pouchGetMaxJumpCombo 8014f9ac
// pouchGetMaxStylishCombo 8014f9bc
// pouchGetEnemiesDefeated 8014f9cc
// pouchGetArcadeTokens 8014f9dc
// pouchSetArcadeTokens 8014f9ec
// pouchCountUseItems 8014fa20
// pouchCountKeyItems 8014fad0
// pouchCountShopItems 8014fb78
// pouchCountChars 8014fc20
// pouchCountPixls 8014fc70
// pouchCountPixlsNotSelected 8014fd18
// pouchCheckFreeUseItem 8014fdb0
// pouchCountMapsOwned 8014fe88
// pouchCountRecipesKnown 8014ff9c
// pouchCountCardsKnown 801500b0

#endif
