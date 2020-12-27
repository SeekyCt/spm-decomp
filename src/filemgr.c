#include <common.h>
#include <filemgr.h>
#include <memory.h>
#include <os.h>
#include <string.h>
#include <tpl.h>

static FileWork fileWork;
static FileWork * afp = &fileWork;

// fileGetWorkPointer inlined, may be needed to match like evtGetWork was

void filemgrInit() {
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
    VALIDATE_TPL_VERSION(palette);

    // Check if the image table field is still an offset and not a pointer (already unpacked otherwise)
    if (IS_TPL_PACKED(palette)) {
        // Convert image table offset to pointer
        palette->imageTable = (ImageTableEntry *) (palette->imageTableOffset + (u32) palette);

        // Unpack all images in table
        for (u16 i = 0; i < palette->imageCount; i++) {
            if (palette->imageTable[i].imageOffset != 0) {
                // Convert image offset to pointer
                palette->imageTable[i].image = (ImageHeader *) ((u32) palette + palette->imageTable[i].imageOffset);

                if (!palette->imageTable[i].image->unpacked) {
                    // Convert image data offset to pointer
                    palette->imageTable[i].image->data = (void *) ((u32) palette + palette->imageTable[i].image->dataOffset);
                    palette->imageTable[i].image->unpacked = true;
                }
            }

            if (palette->imageTable[i].paletteOffset != 0) {
                // Convert palette offset to pointer
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
    VALIDATE_TPL_VERSION(palette);

    if(!IS_TPL_PACKED(palette)) {
        for (u16 i = 0; i < palette->imageCount; i++) {
            if (palette->imageTable[i].image != NULL) {
                if (palette->imageTable[i].image->unpacked) {
                    palette->imageTable[i].image->unpacked = false;
                    palette->imageTable[i].image->dataOffset = (u32) palette->imageTable[i].image->data - (u32) palette;
                }
                palette->imageTable[i].imageOffset = (u32) palette->imageTable[i].image - (u32) palette;
            }

            if (palette->imageTable[i].palette != NULL) {
                if (palette->imageTable[i].palette->unpacked) {
                    palette->imageTable[i].palette->unpacked = false;
                    palette->imageTable[i].palette->dataOffset = (u32) palette->imageTable[i].palette->data - (u32) palette;
                }
                palette->imageTable[i].paletteOffset = (u32) palette->imageTable[i].palette - (u32) palette;
            }
        }
        palette->imageTableOffset = (u32) palette->imageTable - (u32) palette;
    }
}
