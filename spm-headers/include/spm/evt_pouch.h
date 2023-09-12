#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_pouch)

EVT_DECLARE_USER_FUNC(evt_pouch_set_hp, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_get_hp, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_add_hp, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_get_max_hp, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_set_max_hp, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_get_xp, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_add_xp, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_set_attack, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_get_attack, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_add_attack, 1)
EVT_UNKNOWN_USER_FUNC(evt_pouch_set_coins)
EVT_UNKNOWN_USER_FUNC(evt_pouch_get_coins)
EVT_UNKNOWN_USER_FUNC(evt_pouch_add_coins)

// evt_pouch_add_item(s32 id)
EVT_DECLARE_USER_FUNC(evt_pouch_add_item, 1)

// evt_pouch_check_have_item(s32 id, bool& ret)
EVT_DECLARE_USER_FUNC(evt_pouch_check_have_item, 2)

EVT_DECLARE_USER_FUNC(evt_pouch_remove_item, 1)
EVT_UNKNOWN_USER_FUNC(evt_pouch_remove_item_idx)
EVT_UNKNOWN_USER_FUNC(evt_pouch_add_shop_itme)
EVT_UNKNOWN_USER_FUNC(evt_pouch_remove_shop_item)
EVT_UNKNOWN_USER_FUNC(evt_pouch_remove_shop_item_idx)

// evt_pouch_set_pixl_selected(s32 id)
EVT_DECLARE_USER_FUNC(evt_pouch_set_pixl_selected, 1)


EVT_UNKNOWN_USER_FUNC(evt_pouch_count_use_items)
EVT_UNKNOWN_USER_FUNC(evt_pouch_count_free_shop_items)
EVT_UNKNOWN_USER_FUNC(evt_pouch_count_shop_items)
EVT_UNKNOWN_USER_FUNC(evt_pouch_check_free_use_item)
EVT_UNKNOWN_USER_FUNC(evt_pouch_change_char_selectable)
EVT_UNKNOWN_USER_FUNC(evt_pouch_change_pixl_selectable)
EVT_DECLARE_USER_FUNC(evt_pouch_get_level, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_set_level, 1)
EVT_DECLARE_USER_FUNC(evt_pouch_get_next_level_xp, 1)
EVT_UNKNOWN_USER_FUNC(evt_pouch_get_arcade_tokens)
EVT_UNKNOWN_USER_FUNC(evt_pouch_set_arcade_tokens)
EVT_UNKNOWN_USER_FUNC(evt_pouch_get_total_coins_collected)
EVT_UNKNOWN_USER_FUNC(evt_pouch_get_max_jump_combo)
EVT_UNKNOWN_USER_FUNC(evt_pouch_get_max_stylish_combo)
EVT_UNKNOWN_USER_FUNC(evt_pouch_get_enemies_defeated)

//evt_pouch_increment_enemies_defeated(void)
EVT_DECLARE_USER_FUNC(evt_pouch_increment_enemies_defeated, 0)

CPP_WRAPPER_END()
