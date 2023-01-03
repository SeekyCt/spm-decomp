/*
    item_data defines the properties of the game's items
*/

#pragma once

#include <common.h>

#include <spm/item_data_ids.h>

CPP_WRAPPER(spm::item_data)

typedef struct
{
/* 0x00 */ const char * itemName;
/* 0x04 */ s16 iconId;
/* 0x06 */ // padding 0x6-7
/* 0x08 */ const char * animPoseName; // Used for pixls and cards
/* 0x0C */ const char * animName; // Used for pixls and cards
/* 0x10 */ const char * nameMsg;
/* 0x14 */ const char * descMsg;
/* 0x18 */ u16 buyPrice;
/* 0x1A */ u16 sellPrice;
/* 0x1C */ s16 hpGain;
/* 0x1E */ s16 xpGain;
/* 0x20 */ u8 cardBagChance; // weight for showing up in a card bag, if available
/* 0x21 */ u8 cardShopChance; // weight for showing up in Flopside's card shop, if available
    /*
        GSW(0)  -> seq indicator
        000-010 -> 0
        011-017 -> 11
        018-028 -> 12
        029-037 -> 13
        038-065 -> 14
        066-076 -> 21
        077-082 -> 22
        083-088 -> 23
        089-100 -> 24
        101-105 -> 31
        106-111 -> 32
        112-117 -> 33
        118-128 -> 34
        129-138 -> 41
        139-160 -> 42
        161-178 -> 44
        179-186 -> 51
        187-194 -> 52
        195-202 -> 53
        203-224 -> 54
        225-271 -> 61
        272-289 -> 62
        290-290 -> 63
        291-291 -> 64
        292-306 -> 71
        307-327 -> 72
        328-358 -> 74
        359-370 -> 81
        371-386 -> 82
        387-404 -> 83
        405-420 -> 84
        421+    -> 90
        
        If GSW(0) is high enough to give an indicator above or equal to this,
        the weights for card bags and the card shop are multiplied by 10
    */
/* 0x22 */ u16 cardShopBonusSeq;
    /*
        If GSW(0) is below this, the card won't show
        up in either card bags or the card shop
    */
/* 0x24 */ u16 cardShopMinGsw0;
    /*
        If this is true, the card can never show up in card bags or the card
        shop once it's set as known in MarioPouchWork. The exception to this is
        that selling one allows it to show up again in the shop (but not bags)
    */
/* 0x26 */ bool cardShopBlockDuplicate;
    /*
        For the NPCs on cards
        0x2 is dark
        0x1 is unknown
        Others unused
    */
/* 0x27 */ u8 dispFlags;
/* 0x28 */ s16 tribe; // For the NPCs on cards
/* 0x2A */ s16 sortValue; // For sorting in shops, higher value -> lower on the list
} ItemData;
SIZE_ASSERT(ItemData, 0x2c)

extern ItemData itemDataTable[ITEM_ID_MAX];

CPP_WRAPPER_END()
