#pragma once

#include <common.h>
#include <spm/pausewin.h>

CPP_WRAPPER(spm::charpixlitemwin)

USING(spm::pausewin::PausewinEntry)

#define CHAR_PIXL_ITEM_WIN_CHAR_ENTRY_CAPACITY 5
#define CHAR_PIXL_ITEM_WIN_PIXL_ENTRY_CAPACITY 17
#define CHAR_PIXL_ITEM_WIN_ITEM_ENTRY_CAPACITY 11
#define CHAR_PIXL_ITEM_WIN_ENTRY_MAX 8

enum CharPixlItemWinEntryId
{
/* 0x0 */ CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON,
/* 0x1 */ CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON,
/* 0x2 */ CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON,
/* 0x3 */ CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_TAB,
/* 0x4 */ CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_TAB,
/* 0x5 */ CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_TAB,
/* 0x6 */ CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION,
/* 0x7 */ CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION
};

enum CharPixlItemWinResultFlag
{
/* 0x0800 */ CHAR_PIXL_ITEM_WIN_RESULT_RETURN_TO_PLUS = 0x0800,
/* 0x1000 */ CHAR_PIXL_ITEM_WIN_RESULT_CHARACTER = 0x1000,
/* 0x2000 */ CHAR_PIXL_ITEM_WIN_RESULT_PIXL = 0x2000,
/* 0x4000 */ CHAR_PIXL_ITEM_WIN_RESULT_ITEM = 0x4000,
/* 0x8000 */ CHAR_PIXL_ITEM_WIN_RESULT_CANCEL = 0x8000,
/* 0xF800 */ CHAR_PIXL_ITEM_WIN_RESULT_MASK = 0xf800
};

typedef struct
{
/* 0x00 */ bool selectable;
/* 0x04 */ s32 itemType;
/* 0x08 */ void * data;
} CharPixlItemWinTabEntry;
SIZE_ASSERT(CharPixlItemWinTabEntry, 0xc)

typedef struct
{
/* 0x00 */ CharPixlItemWinTabEntry * entries;
/* 0x04 */ s32 entryCount;
/* 0x08 */ s32 selectedIndex;
/* 0x0C */ s32 scrollOffset;
/* 0x10 */ f32 scrollPos;
/* 0x14 */ f32 cursorX;
/* 0x18 */ f32 cursorY;
/* 0x1C */ PausewinEntry * parent;
} CharPixlItemWinTab;
SIZE_ASSERT(CharPixlItemWinTab, 0x20)

typedef struct
{
/* 0x00 */ u16 flags;
/* 0x02 */ u8 unknown_0x2[0x4 - 0x2];
/* 0x04 */ s32 entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_MAX];
/* 0x24 */ u8 unknown_0x24[0x44 - 0x24];
/* 0x44 */ CharPixlItemWinTab characterTab;
/* 0x64 */ CharPixlItemWinTab pixlTab;
/* 0x84 */ CharPixlItemWinTab itemTab;
/* 0xA4 */ s32 pendingDeleteTabs;
} CharPixlItemWinWork;
SIZE_ASSERT(CharPixlItemWinWork, 0xa8)

void charPixlItemWinInit();
void charPixlItemWinReInit();
void charPixlItemWinOpen(s32 initialTab, bool animate);
s32 func_80195e64(bool immediate);
bool charPixlItemWinIsClosed();
s32 charPixlItemWinGetNextWindow();
void func_80196004(s32 * selectedIndexOut, s32 * itemTypeOut);
void initCharTab(PausewinEntry * parent);
void func_80196174(PausewinEntry * entry);
void func_801969dc(PausewinEntry * entry);
void dispCharTab(PausewinEntry * entry);
void deleteCharTab(PausewinEntry * entry);
void initPixlTab(PausewinEntry * parent);
void func_80196c8c(PausewinEntry * entry);
void func_80197544(PausewinEntry * entry);
void dispPixlTab(PausewinEntry * entry);
void deletePixlTab(PausewinEntry * entry);
void initItemTab(PausewinEntry * parent);
void func_801976e8(PausewinEntry * entry);
void func_80198238(PausewinEntry * entry);
void dispItemTab(PausewinEntry * entry);
void deleteItemTab(PausewinEntry * entry);
void func_80198290(PausewinEntry * entry);
void func_80198294(PausewinEntry * entry);
void func_801982ac(PausewinEntry * entry);
void func_801982b0(PausewinEntry * entry);
void func_801982b4(PausewinEntry * entry);
void func_801982c0(PausewinEntry * entry);
void func_801982c4(PausewinEntry * entry);
void func_801984f0(PausewinEntry * entry);
void func_801984f4(PausewinEntry * entry, CharPixlItemWinTab * tab);
void func_80198874(PausewinEntry * entry, CharPixlItemWinTab * tab);
void dispTab(PausewinEntry * entry, CharPixlItemWinTab * tab, s32 type);

CPP_WRAPPER_END()
