#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_shop)

typedef struct
{
/* 0x00 */ s32 type;
/* 0x04 */ const char * npcName;
/* 0x08 */ s32 * items; // [itemId, price, arg] repeated then [-1]
/* 0x0C */ const char * case1Name;
/* 0x10 */ const char * case2Name;
} EvtShopDef;
SIZE_ASSERT(EvtShopDef, 0x14)

EVT_UNKNOWN_USER_FUNC(evt_shop_wait_coin_sfx)
EVT_UNKNOWN_USER_FUNC(func_8010d2b8)
EVT_UNKNOWN_USER_FUNC(evt_shop_get_name_items)
EVT_UNKNOWN_USER_FUNC(func_8010d3d4)
EVT_UNKNOWN_USER_FUNC(evt_shop_get_fortune_message)
EVT_UNKNOWN_USER_FUNC(func_8010d5b8)
EVT_UNKNOWN_USER_FUNC(func_8010d674)
EVT_UNKNOWN_USER_FUNC(func_8010da90)
EVT_UNKNOWN_USER_FUNC(func_8010db80)
EVT_UNKNOWN_USER_FUNC(func_8010dc04)
EVT_UNKNOWN_USER_FUNC(func_8010e020)
EVT_UNKNOWN_USER_FUNC(func_8010e110)
EVT_UNKNOWN_USER_FUNC(evt_shop_get_point_reward)
EVT_UNKNOWN_USER_FUNC(evt_shop_set_point_reward_collected)
EVT_UNKNOWN_USER_FUNC(evt_shop_get_shop_points)
EVT_UNKNOWN_USER_FUNC(evt_shop_add_shop_point)
EVT_UNKNOWN_USER_FUNC(evt_shop_clear_shop_points)
EVT_UNKNOWN_USER_FUNC(evt_shop_howzit_select_item)
EVT_UNKNOWN_USER_FUNC(evt_shop_build_howzit_item_table)
EVT_UNKNOWN_USER_FUNC(func_8010e700)
EVT_UNKNOWN_USER_FUNC(evt_shop_add_charm)
EVT_UNKNOWN_USER_FUNC(func_8010e8bc)
EVT_UNKNOWN_USER_FUNC(func_8010ea10)
EVT_UNKNOWN_USER_FUNC(func_8010ea78)
EVT_UNKNOWN_USER_FUNC(func_8010eb1c)
EVT_UNKNOWN_USER_FUNC(func_8010eb60)
EVT_UNKNOWN_USER_FUNC(func_8010ed70)
EVT_UNKNOWN_USER_FUNC(evt_shop_get_card_give_info)
EVT_UNKNOWN_USER_FUNC(evt_shop_get_card_bag)
EVT_UNKNOWN_USER_FUNC(evt_shop_select_card)
EVT_UNKNOWN_USER_FUNC(evt_shop_count_cards)
EVT_UNKNOWN_USER_FUNC(evt_shop_init_card_item_flags)
EVT_UNKNOWN_USER_FUNC(evt_shop_remove_card)
EVT_UNKNOWN_USER_FUNC(evt_shop_init_flopside_cards)
EVT_UNKNOWN_USER_FUNC(evt_shop_get_buy_price)
EVT_UNKNOWN_USER_FUNC(func_801103b8)
EVT_UNKNOWN_USER_FUNC(evt_shop_get_map_info)
EVT_UNKNOWN_USER_FUNC(evt_shop_build_map_table)
EVT_UNKNOWN_USER_FUNC(func_801108f4)
EVT_UNKNOWN_USER_FUNC(evt_shop_check_all_maps_owned)
EVT_UNKNOWN_USER_FUNC(evt_shop_build_flimm_flipside_item_tables)
EVT_UNKNOWN_USER_FUNC(func_80110ac4)
EVT_UNKNOWN_USER_FUNC(evt_shop_filter_flimm_flipside_items)
EVT_UNKNOWN_USER_FUNC(evt_shop_flimm_select_item)
EVT_UNKNOWN_USER_FUNC(func_80111074)
EVT_UNKNOWN_USER_FUNC(func_80111334)
EVT_UNKNOWN_USER_FUNC(evt_shop_build_flimm_pit_item_tables)
EVT_UNKNOWN_USER_FUNC(evt_shop_restore_itemdata_prices)
EVT_UNKNOWN_USER_FUNC(evt_shop_filter_flimm_pit_items)
EVT_UNKNOWN_USER_FUNC(evt_shop_select_item)
EVT_UNKNOWN_USER_FUNC(evt_shop_remove_item)
EVT_UNKNOWN_USER_FUNC(evt_shop_handle_roten_item_flag)
EVT_UNKNOWN_USER_FUNC(evt_shop_get_otoya_bgm)

// evt_shop_set_defs(EvtShopDef * defs, s32 count)
EVT_DECLARE_USER_FUNC(evt_shop_set_defs, 2)

CPP_WRAPPER_END()
