#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/filemgr.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::pausewin)

USING(wii::mtx::Vec2)
USING(spm::evtmgr::EvtEntry)
USING(spm::filemgr::FileEntry)

#define PAUSE_FLAG_HIGHLIGHT 0x10
#define PAUSE_FLAG_OSCILLATE 0x100

enum PauseTextureId
{
    // Special
/* 0x00 */ PAUSETEX_NONE = 0, // use with boxOnly

    // Actual textures
/* 0x00 */ PAUSETEX_CHARACTERS_BTN = 0,
/* 0x01 */ PAUSETEX_PIXLS_BTN,
/* 0x02 */ PAUSETEX_ITEMS_BTN,
/* 0x03 */ PAUSETEX_KEY_ITEMS_BTN,
/* 0x04 */ PAUSETEX_CHAPTERS_BTN,
/* 0x05 */ PAUSETEX_CARDS_BTN,
/* 0x06 */ PAUSETEX_RECIPES_BTN,
/* 0x07 */ PAUSETEX_MAPS_BTN,
/* 0x08 */ PAUSETEX_CHAPTER_1_BTN,
/* 0x09 */ PAUSETEX_CHAPTER_2_BTN,
/* 0x0A */ PAUSETEX_CHAPTER_3_BTN,
/* 0x0B */ PAUSETEX_CHAPTER_4_BTN,
/* 0x0C */ PAUSETEX_CHAPTER_5_BTN,
/* 0x0D */ PAUSETEX_CHAPTER_6_BTN,
/* 0x0E */ PAUSETEX_CHAPTER_7_BTN,
/* 0x0F */ PAUSETEX_CHAPTER_8_BTN
    // ...
};

enum PlusWinButtonIdx
{
/* 0x0 */ PLUSWIN_BTN_CHARS,
/* 0x1 */ PLUSWIN_BTN_PIXLS,
/* 0x2 */ PLUSWIN_BTN_ITEMS,
/* 0x3 */ PLUSWIN_BTN_KEY_ITEMS,
/* 0x4 */ PLUSWIN_BTN_CHAPTERS,
/* 0x5 */ PLUSWIN_BTN_CARDS,
/* 0x6 */ PLUSWIN_BTN_RECIPES,
/* 0x7 */ PLUSWIN_BTN_MAPS,
/* 0x8 */ PLUSWIN_BTN_BG,
/* 0x9 */ PLUSWIN_BTN_HELP,
/* 0xA */ PLUSWIN_BTN_STATS
};

enum PluswinState
{
/* 0x00 */ PLUSWIN_STATE_MAIN_MENU = 0,
/* 0x0A */ PLUSWIN_STATE_KEY_ITEMS = 10,
/* 0x14 */ PLUSWIN_STATE_CHAPTERS = 20,
/* 0x1E */ PLUSWIN_STATE_CARDS = 30,
/* 0x28 */ PLUSWIN_STATE_RECIPES = 40,
/* 0x32 */ PLUSWIN_STATE_MAPS = 50
};

struct _PausewinEntry;

typedef void (PausewinFunc)(struct _PausewinEntry *);

typedef struct _PausewinEntry
{
/* 0x00 */ u16 flags;
/* 0x02 */ u16 id;
/* 0x04 */ Vec2 pos;
/* 0x0C */ f32 width;
/* 0x10 */ f32 height;
/* 0x14 */ Vec2 moveStart;
/* 0x1C */ Vec2 moveDest;
/* 0x24 */ Vec2 originalPos;
/* 0x2C */ f32 scale;
/* 0x30 */ u8 unknown_0x30[0x34 - 0x30];
/* 0x34 */ s32 alpha;
/* 0x38 */ s32 stateProgress;
/* 0x3C */ s32 state;
/* 0x40 */ s32 boxOnly;
/* 0x44 */ s32 textureId;
/* 0x48 */ s32 priority;
/* 0x4C */ s32 curMsgLine;
/* 0x50 */ s32 msgWidth;
/* 0x54 */ const char * msg;
/* 0x58 */ u8 unknown_0x58[0x64 - 0x58];
/* 0x5C */ PausewinFunc * initFunc;
/* 0x60 */ PausewinFunc * mainFunc;
/* 0x64 */ PausewinFunc * dispFunc;
/* 0x68 */ PausewinFunc * lateDispFunc;
/* 0x6C */ PausewinFunc * deleteFunc;
} PausewinEntry;
SIZE_ASSERT(PausewinEntry, 0x78)

typedef struct 
{
/* 0x00 */ u32 num;
/* 0x04 */ PausewinEntry * entries;
/* 0x08 */ FileEntry * pauseTpl;
/* 0x0C */ s32 entriesUsingPauseTpl;
/* 0x10 */ FileEntry * pause2Tpl;
/* 0x14 */ s32 entriesUsingPause2Tpl;
/* 0x18 */ s32 * entryIdQueue;
/* 0x1C */ s32 curMenuId;
/* 0x20 */ EvtEntry * itemUseEvt;
/* 0x24 */ s32 itemUseEvtId;
/* 0x28 */ u8 unknown_0x28[0x2c - 0x28];
/* 0x2C */ s32 useItemId;
/* 0x30 */ s32 subMenuOpenButtonIdx;
/* 0x34 */ s32 currentPixl;
/* 0x38 */ bool charPixlItemWinOpen;
/* 0x39 */ u8 unknown_0x39[0x3c - 0x39];
/* 0x3C */ f32 gameSpeedBackup;
} PausewinWork;
SIZE_ASSERT(PausewinWork, 0x40)

typedef struct
{
/* 0x000 */ s16 cursorOff;
/* 0x002 */ u8 unknown_0x2[0x4 - 0x2];
/* 0x004 */ s32 entryIds[11];
/* 0x030 */ u8 unknown_0x30[0x84 - 0x30];
/* 0x084 */ s32 state;
/* 0x088 */ s32 selectedButton;
/* 0x08C */ s32 prevSelectedButton;
/* 0x090 */ Vec2 cursorMoveDest;
/* 0x098 */ Vec2 cursorPos;
/* 0x0A0 */ s8 submenuEntryIds[10];
/* 0x0AA */ u8 unknown_0xaa[0x164 - 0xaa];
/* 0x164 */ s32 submenuSelectedButton;
/* 0x168 */ s32 chapterLevel;
/* 0x16C */ u8 unknown_0x16c[0x184 - 0x16c];
/* 0x184 */ EvtEntry * keyItemEvt;
/* 0x188 */ s32 keyItemEvtId;
/* 0x18C */ u8 unknown_0x18c[0x1a4 - 0x18c];
/* 0x1A4 */ s32 selectedItemId;
/* 0x1A8 */ u32 submenuFlags;
/* 0x1AC */ bool submenuIsFullClose;
/* 0x1AD */ u8 unknown_0x1ad[0x1b0 - 0x1ad];
/* 0x1B0 */ s64 openingTime;
} PluswinWork;
SIZE_ASSERT(PluswinWork, 0x1b8)

DECOMP_STATIC(PluswinWork * pausewin_pluswinWp)
DECOMP_STATIC(PausewinWork * pausewin_wp)

UNKNOWN_FUNCTION(pausewinGetEntry);
UNKNOWN_FUNCTION(pausewinGetTpl);
void pausewinInit();
UNKNOWN_FUNCTION(pausewinReInit);
UNKNOWN_FUNCTION(pausewinGetOpenButtonMask);
UNKNOWN_FUNCTION(pausewinCheckMoveUp);
UNKNOWN_FUNCTION(pausewinCheckMoveDown);
UNKNOWN_FUNCTION(pausewinCheckMoveLeft);
UNKNOWN_FUNCTION(pausewinCheckMoveRight);
UNKNOWN_FUNCTION(pausewinCheckPressOk);
UNKNOWN_FUNCTION(pausewinCheckPressBack);
void pausewinMain();
UNKNOWN_FUNCTION(pausewinDisp);
UNKNOWN_FUNCTION(pausewinEntryDisp);
s32 pausewinEntry(f32 x, f32 y, f32 width, f32 height, bool boxOnly, s32 textureId,
                  s32 priority, PausewinFunc * initFunc, PausewinFunc * mainFunc,
                  PausewinFunc * dispFunc, PausewinFunc * lateDispFunc, PausewinFunc * deleteFunc);
void pausewinMoveTo(s32 id, f32 x, f32 y);
void pausewinAppear(s32 id);
void pausewinDisappear(s32 id);
void pausewinDelete(s32 id);
UNKNOWN_FUNCTION(pausewinGoNeutral);
UNKNOWN_FUNCTION(pausewinHide);
UNKNOWN_FUNCTION(pausewinCheckVisible);
UNKNOWN_FUNCTION(_pausewinDelete);
void pausewinPauseGame();
void pausewinUnpauseGame();
UNKNOWN_FUNCTION(pausewinUpdate);
UNKNOWN_FUNCTION(pausewinNumberToString);
UNKNOWN_FUNCTION(pausewinCheckHavePixls);
UNKNOWN_FUNCTION(pausewinCheckHaveCards);
UNKNOWN_FUNCTION(pausewinCheckHaveRecipes);
UNKNOWN_FUNCTION(pausewinCheckHaveMaps);
UNKNOWN_FUNCTION(pluswinStubButtonCheck);
UNKNOWN_FUNCTION(pluswinOpen);
UNKNOWN_FUNCTION(pluswinClose);
UNKNOWN_FUNCTION(pluswinCloseFromSubmenu);
UNKNOWN_FUNCTION(pluswinMain);
UNKNOWN_FUNCTION(pluswinCursorDisp);
UNKNOWN_FUNCTION(pluswinStatsDisp);
UNKNOWN_FUNCTION(pluswinBgDispSub);
UNKNOWN_FUNCTION(pluswinBgDisp);
UNKNOWN_FUNCTION(pausewinMsgBoxInit);
UNKNOWN_FUNCTION(pausewinMsgBoxMain);
UNKNOWN_FUNCTION(pausewinMsgBoxDisp);
UNKNOWN_FUNCTION(pausewinMsgBoxDelete);
void pausewinSetMessage(PausewinEntry * entry, s32 itemId, const char * msgName);
UNKNOWN_FUNCTION(pausewinSetMessageCard);
UNKNOWN_FUNCTION(pluswinKeyItemWinOpen);
UNKNOWN_FUNCTION(pluswinKeyItemMain);
UNKNOWN_FUNCTION(pluswinKeyItemDisp);
UNKNOWN_FUNCTION(evt_pluswin_select_return_pipe);
UNKNOWN_FUNCTION(evt_pluswin_get_return_pipe_msg);
UNKNOWN_FUNCTION(pluswinCheckChapterUnlocked);
void pluswinChapterWinOpen();
UNKNOWN_FUNCTION(pluswinCloseChapters);
UNKNOWN_FUNCTION(pluswinIsChapter1Unlocked);
UNKNOWN_FUNCTION(pluswinIsChapter2Unlocked);
UNKNOWN_FUNCTION(pluswinIsChapter3Unlocked);
UNKNOWN_FUNCTION(pluswinIsChapter4Unlocked);
UNKNOWN_FUNCTION(pluswinIsChapter5Unlocked);
UNKNOWN_FUNCTION(pluswinIsChapter6Unlocked);
UNKNOWN_FUNCTION(pluswinIsChapter7Unlocked);
UNKNOWN_FUNCTION(pluswinIsChapter8Unlocked);
UNKNOWN_FUNCTION(pluswinChapterCtrlMain);
UNKNOWN_FUNCTION(pluswinChapterCtrlDisp);
UNKNOWN_FUNCTION(pluswinChapterLevelDisp);
UNKNOWN_FUNCTION(pluswinChapterLevelPreviewDisp);
UNKNOWN_FUNCTION(pluswinChapterButtonDisp);
UNKNOWN_FUNCTION(pluswinChapterTitleDisp);
UNKNOWN_FUNCTION(func_8018c7dc);
UNKNOWN_FUNCTION(func_8018c85c);
UNKNOWN_FUNCTION(func_8018c8dc);
UNKNOWN_FUNCTION(func_8018c95c);
UNKNOWN_FUNCTION(pluswinCardsWinOpen);
UNKNOWN_FUNCTION(func_8018cf18);
UNKNOWN_FUNCTION(func_8018d1ac);
UNKNOWN_FUNCTION(pluswinCardBgMain);
UNKNOWN_FUNCTION(pluswinCardBgDisp);
UNKNOWN_FUNCTION(func_8018e9cc);
UNKNOWN_FUNCTION(func_8018ea4c);
UNKNOWN_FUNCTION(pluswinCardCtrlDisp);
UNKNOWN_FUNCTION(func_8018f200);
UNKNOWN_FUNCTION(pluswinRecipesWinOpen);
UNKNOWN_FUNCTION(func_801901b0);
UNKNOWN_FUNCTION(func_80190444);
UNKNOWN_FUNCTION(func_80190eb8);
UNKNOWN_FUNCTION(func_80191128);
UNKNOWN_FUNCTION(func_80191350);
UNKNOWN_FUNCTION(pluswinMapsWinOpen);
UNKNOWN_FUNCTION(func_80191ac0);
UNKNOWN_FUNCTION(func_80191d54);
UNKNOWN_FUNCTION(func_80192a98);
UNKNOWN_FUNCTION(func_80192d78);
UNKNOWN_FUNCTION(func_8019345c);
UNKNOWN_FUNCTION(func_80193498);
UNKNOWN_FUNCTION(func_801936b8);
UNKNOWN_FUNCTION(func_80193860);
UNKNOWN_FUNCTION(func_80193874);
UNKNOWN_FUNCTION(func_80193888);
UNKNOWN_FUNCTION(func_80193a04);
UNKNOWN_FUNCTION(minuswinOpen);
UNKNOWN_FUNCTION(minuswinDisp);
UNKNOWN_FUNCTION(minuswinMain);
UNKNOWN_FUNCTION(func_801950f8);

CPP_WRAPPER_END()
