#include <common.h>
#include <spm/camdrv.h>
#include <spm/fadedrv.h>
#include <spm/mario_pouch.h>
#include <spm/mario.h>
#include <spm/hud.h>
#include <spm/item_data.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/system.h>
#include <wii/os.h>
#include <msl/string.h>

// .rodata
#include "orderstrings/80334908_80334bcc.inc"

// .bss
static MarioPouchWork work;
static MarioPouchWork2 work2;

// .sdata
static MarioPouchWork2 * wp2 = &work2;

MarioPouchWork * pouchGetPtr()
{
    return &work;
}

MarioPouchWork2 * pouch2GetPtr()
{
    return wp2;
}

void pouchInit()
{
    MarioPouchWork * pp;
    s32 i;
    s32 j;
    OSTime temp;

    pp = pouchGetPtr();

    memset(pp, 0, sizeof(work));

    // Init stats
    pouchSetLevel(1);
    pouchSetAttack(1);
    pouchSetMaxHp(10);
    pouchSetHp(10);
    pouchSetXp(0);
    pouchSetCoin(0);
    pouchSetArcadeTokens(0);
    pp->flipTimer = 10;
    pp->totalCoinsCollected = 0;
    pp->maxJumpCombo = 0;
    pp->maxStylishCombo = 0;
    pp->enemiesDefeated = 0;

    // Clear items
    for (i = 0; i < POUCH_KEY_ITEM_MAX; i++)
        pp->keyItem[i] = 0;
    for (i = 0; i < POUCH_USE_ITEM_MAX; i++)
        pp->useItem[i] = 0;
    for (i = 0; i < POUCH_SHOP_ITEM_MAX; i++)
        pp->shopItem[i] = 0;
    for (i = 0; i < POUCH_CHAR_ITEM_MAX; i++)
    {
        pp->characters[i].selectable = false;
        pp->characters[i].itemType = ITEM_ID_NULL;
        pp->characters[i].selected = false;
    }
    for (i = 0; i < POUCH_FAIRY_ITEM_MAX; i++)
    {
        pp->pixls[i].selectable = false;
        pp->pixls[i].itemType = ITEM_ID_NULL;
        pp->pixls[i].selected = false;
    }

    // Add all non-cooking usable items (runs out of space)
    for (i = ITEM_ID_USE_START; i < ITEM_ID_COOK_START; i++)
        pouchAddItem(i);

    // Add all characters
    pouchAddItem(ITEM_ID_CHAR_MARIO);
    pouchAddItem(ITEM_ID_CHAR_PEACH);
    pouchAddItem(ITEM_ID_CHAR_KOOPA);
    pouchAddItem(ITEM_ID_CHAR_LUIGI);

    // Add all pixls
    pouchAddItem(ITEM_ID_FAIRY_THROW);
    pouchAddItem(ITEM_ID_FAIRY_BAKUDAN);
    pouchAddItem(ITEM_ID_FAIRY_SLIT);
    pouchAddItem(ITEM_ID_FAIRY_HIPATTACK);
    pouchAddItem(ITEM_ID_FAIRY_MOVE);
    pouchAddItem(ITEM_ID_FAIRY_KUUKAN);
    pouchAddItem(ITEM_ID_FAIRY_PIKKYORO_A);
    pouchAddItem(ITEM_ID_FAIRY_HAMMER);
    pouchAddItem(ITEM_ID_FAIRY_MINI_MINI);
    pouchAddItem(ITEM_ID_FAIRY_COUNTER);
    pouchAddItem(ITEM_ID_FAIRY_SPIN_DASH);

    // Add return pipe and all cooking discs
    pouchAddItem(ITEM_ID_KEY_POCKET_DOKAN);
    pouchAddItem(ITEM_ID_KEY_DISK_R);
    pouchAddItem(ITEM_ID_KEY_DISK_W);
    pouchAddItem(ITEM_ID_KEY_DISK_Y);
    pouchAddItem(ITEM_ID_KEY_DISK_B);
    pouchAddItem(ITEM_ID_KEY_DISK_G);
    pouchAddItem(ITEM_ID_KEY_DISK_PU);

    // Select pixl
    pouchSetPixlSelected(ITEM_ID_FAIRY_THROW);

    pouch2Init();

    // Setup merlee charms
    pp->charmsRemaining = 0;
    pp->killsBeforeNextCharm = 0;

    pp->unknown_0x350 = pp->unknown_0x35c = (Vec3) {0, 0, 0};

    // Setup minigame scores
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 5; j++)
        {
            pp->minigameScores[i][j].characterId = 0;
            pp->minigameScores[i][j].score = 0;
            pp->minigameScores[i][j].unknown_0xc = 0;
            pp->minigameScores[i][j].unknown_0x8 = 0;
            pp->minigameScores[i][j].unknown_0x10 = 0;
        }
    }

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 5; j++)
        {
            pp->unknown_0x548[i][j].unknown_0x0 = 0;
            pp->unknown_0x548[i][j].unknown_0x4 = 50 - (j * 10);
            temp = 4500 - (j * 900);
            pp->unknown_0x548[i][j].unknown_0x8 = OSSecondsToTicks(temp);
            pp->unknown_0x548[i][j].unknown_0x14 = 0;
            pp->unknown_0x548[i][j].unknown_0x10 = 0;
            pp->unknown_0x548[i][j].unknown_0x18 = 0;
        }
    }
}

void pouch2Init()
{
    MarioPouchWork2 * pp2;

    memset(wp2, 0, sizeof(*wp2));

    pp2 = pouch2GetPtr();
    pp2->unknown_0xc = 0;
    pp2->unknown_0x8 = 0;
    pp2->unknown_0x4 = 0;
    pp2->unknown_0x0 = 0;
    pp2->unknown_0x14 = 0;
    // Probably fake match
    wp2->unknown_0x10 = 0;
    wp2->unknown_0x18 = OSGetTime();
}

asm void pouchReInit()
{
    #include "asm/8014c728.s"
}

asm void pouchMain()
{
    #include "asm/8014cd90.s"
}

void pouchResetFlip()
{
    MarioPouchWork * pp;
    MarioPouchWork2 * pp2;

    pp = pouchGetPtr();
    pp2 = pouch2GetPtr();

    pp->flipTimer = 10;
    pp2->unknown_0xc = 0;
    pp2->unknown_0x8 = 0;
    pp2->unknown_0x4 = 0;
    pp2->unknown_0x0 = 0;
    pp2->unknown_0x14 = 0;
    pouch2GetPtr()->unknown_0x10 = 0;
}

void pouchSetLevel(s32 level)
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    pp->level = level;

    if (pp->level > 99)
        pp->level = 99;

    if (pp->level < 1)
        pp->level = 1;
}

s32 pouchGetLevel()
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    return pp->level;
}

asm s32 pouchGetNextLevelXp()
{
    #include "asm/8014d200.s"
}

void pouchSetAttack(s32 attack)
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    pp->attack = attack;

    if (pp->attack > 99)
        pp->attack = 99;

    if (pp->attack < 1)
        pp->attack = 1;
}

s32 pouchGetAttack()
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    return pp->attack;
}

void pouchAddAttack(s32 increase)
{
    s32 attack;
    
    attack = pouchGetAttack();
    pouchSetAttack(attack + increase);
}

void pouchSetHp(s32 hp)
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    pp->hp = hp;

    // Probably fake match
    if (work.hp > work.maxHp)
        work.hp = work.maxHp;

    if (pp->hp < 0)
        pp->hp = 0;

    if (hp != 0)
        pp->flipTimer = 10;
}

s32 pouchGetHp()
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    return pp->hp;
}

void pouchAddHp(s32 increase)
{
    MarioPouchWork * pp;
    s32 maxHp;
    
    pp = pouchGetPtr();
    maxHp = pouchGetMaxHp();    
    
    pp->hp += increase;
    if (pp->hp > maxHp)
        pp->hp = maxHp;
    if (pp->hp < 0)
        pp->hp = 0;

    if (increase != 0)
        pp->flipTimer = 10;
}

void pouchSetMaxHp(s32 maxHp)
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    pp->maxHp = maxHp;

    if (pp->maxHp > 999)
        pp->maxHp = 999;

    if (pp->maxHp < 0)
        pp->maxHp = 0;
}

s32 pouchGetMaxHp()
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    return pp->maxHp;
}

void pouchSetXp(s32 xp)
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    pp->xp = xp;

    if (pp->xp > 99999999)
        pp->xp = 99999999;

    if (pp->xp < 0)
        pp->xp = 0;
}

s32 pouchGetXp()
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    return pp->xp;
}

void pouchAddXp(s32 increase)
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    pp->xp += increase;

    if (pp->xp > 99999999)
        pp->xp = 99999999;

    if (pp->xp < 0)
        pp->xp = 0;
}

void pouchSetCoin(s32 coins)
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    pp->coins = coins;

    if (pp->coins > 999)
        pp->coins = 999;

    if (pp->coins < 0)
        pp->coins = 0;
}

s32 pouchGetCoin()
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    return pp->coins;
}

void pouchAddTotalCoin(s32 increase) // always inlined
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    pp->totalCoinsCollected += increase;

    if (pp->totalCoinsCollected > 99999)
        pp->totalCoinsCollected = 99999;
}

void pouchAddCoin(s32 increase)
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    pp->coins += increase;

    if (pp->coins > 999)
        pp->coins = 999;

    if (pp->coins < 0)
        pp->coins = 0;

    if (increase > 0)
        pouchAddTotalCoin(increase);
}

asm bool pouchAddItem(s32 itemId)
{
    #include "asm/8014d5f8.s"
}

asm bool pouchCheckHaveItem(s16 itemId)
{
    #include "asm/8014dbf4.s"
}

asm void pouchRemoveItem(s16 itemId)
{
    #include "asm/8014df4c.s"
}

asm void pouchRemoveItemIdx(s16 itemId, s32 idx)
{
    #include "asm/8014e6d0.s"
}

asm bool pouchAddShopItem(s16 itemId)
{
    #include "asm/8014ea38.s"
}

asm void pouchRemoveShopItem(s16 itemId)
{
    #include "asm/8014eb68.s"
}

asm void pouchRemoveShopItemIdx(s16 itemId, s32 idx)
{
    #include "asm/8014edb8.s"
}

PouchCharOrPixlInfo * pouchGetCharInfo(s32 slot)
{
    MarioPouchWork * pp;

    pp = pouchGetPtr();

    return &pp->characters[slot];
}

void pouchMakeCharSelectable(s16 itemId)
{
    MarioPouchWork * pp;
    s32 i;

    pp = pouchGetPtr();

    for (i = 0; i < 4; i++)
        if (itemId == pp->characters[i].itemType)
            break;

    if (i < 4)
        pp->characters[i].selectable = true;
}

void pouchMakeCharNotSelectable(s16 itemId)
{
    MarioPouchWork * pp;
    s32 i;

    pp = pouchGetPtr();

    for (i = 0; i < 4; i++)
        if (itemId == pp->characters[i].itemType)
            break;

    if (i < 4)
        pp->characters[i].selectable = false;
}

PouchCharOrPixlInfo * pouchGetPixlInfo(s32 slot)
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    return &pp->pixls[slot];
}

void pouchMakePixlSelectable(s16 itemId)
{
    MarioPouchWork * pp;
    s32 i;

    pp = pouchGetPtr();

    for (i = 0; i < 16; i++)
        if (itemId == pp->pixls[i].itemType)
            break;
    
    if (i < 16)
        pp->pixls[i].selectable = true;
}

void pouchMakePixlNotSelectable(s16 itemId)
{
    MarioPouchWork * pp;
    s32 i;

    pp = pouchGetPtr();

    for (i = 0; i < 16; i++)
        if (itemId == pp->pixls[i].itemType)
            break;
    
    if (i < 16)
        pp->pixls[i].selectable = false;
}

bool pouchCheckPixlSelected(s16 itemId)
{
    MarioPouchWork * pp;
    s32 i;

    pp = pouchGetPtr();

    for (i = 0; i < 16; i++)
    {
        PouchCharOrPixlInfo * p = &pp->pixls[i];
        if (p->itemType != NULL && p->itemType == itemId)
            return p->selected;
    }
    
    return false;
}

u16 pouchGetCurPixl()
{
    MarioPouchWork * pp;
    s32 i;

    pp = pouchGetPtr();

    for (i = 0; i < 16; i++)
    {
        PouchCharOrPixlInfo * p = &pp->pixls[i];
        if (p->itemType != NULL && p->selected)
            return p->itemType;
    }

    return 0;
}

void pouchSetPixlSelected(s16 itemId)
{
    MarioPouchWork * pp;
    s32 i;

    pp = pouchGetPtr();

    for (i = 0; i < 16; i++)
    {
        PouchCharOrPixlInfo * p = &pp->pixls[i];
        if (p->itemType != 0)
            p->selected = false;
    }

    for (i = 0; i < 16; i++)
    {
        PouchCharOrPixlInfo * p = &pp->pixls[i];
        if (p->itemType != 0 && p->itemType == itemId)
            p->selected = true;
    }
}

void pouchRegisterMapFound(s16 itemId)
{
    s32 mapId;
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    assertf(1117, itemId >= ITEM_ID_MAP_START && itemId < ITEM_ID_MAP_MAX,
            "そんな地図はない %d", itemId);

    mapId = itemId - ITEM_ID_MAP_START;
    pp->foundMaps[mapId / 32] |= (1 << (mapId % 32));
}

void pouchRegisterRecipeKnown(s16 itemId) {
    s32 cookId;
    MarioPouchWork * pp;

    pp = pouchGetPtr();
    
    assertf(1151, itemId >= ITEM_ID_COOK_START && itemId < ITEM_ID_COOK_MAX,
            "そんな料理はない %d", itemId);
    
    cookId = itemId - ITEM_ID_COOK_START;
    pp->knownRecipes[cookId / 32] |= (1 << (cookId % 32));
}

bool pouchCheckRecipeKnown(s16 itemId)
{
    MarioPouchWork * pp;
    s32 cookId;

    pp = pouchGetPtr();
    
    assertf(1165, itemId >= ITEM_ID_COOK_START && itemId < ITEM_ID_COOK_MAX,
            "そんな料理はない %d", itemId);

    cookId = itemId - ITEM_ID_COOK_START;
    if (pp->knownRecipes[cookId / 32] & 1 << (cookId % 32))
        return true;
    
    return false;
}

s32 pouchGetCardCount(s16 itemId)
{
    MarioPouchWork * pp;
    s32 cardId;

    pp = pouchGetPtr();

    assertf(1184, itemId >= ITEM_ID_CARD_START && itemId < ITEM_ID_CARD_MAX,
            "おかしなカード番号です %d", itemId);

    cardId = itemId - ITEM_ID_CARD_START;
    return pp->catchCards[cardId];
}

bool pouchCheckCardKnown(s16 itemId)
{
    MarioPouchWork * pp;
    s32 cardId;

    pp = pouchGetPtr();

    assertf(1194, itemId >= ITEM_ID_CARD_START && itemId < ITEM_ID_CARD_MAX,
            "そんなカードはない %d", itemId);

    cardId = itemId - ITEM_ID_CARD_START;
    if (pp->knownCards[cardId / 32] & 1 << (cardId % 32))
        return true;
    
    return false;
}

void pouchRegisterJumpCombo(s32 length)
{
    MarioPouchWork * pp;

    pp = pouchGetPtr();

    if (length > pp->maxJumpCombo)
        pp->maxJumpCombo = length;
    
    if (pp->maxJumpCombo > 99999)
        pp->maxJumpCombo = 99999;
}

void pouchRegisterStylishCombo(s32 length)
{
    MarioPouchWork * pp;
    
    pp = pouchGetPtr();

    if (length > pp->maxStylishCombo)
        pp->maxStylishCombo = length;
    
    if (pp->maxStylishCombo > 99999)
        pp->maxStylishCombo = 99999;
}

void pouchSetEnemiesDefeated(s32 count)
{
    MarioPouchWork * pp;

    pp = pouchGetPtr();

    pp->enemiesDefeated = count;
    if (count > 99999)
        pp->enemiesDefeated = 99999;
}

s32 pouchGetTotalCoinsCollected()
{
    MarioPouchWork * pp;

    pp = pouchGetPtr();

    return pp->totalCoinsCollected;
}

s32 pouchGetMaxJumpCombo()
{
    MarioPouchWork * pp;

    pp = pouchGetPtr();

    return pp->maxJumpCombo;
}

s32 pouchGetMaxStylishCombo()
{
    MarioPouchWork * pp;

    pp = pouchGetPtr();

    return pp->maxStylishCombo;
}

s32 pouchGetEnemiesDefeated()
{
    MarioPouchWork * pp;

    pp = pouchGetPtr();

    return pp->enemiesDefeated;
}

s32 pouchGetArcadeTokens()
{
    MarioPouchWork * pp;

    pp = pouchGetPtr();

    return pp->arcadeTokens;
}

void pouchSetArcadeTokens(s32 tokens)
{
    MarioPouchWork * pp;

    pp = pouchGetPtr();

    pp->arcadeTokens = tokens;
    if (pp->arcadeTokens < 0)
        pp->arcadeTokens = 0;

    if (pp->arcadeTokens > 9999)
        pp->arcadeTokens = 9999;
}

s32 pouchCountUseItems()
{
    MarioPouchWork * pp;
    s32 count;
    s8 i;

    pp = pouchGetPtr();
    count = 0;

    for (i = 0; i < 10; i++)
    {
        if (pp->useItem[i] != NULL)
            count++;
    }

    return count;
}

s32 pouchCountKeyItems()
{
    MarioPouchWork * pp;
    s32 count;
    s32 i;

    pp = pouchGetPtr();
    count = 0;

    for (i = 0; i < 32; i++)
    {
        if (pp->keyItem[i] != NULL)
            count++;
    }

    return count;
}

s32 pouchCountShopItems()
{
    MarioPouchWork * pp;
    s32 count;
    s32 i;

    pp = pouchGetPtr();
    count = 0;

    for (i = 0; i < 32; i++)
    {
        if (pp->shopItem[i] != NULL)
            count++;
    }

    return count;
}

s32 pouchCountChars()
{
    MarioPouchWork * pp;
    s32 count;
    s32 i;

    pp = pouchGetPtr();
    count = 0;

    for (i = 0; i < 4; i++)
    {
        if (pp->characters[i].itemType != NULL)
            count++;
    }

    return count;
}

s32 pouchCountPixls()
{
    MarioPouchWork * pp;
    s32 count;
    s32 i;

    pp = pouchGetPtr();
    count = 0;

    for (i = 0; i < 16; i++)
    {
        if (pp->pixls[i].itemType != NULL)
            count++;
    }

    return count;
}

s32 pouchCountPixlsNotSelected()
{
    MarioPouchWork * pp;
    s32 count;
    s32 i;

    pp = pouchGetPtr();
    count = 0;

    for (i = 0; i < 16; i++)
    {
        if (pp->pixls[i].itemType != NULL && pp->pixls[i].selected)
            count++;
    }

    return count;
}

bool pouchCheckFreeUseItem()
{
    MarioPouchWork * pp;
    s32 i;

    pp = pouchGetPtr();

    for (i = 0; i < 10; i++)
        if (pp->useItem[i] == NULL)
            return true;
    
    return false;
}

s32 pouchCountMapsOwned()
{
    MarioPouchWork * pp;
    s32 count;
    s32 i;

    pp = pouchGetPtr();
    count = 0;

    for (i = 0;  i < 48; i++)
        if (pp->ownedMaps[i / 32] & 1 << (i % 32))
            count++;
    
    return count;
}

s32 pouchCountRecipesKnown()
{
    MarioPouchWork * pp;
    s32 count;
    s32 i;

    pp = pouchGetPtr();
    count = 0;

    for (i = 0; i < 96; i++)
        if (pp->knownRecipes[i / 32] & 1 << (i % 32))
            count++;

    return count;
}

s32 pouchCountCardsKnown()
{
    MarioPouchWork * pp;
    s32 count;
    s32 i;

    pp = pouchGetPtr();
    count = 0;

    for (i = 0; i < 256; i++)
        if (pp->knownCards[i / 32] & 1 << (i % 32))
            count++;
    
    return count;
}
