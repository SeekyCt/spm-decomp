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

UNKNOWN_FUNCTION(func_80038b08)
{
    // Not decompiled
}

UNKNOWN_FUNCTION(func_80038cc0)
{
    // Not decompiled
}

UNKNOWN_FUNCTION(func_80038fb8)
{
    // Not decompiled
}

void windowDispGX_Kanban(s32 type, GXColor * colour, f32 x, f32 y, f32 width, f32 height)
{
    // Not decompiled
    (void) type;
    (void) colour;
    (void) x;
    (void) y;
    (void) width;
    (void) height;
}

UNKNOWN_FUNCTION(func_800393c8)
{
    // Not decompiled
}

void windowDispGX_System(s32 type, u8 alpha, f32 x, f32 y, f32 width, f32 height)
{
    // Not decompiled
    (void) type;
    (void) alpha;
    (void) x;
    (void) y;
    (void) width;
    (void) height;
}

UNKNOWN_FUNCTION(func_80039b80)
{
    // Not decompiled
}

UNKNOWN_FUNCTION(func_80039d40) 
{
    // Not decompiled
}

void windowDispGX_Message(s32 type, Unk param_2, u8 alpha, f32 x, f32 y, f32 width, f32 height, f32, f32)
{
    // Not decompiled
    (void) type;
    (void) param_2;
    (void) alpha;
    (void) x;
    (void) y;
    (void) width;
    (void) height;
}

void windowDispGX_ItemBox(Unk param_1, GXColor * colour, f32 x, f32 y, f32 width, f32 height)
{
    // Not decompiled
    (void) param_1;
    (void) colour;
    (void) x;
    (void) y;
    (void) width;
    (void) height;
}

void windowDispGX2_Waku_col(Mtx34 * mtx, u32 gxTexMapId, const GXColor * colour, f32 x, f32 y, f32 width, f32 height, f32 curve)
{
    // Not decompiled
    (void) mtx;
    (void) gxTexMapId;
    (void) colour;
    (void) x;
    (void) y;
    (void) width;
    (void) height;
    (void) curve;
}

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
