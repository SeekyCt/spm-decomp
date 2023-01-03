/*
    item_event_data defines what happens when certain items are used
*/

#include <common.h>
#include <evt_cmd.h>
#include <spm/evtmgr.h>

CPP_WRAPPER(spm::item_event_data)

USING(spm::evtmgr::EvtScriptCode)

#define ITEM_EVENT_DATA_COUNT 33

typedef struct
{
/* 0x0 */ s32 itemId;
/* 0x4 */ EvtScriptCode * useScript;
/* 0x8 */ const char * useMsgName;
} ItemEventData;
SIZE_ASSERT(ItemEventData, 0xc)

extern ItemEventData itemEventDataTable[ITEM_EVENT_DATA_COUNT];

typedef struct
{
/* 0x0 */ void * wp;
/* 0x4 */ u8 unknown_0x4[0x20 - 0x4];
} ItemEventWork;
SIZE_ASSERT(ItemEventWork, 0x20)

DECOMP_STATIC(ItemEventWork item_event_data_work)
DECOMP_STATIC(ItemEventWork * item_event_data_wp)

/*
    Returns the script for an item in the table by id, or a fallback if the item isn't in there
*/
EvtScriptCode * getItemUseEvt(s32 itemId);

/*
    Returns the name of the use message for an item in the table, or nulll if the item isn't in there
*/
const char * getItemUseMsg(s32 itemId);

UNKNOWN_FUNCTION(func_80025250);

/*
    Frees wp->wp and sets it to null
*/
// evt_item_event_free_work()
EVT_DECLARE_USER_FUNC(evt_item_event_free_work, 0)

/*
    Sets wp->wp to null
*/
void itemEventDataExit();

UNKNOWN_FUNCTION(func_8002574c);
UNKNOWN_FUNCTION(func_80025ac4);
UNKNOWN_FUNCTION(func_80025b20);
UNKNOWN_FUNCTION(func_80025b70);
UNKNOWN_FUNCTION(evt_item_event_apply_statuses);
UNKNOWN_FUNCTION(func_80025c50);
UNKNOWN_FUNCTION(func_80025c84);
UNKNOWN_FUNCTION(func_80025d9c);
UNKNOWN_FUNCTION(func_80025e80);
UNKNOWN_FUNCTION(func_80026364);
UNKNOWN_FUNCTION(func_800264d4);
UNKNOWN_FUNCTION(func_8002650c);
UNKNOWN_FUNCTION(func_80026550);
UNKNOWN_FUNCTION(func_80026584);
UNKNOWN_FUNCTION(func_800265f8);
UNKNOWN_FUNCTION(func_8002691c);
UNKNOWN_FUNCTION(func_80026a70);
UNKNOWN_FUNCTION(func_80026c1c);
UNKNOWN_FUNCTION(func_80027020);
UNKNOWN_FUNCTION(func_80027410);
UNKNOWN_FUNCTION(func_80027484);
UNKNOWN_FUNCTION(func_800274b4);
UNKNOWN_FUNCTION(func_800274e4);
UNKNOWN_FUNCTION(func_800276e8);
UNKNOWN_FUNCTION(func_800277fc);
UNKNOWN_FUNCTION(func_8002792c);
UNKNOWN_FUNCTION(func_80027b74);
UNKNOWN_FUNCTION(func_80027c14);
UNKNOWN_FUNCTION(func_80027cd4);
UNKNOWN_FUNCTION(func_80027d38);
UNKNOWN_FUNCTION(func_80027db4);
UNKNOWN_FUNCTION(func_80027ed8);
UNKNOWN_FUNCTION(func_80027f54);
UNKNOWN_FUNCTION(func_80028040);
UNKNOWN_FUNCTION(func_800283bc);
UNKNOWN_FUNCTION(func_80028418);
UNKNOWN_FUNCTION(func_80028460);
UNKNOWN_FUNCTION(func_800284d8);
UNKNOWN_FUNCTION(func_80028550);
UNKNOWN_FUNCTION(func_800285bc);
UNKNOWN_FUNCTION(func_80028604);
UNKNOWN_FUNCTION(func_800286a0);
UNKNOWN_FUNCTION(func_80028724);
UNKNOWN_FUNCTION(func_80028774);
UNKNOWN_FUNCTION(func_80028828);
UNKNOWN_FUNCTION(func_80028bc0);
UNKNOWN_FUNCTION(func_80028f8c);
UNKNOWN_FUNCTION(func_8002904c);
UNKNOWN_FUNCTION(func_8002912c);
UNKNOWN_FUNCTION(evt_item_event_get_item_hp_gain);
UNKNOWN_FUNCTION(func_800291f0);
UNKNOWN_FUNCTION(func_800292f8);
UNKNOWN_FUNCTION(func_80029374);
UNKNOWN_FUNCTION(func_800293a8);

/*
    Gives the map & door name that the return pipe should go to
*/
// evt_itemdata_get_return_pipe_info(char * &mapNameOut, char * &doorNameOut)
EVT_DECLARE_USER_FUNC(evt_itemdata_get_return_pipe_info, 2)

CPP_WRAPPER_END()
