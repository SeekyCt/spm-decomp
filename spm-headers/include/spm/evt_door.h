#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_door)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0xc - 0x0];
/* 0x0C */ const char * name;
/* 0x10 */ u8 unknown_0x10[0x44 - 0x10];
/* 0x44 */ const char ** npcNameList;
/* 0x48 */ u8 unknown_0x48[0x58 - 0x48];
} DoorDesc;
SIZE_ASSERT(DoorDesc, 0x58)

typedef struct
{
/* 0x00 */ u32 flags;
/* 0x04 */ const char * name_l;
/* 0x08 */ const char * name_r;
/* 0x0C */ const char * hitName2d;
/* 0x10 */ const char * hitName3d;
/* 0x14 */ const char * destMapName;
/* 0x18 */ const char * unknown_0x18;
/* 0x1C */ s32 unknown_0x1c;
} MapDoorDesc;
SIZE_ASSERT(MapDoorDesc, 0x20)

typedef struct
{
/* 0x00 */ s32 scriptNumber;
/* 0x04 */ u16 flags;
/* 0x06 */ u16 _pad_6;
/* 0x08 */ const char * name;
/* 0x0C */ const char * mapName;
/* 0x10 */ const char * hitName2d;
/* 0x14 */ const char * hitName3d;
/* 0x19 */ const char * destMapName;
/* 0x1C */ const char * unknown_0x1c;
} DokanDesc;
SIZE_ASSERT(DokanDesc, 0x20)

#define EVT_DOOR_FLAG_DOOR_OPEN 1

typedef struct
{
/* 0x000 */ u16 flags;
/* 0x002 */ u8 unknown_0x2[0x57c - 0x2];
} EvtDoorWork;
SIZE_ASSERT(EvtDoorWork, 0x57c)

UNKNOWN_FUNCTION(evtDoorInit);
UNKNOWN_FUNCTION(evt_door_flag_onoff);
UNKNOWN_FUNCTION(func_800e1040);
UNKNOWN_FUNCTION(func_800e1058);
UNKNOWN_FUNCTION(func_800e1070);
UNKNOWN_FUNCTION(evtDoorCheckMapDoorDescExists);
UNKNOWN_FUNCTION(evtDoorCheckMapDoorDescInUnkMask);
DoorDesc * evtDoorGetActiveDoorDesc();
UNKNOWN_FUNCTION(func_800e11d0);
EVT_DECLARE_USER_FUNC(evt_door_wait_flag, 1)
UNKNOWN_FUNCTION(func_800e120c);
UNKNOWN_FUNCTION(evt_door_cmd);
UNKNOWN_FUNCTION(func_800e186c);
UNKNOWN_FUNCTION(func_800e18d0);
UNKNOWN_FUNCTION(func_800e1934);
UNKNOWN_FUNCTION(func_800e1c6c);
UNKNOWN_FUNCTION(func_800e1d34);
UNKNOWN_FUNCTION(func_800e1f34);
UNKNOWN_FUNCTION(func_800e200c);
UNKNOWN_FUNCTION(evt_door_set_active_door_desc);
UNKNOWN_FUNCTION(func_800e21d0);
UNKNOWN_FUNCTION(func_800e21d8);
UNKNOWN_FUNCTION(func_800e21e0);
UNKNOWN_FUNCTION(evt_door_get_desc);
UNKNOWN_FUNCTION(func_800e2500);
UNKNOWN_FUNCTION(roomBottomlesssCb);

// evt_door_set_door_descs(DoorDesc * descs, s32 count)
EVT_DECLARE_USER_FUNC(evt_door_set_door_descs, 1)


UNKNOWN_FUNCTION(evt_door_enable_disable_door_desc);
UNKNOWN_FUNCTION(func_800e2b78);
UNKNOWN_FUNCTION(func_800e3050);
UNKNOWN_FUNCTION(func_800e32a0);
UNKNOWN_FUNCTION(func_800e333c);
UNKNOWN_FUNCTION(func_800e34f0);

// evt_door_set_dokan_descs(DokanDesc * descs, s32 count)
EVT_DECLARE_USER_FUNC(evt_door_set_dokan_descs, 1)

// evt_door_enable_disable_dokan_desc(bool enabled, const char * name)
EVT_DECLARE_USER_FUNC(evt_door_enable_disable_dokan_desc, 2)

UNKNOWN_FUNCTION(func_800e3ad4);
UNKNOWN_FUNCTION(func_800e3b08);
UNKNOWN_FUNCTION(func_800e3b60);
UNKNOWN_FUNCTION(func_800e3ef4);
UNKNOWN_FUNCTION(func_800e3f74);
UNKNOWN_FUNCTION(func_800e4014);
UNKNOWN_FUNCTION(func_800e40e0);

// evt_door_set_map_door_descs(MapDoorDesc * descs, s32 count)
EVT_DECLARE_USER_FUNC(evt_door_set_map_door_descs, 1)

// evt_door_enable_disable_map_door_desc(bool enabled, const char * name)
EVT_DECLARE_USER_FUNC(evt_door_enable_disable_map_door_desc, 2)

UNKNOWN_FUNCTION(evt_door_add_map_door_desc_to_unk_mask);

// evt_door_set_event(char *door, int unknown, EvtScriptCode * script)
EVT_DECLARE_USER_FUNC(evt_door_set_event, 3)

EVT_DECLARE_USER_FUNC(evt_door_openable_onoff, 1)
UNKNOWN_FUNCTION(evt_door_get_map_door_desc);
UNKNOWN_FUNCTION(func_800e47a8);
UNKNOWN_FUNCTION(func_800e56b4);
UNKNOWN_FUNCTION(evt_door_get_dokan_descs);
UNKNOWN_FUNCTION(evt_door_get_dokan_desc_num);
UNKNOWN_FUNCTION(evt_door_get_map_door_descs);
UNKNOWN_FUNCTION(evt_door_get_map_door_desc_num);

EVT_DECLARE(lbl_80417e10)
EVT_DECLARE(door_init_evt)

DECOMP_STATIC(EvtDoorWork * evt_door_wp)

CPP_WRAPPER_END()
