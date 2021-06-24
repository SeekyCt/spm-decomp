#include <common.h>
#include <filemgr.h>
#include <memory.h>
#include <os.h>
#include <string.h>
#include <tpl.h>

static FileWork fileWork;
static FileWork * afp = &fileWork;

// fileGetWorkPointer inlined, may be needed to match like evtGetWork was

void fileInit() {
    // Allocate file record array
    afp->records = __memAlloc(0, sizeof(FileRecord[FILE_RECORD_MAX]));

    // Initialise allocated linked list
    afp->allocatedStart = NULL;
    afp->allocatedEnd = NULL;

    // Initialise free linked list
    memset(afp->records, 0, sizeof(FileRecord[FILE_RECORD_MAX]));
    FileRecord * curRecord = afp->records;
    for (s32 i = 0; i < FILE_RECORD_MAX; i++) {
        curRecord->next = curRecord + 1;
        curRecord++;
    }
    afp->freeStart = &afp->records[0];
    afp->freeEnd = &afp->records[FILE_RECORD_MAX - 1];
    afp->freeEnd->next = NULL;
}

void UnpackTexPalette(TPLHeader * palette) {
    VALIDATE_TPL_VERSION(0x35, palette);

    if (IS_TPL_PACKED(palette)) {
        // Convert image table offset to pointer
        palette->imageTable = (ImageTableEntry *) (palette->imageTableOffset + (u32) palette);

        // Unpack all images in table
        for (u16 i = 0; i < palette->imageCount; i++) {
            if (palette->imageTable[i].imageOffset != 0) {
                // Convert image header offset to pointer
                palette->imageTable[i].image = (ImageHeader *) ((u32) palette + palette->imageTable[i].imageOffset);

                if (!palette->imageTable[i].image->unpacked) {
                    // Convert image data offset to pointer
                    palette->imageTable[i].image->data = (void *) ((u32) palette + palette->imageTable[i].image->dataOffset);
                    palette->imageTable[i].image->unpacked = true;
                }
            }

            if (palette->imageTable[i].paletteOffset != 0) {
                // Convert palette header offset to pointer
                palette->imageTable[i].palette = (void *) ((u32) palette + palette->imageTable[i].paletteOffset);

                if (!palette->imageTable[i].palette->unpacked) {
                    // Convert palette data offset to pointer
                    palette->imageTable[i].palette->data = (void *) ((u32) palette + palette->imageTable[i].palette->dataOffset);
                    palette->imageTable[i].palette->unpacked = true;
                }
            }
        }
    }
}

void PackTexPalette(TPLHeader * palette) {
    VALIDATE_TPL_VERSION(0x5e, palette);

    if(!IS_TPL_PACKED(palette)) {
        // Pack all images in table
        for (u16 i = 0; i < palette->imageCount; i++) {
            if (palette->imageTable[i].image != NULL) {
                if (palette->imageTable[i].image->unpacked) {
                    palette->imageTable[i].image->unpacked = false;
                    // Convert image data pointer to offset
                    palette->imageTable[i].image->dataOffset = (u32) palette->imageTable[i].image->data - (u32) palette;
                }
                // Convert image header pointer to offset
                palette->imageTable[i].imageOffset = (u32) palette->imageTable[i].image - (u32) palette;
            }

            if (palette->imageTable[i].palette != NULL) {
                if (palette->imageTable[i].palette->unpacked) {
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

/* WIP - stopped for now since so much of it is unknown
typedef union {
    u32 offset;
    void * ptr;
} PtrOrOffset;
typedef struct {
    u8 unknown_0x0[0x14c - 0x0];
    PtrOrOffset subs[25];
    // unknown size
} FileType1;
typedef struct {
    u8 unknown_0x0[0x24 - 0x0];
    PtrOrOffset subs[8];
    // unknown size
} FileType2;
typedef struct {
    u8 unknown_0x0[0x48 - 0x0];
    PtrOrOffset p;
    // unknown size
} FileType3;
typedef struct {
    u8 unknown_0x0[0x14c - 0x0];
    PtrOrOffset subs[25];
    // unknown size
} FileType5;
void fileGarbageDataAdrClear(FileRecord * record) {
    switch (record->fileType) {
        // might need case for 0 to match
        case FILETYPE_1:
            FileType1 * f1 = record->sp->data;
            if (f1->subs[0].offset > (u32) f1) {
                for (s32 i = 0; i < 25; i++) {
                    f1->subs[i].offset = (u32) f1->subs[i].ptr - (u32) f1;
                }
            }
            break;
        case FILETYPE_2:
            FileType2 * f2 = record->sp->data;
            if (f2->subs[0].offset > (u32) f2) {
                for (s32 i = 0; i < 8; i++) {
                    f2->subs[i].offset = (u32) f2->subs[i].ptr - (u32) f2;
                }
            }
            break;
        case FILETYPE_3:
            FileType3 * f3 = record->sp->data;
            if (f3->p.offset > (u32) f3) {
                f3->p.offset = (u32) f3->p.ptr - (u32) f3;
            }
            break;
        case FILETYPE_TPL:
            PackTexPalette((TPLHeader *) record->sp->data);
            break;
        case FILETYPE_5:
            
            break;
    }
}
*/

// fileGarbageDataAdrSet

void fileGarbageMoveMem(void * dest, FileRecord * src) {
    // Turn any pointers into the data to offsets 
    if (src->state == 3) {
        if (src->unknown_0xb0 == 0) {
            fileGarbageDataAdrClear(src);
        }
    }
    else {
        fileGarbageDataAdrClear(src);
    }

    // Move data
    memmove(dest, src->sp->data, src->sp->size);
    src->sp->data = dest;

    // Turn offsets back to pointers
    if (src->state == 3) {
        if (src->unknown_0xb0 != 0) {
            src->unknown_0xb0->unknown_0x7c = dest;
        }
        else {
            fileGarbageDataAdrSet(src->sp->data, src->fileType);
        }
    }
    else {
        fileGarbageDataAdrSet(src->sp->data, src->fileType);
    }
}

// _fileGarbage
// fileAllocf
// fileAlloc
// _fileAlloc
// fileFree
// dvdReadDoneCallback
// fileAsyncf
// fileAsync
