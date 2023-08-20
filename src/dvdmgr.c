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

s32 DVDMgrRead(DVDEntry * entry, void * dest, s32 length, s32 offset)
{
    s32 ret;
    s32 lengthToRead;

    // Init temp
    entry->temp[0] = (s32) dest;
    entry->temp[1] = length;
    entry->temp[2] = offset;
    entry->temp[3] = 0;

    // Read until length reached
    // (will always just read it in one go)
    while (lengthToRead = entry->temp[1], lengthToRead > 0)
    {
        ret = DVDReadPrio(&entry->fileInfo, (void *) entry->temp[0], lengthToRead,
                          entry->temp[2] + entry->temp[3], entry->priority);
        if (ret == -3 || ret < 0)
            break;
        
        entry->temp[1] -= lengthToRead;
        entry->temp[3] += lengthToRead;
        entry->temp[0] += lengthToRead;
    }

    // Output error code or number of bytes read
    if (ret < 0)
        return ret;
    else
        return (s32) entry->fileInfo.length;
}

static void _cb(s32 result, DVDFileInfo * fileInfo)
{
    // Get context
    DVDEntry * entry = (DVDEntry *)fileInfo->commandBlock.userData;

    // Forward to user callback
    entry->readCallback(result, fileInfo);
}

s32 DVDMgrReadAsync(DVDEntry * entry, void * dest, s32 length, s32 offset,
                        DVDMgrCallback * callback)
{
    // Backup data
    entry->temp[2] = offset;
    entry->readCallback = callback;
    entry->temp[1] = length;
    entry->temp[1] -= length;
    entry->temp[3] = length;
    entry->temp[0] = (s32) dest + length;

    // Setup callback context
    entry->fileInfo.commandBlock.userData = entry;

    // Start read
    return DVDReadAsyncPrio(&entry->fileInfo, dest, length, offset, _cb, entry->priority);

}

asm void DVDMgrClose(DVDEntry * entry)
{
    #include "asm/8019e2e0.s"
}

asm s32 DVDMgrGetLength(DVDEntry * entry)
{
    #include "asm/8019e320.s"
}
