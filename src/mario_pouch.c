#include <common.h>
#include <mario_pouch.h>
#include <string.h>
#include <system.h>

#pragma push
#pragma inline_max_auto_size(10)

static MarioPouchWork work;

MarioPouchWork * pouchGetPtr() {
    return &work;
}

// pouchInit
// pouchReInit
// pouchMain
// 8014d184

void pouchSetLevel(int level) {
    MarioPouchWork * pp = pouchGetPtr();
    pp->level = level;
    if (pp->level > 99)
        pp->level = 99;
    if (pp->level < 1)
        pp->level = 1;
}

int pouchGetLevel() {
    MarioPouchWork * pp = pouchGetPtr();
    return pp->level;
}

// pouchGetNextLevelXp

void pouchSetAttack(int attack) {
    MarioPouchWork * pp = pouchGetPtr();
    pp->attack = attack;
    if (pp->attack > 99)
        pp->attack = 99;
    if (pp->attack < 1)
        pp->attack = 1;
}

int pouchGetAttack() {
    MarioPouchWork * pp = pouchGetPtr();
    return pp->attack;
}

void pouchAddAttack(int increase) {
    pouchSetAttack(pouchGetAttack() + increase);
}

void pouchSetHp(int hp) {
    MarioPouchWork * pp = pouchGetPtr();
    pp->hp = hp;
    if (pp->hp > pp->maxHp)
        pp->hp = pp->maxHp;
    if (pp->hp < 0)
        pp->hp = 0;
    if (hp != 0)
        pp->flipTimer = 10;
}

int pouchGetHp() {
    MarioPouchWork * pp = pouchGetPtr();
    return pp->hp;
}

// Register usage not matching
void pouchAddHp(int increase) {
    MarioPouchWork * pp = pouchGetPtr();
    pp->hp += increase;
    if (pp->hp > pp->maxHp)
        pp->hp = pp->maxHp;
    if (pp->hp < 0)
        pp->hp = 0;
    if (increase != 0)
        pp->flipTimer = 10;
}

void pouchSetMaxHp(int maxHp) {
    MarioPouchWork * pp = pouchGetPtr();
    pp->maxHp = maxHp;
    if (pp->maxHp > 999)
        pp->maxHp = 999;
    if (pp->maxHp < 0)
        pp->maxHp = 0;
}

int pouchGetMaxHp() {
    MarioPouchWork * pp = pouchGetPtr();
    return pp->maxHp;
}

int pouchGetXp() {
    MarioPouchWork * pp = pouchGetPtr();
    return pp->xp;
}

void pouchAddXp(int increase) {
    MarioPouchWork * pp = pouchGetPtr();
    pp->xp += increase;
    if (pp->xp > 99999999)
        pp->xp = 99999999;
    if (pp->xp < 0)
        pp->xp = 0;
}

void pouchSetCoin(int coins) {
    MarioPouchWork * pp = pouchGetPtr();
    pp->coins = coins;
    if (pp->coins > 999)
        pp->coins = 999;
    if (pp->coins < 0)
        pp->coins = 0;
}

int pouchGetCoin() {
    MarioPouchWork * pp = pouchGetPtr();
    return pp->coins;
}

void pouchAddTotalCoin(int increase) { // always inlined
    MarioPouchWork * pp = pouchGetPtr();
    pp->totalCoinsCollected += increase;
    if (pp->totalCoinsCollected > 99999)
        pp->totalCoinsCollected = 99999;
}

void pouchAddCoin(int increase) {
    MarioPouchWork * pp = pouchGetPtr();
    pp->coins += increase;
    if (pp->coins > 999)
        pp->coins = 999;
    if (pp->coins < 0)
        pp->coins = 0;

    if (increase > 0)
        pouchAddTotalCoin(increase);
}

// Not matching
bool pouchAddItem(short itemId) {
    MarioPouchWork * pp = pouchGetPtr();
    if (itemId >= ITEM_ID_KEY_START && itemId < ITEM_ID_KEY_MAX) {
        int i;
        for (i = 0; i < 32; i++) {
            if (pp->keyItem[i] == 0)
                break;
        }

        if (i >= 32)
            return false;
        else
            pp->keyItem[i] = itemId;
    }
    else if (itemId >= ITEM_ID_USE_START && itemId < ITEM_ID_USE_MAX) {
        int i;
        for (i = 9; i >= 0; i--) {
            if (pp->useItem[i] == 0) {
                // Maybe all an inline function?

                // Move back current items
                for (int j = 0; j < i; j++)
                    pp->useItem[i + j] = pp->useItem[i + j - 1];

                // Insert new item at the start
                pp->useItem[0] = itemId;

                if (itemId >= ITEM_ID_COOK_START && itemId < ITEM_ID_COOK_MAX)
                    pouchRegisterRecipeKnown(itemId); // inline

                return true;
            }
        }
        return false;
    }
    else if (itemId >= ITEM_ID_CHAR_START && itemId < ITEM_ID_CHAR_MAX) {
        // Check if this character is already in inventory
        int i;
        for (i = 0; i < 4; i++) {
            if (pp->characters[i].id != 0 && pp->characters[i].id == itemId)
                break;
        }
        if (i < 4)
            return true;
        
        for (i = 0; i < 4; i++) {
            if (pp->characters[i].id == 0)
                break;
        }
        if (i >= 4)
            return false;
        
        pp->characters[i].selectable = true;
        pp->characters[i].id = itemId;
        pp->characters[i].selected = true;   
    }
    else if (itemId >= ITEM_ID_FAIRY_START && itemId < ITEM_ID_CHAR_MAX) {
        // Check if this pixl is already in inventory
        int i;
        for (i = 0; i < 16; i++) {
            if (pp->pixls[i].id != 0 && pp->pixls[i].id == itemId)
                break;
        }
        if (i < 16)
            return true;
        
        for (i = 0; i < 4; i++) {
            if (pp->characters[i].id == 0)
                break;
        }
        if (i >= 16)
            return false;
        pp->pixls[i].selectable = true;
        pp->pixls[i].id = itemId;
        pp->pixls[i].selected = false;
    }
    else if (itemId >= ITEM_ID_MAP_START && itemId < ITEM_ID_MAP_MAX) {
        itemId -= ITEM_ID_MAP_START;
        pp->ownedMaps[itemId / 32] |= 1 << (itemId % 32);
    }
    else if (itemId >= ITEM_ID_CARD_START && itemId < ITEM_ID_CARD_MAX) {
        itemId -= ITEM_ID_CARD_START;
        pp->catchCards[itemId] += 1;
        if (pp->catchCards[itemId] > 99)
            pp->catchCards[itemId] = 99;
        pp->knownCards[itemId / 32] |= 1 << (itemId % 32);
    }
    else {
        assertf(742, 0, "おかしいで�? id = %d", itemId);
    }
    return true;
}

// pouchCheckHaveItem

// Not matching
void pouchRemoveItem(int itemId) {
    MarioPouchWork * pp = pouchGetPtr();
    if (itemId >= ITEM_ID_KEY_START && itemId < ITEM_ID_KEY_MAX) {
        int i;
        for (i = 0; i < 32; i++) {
            if (pp->keyItem[i] == itemId)
                break;
        }
        if (i >= 32)
            return;
        
        pp->keyItem[i] = 0;
        for (int j = i; j < 32; j++)
            pp->keyItem[j] = pp->keyItem[j + 1];
        pp->keyItem[i] = 0;
    }
    else if (itemId >= ITEM_ID_USE_START && itemId < ITEM_ID_USE_MAX) {
        int i;
        for (i = 0; i < 10; i++) {
            if (pp->useItem[i] == itemId)
                break;
        }
        if (i >= 10)
            return;
        
        pp->useItem[i] = 0;
    }
}

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
// pouchRegisterRecipeKnown
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
