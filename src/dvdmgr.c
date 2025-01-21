#include "wii/dvd.h"
#include <common.h>
#include <spm/dvdmgr.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <msl/string.h>

extern "C" {

static DVDWork work;
static DVDWork * wp = &work;

void DVDMgrInit()
{
    // Allocate entries
    wp->num = MAX_DVD_ENTRY;
    wp->entries = (DVDEntry *) __memAlloc(HEAP_MAIN, wp->num * sizeof(DVDEntry));
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
    entry->dest = 0;
    entry->lengthRemaining = 0;
    entry->offset = 0;
    entry->lengthRead = 0;
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

    // Backup data
    entry->dest = dest;
    entry->lengthRemaining = length;
    entry->offset = offset;
    entry->lengthRead = 0;

    // Read until length reached
    // (will always just read it in one go)
    while (lengthToRead = entry->lengthRemaining, lengthToRead > 0)
    {
        ret = DVDReadPrio(&entry->fileInfo, entry->dest, lengthToRead,
                          entry->offset + entry->lengthRead, entry->priority);
        if (ret == DVD_RESULT_CANCELED || ret < 0)
            break;
        
        entry->lengthRemaining -= lengthToRead;
        entry->lengthRead += lengthToRead;
        entry->dest = (void *) ((s32)entry->dest + lengthToRead);
    }

    // Output error code or number of bytes read
    if (ret < 0)
        return ret;
    else
        return (s32) entry->fileInfo.length;
}

#if DVDMGR_C_VERSION >= 2
static void _cb(s32 result, DVDFileInfo * fileInfo)
{
    DVDEntry * entry;
    
    // Get context
    entry = (DVDEntry *)fileInfo->commandBlock.userData;

    // Forward to user callback
    entry->readCallback(result, fileInfo);
}

void DVDMgrReadAsync(DVDEntry * entry, void * dest, s32 length, s32 offset,
                        DVDMgrCallback * callback)
{
    // Backup data
    // None of these ever seem to be used..?
    entry->offset = offset;
    entry->readCallback = callback;
    entry->lengthRemaining = length;
    entry->lengthRemaining -= length; // what
    entry->lengthRead = length;
    entry->dest = (void *) ((s32) dest + length);

    // Setup callback context
    entry->fileInfo.commandBlock.userData = entry;

    // Start read
    DVDReadAsyncPrio(&entry->fileInfo, dest, length, offset, _cb, entry->priority);
}
#else // 1
static void readAsync(DVDEntry * entry, s32 lengthToRead, DVDFICallback * callback)
{
    // Setup callback context
    entry->fileInfo.commandBlock.userData = entry;

    // Start read
    DVDReadAsyncPrio(&entry->fileInfo, entry->dest, lengthToRead, entry->offset + entry->lengthRead, callback, entry->priority);

    entry->lengthRemaining -= lengthToRead;
    entry->lengthRead += lengthToRead;
    entry->dest = (void *) ((u32)entry->dest + lengthToRead);
}

static void _cb(s32 result, DVDFileInfo * fileInfo)
{
    DVDEntry * entry;
    
    // Get context
    entry = (DVDEntry *)fileInfo->commandBlock.userData;

    if (result < 0)
    {
        // Run user callback on failure
        entry->readCallback(result, fileInfo);
    }
    else if (entry->lengthRemaining == 0)
    {
        // Run user callback on completion
        entry->readCallback(result, fileInfo);
    }
    else
    {
        readAsync(entry, entry->lengthRemaining, _cb); // inline
    }
}

void DVDMgrReadAsync(DVDEntry * entry, void * dest, s32 length, s32 offset,
                        DVDMgrCallback * callback)
{
    // Backup data
    entry->dest = dest;
    entry->readCallback = callback;
    entry->lengthRemaining = length;
    entry->offset = offset;
    entry->lengthRead = 0;

    readAsync(entry, entry->lengthRemaining, _cb); // inline
}

#endif

void DVDMgrClose(DVDEntry * entry)
{
    // Close file
    DVDClose(&entry->fileInfo);

    // Reset entry
    memset(entry, 0, sizeof(*entry));
}

s32 DVDMgrGetLength(DVDEntry * entry)
{
    return (s32) entry->fileInfo.length;
}

}
