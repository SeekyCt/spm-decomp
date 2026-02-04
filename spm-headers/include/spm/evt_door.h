#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/evtmgr.h>

CPP_WRAPPER(spm::evt_door)

USING(spm::evtmgr::EvtScriptCode)

typedef struct
{
    u32 flags;
    s32 type;
    s32 field2_0x8;
    const char *name;
    const char *openCase1Name;
    const char *openCase2Name;
    const char *closeCase1Name;
    const char *closeCase2Name;
    const char *field8_0x20;
    const char *field9_0x24;
    const char *field10_0x28;
    const char *mapGrpName;
    const char *hitGrpName1;
    const char *hitGrpName2;
    const char *field14_0x38;
    const char *field15_0x3c;
    EvtScriptCode *interactScript;
    const char **npcNameList;
    const char **mobjNameList;
    const char **itemNameList;
    EvtScriptCode *initScript;
    EvtScriptCode *moveScript;
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
/* 0x18 */ const char * destDoorName; // loading zone; door, pipe, etc.
/* 0x1C */ u32 doorSfxIdx;
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
/* 0x1C */ const char * destDoorName;
} DokanDesc;
SIZE_ASSERT(DokanDesc, 0x20)

#define EVT_DOOR_FLAG_DOOR_OPEN 1

typedef struct
{
/* 0x000 */ u16 flags;
/* 0x002 */ u8 unknown_0x2[0x57c - 0x2];
} EvtDoorWork;
SIZE_ASSERT(EvtDoorWork, 0x57c)

UNKNOWN_FUNCTION(evtDoorInit)
UNKNOWN_FUNCTION(evt_door_flag_onoff)
UNKNOWN_FUNCTION(func_800e1040)
UNKNOWN_FUNCTION(func_800e1058)
UNKNOWN_FUNCTION(func_800e1070)
UNKNOWN_FUNCTION(evtDoorCheckMapDoorDescExists)
UNKNOWN_FUNCTION(evtDoorCheckMapDoorDescInUnkMask)
DoorDesc * evtDoorGetActiveDoorDesc();
UNKNOWN_FUNCTION(func_800e11d0)
EVT_DECLARE_USER_FUNC(evt_door_wait_flag, 1)
UNKNOWN_FUNCTION(func_800e120c)
UNKNOWN_FUNCTION(evt_door_cmd)
UNKNOWN_FUNCTION(func_800e186c)
UNKNOWN_FUNCTION(func_800e18d0)
UNKNOWN_FUNCTION(func_800e1934)
UNKNOWN_FUNCTION(func_800e1c6c)
UNKNOWN_FUNCTION(func_800e1d34)
UNKNOWN_FUNCTION(func_800e1f34)
UNKNOWN_FUNCTION(func_800e200c)
UNKNOWN_FUNCTION(evt_door_set_active_door_desc)
UNKNOWN_FUNCTION(func_800e21d0)
UNKNOWN_FUNCTION(func_800e21d8)
UNKNOWN_FUNCTION(func_800e21e0)
UNKNOWN_FUNCTION(evt_door_get_desc)
UNKNOWN_FUNCTION(func_800e2500)
UNKNOWN_FUNCTION(roomBottomlesssCb)

// evt_door_set_door_descs(DoorDesc * descs, s32 count)
EVT_DECLARE_USER_FUNC(evt_door_set_door_descs, 1)

// evt_door_enable_disable_door_desc(bool enableDisable, const char * doorName)
EVT_DECLARE_USER_FUNC(evt_door_enable_disable_door_desc, 2)

UNKNOWN_FUNCTION(func_800e2b78)
UNKNOWN_FUNCTION(func_800e3050)
UNKNOWN_FUNCTION(func_800e32a0)
UNKNOWN_FUNCTION(func_800e333c)
UNKNOWN_FUNCTION(func_800e34f0)

// evt_door_set_dokan_descs(DokanDesc * descs, s32 count)
EVT_DECLARE_USER_FUNC(evt_door_set_dokan_descs, 2)

// evt_door_enable_disable_dokan_desc(bool enabled, const char * name)
EVT_DECLARE_USER_FUNC(evt_door_enable_disable_dokan_desc, 2)

UNKNOWN_FUNCTION(func_800e3ad4)
UNKNOWN_FUNCTION(func_800e3b08)
UNKNOWN_FUNCTION(func_800e3b60)
UNKNOWN_FUNCTION(func_800e3ef4)
UNKNOWN_FUNCTION(func_800e3f74)
UNKNOWN_FUNCTION(func_800e4014)
UNKNOWN_FUNCTION(func_800e40e0)

// evt_door_set_map_door_descs(MapDoorDesc * descs, s32 count)
EVT_DECLARE_USER_FUNC(evt_door_set_map_door_descs, 2)

// evt_door_enable_disable_map_door_desc(bool enabled, const char * name)
EVT_DECLARE_USER_FUNC(evt_door_enable_disable_map_door_desc, 2)

UNKNOWN_FUNCTION(evt_door_add_map_door_desc_to_unk_mask)

// evt_door_set_event(char *door, int unknown, EvtScriptCode * script)
EVT_DECLARE_USER_FUNC(evt_door_set_event, 3)

EVT_DECLARE_USER_FUNC(evt_door_openable_onoff, 1)
UNKNOWN_FUNCTION(evt_door_get_map_door_desc)
UNKNOWN_FUNCTION(func_800e47a8)
UNKNOWN_FUNCTION(func_800e56b4)
UNKNOWN_FUNCTION(evt_door_get_dokan_descs)
UNKNOWN_FUNCTION(evt_door_get_dokan_desc_num)
UNKNOWN_FUNCTION(evt_door_get_map_door_descs)
UNKNOWN_FUNCTION(evt_door_get_map_door_desc_num)

EVT_DECLARE(evt_door_dan_dokan)
EVT_DECLARE(lbl_80417e10)
EVT_DECLARE(door_init_evt)
EVT_DECLARE(evt_door_dan_dokan_left_one)
EVT_DECLARE(evt_door_dan_dokan_left_two)
EVT_DECLARE(evt_door_dan_dokan_left_three)
EVT_DECLARE(evt_door_dan_dokan_left_four)
EVT_DECLARE(evt_door_dan_dokan_right_one)
EVT_DECLARE(evt_door_dan_dokan_right_two)
EVT_DECLARE(evt_door_dan_dokan_right_three)
EVT_DECLARE(evt_door_dan_dokan_right_four)
        
DECOMP_STATIC(EvtDoorWork * evt_door_wp)

CPP_WRAPPER_END()
