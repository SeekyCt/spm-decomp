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

MarioPouchWork * pouchGetPtr();
void pouchInit();
void pouchReInit();
void pouchMain();
// 8014d184
void pouchSetLevel(int level);
int pouchGetLevel();
int pouchGetNextLevelXp();
void pouchSetAttack(int attack);
int pouchGetAttack();
void pouchAddAttack(int increase);
void pouchSetHp(int attack);
int pouchGetHp();
void pouchAddHp(int increase);
void pouchSetMaxHp(int maxHp);
int pouchGetMaxHp();
int pouchGetXp();
void pouchAddXp(int increase);
void pouchSetCoin(int coins);
int pouchGetCoin();
void pouchAddTotalCoin(int increase);
void pouchAddCoin(int increase);
bool pouchAddItem(short itemId);
bool pouchCheckHaveItem(int itemId);
void pouchRemoveItem(int itemId);
// pouchRemoveItemIdx
// pouchAddShopItem
// pouchRemoveShopItem
// pouchRemoveShopItemIdx
// pouchGetCharInfo
// pouchMakeCharSelectable
// pouchMakeCharNotSelectable
// pouchGetPixlInfo
// pouchMakePixlSelectable
// pouchMakePixlNotSelectable
// pouchCheckPixlSelected
// pouchGetCurPixl
// pouchSetPixlSelected
// pouchRegisterMapFound
void pouchRegisterRecipeKnown(int itemId);
// pouchCheckRecipeKnown
// pouchGetCardCount
// pouchCheckCardKnown
// pouchRegisterJumpCombo
// pouchRegisterStylishCombo
// pouchSetEnemiesDefeated
// pouchGetTotalCoinsCollected
// pouchGetMaxJumpCombo
// pouchGetMaxStylishCombo
// pouchGetEnemiesDefeated
// pouchGetArcadeTokens
// pouchSetArcadeTokens
// pouchCountUseItems
// pouchCountKeyItems
// pouchCountShopItems
// pouchCountChars
// pouchCountPixls
// pouchCountPixlsNotSelected
// pouchCheckFreeUseItem
// pouchCountMapsOwned
// pouchCountRecipesKnown
// pouchCountCardsKnown

#endif
