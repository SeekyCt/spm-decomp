#pragma once

#include <common.h>
#include <evt_cmd.h>

typedef struct
{
    u32 flags;
    const char * name_l;
    const char * name_r;
    const char * hitName2d;
    const char * hitName3d;
    const char * destMapName;
    const char * unknown_0x18;
    s32 unknown_0x1c;
} MapDoorDesc;

typedef struct
{
    s32 scriptNumber;
    u16 flags;
    u16 _pad_6;
    const char * name;
    const char * mapName;
    const char * hitName2d;
    const char * hitName3d;
    const char * destMapName;
    const char * unknown_0x1c;
} DokanDesc;

EVT_UNKNOWN_USER_FUNC(evt_door_wait_flag)

// evt_door_set_door_descs(DoorDesc * descs, s32 count)
EVT_DECLARE_USER_FUNC(evt_door_set_door_descs, 1)

// evt_door_set_dokan_descs(DokanDesc * descs, s32 count)
EVT_DECLARE_USER_FUNC(evt_door_set_dokan_descs, 1)

// evt_door_enable_disable_dokan_desc(bool enabled, const char * name)
EVT_DECLARE_USER_FUNC(evt_door_enable_disable_dokan_desc, 2)

// evt_door_set_map_door_descs(MapDoorDesc * descs, s32 count)
EVT_DECLARE_USER_FUNC(evt_door_set_map_door_descs, 1)

// evt_door_enable_disable_map_door_desc(bool enabled, const char * name)
EVT_DECLARE_USER_FUNC(evt_door_enable_disable_map_door_desc, 2)

EVT_DECLARE(lbl_80417e10)
EVT_DECLARE(door_init_evt)

