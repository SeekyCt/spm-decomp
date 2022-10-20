#include <common.h>
#include <spm/camdrv.h>
#include <spm/fadedrv.h>
#include <spm/mario_pouch.h>
#include <spm/mario.h>
#include <spm/hud.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/system.h>
#include <wii/os.h>
#include <wii/string.h>

// .rodata
#include "orderstrings/80334908_80334bcc.inc"

// .bss
static MarioPouchWork2 work2;
static MarioPouchWork work;

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

asm void pouchInit()
{
    #include "asm/8014c094.s"
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

    if (pp->hp > pp->maxHp)
        pp->hp = pp->maxHp;

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

asm bool pouchAddItem(s16 itemId)
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

asm PouchCharOrPixlInfo * pouchGetCharInfo(s32 slot)
{
    #include "asm/8014ef98.s"
}

asm void pouchMakeCharSelectable(s16 itemId)
{
    #include "asm/8014efb0.s"
}

asm void pouchMakeCharNotSelectable(s16 itemId)
{
    #include "asm/8014f018.s"
}

asm PouchCharOrPixlInfo * pouchGetPixlInfo(s32 slot)
{
    #include "asm/8014f080.s"
}

asm void pouchMakePixlSelectable(s16 itemId)
{
    #include "asm/8014f098.s"
}

asm void pouchMakePixlNotSelectable(s16 itemId)
{
    #include "asm/8014f154.s"
}

asm bool pouchCheckPixlSelected(s16 id)
{
    #include "asm/8014f210.s"
}

asm s16 pouchGetCurPixl()
{
    #include "asm/8014f31c.s"
}

asm void pouchSetPixlSelected(s16 itemId)
{
    #include "asm/8014f408.s"
}

asm void pouchRegisterMapFound(s16 itemId)
{
    #include "asm/8014f594.s"
}

asm void pouchRegisterRecipeKnown(s16 itemId)
{
    #include "asm/8014f64c.s"
}

asm bool pouchCheckRecipeKnown(s16 itemId)
{
    #include "asm/8014f704.s"
}

asm s32 pouchGetCardCount(s16 itemId)
{
    #include "asm/8014f7c8.s"
}

asm bool pouchCheckCardKnown(s16 itemId)
{
    #include "asm/8014f84c.s"
}

asm void pouchRegisterJumpCombo(s32 length)
{
    #include "asm/8014f910.s"
}

asm void pouchRegisterStylishCombo(s32 length)
{
    #include "asm/8014f944.s"
}

asm void pouchSetEnemiesDefeated(s32 count)
{
    #include "asm/8014f978.s"
}

asm void pouchGetTotalCoinsCollected(s32 count)
{
    #include "asm/8014f99c.s"
}

asm s32 pouchGetMaxJumpCombo()
{
    #include "asm/8014f9ac.s"
}

asm s32 pouchGetMaxStylishCombo()
{
    #include "asm/8014f9bc.s"
}

asm s32 pouchGetEnemiesDefeated()
{
    #include "asm/8014f9cc.s"
}

asm s32 pouchGetArcadeTokens()
{
    #include "asm/8014f9dc.s"
}

asm void pouchSetArcadeTokens(s32 tokens)
{
    #include "asm/8014f9ec.s"
}

asm s32 pouchCountUseItems()
{
    #include "asm/8014fa20.s"
}

asm s32 pouchCountKeyItems()
{
    #include "asm/8014fad0.s"
}

asm s32 pouchCountShopItems()
{
    #include "asm/8014fb78.s"
}

asm s32 pouchCountChars()
{
    #include "asm/8014fc20.s"
}

asm s32 pouchCountPixls()
{
    #include "asm/8014fc70.s"
}

asm s32 pouchCountPixlsNotSelected()
{
    #include "asm/8014fd18.s"
}

asm bool pouchCheckFreeUseItem()
{
    #include "asm/8014fdb0.s"
}

asm s32 pouchCountMapsOwned()
{
    #include "asm/8014fe88.s"
}

asm s32 pouchCountRecipesKnown()
{
    #include "asm/8014ff9c.s"
}

asm s32 pouchCountCardsKnown()
{
    #include "asm/801500b0.s"
}
