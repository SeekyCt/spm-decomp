#pragma once

#include <common.h>
#include <evt_cmd.h>

typedef struct
{
    s32 type;
    const char * npcName;
    s32 * items; // [itemId, price, arg] repeated then [-1]
    const char * case1Name;
    const char * case2Name;
} EvtShopDef;

// evt_shop_set_defs(EvtShopDef * defs, s32 count)
EVT_DECLARE_USER_FUNC(evt_shop_set_defs, 2)
