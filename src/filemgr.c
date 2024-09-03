/*
    WARNING: Not fully decompiled
    This file is currently not linked into the final dol
*/

#include <spm/dvdmgr.h>
#include <spm/filemgr.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/dvd.h>
#include <wii/os.h>
#include <msl/stdio.h>
#include <msl/string.h>

extern "C" {

// .bss
static FileWork work;
static char lbl_80516fa0[1024];

// .sdata
static FileWork * afp = &work;

// fileGetWorkPointer inlined, may be needed to match like evtGetWork was

void fileInit()
{
    FileEntry * curRecord;
    s32 i;

    // Allocate file entry array
    afp->entries = (FileEntry *) __memAlloc(0, sizeof(FileEntry[FILE_ENTRY_MAX]));

    // Initialise allocated linked list
    afp->allocatedStart = NULL;
    afp->allocatedEnd = NULL;

    // Initialise entries
    memset(afp->entries, 0, sizeof(FileEntry[FILE_ENTRY_MAX]));

    // Initialise free linked list
    curRecord = afp->entries;
    for (i = 0; i < FILE_ENTRY_MAX; i++)
    {
        curRecord->next = curRecord + 1;
        curRecord++;
    }
    afp->freeStart = &afp->entries[0];
    afp->freeEnd = &afp->entries[FILE_ENTRY_MAX - 1];
    afp->freeEnd->next = NULL;
}

void UnpackTexPalette(TPLHeader * palette)
{
    u16 i;

    VALIDATE_TPL_VERSION(0x35, palette);

    if (IS_TPL_PACKED(palette))
    {
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
}

void PackTexPalette(TPLHeader * palette)
{
    u16 i;

    VALIDATE_TPL_VERSION(0x5e, palette);

    if(!IS_TPL_PACKED(palette))
    {
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

// NOT_DECOMPILED fileAllocf

// NOT_DECOMPILED fileAlloc

// NOT_DECOMPILED _fileAlloc

// NOT_DECOMPILED fileFree

// NOT_DECOMPILED dvdReadDoneCallback

// NOT_DECOMPILED fileAsyncf

// NOT_DECOMPILED fileAsync

}
