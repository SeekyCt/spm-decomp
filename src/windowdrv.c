#include <common.h>
#include <memory.h>
#include <string.h>
#include <windowdrv.h>

static WindowEntry * wp; // 805ae790

void windowInit()
{
    WindowEntry * entries = __memAlloc(0, sizeof(WindowEntry[WINDOW_MAX]));
    wp = entries;
    for (s32 i = 0; i < WINDOW_MAX; i++)
    {
        entries[i].flags = 0;
        entries[i].unknown_0x28 = 0;
    }
}

void windowReInit()
{
    WindowEntry * entries = wp;
    for (s32 i = 0; i < WINDOW_MAX; i++)
    {
        entries[i].flags = 0;
        entries[i].unknown_0x28 = 0;
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
            entry->pri = pri;
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
    entry->unknown_0x28 = 0;

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

// func_80038b08
// func_80038cc0
// func_80038fb8
// func_80039188
// func_800393c8
// func_800396e0
// func_80039b80
// func_80039d40
// func_8003a440
// func_8003a5e4
// windowDispGX2_Waku_col

s32 windowCheckID(s32 id)
{
    return (int) wp[id].flags & 2;
}

WindowEntry * windowGetPointer(s32 id)
{
    return &wp[id];
}

bool func_8003b180()
{
    WindowEntry * entry = wp;
    for (s32 i = 0; i < WINDOW_MAX; i++, entry++)
    {
        if ((entry->flags & 1) && (entry->unknown_0x8 != 1))
            return true;
    }

    return false;
}