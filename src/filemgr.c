/*
    WARNING: Not fully decompiled
    This file is currently not linked into the final dol
*/

#include "wii/os/OSThread.h"
#include <spm/dvdmgr.h>
#include <spm/filemgr.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/dvd.h>
#include <wii/os.h>
#include <msl/stdarg.h>
#include <msl/stdio.h>
#include <msl/string.h>

extern "C" {

// .bss
static FileWork work;
static char PathBuffer[1024];

// .sdata
static FileWork * afp = &work;

// fileGetWorkPointer inlined, may be needed to match like evtGetWork was

void fileInit()
{
    FileEntry * entry;
    s32 i;

    // Allocate file entry array
    afp->entries = (FileEntry *) __memAlloc(0, sizeof(FileEntry[FILE_ENTRY_MAX]));

    // Initialise allocated linked list
    afp->allocatedStart = NULL;
    afp->allocatedEnd = NULL;

    // Initialise entries
    memset(afp->entries, 0, sizeof(FileEntry[FILE_ENTRY_MAX]));

    // Initialise free linked list
    entry = afp->entries;
    for (i = 0; i < FILE_ENTRY_MAX; i++)
    {
        entry->next = entry + 1;
        entry++;
    }
    afp->freeStart = &afp->entries[0];
    afp->freeEnd = &afp->entries[FILE_ENTRY_MAX - 1];
    afp->freeEnd->next = NULL;
}

void UnpackTexPalette(TPLHeader * palette)
{
    u16 i;

    VALIDATE_TPL_VERSION(0x35, palette);

    if (!IS_TPL_PACKED(palette))
        return;

    // Convert image table offset to pointer
    palette->imageTable = (ImageTableEntry *) (palette->imageTableOffset + (u32) palette);

    // Unpack all images in table
    for (i = 0; i < palette->imageCount; i++)
    {
        if (palette->imageTable[i].imageOffset != 0)
        {
            // Convert image header offset to pointer
            palette->imageTable[i].image = (ImageHeader *) ((u32) palette + palette->imageTable[i].imageOffset);

            if (!palette->imageTable[i].image->unpacked)
            {
                // Convert image data offset to pointer
                palette->imageTable[i].image->data = (void *) ((u32) palette + palette->imageTable[i].image->dataOffset);
                palette->imageTable[i].image->unpacked = true;
            }
        }

        if (palette->imageTable[i].paletteOffset != 0)
        {
            // Convert palette header offset to pointer
            palette->imageTable[i].palette = (PaletteHeader *) ((u32) palette + palette->imageTable[i].paletteOffset);

            if (!palette->imageTable[i].palette->unpacked)
            {
                // Convert palette data offset to pointer
                palette->imageTable[i].palette->data = (void *) ((u32) palette + palette->imageTable[i].palette->dataOffset);
                palette->imageTable[i].palette->unpacked = true;
            }
        }
    }
}

void PackTexPalette(TPLHeader * palette)
{
    u16 i;

    VALIDATE_TPL_VERSION(0x5e, palette);

    if(IS_TPL_PACKED(palette))
        return;

    // Pack all images in table
    for (i = 0; i < palette->imageCount; i++)
    {
        if (palette->imageTable[i].image != NULL)
        {
            if (palette->imageTable[i].image->unpacked)
            {
                palette->imageTable[i].image->unpacked = false;

                // Convert image data pointer to offset
                palette->imageTable[i].image->dataOffset = (u32) palette->imageTable[i].image->data - (u32) palette;
            }

            // Convert image header pointer to offset
            palette->imageTable[i].imageOffset = (u32) palette->imageTable[i].image - (u32) palette;
        }

        if (palette->imageTable[i].palette != NULL)
        {
            if (palette->imageTable[i].palette->unpacked)
            {
                palette->imageTable[i].palette->unpacked = false;

                // Convert palette data pointer to offset
                palette->imageTable[i].palette->dataOffset = (u32) palette->imageTable[i].palette->data - (u32) palette;
            }

            // Convert palette header pointer to offset
            palette->imageTable[i].paletteOffset = (u32) palette->imageTable[i].palette - (u32) palette;
        }
    }
    // Convert image table pointer to offset
    palette->imageTableOffset = (u32) palette->imageTable - (u32) palette;
}

// NOT_DECOMPILED fileGarbageDataAdrClear

// NOT_DECOMPILED fileGarbageDataAdrSet

void fileGarbageMoveMem(void * dest, FileEntry * src)
{
    // Turn any pointers into the data to offsets 
    if (src->state == 3)
    {
        if (src->dvdEntry == 0)
            fileGarbageDataAdrClear(src);
    }
    else
    {
        fileGarbageDataAdrClear(src);
    }

    // Move data
    memmove(dest, src->sp->data, src->sp->size);
    src->sp->data = dest;

    // Turn offsets back to pointers
    if (src->state == 3)
    {
        if (src->dvdEntry != 0)
            src->dvdEntry->dest = dest;
        else
            fileGarbageDataAdrSet(src->sp->data, src->fileType);
    }
    else
    {
        fileGarbageDataAdrSet(src->sp->data, src->fileType);
    }
}

// NOT_DECOMPILED _fileGarbage

void _fileGarbage(s32 patience)
{
    // Wait on any files being read
    if (patience == 0)
    {
        for (FileEntry * entry = afp->allocatedStart; entry != NULL; entry = entry->next)
        {
            if (entry->state == FILE_ASYNC_CALLED)
            {
                if (entry->dvdEntry != NULL)
                {
                    while (entry->dvdEntry != NULL)
                        OSYieldThread();
                }

                entry->state = FILE_WAITING_GARBAGE;
            }
        }
    }
    else if (patience == 1)
    {
        for (FileEntry * entry = afp->allocatedStart; entry != NULL; entry = entry->next)
        {
            if (entry->state == FILE_ASYNC_CALLED)
            {
                if (entry->dvdEntry != NULL)
                {
                    while (entry->dvdEntry != NULL)
                        OSYieldThread();
                }
                else
                {
                    entry->touchCnt--;
                    if (entry->touchCnt <= -2)
                        entry->state = FILE_WAITING_GARBAGE;
                }
            }
        }
    }
    
    // Free any unused files and update lists
    FileEntry * entry = afp->allocatedStart;
    FileEntry * allocatedEnd = NULL;
    FileEntry * allocatedStart = NULL;
    while (entry != NULL)
    {
        // Backup next pointer
        FileEntry * next = entry->next;

        if (entry->state == FILE_WAITING_GARBAGE)
        {
            // Deallocate
            entry->state = FILE_EMPTY;
            if ((void *)entry->unknown_0x4 != (void *)entry->sp)
                smartFree(entry->sp);

            // Remove from allocated list
            if (allocatedEnd != NULL)
                allocatedEnd->next = entry->next;

            // Append to free list
            if (afp->freeStart == NULL)
            {
                afp->freeStart = entry;
            }
            else
            {
                // "The list structure is broken"
                assert(772, afp->freeEnd, "リスト構造が壊れています");
                afp->freeEnd->next = entry;
            }
            entry->next = NULL;
            afp->freeEnd = entry;
        }
        else
        {
            // Add to allocated list
            if (allocatedEnd == NULL)
                allocatedStart = entry;
            allocatedEnd = entry;
        }

        entry = next;
    }
    afp->allocatedStart = allocatedStart;
    afp->allocatedEnd = allocatedEnd;
}

FileEntry * fileAllocf(s32 fileType, const char * format, ...)
{
    va_list args;

    va_start(args, format);
    vsprintf(PathBuffer, format, args);
    va_end(args);

    return _fileAlloc(PathBuffer, fileType, 0);
}

FileEntry * fileAlloc(const char * path, s32 fileType)
{
    return _fileAlloc(path, fileType, 0);
}

FileEntry * _fileAlloc(const char * path, s32 fileType, s32 unused)
{
    (void) unused;

    // Try find existing entry
    for (FileEntry * lp = afp->allocatedStart; lp != NULL; lp = lp->next)
    {
        // Check if name matches
        if (lp->state == FILE_EMPTY || strcmp(lp->path, path) != 0)
            continue;

        // "The file type is different from before"
        assert(876, lp->fileType == fileType, "ファイルタイプが以前と違います\n");

        // Wait for reading to finish
        while (lp->dvdEntry != NULL)
            OSYieldThread();

        // Update state and reference count
        if (lp->state == FILE_ASYNC_CALLED)
        {
            lp->state = FILE_ALLOC_CALLED;
            lp->touchCnt = 0;
        }
        else if (lp->state == FILE_WAITING_GARBAGE)
        {
            lp->state = FILE_ALLOC_CALLED;
        }
        lp->touchCnt++;

        return lp;
    }
    
    // Read file synchronously if not found

    // Try get a free entry
    FileEntry * new_lp = afp->freeStart; // r31
    if (new_lp == NULL) {
        _fileGarbage(1);
        new_lp = afp->freeStart;
    }
    if (new_lp == NULL) {
        _fileGarbage(0);
        new_lp = afp->freeStart;
    }
    // "File list is missing"
    assert(917, new_lp, "ファイルリストが足りない\n");

    // Backup next pointer
    FileEntry * newFreeStart = new_lp->next;

    // Try open file
    DVDEntry * dvdEntry = DVDMgrOpen(path, 2, 0);
    if (dvdEntry == NULL)
        return NULL;

    // Check file isn't empty
    s32 length = DVDMgrGetLength(dvdEntry);
    u32 roundedLength = (length + 0x1f) & 0xffffffe0;
    if (roundedLength == 0) {
        DVDMgrClose(dvdEntry);
        return NULL;
    }

    // No effect, length is already aligned
    roundedLength = (roundedLength + 0x1f) & 0xffffffe0;

    // Create space on smart heap
    SmartAllocation * new_sp = smartAlloc(roundedLength, 0);
    memset(new_sp->data, 0, new_sp->size);
    // Memory allocation failure
    assert(969, new_sp, "メモリ確保に失敗\n");
    new_sp->fileEntry = new_lp;

    // Read into memory
    if (DVDMgrRead(dvdEntry, new_sp->data, (s32)roundedLength, 0) <= 0)
        assert(978, 0, "ＤＶＤだめでっすちめいてきえらー\n");
    DVDMgrClose(dvdEntry);

    // Update file entry
    new_lp->sp = new_sp;
    new_lp->state = FILE_ALLOC_CALLED;
    new_lp->touchCnt = 1;
    new_lp->fileType = (s8) fileType;
    new_lp->next = NULL;
    new_lp->length = (u32)length;
    strcpy(new_lp->path, path);
    new_lp->dvdEntry = NULL;

    // Apply relocations to file contents
    fileGarbageDataAdrSet(new_sp->data, fileType);

    // Remove from free list
    afp->freeStart = newFreeStart;
    if (afp->freeStart == NULL) {
        afp->freeEnd = NULL;
    }

    // Insert into allocated list
    if (afp->allocatedStart == NULL) {
        afp->allocatedStart = new_lp;
        afp->allocatedEnd = new_lp;
    }
    else {
        afp->allocatedEnd->next = new_lp;
        afp->allocatedEnd = new_lp;
    }

    return new_lp;
}

void fileFree(FileEntry * lp)
{
    // Ignore if nothing to free
    if (lp == NULL)
        return;
    if (lp->sp == NULL)
        return;

    // Check file has actually been claimed
    if (lp->state == FILE_ALLOC_CALLED) {
        // Decrement reference count
        lp->touchCnt--;
        // "It's too free"
        assert(1039, lp->touchCnt>=0, "フリーしすぎです。\n");
        if (lp->touchCnt == 0) {
            lp->state = FILE_WAITING_GARBAGE;
        }
    }
    else
    {
        // "!An unalocated pointer was pointed to"
        _assert(1049, !"Alloc されていないポインタを示しました\n");
    }
}

// NOT_DECOMPILED dvdReadDoneCallback

// NOT_DECOMPILED fileAsyncf

// NOT_DECOMPILED fileAsync

}
