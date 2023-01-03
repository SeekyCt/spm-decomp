/*
    winmgr is a higher level wrapper for windowdrv
*/

#pragma once

#include <common.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::winmgr)

USING(wii::mtx::Vec2)
USING(wii::mtx::Vec2i)

struct _WinmgrEntry;
typedef void (WinmgrFunc)(struct _WinmgrEntry * entry);

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x8 - 0x0];
/* 0x08 */ s32 cameraId;
/* 0x0C */ Vec2i pos;
/* 0x14 */ Vec2i scale;
/* 0x1C */ u8 unknown_0x1c[0x20 - 0x1c];
/* 0x20 */ WinmgrFunc * mainFunc;
/* 0x24 */ WinmgrFunc * deleteFunc;
} WindowDesc;
SIZE_ASSERT(WindowDesc, 0x28)

typedef struct
{
/* 0x0 */ u16 entryCount; // number of winmgr entries to create
/* 0x4 */ WindowDesc * descs; // array of entryCount
} WinmgrSelectDesc;
SIZE_ASSERT(WinmgrSelectDesc, 0x8)

DECOMP_STATIC(WindowDesc winmgr_select_desc[2])
DECOMP_STATIC(WindowDesc winmgr_select_recipe_desc[3]) // 3rd is unused
DECOMP_STATIC(WindowDesc winmgr_select_card_desc[2])
DECOMP_STATIC(WinmgrSelectDesc winmgr_select_desc_tbl[8]) // index is winmgr select type

typedef struct
{
/* 0x0 */ s16 itemId;
/* 0x2 */ u8 unknown_0x2[0x4 - 0x2];
/* 0x4 */ s32 animPoseId;
/* 0x8 */ u8 unknown_0x8[0xc - 0x8];
} WinmgrItem;
SIZE_ASSERT(WinmgrItem, 0xc)

typedef struct
{
/* 0x00 */ u32 flags;
/* 0x04 */ u8 unknown_0x4[0x8 - 0x4];
/* 0x08 */ s32 typeOrItemTable;
/* 0x0C */ u8 unknown_0xc[0x10 - 0xc];
/* 0x10 */ s32 selectedItemIdx; // index into item table currently hovered over
/* 0x14 */ u8 unknown_0x14[0x18 - 0x14];
/* 0x18 */ Vec2 pos;
/* 0x20 */ u8 unknown_0x20[0x24 - 0x20];
/* 0x24 */ s32 entryCount; // number of winmgr entries
/* 0x28 */ s32 entryIds[3]; // winmgr entry ids
/* 0x34 */ s32 pausewinId; // id for pausewin entry drawing background box
/* 0x38 */ WinmgrItem * items;
/* 0x3C */ s32 itemCount;
/* 0x40 */ s32 extraItem;
/* 0x44 */ u8 unknown_0x44[0x6c - 0x44];
} WinmgrSelect;
SIZE_ASSERT(WinmgrSelect, 0x6c)

typedef struct _WinmgrEntry{
    /*
        8 is scheduled for deletion
        1 is active
        Others unknown
    */
/* 0x00 */ u32 flags;
/* 0x04 */ u8 unknown_0x4[0x10 - 0x4];
/* 0x10 */ f32 scalarScale;
/* 0x14 */ u8 unknown_0x14[0x18 - 0x14];
/* 0x18 */ Vec2i pos;
/* 0x20 */ Vec2i scale;
/* 0x28 */ WindowDesc * desc;
/* 0x2C */ void * param; // for select windows, pointer to WinmgrSelect
/* 0x30 */ s32 priority;
} WinmgrEntry;
SIZE_ASSERT(WinmgrEntry, 0x34)

typedef struct
{
/* 0x0 */ s32 entryCount; // 0x20
/* 0x4 */ WinmgrEntry * entries; // array of entryCount
} WinmgrWork;
SIZE_ASSERT(WinmgrWork, 0x8)

DECOMP_STATIC(WinmgrWork winmgr_work)
DECOMP_STATIC(WinmgrWork * winmgr_wp)

/*
    Allocates entries
*/
void winMgrInit();

/*
    Clears entries
*/
void winmgrReInit();

/*
    Displays a window & calls its disp func
*/
void winMgrDisp(s32 cameraId, WinmgrEntry * entry);

/*
    Calls winMgrSeq on each entry & its main func, and schedules winMgrDisp for each func
*/
void winMgrMain();

void winMgrSeq(WinmgrEntry * entry);

/*
    Creates an entry from a desc and returns its id
*/
s32 winMgrEntry(WindowDesc * desc);

/*
    Sets user data parameter for a window
*/
void winMgrSetParam(void * param);

/*
    Opens a window
*/
void winMgrOpen(s32 id);

/*
    Closes a window and schedules it to be deleted
*/
void winMgrCloseAutoDelete(s32 id);

/*
    Checks the action flag of a window
*/
void winMgrAction(s32 id);

/*
    Sets the position and scale of a window
*/
void winMgrSetSize(s32 id, s32 posX, s32 posY, s32 scaleX, s32 scaleY);

/*
    Sets the priority of a window
*/
void winMgrSetPriority(s32 id, s32 priority);

/*
    Creates an item selection window

    Type values: 
        0 is keyItem selection
        1 is useItem selection
        2 is useItem sale
        3 is useItem store
        4 is shopItem pickup
        5 is card selection
        6 is card sale
        7 is recipes selection 
    Type can also be a pointer to a list of s32 item ids terminated by -1 (uses type 0's properties)

    Extra item is added at the start of the list
*/
WinmgrSelect * winMgrSelectEntry(s32 type, s32 extraItem, u32 flags);

/*
    Get the resulting item id of a selection window
    -1 means the user backed out
    0 means undecided
*/
s32 winMgrSelect(WinmgrSelect * select);


/*
    Deletes a selection window
*/
void winMgrSelectDelete(WinmgrSelect * select);

/*
    Returns the index of the item currently selected
*/
s32 winMgrSelectGetIndex(WinmgrSelect * select);

/*
    WindowDesc.mainFunc/dispFuncs for a WinmgrSelect window
    Update & draw the main window
*/
void select_main(WinmgrEntry * entry);
void select_disp(WinmgrEntry * entry);

/*
    Displays the instruction to choose one
*/
void select_disp2(WinmgrEntry * entry);

/*
    Unused function to display "msg_window_item_cancel"
    This message is defined twice, as either a go back with 1 or 
    select with 2 instruction
    Seems to have originally been intended for recipe selection
*/
void select_disp3(WinmgrEntry * entry);

CPP_WRAPPER_END()
