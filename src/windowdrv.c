/*
    WARNING: Not fully decompiled
    This file is currently not linked into the final dol
*/

#include <common.h>
#include <spm/camdrv.h>
#include <spm/gxsub.h>
#include <spm/effdrv.h>
#include <spm/memory.h>
#include <spm/spmario.h>
#include <spm/sptexture.h>
#include <spm/system.h>
#include <spm/windowdrv.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <msl/string.h>

extern "C" {

// .sbss
static WindowEntry * wp;
static u32 lbl_805ae794;
static u32 lbl_805ae798;
static u32 lbl_805ae79c;

void windowInit()
{
    WindowEntry * entries = (WindowEntry *) __memAlloc(0, sizeof(WindowEntry[WINDOW_MAX]));
    wp = entries;
    for (s32 i = 0; i < WINDOW_MAX; i++)
    {
        entries[i].flags = 0;
        entries[i].speakerSp = 0;
    }
}

void windowReInit()
{
    WindowEntry * entries = wp;
    for (s32 i = 0; i < WINDOW_MAX; i++)
    {
        entries[i].flags = 0;
        entries[i].speakerSp = 0;
    }
}

s32 windowEntry(u16 pri)
{
    WindowEntry * entry = wp;
    for (s32 i = 0; i < WINDOW_MAX; i++, entry++)
    {
        if ((entry->flags & 1) == 0)
        {
            memset(entry, 0, sizeof(*entry));
            entry->flags = 1;
            entry->priority = pri;
            return i;
        }
    }
    return -1;
}

bool windowDelete(WindowEntry * entry)
{
    if (entry->deleteFunc != NULL)
        entry->deleteFunc(entry);

    entry->flags = 0;
    entry->speakerSp = 0;

    return true;
}

bool windowDeleteID(s32 id)
{
    WindowEntry * entry = &wp[id];
    if ((entry->flags & 1) == 0)
        return false;
    else
        return windowDelete(entry);
}

void windowMain()
{
    WindowEntry * entry = wp;
    for (s32 i = 0; i < WINDOW_MAX; i++, entry++)
    {
        if ((entry->flags & 1) && (entry->mainFunc != NULL))
            entry->mainFunc(entry);
    }
}

// NOT_DECOMPILED func_80038b08

// NOT_DECOMPILED func_80038cc0

// NOT_DECOMPILED func_80038fb8

// NOT_DECOMPILED windowDispGX_Kanban

// NOT_DECOMPILED func_800393c8

// NOT_DECOMPILED windowDispGX_System

// NOT_DECOMPILED func_80039b80

// NOT_DECOMPILED func_80039d40

// NOT_DECOMPILED windowDispGX_Message

// NOT_DECOMPILED windowDispGX_ItemBox

// NOT_DECOMPILED windowDispGX2_Waku_col

s32 windowCheckID(s32 id)
{
    return wp[id].flags & 2;
}

WindowEntry * windowGetPointer(s32 id)
{
    return &wp[id];
}

bool windowCheckOpen()
{
    WindowEntry * entry = wp;
    for (s32 i = 0; i < WINDOW_MAX; i++, entry++)
    {
        if ((entry->flags & 1) && (entry->type != 1))
            return true;
    }

    return false;
}

}
