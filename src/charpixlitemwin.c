#include <common.h>
#include <evt_cmd.h>
#include <msl/string.h>
#include <spm/charpixlitemwin.h>
#include <spm/evt_door.h>
#include <spm/evtmgr_cmd.h>
#include <spm/fontmgr.h>
#include <spm/gxsub.h>
#include <spm/icondrv.h>
#include <spm/item_data.h>
#include <spm/item_event_data.h>
#include <spm/mario.h>
#include <spm/mario_motion.h>
#include <spm/mario_pouch.h>
#include <spm/memory.h>
#include <spm/msgdrv.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/wpadmgr.h>
#include <wii/gx.h>
#include <wii/mtx.h>

extern "C" {

// .bss
static CharPixlItemWinWork work;

extern const f32 lbl_80337370[20];

// .sdata
static CharPixlItemWinWork * wp = &work;

extern PausewinWork * pausewin_wp;

void func_801950f8(f32 value)
{
    pausewin_wp->gameSpeedBackup = value;
}

void charPixlItemWinInit()
{
    memset(wp, 0, sizeof(*wp));
}

void charPixlItemWinReInit()
{
    memset(wp, 0, sizeof(*wp));
}

void charPixlItemWinOpen(s32 initialTab, bool animate)
{
    f32 buttonWidth;
    f32 height;
    s32 initialEntry;

    memset(wp, 0, sizeof(*wp));
    buttonWidth = 148.0f;

    if (!animate)
    {
        wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON] =
            pausewinEntry(-buttonWidth / 2.0f - 180.0f + 5.0f, 160.0f, 0.0f, 0.0f, false,
                          PAUSETEX_CHARACTERS_BTN, 10, NULL, NULL, NULL, NULL, NULL);

        if (pouchCountPixls() > 0)
        {
            wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON] =
                pausewinEntry(-buttonWidth / 2.0f + 5.0f, 160.0f, 0.0f, 0.0f, false,
                              PAUSETEX_PIXLS_BTN, 10, NULL, NULL, NULL, NULL, NULL);
        }
        else
        {
            wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON] = -1;
        }

        wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON] =
            pausewinEntry(-buttonWidth / 2.0f + 180.0f + 5.0f, 160.0f, 0.0f, 0.0f, false,
                          PAUSETEX_ITEMS_BTN, 10, NULL, NULL, NULL, NULL, NULL);

        pausewinAppear(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON]);
        pausewinAppear(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON]);
        pausewinAppear(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON]);
    }
    else
    {
        wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON] =
            pausewinEntry(-buttonWidth / 2.0f - 180.0f + 5.0f, 190.0f, 0.0f, 0.0f, false,
                          PAUSETEX_CHARACTERS_BTN, 10, NULL, NULL, NULL, NULL, NULL);

        if (pouchCountPixls() > 0)
        {
            wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON] =
                pausewinEntry(-buttonWidth / 2.0f + 5.0f, 190.0f, 0.0f, 0.0f, false,
                              PAUSETEX_PIXLS_BTN, 10, NULL, NULL, NULL, NULL, NULL);
        }
        else
        {
            wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON] = -1;
        }

        wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON] =
            pausewinEntry(-buttonWidth / 2.0f + 180.0f + 5.0f, 190.0f, 0.0f, 0.0f, false,
                          PAUSETEX_ITEMS_BTN, 10, NULL, NULL, NULL, NULL, NULL);

        pausewinGoNeutral(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON]);
        pausewinGoNeutral(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON]);
        pausewinGoNeutral(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON]);

        pausewinMoveTo(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON],
                       -buttonWidth / 2.0f - 180.0f + 5.0f, 160.0f);
        pausewinMoveTo(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON],
                       -buttonWidth / 2.0f + 5.0f, 160.0f);
        pausewinMoveTo(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON],
                       -buttonWidth / 2.0f + 180.0f + 5.0f, 160.0f);
    }

    spsndSFXOn("SFX_SYS_MENU_OPEN2");

    height = 50.0f + 25.0f * pouchCountChars();
    if (height > 250.0f || height < 0.0f)
        height = 250.0f;
    wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_TAB] = pausewinEntry(
        -buttonWidth / 2.0f - 180.0f + buttonWidth / 2.0f - 90.0f, 140.0f, 180.0f, height, true,
        PAUSETEX_NONE, 5, initCharTab, func_80196174, func_801969dc, dispCharTab, deleteCharTab);

    height = 50.0f + 25.0f * pouchCountPixls();
    if (height > 250.0f || height < 0.0f)
        height = 250.0f;
    wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_TAB] = pausewinEntry(
        -buttonWidth / 2.0f + buttonWidth / 2.0f - 90.0f, 140.0f, 180.0f, height, true,
        PAUSETEX_NONE, 5, initPixlTab, func_80196c8c, func_80197544, dispPixlTab, deletePixlTab);

    height = 50.0f + 25.0f * pouchCountUseItems();
    if (height > 250.0f || height < 0.0f)
        height = 250.0f;
    wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_TAB] = pausewinEntry(
        -buttonWidth / 2.0f + 180.0f + buttonWidth / 2.0f - 90.0f, 140.0f, 180.0f, height, true,
        PAUSETEX_NONE, 5, initItemTab, func_801976e8, func_80198238, dispItemTab, deleteItemTab);

    initialEntry = initialTab + CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_TAB;
    pausewinAppear(wp->entryIds[initialEntry]);

    wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION] =
        pausewinEntry(-262.0f, -120.0f, 524.0f, 80.0f, true, PAUSETEX_NONE, 0, func_80198290,
                      func_80198294, func_801982ac, NULL, func_801982b0);

    wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION] =
        pausewinEntry(-270.0f, 110.0f, 340.0f, 60.0f, true, PAUSETEX_NONE, 0, func_801982b4,
                      func_801982c0, func_801982c4, NULL, func_801984f0);

    pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION])->flags |= 8;

    switch (initialTab)
    {
        case 0:
        {
            CharPixlItemWinWork * tabWork = wp;
            PausewinEntry * entry;
            s32 useItemType;
            u16 windowId;
            const char * useMsgName;
            s32 itemType;
            u16 outlines;

            tabWork->characterTab.scrollPos = 25.0f * tabWork->characterTab.scrollOffset;
            tabWork->characterTab.cursorX = tabWork->characterTab.parent->pos.x;
            tabWork->characterTab.cursorY =
                tabWork->characterTab.parent->pos.y - 10.0f - 20.0f - 24.0f -
                25.0f * (tabWork->characterTab.selectedIndex - tabWork->characterTab.scrollOffset);

            itemType = tabWork->characterTab.entries[tabWork->characterTab.selectedIndex].itemType;

            entry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION]);
            windowId = entry->id;
            if (itemType == ITEM_ID_NULL)
            {
                if (pausewinCheckVisible(windowId))
                    pausewinDisappear(windowId);
            }
            else if (!pausewinCheckVisible(windowId))
            {
                pausewinAppear(windowId);
            }

            if (itemType == ITEM_ID_CHAR_MARIO && evtGetValue(NULL, GSW(0)) < 16)
                pausewinSetMessage(entry, ITEM_ID_NULL, "msg_pc_mario_0");
            else
                pausewinSetMessage(entry, itemType, NULL);

            useItemType =
                tabWork->characterTab.entries[tabWork->characterTab.selectedIndex].itemType;
            entry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
            useMsgName = getItemUseMsg(useItemType);
            if (useMsgName != NULL)
            {
                entry->msg = msgSearch(useMsgName);
                FontGetMessageWidthLine(entry->msg, &outlines);
                entry->height = (outlines + 2) * 25 + 20;
                if (!pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))
                {
                    pausewinGoNeutral(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
                }
            }
            else
            {
                entry->msg = NULL;
                if (pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))
                {
                    pausewinHide(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
                }
            }

            pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON])->flags &= ~0x40;
            pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON])->flags |= 0x40;
            pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON])->flags |= 0x40;
            break;
        }

        case 1:
        {
            CharPixlItemWinWork * tabWork = wp;
            PausewinEntry * entry;
            u16 windowId;
            const char * useMsgName;
            s32 itemType;
            s32 useItemType;
            u16 outlines;

            tabWork->pixlTab.scrollPos = 25.0f * tabWork->pixlTab.scrollOffset;
            tabWork->pixlTab.cursorX = tabWork->pixlTab.parent->pos.x;
            tabWork->pixlTab.cursorY =
                tabWork->pixlTab.parent->pos.y - 10.0f - 20.0f - 24.0f -
                25.0f * (tabWork->pixlTab.selectedIndex - tabWork->pixlTab.scrollOffset);

            itemType = tabWork->pixlTab.entries[tabWork->pixlTab.selectedIndex].itemType;

            entry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION]);
            windowId = entry->id;
            if (itemType == ITEM_ID_NULL)
            {
                if (pausewinCheckVisible(windowId))
                    pausewinDisappear(windowId);
            }
            else if (!pausewinCheckVisible(windowId))
            {
                pausewinAppear(windowId);
            }

            if (itemType == ITEM_ID_CHAR_MARIO && evtGetValue(NULL, GSW(0)) < 16)
                pausewinSetMessage(entry, ITEM_ID_NULL, "msg_pc_mario_0");
            else
                pausewinSetMessage(entry, itemType, NULL);

            useItemType = tabWork->pixlTab.entries[tabWork->pixlTab.selectedIndex].itemType;
            entry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
            useMsgName = getItemUseMsg(useItemType);
            if (useMsgName != NULL)
            {
                entry->msg = msgSearch(useMsgName);
                FontGetMessageWidthLine(entry->msg, &outlines);
                entry->height = (outlines + 2) * 25 + 20;
                if (!pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))
                {
                    pausewinGoNeutral(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
                }
            }
            else
            {
                entry->msg = NULL;
                if (pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))
                {
                    pausewinHide(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
                }
            }

            pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON])->flags |= 0x40;
            pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON])->flags &= ~0x40;
            pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON])->flags |= 0x40;
            break;
        }

        case 2:
        {
            CharPixlItemWinWork * tabWork = wp;
            PausewinEntry * entry;
            u16 windowId;
            const char * useMsgName;
            s32 itemType;
            s32 useItemType;
            u16 outlines;

            tabWork->itemTab.scrollPos = 25.0f * tabWork->itemTab.scrollOffset;
            tabWork->itemTab.cursorX = tabWork->itemTab.parent->pos.x;
            tabWork->itemTab.cursorY =
                tabWork->itemTab.parent->pos.y - 10.0f - 20.0f - 24.0f -
                25.0f * (tabWork->itemTab.selectedIndex - tabWork->itemTab.scrollOffset);

            itemType = tabWork->itemTab.entries[tabWork->itemTab.selectedIndex].itemType;

            entry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION]);
            windowId = entry->id;
            if (itemType == ITEM_ID_NULL)
            {
                if (pausewinCheckVisible(windowId))
                    pausewinDisappear(windowId);
            }
            else if (!pausewinCheckVisible(windowId))
            {
                pausewinAppear(windowId);
            }

            if (itemType == ITEM_ID_CHAR_MARIO && evtGetValue(NULL, GSW(0)) < 16)
                pausewinSetMessage(entry, ITEM_ID_NULL, "msg_pc_mario_0");
            else
                pausewinSetMessage(entry, itemType, NULL);

            useItemType = tabWork->itemTab.entries[tabWork->itemTab.selectedIndex].itemType;
            entry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
            useMsgName = getItemUseMsg(useItemType);
            if (useMsgName != NULL)
            {
                entry->msg = msgSearch(useMsgName);
                FontGetMessageWidthLine(entry->msg, &outlines);
                entry->height = (outlines + 2) * 25 + 20;
                if (!pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))
                {
                    pausewinGoNeutral(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
                }
            }
            else
            {
                entry->msg = NULL;
                if (pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))
                {
                    pausewinHide(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
                }
            }

            pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON])->flags |= 0x40;
            pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON])->flags |= 0x40;
            pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON])->flags &= ~0x40;
            break;
        }
    }

    if (pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION])->msg != NULL)
    {
        pausewinAppear(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION]);
    }

    if (pouchCountChars() == 0 || func_801472b4() == 0)
    {
        pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON])->flags |= 4;
    }
    if (pouchCountPixls() == 0 || func_801471e8() == 0)
    {
        pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON])->flags |= 4;
    }
}

s32 func_80195e64(bool immediate)
{
    bool playCloseSound = false;
    s32 hiddenTabs;
    s32 i;

    if (wp->flags & 0x8800)
        playCloseSound = true;

    for (hiddenTabs = 0; hiddenTabs < 3; hiddenTabs++)
    {
        if (pausewinCheckVisible(wp->entryIds[hiddenTabs + 3]))
            break;
    }

    wp->pendingDeleteTabs = 3;
    if (immediate)
    {
        for (i = 0; i < 3; i++)
            _pausewinDelete(wp->entryIds[i]);

        for (i = 3; i < CHAR_PIXL_ITEM_WIN_ENTRY_MAX; i++)
            pausewinDelete(wp->entryIds[i]);
    }
    else
    {
        for (i = 0; i < CHAR_PIXL_ITEM_WIN_ENTRY_MAX; i++)
            pausewinDelete(wp->entryIds[i]);
    }

    if (playCloseSound)
        spsndSFXOn("SFX_SYS_MENU_CLOSE1");

    return hiddenTabs;
}

bool charPixlItemWinIsClosed()
{
    return wp->pendingDeleteTabs == 0;
}

s32 charPixlItemWinGetNextWindow()
{
    u16 flags = wp->flags;

    if (flags & CHAR_PIXL_ITEM_WIN_RESULT_CANCEL)
        return -1;
    if (flags & CHAR_PIXL_ITEM_WIN_RESULT_RETURN_TO_PLUS)
        return -2;
    if (flags & CHAR_PIXL_ITEM_WIN_RESULT_CHARACTER)
        return 1;
    if (flags & CHAR_PIXL_ITEM_WIN_RESULT_PIXL)
        return 2;
    if (flags & CHAR_PIXL_ITEM_WIN_RESULT_ITEM)
        return 3;
    return 0;
}

void func_80196004(s32 * selectedIndexOut, s32 * itemTypeOut)
{
    CharPixlItemWinTab * tab;

    if (wp->flags & CHAR_PIXL_ITEM_WIN_RESULT_CHARACTER)
        tab = &wp->characterTab;
    else if (wp->flags & CHAR_PIXL_ITEM_WIN_RESULT_PIXL)
        tab = &wp->pixlTab;
    else if (wp->flags & CHAR_PIXL_ITEM_WIN_RESULT_ITEM)
        tab = &wp->itemTab;

    *selectedIndexOut = tab->selectedIndex;
    *itemTypeOut = tab->entries[tab->selectedIndex].itemType;
}

void initCharTab(PausewinEntry * parent)
{
    CharPixlItemWinTab * tab = &wp->characterTab;
    s32 i;

    memset(tab, 0, sizeof(*tab));
    tab->entries = (CharPixlItemWinTabEntry *) __memAlloc(
        HEAP_MAP, sizeof(CharPixlItemWinTabEntry[CHAR_PIXL_ITEM_WIN_CHAR_ENTRY_CAPACITY]));
    memset(tab->entries, 0,
           sizeof(CharPixlItemWinTabEntry[CHAR_PIXL_ITEM_WIN_CHAR_ENTRY_CAPACITY]));

    tab->entryCount = 0;
    for (i = 0; i < POUCH_CHAR_ITEM_MAX; i++)
    {
        PouchCharOrPixlInfo * charInfo = pouchGetCharInfo(i);
        s32 itemType = charInfo->itemType;

        if (itemType != ITEM_ID_NULL)
        {
            if (itemType - ITEM_ID_CHAR_MARIO == marioGetPtr()->character)
                tab->selectedIndex = tab->entryCount;

            tab->entries[tab->entryCount].selectable = charInfo->selectable;
            tab->entries[tab->entryCount].itemType = itemType;
            tab->entries[tab->entryCount].data = NULL;
            tab->entryCount++;
        }
    }

    tab->scrollOffset = 0;
    tab->scrollPos = 0.0f;
    tab->cursorY = 0.0f;
    tab->cursorX = 0.0f;
    tab->parent = parent;
}

void func_80196174(PausewinEntry * entry)
{
    CharPixlItemWinTabEntry * entries;
    s32 selectedIndex;
    PausewinEntry * pauseEntry;
    s32 firstUseItemType;
    s32 windowId;
    PausewinEntry * parentEntry = entry;

    // Reusing the callback parameter preserves the target register allocation.
    entry = (PausewinEntry *) wp;

#define windowWork ((CharPixlItemWinWork *) entry)

    if (windowWork->flags & CHAR_PIXL_ITEM_WIN_RESULT_MASK)
        return;

// The target inlines this transition at each character-tab branch.
#define SWITCH_CHAR_TAB(tabField, tabIndex, tabPauseEntry, tabWindowId, tabUseItemType)            \
    do                                                                                             \
    {                                                                                              \
        s32 itemType;                                                                              \
        PausewinEntry * usePauseEntry;                                                             \
        const char * useMsgName;                                                                   \
        u16 outlines;                                                                              \
                                                                                                   \
        pausewinDisappear(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_TAB]);                        \
        pausewinAppear(wp->entryIds[(tabIndex) + CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_TAB]);              \
        spsndSFXOn("SFX_SYS_MENU_CURSOR_MOVE1");                                                   \
                                                                                                   \
        CharPixlItemWinWork * tabWork = wp;                                                        \
        tabWork->tabField.scrollPos = 25.0f * tabWork->tabField.scrollOffset;                      \
        tabWork->tabField.cursorX = tabWork->tabField.parent->pos.x;                               \
        tabWork->tabField.cursorY =                                                                \
            tabWork->tabField.parent->pos.y - 10.0f - 20.0f - 24.0f -                              \
            25.0f * (tabWork->tabField.selectedIndex - tabWork->tabField.scrollOffset);            \
        itemType = tabWork->tabField.entries[tabWork->tabField.selectedIndex].itemType;            \
                                                                                                   \
        tabPauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION]);      \
        tabWindowId = (tabPauseEntry)->id;                                                         \
        if (itemType == ITEM_ID_NULL)                                                              \
        {                                                                                          \
            if (pausewinCheckVisible(tabWindowId))                                                 \
                pausewinDisappear(tabWindowId);                                                    \
        }                                                                                          \
        else if (!pausewinCheckVisible(tabWindowId))                                               \
        {                                                                                          \
            pausewinAppear(tabWindowId);                                                           \
        }                                                                                          \
                                                                                                   \
        if (itemType == ITEM_ID_CHAR_MARIO && evtGetValue(NULL, GSW(0)) < 16)                      \
            pausewinSetMessage((tabPauseEntry), ITEM_ID_NULL, "msg_pc_mario_0");                   \
        else                                                                                       \
            pausewinSetMessage((tabPauseEntry), itemType, NULL);                                   \
                                                                                                   \
        tabUseItemType = tabWork->tabField.entries[tabWork->tabField.selectedIndex].itemType;      \
        usePauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);  \
        useMsgName = getItemUseMsg(tabUseItemType);                                                \
        if (useMsgName != NULL)                                                                    \
        {                                                                                          \
            usePauseEntry->msg = msgSearch(useMsgName);                                            \
            FontGetMessageWidthLine(usePauseEntry->msg, &outlines);                                \
            usePauseEntry->height = (outlines + 2) * 25 + 20;                                      \
            if (!pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))     \
                pausewinGoNeutral(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);         \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            usePauseEntry->msg = NULL;                                                             \
            if (pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))      \
                pausewinHide(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);              \
        }                                                                                          \
                                                                                                   \
        tabPauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON]);      \
        if ((tabIndex) == 0)                                                                       \
            (tabPauseEntry)->flags &= ~0x40;                                                       \
        else                                                                                       \
            (tabPauseEntry)->flags |= 0x40;                                                        \
        tabPauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON]);      \
        if ((tabIndex) == 1)                                                                       \
            (tabPauseEntry)->flags &= ~0x40;                                                       \
        else                                                                                       \
            (tabPauseEntry)->flags |= 0x40;                                                        \
        tabPauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON]);      \
        if ((tabIndex) == 2)                                                                       \
            (tabPauseEntry)->flags &= ~0x40;                                                       \
        else                                                                                       \
            (tabPauseEntry)->flags |= 0x40;                                                        \
        return;                                                                                    \
    }                                                                                              \
    while (0)

    if (pausewinCheckMoveLeft())
    {
        SWITCH_CHAR_TAB(itemTab, 2, pauseEntry, windowId, firstUseItemType);
    }
    else if (pausewinCheckMoveRight())
    {
        if (pouchCountPixls() != 0 && func_801471e8() != 0)
        {
            PausewinEntry * rightPauseEntry;
            s32 rightWindowId;
            s32 rightUseItemType;

            SWITCH_CHAR_TAB(pixlTab, 1, rightPauseEntry, rightWindowId, rightUseItemType);
        }
        else
        {
            PausewinEntry * rightPauseEntry;
            s32 rightWindowId;
            s32 rightUseItemType;

            SWITCH_CHAR_TAB(itemTab, 2, rightPauseEntry, rightWindowId, rightUseItemType);
        }
    }

#undef SWITCH_CHAR_TAB

    if (pausewinCheckPressOk())
    {
        entries = windowWork->characterTab.entries;
        selectedIndex = windowWork->characterTab.selectedIndex;

        if (!entries[selectedIndex].selectable)
        {
            spsndSFXOn("SFX_SYS_SELECT_NG1");
        }
        else
        {
            spsndSFXOn("SFX_SYS_MENU_DESIDE1");
            wp->flags |= CHAR_PIXL_ITEM_WIN_RESULT_CHARACTER;
        }
    }
    else if (pausewinCheckPressBack())
    {
        wp->flags |= CHAR_PIXL_ITEM_WIN_RESULT_CANCEL;
    }
    else if (wpadGetButtonsPressed(0) & pausewinGetOpenButtonMask())
    {
        if (!(gp->flags & 0x1000))
            wp->flags |= CHAR_PIXL_ITEM_WIN_RESULT_RETURN_TO_PLUS;
    }
    else
    {
        func_801984f4(parentEntry, &wp->characterTab);
    }

#undef windowWork
}

void func_801969dc(PausewinEntry * entry)
{
    func_80198874(entry, &wp->characterTab);
}

void dispCharTab(PausewinEntry * entry)
{
    dispTab(entry, &wp->characterTab, 0);
}

void deleteCharTab(PausewinEntry * entry)
{
    (void) entry;
    __memFree(HEAP_MAP, wp->characterTab.entries);
    wp->pendingDeleteTabs--;
}

void initPixlTab(PausewinEntry * parent)
{
    CharPixlItemWinTab * tab = &wp->pixlTab;
    s32 selectedItemType;
    s32 maxScrollOffset;
    s32 i;
    s32 j;

    memset(tab, 0, sizeof(*tab));
    tab->entries = (CharPixlItemWinTabEntry *) __memAlloc(
        HEAP_MAP, sizeof(CharPixlItemWinTabEntry[CHAR_PIXL_ITEM_WIN_PIXL_ENTRY_CAPACITY]));
    memset(tab->entries, 0,
           sizeof(CharPixlItemWinTabEntry[CHAR_PIXL_ITEM_WIN_PIXL_ENTRY_CAPACITY]));

    tab->entryCount = 0;
    for (i = 0; i < POUCH_FAIRY_ITEM_MAX; i++)
    {
        PouchCharOrPixlInfo * pixlInfo = pouchGetPixlInfo(i);
        s32 itemType = pixlInfo->itemType;

        if (itemType != ITEM_ID_NULL)
        {
            if (pixlInfo->selected)
                tab->selectedIndex = tab->entryCount;

            tab->entries[tab->entryCount].selectable = pixlInfo->selectable;
            tab->entries[tab->entryCount].itemType = itemType;
            tab->entries[tab->entryCount].data = pixlInfo;
            tab->entryCount++;
        }
    }

    selectedItemType = tab->entries[tab->selectedIndex].itemType;
    for (i = 0; i < tab->entryCount - 1; i++)
    {
        for (j = i + 1; j < tab->entryCount; j++)
        {
            if (itemDataTable[tab->entries[i].itemType].sortValue >
                itemDataTable[tab->entries[j].itemType].sortValue)
            {
                CharPixlItemWinTabEntry temp = tab->entries[i];

                tab->entries[i] = tab->entries[j];
                tab->entries[j] = temp;
            }
        }
    }

    for (i = 0; i < tab->entryCount; i++)
    {
        if (tab->entries[i].itemType == selectedItemType)
            break;
    }

    tab->selectedIndex = i;
    tab->scrollOffset = i > 5 ? i - 5 : 0;
    maxScrollOffset = tab->entryCount - 8;
    if (maxScrollOffset > 0)
    {
        if (tab->scrollOffset > maxScrollOffset)
            tab->scrollOffset = maxScrollOffset;
    }
    else
    {
        tab->scrollOffset = 0;
    }

    tab->scrollPos = 0.0f;
    tab->cursorY = 0.0f;
    tab->cursorX = 0.0f;
    tab->parent = parent;
}

void func_80196c8c(PausewinEntry * entry)
{
    CharPixlItemWinWork * windowWork = wp;

    if (windowWork->flags & CHAR_PIXL_ITEM_WIN_RESULT_MASK)
        return;

// The target inlines this transition at each Pixl-tab branch.
#define SWITCH_PIXL_TAB(tabField, tabIndex)                                                        \
    do                                                                                             \
    {                                                                                              \
        PausewinEntry * pauseEntry;                                                                \
        s32 useItemType;                                                                           \
        const char * useMsgName;                                                                   \
        u16 outlines;                                                                              \
        u16 windowId;                                                                              \
        s32 itemType;                                                                              \
                                                                                                   \
        pausewinDisappear(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_TAB]);                        \
        pausewinAppear(wp->entryIds[(tabIndex) + CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_TAB]);              \
        spsndSFXOn("SFX_SYS_MENU_CURSOR_MOVE1");                                                   \
                                                                                                   \
        CharPixlItemWinWork * tabWork = wp;                                                        \
        tabWork->tabField.scrollPos = 25.0f * tabWork->tabField.scrollOffset;                      \
        tabWork->tabField.cursorX = tabWork->tabField.parent->pos.x;                               \
        tabWork->tabField.cursorY =                                                                \
            tabWork->tabField.parent->pos.y - 10.0f - 20.0f - 24.0f -                              \
            25.0f * (tabWork->tabField.selectedIndex - tabWork->tabField.scrollOffset);            \
        itemType = tabWork->tabField.entries[tabWork->tabField.selectedIndex].itemType;            \
                                                                                                   \
        pauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION]);         \
        windowId = pauseEntry->id;                                                                 \
        if (itemType == ITEM_ID_NULL)                                                              \
        {                                                                                          \
            if (pausewinCheckVisible(windowId))                                                    \
                pausewinDisappear(windowId);                                                       \
        }                                                                                          \
        else if (!pausewinCheckVisible(windowId))                                                  \
        {                                                                                          \
            pausewinAppear(windowId);                                                              \
        }                                                                                          \
                                                                                                   \
        if (itemType == ITEM_ID_CHAR_MARIO && evtGetValue(NULL, GSW(0)) < 16)                      \
            pausewinSetMessage(pauseEntry, ITEM_ID_NULL, "msg_pc_mario_0");                        \
        else                                                                                       \
            pausewinSetMessage(pauseEntry, itemType, NULL);                                        \
                                                                                                   \
        useItemType = tabWork->tabField.entries[tabWork->tabField.selectedIndex].itemType;         \
        pauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);     \
        useMsgName = getItemUseMsg(useItemType);                                                   \
        if (useMsgName != NULL)                                                                    \
        {                                                                                          \
            pauseEntry->msg = msgSearch(useMsgName);                                               \
            FontGetMessageWidthLine(pauseEntry->msg, &outlines);                                   \
            pauseEntry->height = (outlines + 2) * 25 + 20;                                         \
            if (!pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))     \
                pausewinGoNeutral(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);         \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            pauseEntry->msg = NULL;                                                                \
            if (pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))      \
                pausewinHide(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);              \
        }                                                                                          \
                                                                                                   \
        pauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON]);         \
        if ((tabIndex) == 0)                                                                       \
            pauseEntry->flags &= ~0x40;                                                            \
        else                                                                                       \
            pauseEntry->flags |= 0x40;                                                             \
        pauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON]);         \
        if ((tabIndex) == 1)                                                                       \
            pauseEntry->flags &= ~0x40;                                                            \
        else                                                                                       \
            pauseEntry->flags |= 0x40;                                                             \
        pauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON]);         \
        if ((tabIndex) == 2)                                                                       \
            pauseEntry->flags &= ~0x40;                                                            \
        else                                                                                       \
            pauseEntry->flags |= 0x40;                                                             \
        return;                                                                                    \
    }                                                                                              \
    while (0)

    if (pausewinCheckMoveLeft())
    {
        if (pouchCountChars() != 0 && func_801472b4() != 0)
            SWITCH_PIXL_TAB(characterTab, 0);
        else
            SWITCH_PIXL_TAB(itemTab, 2);
    }
    else if (pausewinCheckMoveRight())
    {
        SWITCH_PIXL_TAB(itemTab, 2);
    }

#undef SWITCH_PIXL_TAB

    if (pausewinCheckPressOk())
    {
        CharPixlItemWinWork * selectWork = wp;
        CharPixlItemWinTabEntry * entries = selectWork->pixlTab.entries;
        s32 selectedIndex = selectWork->pixlTab.selectedIndex;
        s32 entryOffset;
        PouchCharOrPixlInfo * pixlInfo;
        s32 i;

        pixlInfo = (PouchCharOrPixlInfo *) entries[selectedIndex].data;
        if (!entries[selectedIndex].selectable)
        {
            spsndSFXOn("SFX_SYS_SELECT_NG1");
        }
        else
        {
            if (pixlInfo != NULL)
            {
                // The target keeps a byte offset alongside the loop index.
                for (i = 0, entryOffset = 0; i < selectWork->pixlTab.entryCount;
                     i++, entryOffset += sizeof(CharPixlItemWinTabEntry))
                {
                    CharPixlItemWinTabEntry * currentEntry =
                        (CharPixlItemWinTabEntry *) ((u8 *) selectWork->pixlTab.entries +
                                                     entryOffset);
                    PouchCharOrPixlInfo * info = (PouchCharOrPixlInfo *) currentEntry->data;
                    info->selected = false;
                }
                pixlInfo->selected = true;
            }

            spsndSFXOn("SFX_SYS_MENU_DESIDE1");
            wp->flags |= CHAR_PIXL_ITEM_WIN_RESULT_PIXL;
        }
    }
    else if (pausewinCheckPressBack())
    {
        wp->flags |= CHAR_PIXL_ITEM_WIN_RESULT_CANCEL;
    }
    else if (wpadGetButtonsPressed(0) & pausewinGetOpenButtonMask())
    {
        if (!(gp->flags & 0x1000))
            wp->flags |= CHAR_PIXL_ITEM_WIN_RESULT_RETURN_TO_PLUS;
    }
    else
    {
        func_801984f4(entry, &wp->pixlTab);
    }
}

void func_80197544(PausewinEntry * entry)
{
    func_80198874(entry, &wp->pixlTab);
}

void dispPixlTab(PausewinEntry * entry)
{
    dispTab(entry, &wp->pixlTab, 1);
}

void deletePixlTab(PausewinEntry * entry)
{
    (void) entry;
    __memFree(HEAP_MAP, wp->pixlTab.entries);
    wp->pendingDeleteTabs--;
}

void initItemTab(PausewinEntry * parent)
{
    MarioWork * mp;
    CharPixlItemWinTab * tab;
    s32 i;

    tab = &wp->itemTab;
    mp = marioGetPtr();

    memset(tab, 0, sizeof(*tab));
    tab->entries = (CharPixlItemWinTabEntry *) __memAlloc(
        HEAP_MAP, sizeof(CharPixlItemWinTabEntry[CHAR_PIXL_ITEM_WIN_ITEM_ENTRY_CAPACITY]));
    memset(tab->entries, 0,
           sizeof(CharPixlItemWinTabEntry[CHAR_PIXL_ITEM_WIN_ITEM_ENTRY_CAPACITY]));

    tab->entryCount = 0;
    for (i = 0; i < POUCH_USE_ITEM_MAX; i++)
    {
        MarioPouchWork * pouch = pouchGetPtr();
        const u16 itemType = pouch->useItem[i];

        if (itemType != ITEM_ID_NULL)
        {
            tab->entries[tab->entryCount].selectable = true;
            if (mp->flags & 0x02000000)
                tab->entries[tab->entryCount].selectable = false;

            switch (itemType)
            {
                case ITEM_ID_USE_KOURA_DE_PON:
                case ITEM_ID_USE_KINKAI_100:
                case ITEM_ID_USE_KINKAI_300:
                case ITEM_ID_USE_NANIGA_OKORUKANA:
                    if (func_800e1058() & 0x400)
                        tab->entries[tab->entryCount].selectable = false;
                    break;
            }

            tab->entries[tab->entryCount].itemType = itemType;
            tab->entryCount++;
        }
    }

    tab->selectedIndex = 0;
    tab->scrollOffset = 0;
    tab->scrollPos = 0.0f;
    tab->cursorY = 0.0f;
    tab->cursorX = 0.0f;
    tab->parent = parent;
}

void func_801976e8(PausewinEntry * entry)
{
    PausewinEntry * pauseEntry;
    s32 firstUseItemType;
    s32 windowId;
    PausewinEntry * parentEntry = entry;

    // Reusing the callback parameter preserves the target register allocation.
    entry = (PausewinEntry *) wp;

#define itemWork ((CharPixlItemWinWork *) entry)

    if (itemWork->flags & CHAR_PIXL_ITEM_WIN_RESULT_MASK)
        return;

// The target inlines this transition at each item-tab branch.
#define SWITCH_ITEM_TAB(tabField, tabIndex, tabPauseEntry, tabWindowId, tabUseItemType, leave)     \
    do                                                                                             \
    {                                                                                              \
        s32 itemType;                                                                              \
        PausewinEntry * usePauseEntry;                                                             \
        const char * useMsgName;                                                                   \
        u16 outlines;                                                                              \
                                                                                                   \
        pausewinDisappear(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_TAB]);                        \
        pausewinAppear(wp->entryIds[(tabIndex) + CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_TAB]);              \
        spsndSFXOn("SFX_SYS_MENU_CURSOR_MOVE1");                                                   \
                                                                                                   \
        CharPixlItemWinWork * tabWork = wp;                                                        \
        tabWork->tabField.scrollPos = 25.0f * tabWork->tabField.scrollOffset;                      \
        tabWork->tabField.cursorX = tabWork->tabField.parent->pos.x;                               \
        tabWork->tabField.cursorY =                                                                \
            tabWork->tabField.parent->pos.y - 10.0f - 20.0f - 24.0f -                              \
            25.0f * (tabWork->tabField.selectedIndex - tabWork->tabField.scrollOffset);            \
        itemType = tabWork->tabField.entries[tabWork->tabField.selectedIndex].itemType;            \
                                                                                                   \
        tabPauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION]);      \
        tabWindowId = (tabPauseEntry)->id;                                                         \
        if (itemType == ITEM_ID_NULL)                                                              \
        {                                                                                          \
            if (pausewinCheckVisible(tabWindowId))                                                 \
                pausewinDisappear(tabWindowId);                                                    \
        }                                                                                          \
        else if (!pausewinCheckVisible(tabWindowId))                                               \
        {                                                                                          \
            pausewinAppear(tabWindowId);                                                           \
        }                                                                                          \
                                                                                                   \
        if (itemType == ITEM_ID_CHAR_MARIO && evtGetValue(NULL, GSW(0)) < 16)                      \
            pausewinSetMessage((tabPauseEntry), ITEM_ID_NULL, "msg_pc_mario_0");                   \
        else                                                                                       \
            pausewinSetMessage((tabPauseEntry), itemType, NULL);                                   \
                                                                                                   \
        tabUseItemType = tabWork->tabField.entries[tabWork->tabField.selectedIndex].itemType;      \
        usePauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);  \
        useMsgName = getItemUseMsg(tabUseItemType);                                                \
        if (useMsgName != NULL)                                                                    \
        {                                                                                          \
            usePauseEntry->msg = msgSearch(useMsgName);                                            \
            FontGetMessageWidthLine(usePauseEntry->msg, &outlines);                                \
            usePauseEntry->height = (outlines + 2) * 25 + 20;                                      \
            if (!pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))     \
                pausewinGoNeutral(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);         \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            usePauseEntry->msg = NULL;                                                             \
            if (pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))      \
                pausewinHide(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);              \
        }                                                                                          \
                                                                                                   \
        tabPauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_CHAR_BUTTON]);      \
        if ((tabIndex) == 0)                                                                       \
            (tabPauseEntry)->flags &= ~0x40;                                                       \
        else                                                                                       \
            (tabPauseEntry)->flags |= 0x40;                                                        \
        tabPauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_PIXL_BUTTON]);      \
        if ((tabIndex) == 1)                                                                       \
            (tabPauseEntry)->flags &= ~0x40;                                                       \
        else                                                                                       \
            (tabPauseEntry)->flags |= 0x40;                                                        \
        tabPauseEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_ITEM_BUTTON]);      \
        if ((tabIndex) == 2)                                                                       \
            (tabPauseEntry)->flags &= ~0x40;                                                       \
        else                                                                                       \
            (tabPauseEntry)->flags |= 0x40;                                                        \
        goto leave;                                                                                \
    }                                                                                              \
    while (0)

    if (pausewinCheckMoveLeft())
    {
        if (pouchCountPixls() != 0 && func_801471e8() != 0)
        {
            SWITCH_ITEM_TAB(pixlTab, 1, pauseEntry, windowId, firstUseItemType, done);
        }
        else
        {
            PausewinEntry * leftPauseEntry;
            s32 leftWindowId;
            s32 leftUseItemType;

            if (pouchCountChars() == 0)
                return;
            if (func_801472b4() == 0)
                return;
            SWITCH_ITEM_TAB(characterTab, 0, leftPauseEntry, leftWindowId, leftUseItemType, done);
        }
    }
    else if (pausewinCheckMoveRight())
    {
        if (pouchCountChars() != 0 && func_801472b4() != 0)
        {
            PausewinEntry * rightPauseEntry;
            s32 rightWindowId;
            s32 rightUseItemType;

            SWITCH_ITEM_TAB(characterTab, 0, rightPauseEntry, rightWindowId, rightUseItemType,
                            done);
        }
        else
        {
            PausewinEntry * rightPauseEntry;
            s32 rightWindowId;
            s32 rightUseItemType;

            if (pouchCountPixls() == 0)
                return;
            if (func_801471e8() == 0)
                return;
            SWITCH_ITEM_TAB(pixlTab, 1, rightPauseEntry, rightWindowId, rightUseItemType, done);
        }
    }

#undef SWITCH_ITEM_TAB

    if (pausewinCheckPressOk())
    {
        MarioWork * mp = marioGetPtr();

        if (mp->flags & 0x02000000)
        {
            spsndSFXOn("SFX_SYS_SELECT_NG1");
        }
        else if (itemWork->itemTab.entryCount == 0)
        {
            spsndSFXOn("SFX_SYS_SELECT_NG1");
        }
        else if (!itemWork->itemTab.entries[itemWork->itemTab.selectedIndex].selectable)
        {
            spsndSFXOn("SFX_SYS_SELECT_NG1");
        }
        else
        {
            spsndSFXOn("SFX_SYS_MENU_DESIDE1");
            wp->flags |= CHAR_PIXL_ITEM_WIN_RESULT_ITEM;
        }
    }
    else if (pausewinCheckPressBack())
    {
        wp->flags |= CHAR_PIXL_ITEM_WIN_RESULT_CANCEL;
    }
    else if (wpadGetButtonsPressed(0) & pausewinGetOpenButtonMask())
    {
        if (!(gp->flags & 0x1000))
            wp->flags |= CHAR_PIXL_ITEM_WIN_RESULT_RETURN_TO_PLUS;
    }
    else
    {
        func_801984f4(parentEntry, &wp->itemTab);
    }

done:
#undef itemWork
    return;
}

void func_80198238(PausewinEntry * entry)
{
    func_80198874(entry, &wp->itemTab);
}

void dispItemTab(PausewinEntry * entry)
{
    dispTab(entry, &wp->itemTab, 2);
}

void deleteItemTab(PausewinEntry * entry)
{
    (void) entry;
    __memFree(HEAP_MAP, wp->itemTab.entries);
    wp->pendingDeleteTabs--;
}

void func_80198290(PausewinEntry * entry)
{
    pausewinMsgBoxInit(entry);
}

void func_80198294(PausewinEntry * entry)
{
    if (!(wp->flags & CHAR_PIXL_ITEM_WIN_RESULT_MASK))
        pausewinMsgBoxMain(entry);
}

void func_801982ac(PausewinEntry * entry)
{
    pausewinMsgBoxDisp(entry);
}

void func_801982b0(PausewinEntry * entry)
{
    pausewinMsgBoxDelete(entry);
}

void func_801982b4(PausewinEntry * entry)
{
    entry->msg = NULL;
}

void func_801982c0(PausewinEntry * entry)
{
    (void) entry;
}

void func_801982c4(PausewinEntry * entry)
{
    Mtx34 translation;
    Mtx34 scale;
    const char * title;
    f32 drawnWidth;
    f32 availableWidth;

    if (entry->msg == NULL)
        return;

    title = msgSearch("msg_AC_title");
    drawnWidth = 0.8f * FontGetMessageWidth(title);
    availableWidth = entry->width - 30.0f;
    if (drawnWidth > availableWidth)
    {
        PSMTXScale(scale, availableWidth / drawnWidth, 0.8f, 1.0f);
        drawnWidth = availableWidth;
    }
    else
    {
        PSMTXScale(scale, 0.8f, 0.8f, 1.0f);
    }
    PSMTXTrans(translation, (entry->width - drawnWidth) / 2.0f + entry->pos.x, entry->pos.y - 10.0f,
               0.0f);
    PSMTXConcat(translation, scale, translation);
    FontDrawStart_alpha((u8) (entry->alpha / 2));
    {
        GXColor color = {255, 0, 0, 255};
        FontDrawColor(&color);
    }
    FontDrawMessageMtx(translation, title);

    drawnWidth = FontGetMessageWidth(entry->msg);
    availableWidth = entry->width - 30.0f;
    if (drawnWidth > availableWidth)
    {
        PSMTXScale(scale, availableWidth / drawnWidth, 1.0f, 1.0f);
        drawnWidth = availableWidth;
    }
    else
    {
        PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
    }
    PSMTXTrans(translation, (entry->width - drawnWidth) / 2.0f + entry->pos.x,
               entry->pos.y - 10.0f - 24.0f, 0.0f);
    PSMTXConcat(translation, scale, translation);
    FontDrawStart_alpha((u8) (entry->alpha / 2));
    FontDrawMessageMtx(translation, entry->msg);
}

void func_801984f0(PausewinEntry * entry)
{
    (void) entry;
}

void func_801984f4(PausewinEntry * entry, CharPixlItemWinTab * tab)
{
    s32 previousIndex;

    if (tab->entryCount == 0)
        return;

    previousIndex = tab->selectedIndex;
    if (pausewinCheckMoveDown())
        tab->selectedIndex++;
    else if (pausewinCheckMoveUp())
        tab->selectedIndex--;

    if (tab->selectedIndex >= tab->entryCount)
    {
        if (pausewinCheckMoveDown())
        {
            tab->selectedIndex = 0;
            tab->scrollOffset = 0;
        }
        else
        {
            tab->selectedIndex--;
        }
    }

    if (tab->selectedIndex < 0)
    {
        if (pausewinCheckMoveUp())
        {
            tab->selectedIndex = tab->entryCount - 1;
            if (tab->entryCount - 8 > 0)
                tab->scrollOffset = tab->entryCount - 8;
        }
        else
        {
            tab->selectedIndex++;
        }
    }

    if (previousIndex < tab->selectedIndex)
    {
        if (tab->selectedIndex > 5 && tab->scrollOffset < tab->entryCount - 8)
            tab->scrollOffset++;
    }
    else if (previousIndex > tab->selectedIndex)
    {
        if (tab->selectedIndex - tab->scrollOffset < 3 && tab->scrollOffset > 0)
            tab->scrollOffset--;
    }

    if (previousIndex != tab->selectedIndex)
    {
        spsndSFXOn("SFX_SYS_MENU_CURSOR_MOVE2\0");
        {
            s32 itemType;
            u16 windowId;
            PausewinEntry * descriptionEntry;

            itemType = tab->entries[tab->selectedIndex].itemType;
            descriptionEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_DESCRIPTION]);
            windowId = descriptionEntry->id;
            if (itemType == ITEM_ID_NULL)
            {
                if (pausewinCheckVisible(windowId))
                    pausewinDisappear(windowId);
            }
            else if (!pausewinCheckVisible(windowId))
            {
                pausewinAppear(windowId);
            }

            if (itemType == ITEM_ID_CHAR_MARIO && evtGetValue(NULL, GSW(0)) < 16)
                pausewinSetMessage(descriptionEntry, ITEM_ID_NULL, "msg_pc_mario_0");
            else
                pausewinSetMessage(descriptionEntry, itemType, NULL);
        }
        {
            PausewinEntry * useEntry;
            const char * useMsgName;
            s32 useItemType;
            u16 outlines;

            useItemType = tab->entries[tab->selectedIndex].itemType;
            useEntry = pausewinGetEntry(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
            useMsgName = getItemUseMsg(useItemType);
            if (useMsgName != NULL)
            {
                useEntry->msg = msgSearch(useMsgName);
                FontGetMessageWidthLine(useEntry->msg, &outlines);
                useEntry->height = (outlines + 2) * 25 + 20;
                if (!pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))
                    pausewinGoNeutral(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
            }
            else
            {
                useEntry->msg = NULL;
                if (pausewinCheckVisible(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]))
                    pausewinHide(wp->entryIds[CHAR_PIXL_ITEM_WIN_ENTRY_USE_DESCRIPTION]);
            }
        }
    }

    tab->scrollPos += (25.0f * tab->scrollOffset - tab->scrollPos) / 6.0f;
    tab->cursorX += (entry->pos.x - tab->cursorX) / 6.0f;
    tab->cursorY += (entry->pos.y - 10.0f - 20.0f - 24.0f -
                     25.0f * (tab->selectedIndex - tab->scrollOffset) - tab->cursorY) /
                    6.0f;
}

static inline void drawFontColor(GXColor color)
{
    FontDrawColor(&color);
}

static inline GXColor getInitialFontColor()
{
    GXColor color = {0, 0, 0, 255};
    return color;
}

static inline GXColor getInitialIconColor()
{
    GXColor color = {255, 255, 255, 255};
    return color;
}

static inline GXColor getDisabledFontColor()
{
    GXColor color = {160, 160, 160, 0};
    return color;
}

static inline GXColor getDisabledIconColor()
{
    GXColor color = {160, 160, 160, 0};
    return color;
}

static inline GXColor getEnabledIconColor()
{
    GXColor color = {255, 255, 255, 0};
    return color;
}

void func_80198874(PausewinEntry * entry, CharPixlItemWinTab * tab)
{
    u32 oldX;
    u32 oldY;
    u32 oldWidth;
    u32 oldHeight;
    s32 i;
    GXColor fontColor = getInitialFontColor();
    GXColor iconColor = getInitialIconColor();

    i = entry->alpha;
    fontColor.a = (u8) i;
    iconColor.a = (u8) i;
    GXColor fogColor = {255, 255, 255, 255};
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fogColor);
    GXGetScissor(&oldX, &oldY, &oldWidth, &oldHeight);

    if (tab != &wp->characterTab)
    {
        func_80193888((u16) (u32) (304.0f + entry->pos.x),
                      (u16) (u32) (240.0f - entry->pos.y + 22.0f + 12.0f), (u16) (u32) entry->width,
                      (u16) (u32) (entry->height - 22.0f - 8.0f - 20.0f));
    }

    {
        s32 entryOffset;
        s32 i;
        // The target keeps a byte offset alongside the loop index.
        i = 0;
        entryOffset = 0;
        for (; i < tab->entryCount; entryOffset += (s32) sizeof(CharPixlItemWinTabEntry), i++)
        {
            ItemData * itemData =
                &itemDataTable[((CharPixlItemWinTabEntry *) ((u8 *) tab->entries + entryOffset))
                                   ->itemType];
            const char * name = msgSearch(itemData->nameMsg);
            f32 rowY = entry->pos.y - 12.0f - 20.0f - 12.0f - 25.0f * i + tab->scrollPos - 14.0f;
            // This call is present in the target even though its return value is unused.
            (void) marioGetPtr();

            f32 iconXOffset = 0.0f;
            f32 iconYOffset = 0.0f;
            Mtx34 matrix;
            Mtx34 scale;

            if (tab == &wp->characterTab)
                iconXOffset = 16.0f;
            if (((CharPixlItemWinTabEntry *) ((u8 *) tab->entries + entryOffset))->itemType ==
                ITEM_ID_CHAR_LUIGI)
                iconYOffset = -3.0f;

            if (entry->pos.y < rowY - 32.0f || entry->pos.y - entry->height > rowY + 32.0f)
                continue;

            if (!((CharPixlItemWinTabEntry *) ((u8 *) tab->entries + entryOffset))->selectable)
            {
                GXColor disabledFontColor = getDisabledFontColor();
                disabledFontColor.a = (u8) entry->alpha;
                fontColor = disabledFontColor;

                GXColor disabledIconColor = getDisabledIconColor();
                disabledIconColor.a = (u8) entry->alpha;
                iconColor = disabledIconColor;
            }
            else
            {
                GXColor enabledFontColor = {0, 0, 0, 0};
                enabledFontColor.a = (u8) entry->alpha;
                fontColor = enabledFontColor;

                GXColor enabledIconColor = getEnabledIconColor();
                enabledIconColor.a = (u8) entry->alpha;
                iconColor = enabledIconColor;
            }

            PSMTXTrans(matrix, entry->pos.x + 25.0f + iconXOffset,
                       entry->pos.y - 12.0f - 20.0f - 12.0f - 25.0f * i + tab->scrollPos - 14.0f +
                           iconYOffset,
                       0.0f);
            if (tab != &wp->characterTab)
            {
                PSMTXScale(scale, 0.6f, 0.6f, 1.0f);
                PSMTXConcat(matrix, scale, matrix);
            }
            else
            {
                PSMTXScale(scale, 0.8f, 0.8f, 1.0f);
                PSMTXConcat(matrix, scale, matrix);
            }
            iconDispGxCol(matrix, 16, (u16) itemData->iconId, iconColor);

            PSMTXTrans(matrix, entry->pos.x + 45.0f + iconXOffset,
                       entry->pos.y - 12.0f - 20.0f - 25.0f * i + tab->scrollPos + iconYOffset,
                       0.0f);
            {
                f32 messageWidth = FontGetMessageWidth(name);
                f32 availableWidth = entry->width - 45.0f - 10.0f;
                if (messageWidth > availableWidth)
                    PSMTXScale(scale, availableWidth / messageWidth, 1.0f, 1.0f);
                else
                    PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
            }
            PSMTXConcat(matrix, scale, matrix);
            FontDrawStart();
            drawFontColor(fontColor);
            FontDrawMessageMtx(matrix, name);
        }
    }

    func_80193888((u16) oldX, (u16) oldY, (u16) oldWidth, (u16) oldHeight);
    {
        Vec3 cursorPos = {0.0f, 0.0f, 1.0f};
        cursorPos.x = tab->cursorX;
        cursorPos.y = tab->cursorY;
        iconDispGx(1.0f, &cursorPos, 20, 179);
    }
}

void dispTab(PausewinEntry * entry, CharPixlItemWinTab * tab, s32 type)
{
    Mtx34 matrix;
    Mtx34 scale;
    Mtx34 rotation;
    Mtx34 bounce;
    Mtx34 offset;
    GXColor color = {255, 255, 255, 255};
    TPLHeader * tpl;

    color.a = (u8) entry->alpha;
    PSMTXTrans(bounce, 0.0f, lbl_80337370[gp->frameCounter % 20], 0.0f);
    PSMTXTrans(offset, 0.0f, -5.0f, 0.0f);

    if (tab->entryCount > 8)
    {
        if (tab->scrollOffset != 0)
        {
            f32 width = entry->width;
            PSMTXTrans(matrix, width / 2.0f + entry->pos.x, entry->pos.y - 20.0f, 0.0f);
            PSMTXRotRad(rotation, 'z', 3.1415927f);
            PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
            PSMTXConcat(matrix, rotation, matrix);
            PSMTXConcat(matrix, bounce, matrix);
            PSMTXConcat(matrix, scale, matrix);
            tpl = (TPLHeader *) pausewinGetTpl()->sp->data;
            gxsubInit_Tpl(tpl);
            {
                GXColor drawColor = color;
                gxsubDrawTextureMtx(25, matrix, &drawColor);
            }
        }

        if (tab->scrollOffset != tab->entryCount - 8)
        {
            f32 width = entry->width;
            PSMTXTrans(matrix, width / 2.0f + entry->pos.x, entry->pos.y - entry->height, 0.0f);
            PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
            PSMTXConcat(matrix, bounce, matrix);
            PSMTXConcat(matrix, scale, matrix);
            tpl = (TPLHeader *) pausewinGetTpl()->sp->data;
            gxsubInit_Tpl(tpl);
            {
                GXColor drawColor = color;
                gxsubDrawTextureMtx(25, matrix, &drawColor);
            }
        }
    }

    PausewinEntry * buttonEntry = pausewinGetEntry(wp->entryIds[type]);
    color.a = (u8) (color.a >> 1);

    PSMTXTrans(matrix, buttonEntry->pos.x + buttonEntry->width + 10.0f - 5.0f,
               buttonEntry->pos.y - buttonEntry->height / 2.0f, 0.0f);
    PSMTXRotRad(rotation, 'z', 1.5707964f);
    PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
    PSMTXConcat(matrix, rotation, matrix);
    PSMTXConcat(matrix, bounce, matrix);
    PSMTXConcat(matrix, offset, matrix);
    PSMTXConcat(matrix, scale, matrix);
    tpl = (TPLHeader *) pausewinGetTpl()->sp->data;
    gxsubInit_Tpl(tpl);
    {
        GXColor drawColor = color;
        gxsubDrawTextureMtx(25, matrix, &drawColor);
    }

    PSMTXTrans(matrix, buttonEntry->pos.x - 10.0f - 5.0f,
               buttonEntry->pos.y - buttonEntry->height / 2.0f, 0.0f);
    PSMTXRotRad(rotation, 'z', 4.712389f);
    PSMTXScale(scale, 0.6f, 0.6f, 0.6f);
    PSMTXConcat(matrix, rotation, matrix);
    PSMTXConcat(matrix, bounce, matrix);
    PSMTXConcat(matrix, offset, matrix);
    PSMTXConcat(matrix, scale, matrix);
    tpl = (TPLHeader *) pausewinGetTpl()->sp->data;
    gxsubInit_Tpl(tpl);
    {
        GXColor drawColor = color;
        gxsubDrawTextureMtx(25, matrix, &drawColor);
    }
}

extern const f32 lbl_80337370[20] = {
    0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.25f, 1.5f, 1.75f, 2.0f, 2.25f,
    2.5f, 2.75f, 3.0f, 3.25f, 3.5f, 3.75f, 4.0f, 3.0f,  2.0f, 1.0f,
};
}
