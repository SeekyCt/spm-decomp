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

// .rodata
static const f64 lbl_80326360 = 4503599627370496.0; // casting float
#include "orderstrings/80326368_80326376.inc"

// .sbss
static u32 lbl_805ae79c;
static u32 lbl_805ae798;
static u32 lbl_805ae794;
static WindowEntry * wp;

// .sdata2
const GXColor lbl_805af7e0 = {0xff, 0xff, 0xff, 0xff};
#include "orderfloatsm/805af7e4_805af7e8.inc"
const GXColor lbl_805af7e8 = {0xff, 0xff, 0xff, 0xff};
#include "orderfloatsm/805af7ec_805af820.inc"
const GXColor lbl_805af820 = {0xff, 0xff, 0xff, 0x00};
#include "orderfloatsm/805af824_805af82c.inc"
const GXColor lbl_805af82c = {0x3d, 0x00, 0x89, 0x00};
const GXColor lbl_805af830 = {0xff, 0xff, 0xff, 0x00};
#include "orderfloatsm/805af834_805af83c.inc"
const GXColor lbl_805af83c = {0xff, 0xff, 0xff, 0x00};
#include "orderfloatsm/805af840_805af860.inc"
const GXColor lbl_805af860 = {0xff, 0xff, 0xff, 0x00};
#include "orderfloatsm/805af864_805af86c.inc"
const GXColor lbl_805af86c = {0xff, 0xff, 0xff, 0x00};

void windowInit()
{
    WindowEntry * entries = __memAlloc(0, sizeof(WindowEntry[WINDOW_MAX]));
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

asm UNKNOWN_FUNCTION(func_80038b08)
{
    #include "asm/80038b08.s"
}

asm UNKNOWN_FUNCTION(func_80038cc0)
{
    #include "asm/80038cc0.s"
}

asm UNKNOWN_FUNCTION(func_80038fb8)
{
    #include "asm/80038fb8.s"
}

#include "jumptable/80406c98.inc"
asm void windowDispGX_Kanban(s32 type, GXColor * colour, f32 x, f32 y, f32 width, f32 height)
{
    #include "asm/80039188.s"
}

asm UNKNOWN_FUNCTION(func_800393c8)
{
    #include "asm/800393c8.s"
}

asm void windowDispGX_System(s32 type, u8 alpha, f32 x, f32 y, f32 width, f32 height)
{
    #include "asm/800396e0.s"
}

asm UNKNOWN_FUNCTION(func_80039b80)
{
    #include "asm/80039b80.s"
}

#include "jumptable/80406cc0.inc"
#include "jumptable/80406cf4.inc"
asm UNKNOWN_FUNCTION(func_80039d40) 
{
    #include "asm/80039d40.s"
}

asm void windowDispGX_Message(s32 type, Unk param_2, u8 alpha, f32 x, f32 y, f32 width, f32 height, f32, f32)
{
    #include "asm/8003a440.s"
}

asm void windowDispGX_ItemBox(Unk param_1, GXColor * colour, f32 x, f32 y, f32 width, f32 height)
{
    #include "asm/8003a5e4.s"
}

asm void windowDispGX2_Waku_col(Mtx34 * mtx, u32 gxTexMapId, const GXColor * colour, f32 x, f32 y, f32 width, f32 height, f32 curve)
{
    #include "asm/8003ab80.s"
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
