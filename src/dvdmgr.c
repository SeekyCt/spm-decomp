#include <common.h>
#include <spm/dvdmgr.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <msl/string.h>

#include "orderstrings/803377a8_803377f8.inc"

static DVDWork work;
static DVDWork * wp = &work;

void DVDMgrInit()
{
    // Allocate entries
    wp->num = MAX_DVD_ENTRY;
    wp->entries = __memAlloc(HEAP_MAIN, wp->num * sizeof(DVDEntry));
}

void DVDMgrDelete()
{
    return;
}

DVDEntry* DVDMgrOpen(const char * name, s32 priority, s16 param_3)
{
    DVDEntry * entry;
    s32 entrynum;
    s32 i;

    // Try get entrynum
    entrynum = DVDConvertPathToEntrynum(name);
    if (entrynum == -1)
        return NULL;
    
    // Find a free entry
    entry = wp->entries;
    for (i = 0; i < wp->num; i++, entry++)
    {
        if ((entry->flags & DVD_FLAG_IN_USE) == 0)
            break;
    }
    if (i >= wp->num)
    {
        assert(51, i < wp->num, "DVDMgrOpen Error");
        return NULL;
    }

    // Init entry
    memset(entry, 0, sizeof(*entry));
    assertf(56, strlen(name) < 64, "名前長すぎ [ %s ]", name);
    strcpy(entry->path, name);
    entry->priority = (u8) priority;
    entry->openP3 = param_3;
    entry->temp[0] = 0;
    entry->temp[1] = 0;
    entry->temp[2] = 0;
    entry->temp[3] = 0;
    entry->entrynum = entrynum;
    entry->flags = 0;
    entry->flags |= DVD_FLAG_IN_USE;

    // Try open file
    if (DVDFastOpen(entrynum, &entry->fileInfo) != 1)
    {
        memset(entry, 0, sizeof(*entry));
        return NULL;
    }

    return entry;
}

asm s32 DVDMgrRead(DVDEntry * entry, void * dest, size_t length, u32 offset)
{
    #include "asm/8019e1e4.s"
}

static asm void _cb(s32 result, DVDFileInfo * fileInfo)
{
    #include "asm/8019e29c.s"
}

asm s32 DVDMgrReadAsync(DVDEntry * entry, void * dest, size_t length, u32 offset,
                        DVDMgrCallback * callback)
{
    #include "asm/8019e2ac.s"
}

asm void DVDMgrClose(DVDEntry * entry)
{
    #include "asm/8019e2e0.s"
}

asm size_t DVDMgrGetLength(DVDEntry * entry)
{
    #include "asm/8019e320.s"
}
