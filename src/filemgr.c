/*
    WARNING: Not fully decompiled
    This file is currently not linked into the final dol
*/

#include "spm/animdrv.h"
#include "spm/effdrv.h"
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

typedef struct {
/* 0x000 */ u8 unknown_0x0[0x14c - 0x0];
/* 0x14C */ u32 unknown_0x14c;
/* 0x150 */ u32 unknown_0x150;
/* 0x154 */ u32 unknown_0x154;
/* 0x158 */ u32 unknown_0x158;
/* 0x15C */ u32 unknown_0x15c;
/* 0x160 */ u32 unknown_0x160;
/* 0x164 */ u32 unknown_0x164;
/* 0x168 */ u32 unknown_0x168;
/* 0x16C */ u32 unknown_0x16c;
/* 0x170 */ u32 unknown_0x170;
/* 0x174 */ u32 unknown_0x174;
/* 0x178 */ u32 unknown_0x178;
/* 0x17C */ u32 unknown_0x17c;
/* 0x180 */ u32 unknown_0x180;
/* 0x184 */ u32 unknown_0x184;
/* 0x188 */ u32 unknown_0x188;
/* 0x18C */ u32 unknown_0x18c;
/* 0x190 */ u32 unknown_0x190;
/* 0x194 */ u32 unknown_0x194;
/* 0x198 */ u32 unknown_0x198;
/* 0x19C */ u32 unknown_0x19c;
/* 0x1A0 */ u32 unknown_0x1a0;
/* 0x1A4 */ u32 unknown_0x1a4;
/* 0x1A8 */ u32 unknown_0x1a8;
/* 0x1AC */ u32 unknown_0x1ac;
    // Unknown total size
} UnkFileData1;

typedef struct {
/* 0x00 */ u8 unknown_0x0[0x24 - 0x0];
/* 0x24 */ u32 unknown_0x24; 
/* 0x28 */ u32 unknown_0x28; 
/* 0x2C */ u32 unknown_0x2c; 
/* 0x30 */ u32 unknown_0x30; 
/* 0x34 */ u32 unknown_0x34; 
/* 0x38 */ u32 unknown_0x38; 
/* 0x3C */ u32 unknown_0x3c; 
/* 0x40 */ u32 unknown_0x40; 
    // Unknown total size
} UnkFileData2;

typedef struct {
/* 0x00 */ u8 unkown_0x0[0x48 - 0x0];
/* 0x48 */ u32 unknown_0x48;
    // Unknown total size
} UnkFileData3;

typedef struct {
/* 0x00 */ u8 unkown_0x0[0x48 - 0x0];
/* 0x48 */ u32 unknown_0x48;
    // Unknown total size
} UnkFileData6;

typedef struct {
/* 0x00 */ u8 unkown_0x0[0x64 - 0x0];
/* 0x64 */ u32 unknown_0x64;
/* 0x68 */ u32 unknown_0x68;
/* 0x6C */ u32 unknown_0x6c;
    // Unknown total size
} UnkFileData7;

typedef struct {
/* 0x000 */ u8 unknown_0x0[0xc8 - 0x0];
/* 0x0c8 */ u32 unknown_0xc8;
/* 0x0cc */ u32 unknown_0xcc;
/* 0x0d0 */ u32 unknown_0xd0;
/* 0x0d4 */ u32 unknown_0xd4;
/* 0x0d8 */ u32 unknown_0xd8;
/* 0x0dc */ u32 unknown_0xdc;
/* 0x0e0 */ u32 unknown_0xe0;
/* 0x0e4 */ u32 unknown_0xe4;
/* 0x0e8 */ u32 unknown_0xe8;
/* 0x0ec */ u32 unknown_0xec;
/* 0x0f0 */ u32 unknown_0xf0;
/* 0x0f4 */ u32 unknown_0xf4;
/* 0x0f8 */ u32 unknown_0xf8;
/* 0x0fc */ u32 unknown_0xfc;
/* 0x100 */ u32 unknown_0x100;
/* 0x104 */ u32 unknown_0x104;
/* 0x108 */ u32 unknown_0x108;
/* 0x10c */ u32 unknown_0x10c;
/* 0x110 */ u32 unknown_0x110;
/* 0x114 */ u32 unknown_0x114;
/* 0x118 */ u32 unknown_0x118;
/* 0x11c */ u32 unknown_0x11c;
/* 0x120 */ u32 unknown_0x120;
    // Unknown total size
} UnkFileData8;

typedef struct {
/* 0x00 */ u8 unknown_0x0[0x58 - 0x0];
/* 0x58 */ u32 unknown_0x58;
/* 0x5c */ u32 unknown_0x5c;
/* 0x60 */ u32 unknown_0x60;
/* 0x64 */ u32 unknown_0x64;
    // Unknown total size
} UnkFileData9;

typedef struct {
/* 0x00 */ u8 unknown_0x0[0x48 - 0x0];
/* 0x48 */ u32 unknown_0x48;
    // Unknown total size
} UnkFileData10;

#define IS_RELOCATED(field, base) ((u32)(field) >= (u32)(base))
#define IS_NOT_RELOCATED(field, base) ((u32)(field) < (u32)(base))
#define APPLY_RELOC(field, base) ((u32)(field) + (u32)(base))
#define CLEAR_RELOC(field, base) ((u32)(field) - (u32)(base))

static void fileGarbageDataAdrClear(FileEntry * entry)
{
    void * data = entry->sp->data;
    switch(entry->fileType)
    {
        case FILETYPE_1:
        {
            UnkFileData1 * unk1 = (UnkFileData1 *)data;
            // Skip if not already relocated
            if (IS_NOT_RELOCATED(unk1->unknown_0x14c, unk1))
                return;

            unk1->unknown_0x14c = CLEAR_RELOC(unk1->unknown_0x14c, unk1);
            unk1->unknown_0x150 = CLEAR_RELOC(unk1->unknown_0x150, unk1);
            unk1->unknown_0x154 = CLEAR_RELOC(unk1->unknown_0x154, unk1);
            unk1->unknown_0x158 = CLEAR_RELOC(unk1->unknown_0x158, unk1);
            unk1->unknown_0x15c = CLEAR_RELOC(unk1->unknown_0x15c, unk1);
            unk1->unknown_0x160 = CLEAR_RELOC(unk1->unknown_0x160, unk1);
            unk1->unknown_0x164 = CLEAR_RELOC(unk1->unknown_0x164, unk1);
            unk1->unknown_0x168 = CLEAR_RELOC(unk1->unknown_0x168, unk1);
            unk1->unknown_0x16c = CLEAR_RELOC(unk1->unknown_0x16c, unk1);
            unk1->unknown_0x170 = CLEAR_RELOC(unk1->unknown_0x170, unk1);
            unk1->unknown_0x174 = CLEAR_RELOC(unk1->unknown_0x174, unk1);
            unk1->unknown_0x178 = CLEAR_RELOC(unk1->unknown_0x178, unk1);
            unk1->unknown_0x17c = CLEAR_RELOC(unk1->unknown_0x17c, unk1);
            unk1->unknown_0x180 = CLEAR_RELOC(unk1->unknown_0x180, unk1);
            unk1->unknown_0x184 = CLEAR_RELOC(unk1->unknown_0x184, unk1);
            unk1->unknown_0x188 = CLEAR_RELOC(unk1->unknown_0x188, unk1);
            unk1->unknown_0x18c = CLEAR_RELOC(unk1->unknown_0x18c, unk1);
            unk1->unknown_0x190 = CLEAR_RELOC(unk1->unknown_0x190, unk1);
            unk1->unknown_0x194 = CLEAR_RELOC(unk1->unknown_0x194, unk1);
            unk1->unknown_0x198 = CLEAR_RELOC(unk1->unknown_0x198, unk1);
            unk1->unknown_0x19c = CLEAR_RELOC(unk1->unknown_0x19c, unk1);
            unk1->unknown_0x1a0 = CLEAR_RELOC(unk1->unknown_0x1a0, unk1);
            unk1->unknown_0x1a4 = CLEAR_RELOC(unk1->unknown_0x1a4, unk1);
            unk1->unknown_0x1a8 = CLEAR_RELOC(unk1->unknown_0x1a8, unk1);
            unk1->unknown_0x1ac = CLEAR_RELOC(unk1->unknown_0x1ac, unk1);
            return;
        }

        case FILETYPE_2:
        {
            UnkFileData2 * unk2 = (UnkFileData2 *)data;
            if (IS_NOT_RELOCATED(unk2->unknown_0x24, data))
                return;

            unk2->unknown_0x24 = CLEAR_RELOC(unk2->unknown_0x24, unk2);
            unk2->unknown_0x28 = CLEAR_RELOC(unk2->unknown_0x28, unk2);
            unk2->unknown_0x2c = CLEAR_RELOC(unk2->unknown_0x2c, unk2);
            unk2->unknown_0x30 = CLEAR_RELOC(unk2->unknown_0x30, unk2);
            unk2->unknown_0x34 = CLEAR_RELOC(unk2->unknown_0x34, unk2);
            unk2->unknown_0x38 = CLEAR_RELOC(unk2->unknown_0x38, unk2);
            unk2->unknown_0x3c = CLEAR_RELOC(unk2->unknown_0x3c, unk2);
            unk2->unknown_0x40 = CLEAR_RELOC(unk2->unknown_0x40, unk2);
            return;
        }

        case FILETYPE_3:
        {
            UnkFileData3 * unk3 = (UnkFileData3 *) data;
            if (IS_NOT_RELOCATED(unk3->unknown_0x48, unk3))
                return;

            unk3->unknown_0x48 = CLEAR_RELOC(unk3->unknown_0x48, unk3);
            return;
        }

        case FILETYPE_TPL:
            PackTexPalette((TPLHeader *) entry->sp->data);
            return;

        case FILETYPE_ANIMPOSE:
        {
            AnimPoseData * pose = (AnimPoseData *) data;
            if (IS_NOT_RELOCATED(pose->shapes, pose))
                return;

            s32 animCount = pose->animCount;
            for (s32 i = 0; i < animCount; i++)
            {
                AnimPoseData_AnimData * animData = pose->anims[i].data;
                pose->anims[i].data = (AnimPoseData_AnimData *) CLEAR_RELOC(pose->anims[i].data, pose);
                if (IS_NOT_RELOCATED(animData->loopData, animData))
                    continue;

                animData->loopData = (void *) CLEAR_RELOC(animData->loopData, animData);
                animData->keyframes = (void *) CLEAR_RELOC(animData->keyframes, animData);
                animData->vertexPositionDeltas = (void *) CLEAR_RELOC(animData->vertexPositionDeltas, animData);
                animData->vertexNormalDeltras = (void *) CLEAR_RELOC(animData->vertexNormalDeltras, animData);
                animData->textureCoordinateTransformDeltas = (void *) CLEAR_RELOC(animData->textureCoordinateTransformDeltas, animData);
                animData->visibilityGroupDeltas = (void *) CLEAR_RELOC(animData->visibilityGroupDeltas, animData);
                animData->groupTransformDataDeltas = (void *) CLEAR_RELOC(animData->groupTransformDataDeltas, animData);
                animData->animDataType8Data = (void *) CLEAR_RELOC(animData->animDataType8Data, animData);
            }

            pose->shapes = (void *) CLEAR_RELOC(pose->shapes, pose);
            pose->polygons = (void *) CLEAR_RELOC(pose->polygons, pose);
            pose->vertexPositions = (void *) CLEAR_RELOC(pose->vertexPositions, pose);
            pose->vertexPositionIndices = (void *) CLEAR_RELOC(pose->vertexPositionIndices, pose);
            pose->vertexNormals = (void *) CLEAR_RELOC(pose->vertexNormals, pose);
            pose->vertexNormalIndices = (void *) CLEAR_RELOC(pose->vertexNormalIndices, pose);
            pose->vertexColors = (void *) CLEAR_RELOC(pose->vertexColors, pose);
            pose->vertexColorIndices = (void *) CLEAR_RELOC(pose->vertexColorIndices, pose);
            pose->vertexTextureCoordinate0Indices = (void *) CLEAR_RELOC(pose->vertexTextureCoordinate0Indices, pose);
            pose->vertexTextureCoordinate1Indices = (void *) CLEAR_RELOC(pose->vertexTextureCoordinate1Indices, pose);
            pose->vertexTextureCoordinate2Indices = (void *) CLEAR_RELOC(pose->vertexTextureCoordinate2Indices, pose);
            pose->vertexTextureCoordinate3Indices = (void *) CLEAR_RELOC(pose->vertexTextureCoordinate3Indices, pose);
            pose->vertexTextureCoordinate4Indices = (void *) CLEAR_RELOC(pose->vertexTextureCoordinate4Indices, pose);
            pose->vertexTextureCoordinate5Indices = (void *) CLEAR_RELOC(pose->vertexTextureCoordinate5Indices, pose);
            pose->vertexTextureCoordinate6Indices = (void *) CLEAR_RELOC(pose->vertexTextureCoordinate6Indices, pose);
            pose->vertexTextureCoordinate7Indices = (void *) CLEAR_RELOC(pose->vertexTextureCoordinate7Indices, pose);
            pose->vertexTextureCoordinates = (void *) CLEAR_RELOC(pose->vertexTextureCoordinates, pose);
            pose->textureCoordinateTransforms = (void *) CLEAR_RELOC(pose->textureCoordinateTransforms, pose);
            pose->samplers = (void *) CLEAR_RELOC(pose->samplers, pose);
            pose->textures = (void *) CLEAR_RELOC(pose->textures, pose);
            pose->subshapes = (void *) CLEAR_RELOC(pose->subshapes, pose);
            pose->visibilityGroups = (void *) CLEAR_RELOC(pose->visibilityGroups, pose);
            pose->groupTransformData = (void *) CLEAR_RELOC(pose->groupTransformData, pose);
            pose->groups = (void *) CLEAR_RELOC(pose->groups, pose);
            pose->anims = (AnimPoseData_AnimTableEntry *) CLEAR_RELOC(pose->anims, pose);
            return;
        }
        case FILETYPE_6:
        {
            UnkFileData6 * unk6 = (UnkFileData6 *) data;
            if (IS_NOT_RELOCATED(unk6->unknown_0x48, data))
                return;

            unk6->unknown_0x48 = CLEAR_RELOC(unk6->unknown_0x48, unk6);
            return;
        }

        case FILETYPE_7:
        {
            UnkFileData7 * unk7 = (UnkFileData7 *) data;
            if (IS_NOT_RELOCATED(unk7->unknown_0x64, unk7))
                return;

            unk7->unknown_0x64 = CLEAR_RELOC(unk7->unknown_0x64, unk7);
            unk7->unknown_0x68 = CLEAR_RELOC(unk7->unknown_0x68, unk7);
            unk7->unknown_0x6c = CLEAR_RELOC(unk7->unknown_0x6c, unk7);
            return;
        }

        case FILETYPE_8:
        {
            UnkFileData8 * unk8 = (UnkFileData8 *) data;
            if (IS_NOT_RELOCATED(unk8->unknown_0xc8, unk8))
                return;

            unk8->unknown_0xc8 = CLEAR_RELOC(unk8->unknown_0xc8, unk8);
            unk8->unknown_0xcc = CLEAR_RELOC(unk8->unknown_0xcc, unk8);
            unk8->unknown_0xd0 = CLEAR_RELOC(unk8->unknown_0xd0, unk8);
            unk8->unknown_0xd4 = CLEAR_RELOC(unk8->unknown_0xd4, unk8);
            unk8->unknown_0xd8 = CLEAR_RELOC(unk8->unknown_0xd8, unk8);
            unk8->unknown_0xdc = CLEAR_RELOC(unk8->unknown_0xdc, unk8);
            unk8->unknown_0xe0 = CLEAR_RELOC(unk8->unknown_0xe0, unk8);
            unk8->unknown_0xe4 = CLEAR_RELOC(unk8->unknown_0xe4, unk8);
            unk8->unknown_0xe8 = CLEAR_RELOC(unk8->unknown_0xe8, unk8);
            unk8->unknown_0xec = CLEAR_RELOC(unk8->unknown_0xec, unk8);
            unk8->unknown_0xf0 = CLEAR_RELOC(unk8->unknown_0xf0, unk8);
            unk8->unknown_0xf4 = CLEAR_RELOC(unk8->unknown_0xf4, unk8);
            unk8->unknown_0xf8 = CLEAR_RELOC(unk8->unknown_0xf8, unk8);
            unk8->unknown_0xfc = CLEAR_RELOC(unk8->unknown_0xfc, unk8);
            unk8->unknown_0x100 = CLEAR_RELOC(unk8->unknown_0x100, unk8);
            unk8->unknown_0x104 = CLEAR_RELOC(unk8->unknown_0x104, unk8);
            unk8->unknown_0x108 = CLEAR_RELOC(unk8->unknown_0x108, unk8);
            unk8->unknown_0x10c = CLEAR_RELOC(unk8->unknown_0x10c, unk8);
            unk8->unknown_0x110 = CLEAR_RELOC(unk8->unknown_0x110, unk8);
            unk8->unknown_0x114 = CLEAR_RELOC(unk8->unknown_0x114, unk8);
            unk8->unknown_0x118 = CLEAR_RELOC(unk8->unknown_0x118, unk8);
            unk8->unknown_0x11c = CLEAR_RELOC(unk8->unknown_0x11c, unk8);
            unk8->unknown_0x120 = CLEAR_RELOC(unk8->unknown_0x120, unk8);
            return;
        }

        case FILETYPE_9:
        {
            UnkFileData9 * unk9 = (UnkFileData9 *) data;
            if (IS_NOT_RELOCATED(unk9->unknown_0x58, unk9))
                return;

            unk9->unknown_0x58 = CLEAR_RELOC(unk9->unknown_0x58, unk9);
            unk9->unknown_0x5c = CLEAR_RELOC(unk9->unknown_0x5c, unk9);
            unk9->unknown_0x60 = CLEAR_RELOC(unk9->unknown_0x60, unk9);
            unk9->unknown_0x64 = CLEAR_RELOC(unk9->unknown_0x64, unk9);
            return;
        }

        case FILETYPE_10:
        {
            UnkFileData10 * unk10 = (UnkFileData10 *) data;
            if (IS_NOT_RELOCATED(unk10->unknown_0x48, unk10))
                return;

            unk10->unknown_0x48 = CLEAR_RELOC(unk10->unknown_0x48, unk10);
            return;
        }

        case FILETYPE_EFFDAT:
        {
            EffDataHeader * eff = (EffDataHeader *) data;
            if (IS_NOT_RELOCATED(eff->offsets[0], eff))
                return;

            for (int i = 0; i < 16; i++)
                eff->offsets[i] = (void *) CLEAR_RELOC(eff->offsets[i], eff);
            return;
        }
    }
}

static void fileGarbageDataAdrSet(void * data, s32 fileType)
{
    switch(fileType)
    {
        case FILETYPE_1:
        {
            UnkFileData1 * unk1 = (UnkFileData1 *)data;
            // Skip if not already relocated
            if (IS_RELOCATED(unk1->unknown_0x14c, unk1))
                return;

            unk1->unknown_0x14c = APPLY_RELOC(unk1->unknown_0x14c, unk1);
            unk1->unknown_0x150 = APPLY_RELOC(unk1->unknown_0x150, unk1);
            unk1->unknown_0x154 = APPLY_RELOC(unk1->unknown_0x154, unk1);
            unk1->unknown_0x158 = APPLY_RELOC(unk1->unknown_0x158, unk1);
            unk1->unknown_0x15c = APPLY_RELOC(unk1->unknown_0x15c, unk1);
            unk1->unknown_0x160 = APPLY_RELOC(unk1->unknown_0x160, unk1);
            unk1->unknown_0x164 = APPLY_RELOC(unk1->unknown_0x164, unk1);
            unk1->unknown_0x168 = APPLY_RELOC(unk1->unknown_0x168, unk1);
            unk1->unknown_0x16c = APPLY_RELOC(unk1->unknown_0x16c, unk1);
            unk1->unknown_0x170 = APPLY_RELOC(unk1->unknown_0x170, unk1);
            unk1->unknown_0x174 = APPLY_RELOC(unk1->unknown_0x174, unk1);
            unk1->unknown_0x178 = APPLY_RELOC(unk1->unknown_0x178, unk1);
            unk1->unknown_0x17c = APPLY_RELOC(unk1->unknown_0x17c, unk1);
            unk1->unknown_0x180 = APPLY_RELOC(unk1->unknown_0x180, unk1);
            unk1->unknown_0x184 = APPLY_RELOC(unk1->unknown_0x184, unk1);
            unk1->unknown_0x188 = APPLY_RELOC(unk1->unknown_0x188, unk1);
            unk1->unknown_0x18c = APPLY_RELOC(unk1->unknown_0x18c, unk1);
            unk1->unknown_0x190 = APPLY_RELOC(unk1->unknown_0x190, unk1);
            unk1->unknown_0x194 = APPLY_RELOC(unk1->unknown_0x194, unk1);
            unk1->unknown_0x198 = APPLY_RELOC(unk1->unknown_0x198, unk1);
            unk1->unknown_0x19c = APPLY_RELOC(unk1->unknown_0x19c, unk1);
            unk1->unknown_0x1a0 = APPLY_RELOC(unk1->unknown_0x1a0, unk1);
            unk1->unknown_0x1a4 = APPLY_RELOC(unk1->unknown_0x1a4, unk1);
            unk1->unknown_0x1a8 = APPLY_RELOC(unk1->unknown_0x1a8, unk1);
            unk1->unknown_0x1ac = APPLY_RELOC(unk1->unknown_0x1ac, unk1);
            return;
        }

        case FILETYPE_2:
        {
            UnkFileData2 * unk2 = (UnkFileData2 *)data;
            if (IS_RELOCATED(unk2->unknown_0x24, data))
                return;

            unk2->unknown_0x24 = APPLY_RELOC(unk2->unknown_0x24, unk2);
            unk2->unknown_0x28 = APPLY_RELOC(unk2->unknown_0x28, unk2);
            unk2->unknown_0x2c = APPLY_RELOC(unk2->unknown_0x2c, unk2);
            unk2->unknown_0x30 = APPLY_RELOC(unk2->unknown_0x30, unk2);
            unk2->unknown_0x34 = APPLY_RELOC(unk2->unknown_0x34, unk2);
            unk2->unknown_0x38 = APPLY_RELOC(unk2->unknown_0x38, unk2);
            unk2->unknown_0x3c = APPLY_RELOC(unk2->unknown_0x3c, unk2);
            unk2->unknown_0x40 = APPLY_RELOC(unk2->unknown_0x40, unk2);
            return;
        }

        case FILETYPE_3:
        {
            UnkFileData3 * unk3 = (UnkFileData3 *) data;
            if (IS_RELOCATED(unk3->unknown_0x48, unk3))
                return;

            unk3->unknown_0x48 = APPLY_RELOC(unk3->unknown_0x48, unk3);
            return;
        }

        case FILETYPE_TPL:
            UnpackTexPalette((TPLHeader *) data);
            return;

        case FILETYPE_ANIMPOSE:
        {
            AnimPoseData * pose = (AnimPoseData *) data;
            if (IS_RELOCATED(pose->shapes, pose))
                return;

            pose->shapes = (void *) APPLY_RELOC(pose->shapes, pose);
            pose->polygons = (void *) APPLY_RELOC(pose->polygons, pose);
            pose->vertexPositions = (void *) APPLY_RELOC(pose->vertexPositions, pose);
            pose->vertexPositionIndices = (void *) APPLY_RELOC(pose->vertexPositionIndices, pose);
            pose->vertexNormals = (void *) APPLY_RELOC(pose->vertexNormals, pose);
            pose->vertexNormalIndices = (void *) APPLY_RELOC(pose->vertexNormalIndices, pose);
            pose->vertexColors = (void *) APPLY_RELOC(pose->vertexColors, pose);
            pose->vertexColorIndices = (void *) APPLY_RELOC(pose->vertexColorIndices, pose);
            pose->vertexTextureCoordinate0Indices = (void *) APPLY_RELOC(pose->vertexTextureCoordinate0Indices, pose);
            pose->vertexTextureCoordinate1Indices = (void *) APPLY_RELOC(pose->vertexTextureCoordinate1Indices, pose);
            pose->vertexTextureCoordinate2Indices = (void *) APPLY_RELOC(pose->vertexTextureCoordinate2Indices, pose);
            pose->vertexTextureCoordinate3Indices = (void *) APPLY_RELOC(pose->vertexTextureCoordinate3Indices, pose);
            pose->vertexTextureCoordinate4Indices = (void *) APPLY_RELOC(pose->vertexTextureCoordinate4Indices, pose);
            pose->vertexTextureCoordinate5Indices = (void *) APPLY_RELOC(pose->vertexTextureCoordinate5Indices, pose);
            pose->vertexTextureCoordinate6Indices = (void *) APPLY_RELOC(pose->vertexTextureCoordinate6Indices, pose);
            pose->vertexTextureCoordinate7Indices = (void *) APPLY_RELOC(pose->vertexTextureCoordinate7Indices, pose);
            pose->vertexTextureCoordinates = (void *) APPLY_RELOC(pose->vertexTextureCoordinates, pose);
            pose->textureCoordinateTransforms = (void *) APPLY_RELOC(pose->textureCoordinateTransforms, pose);
            pose->samplers = (void *) APPLY_RELOC(pose->samplers, pose);
            pose->textures = (void *) APPLY_RELOC(pose->textures, pose);
            pose->subshapes = (void *) APPLY_RELOC(pose->subshapes, pose);
            pose->visibilityGroups = (void *) APPLY_RELOC(pose->visibilityGroups, pose);
            pose->groupTransformData = (void *) APPLY_RELOC(pose->groupTransformData, pose);
            pose->groups = (void *) APPLY_RELOC(pose->groups, pose);
            pose->anims = (AnimPoseData_AnimTableEntry *) APPLY_RELOC(pose->anims, pose);

            s32 animCount = pose->animCount;
            for (s32 i = 0; i < animCount; i++)
            {
                AnimPoseData_AnimTableEntry * anim = pose->anims + i;
                anim->data = (AnimPoseData_AnimData *) ((u32) anim->data + (u32) pose);

                AnimPoseData_AnimData * animData = pose->anims[i].data;
                if (IS_RELOCATED(animData->loopData, animData))
                    continue;

                animData->loopData = (void *) APPLY_RELOC(animData->loopData, animData);
                animData->keyframes = (void *) APPLY_RELOC(animData->keyframes, animData);
                animData->vertexPositionDeltas = (void *) APPLY_RELOC(animData->vertexPositionDeltas, animData);
                animData->vertexNormalDeltras = (void *) APPLY_RELOC(animData->vertexNormalDeltras, animData);
                animData->textureCoordinateTransformDeltas = (void *) APPLY_RELOC(animData->textureCoordinateTransformDeltas, animData);
                animData->visibilityGroupDeltas = (void *) APPLY_RELOC(animData->visibilityGroupDeltas, animData);
                animData->groupTransformDataDeltas = (void *) APPLY_RELOC(animData->groupTransformDataDeltas, animData);
                animData->animDataType8Data = (void *) APPLY_RELOC(animData->animDataType8Data, animData);
            }

            return;
        }
        case FILETYPE_6:
        {
            UnkFileData6 * unk6 = (UnkFileData6 *) data;
            if (IS_RELOCATED(unk6->unknown_0x48, data))
                return;

            unk6->unknown_0x48 = APPLY_RELOC(unk6->unknown_0x48, unk6);
            return;
        }

        case FILETYPE_7:
        {
            UnkFileData7 * unk7 = (UnkFileData7 *) data;
            if (IS_RELOCATED(unk7->unknown_0x64, unk7))
                return;

            unk7->unknown_0x64 = APPLY_RELOC(unk7->unknown_0x64, unk7);
            unk7->unknown_0x68 = APPLY_RELOC(unk7->unknown_0x68, unk7);
            unk7->unknown_0x6c = APPLY_RELOC(unk7->unknown_0x6c, unk7);
            return;
        }

        case FILETYPE_8:
        {
            UnkFileData8 * unk8 = (UnkFileData8 *) data;
            if (IS_RELOCATED(unk8->unknown_0xc8, unk8))
                return;

            unk8->unknown_0xc8 = APPLY_RELOC(unk8->unknown_0xc8, unk8);
            unk8->unknown_0xcc = APPLY_RELOC(unk8->unknown_0xcc, unk8);
            unk8->unknown_0xd0 = APPLY_RELOC(unk8->unknown_0xd0, unk8);
            unk8->unknown_0xd4 = APPLY_RELOC(unk8->unknown_0xd4, unk8);
            unk8->unknown_0xd8 = APPLY_RELOC(unk8->unknown_0xd8, unk8);
            unk8->unknown_0xdc = APPLY_RELOC(unk8->unknown_0xdc, unk8);
            unk8->unknown_0xe0 = APPLY_RELOC(unk8->unknown_0xe0, unk8);
            unk8->unknown_0xe4 = APPLY_RELOC(unk8->unknown_0xe4, unk8);
            unk8->unknown_0xe8 = APPLY_RELOC(unk8->unknown_0xe8, unk8);
            unk8->unknown_0xec = APPLY_RELOC(unk8->unknown_0xec, unk8);
            unk8->unknown_0xf0 = APPLY_RELOC(unk8->unknown_0xf0, unk8);
            unk8->unknown_0xf4 = APPLY_RELOC(unk8->unknown_0xf4, unk8);
            unk8->unknown_0xf8 = APPLY_RELOC(unk8->unknown_0xf8, unk8);
            unk8->unknown_0xfc = APPLY_RELOC(unk8->unknown_0xfc, unk8);
            unk8->unknown_0x100 = APPLY_RELOC(unk8->unknown_0x100, unk8);
            unk8->unknown_0x104 = APPLY_RELOC(unk8->unknown_0x104, unk8);
            unk8->unknown_0x108 = APPLY_RELOC(unk8->unknown_0x108, unk8);
            unk8->unknown_0x10c = APPLY_RELOC(unk8->unknown_0x10c, unk8);
            unk8->unknown_0x110 = APPLY_RELOC(unk8->unknown_0x110, unk8);
            unk8->unknown_0x114 = APPLY_RELOC(unk8->unknown_0x114, unk8);
            unk8->unknown_0x118 = APPLY_RELOC(unk8->unknown_0x118, unk8);
            unk8->unknown_0x11c = APPLY_RELOC(unk8->unknown_0x11c, unk8);
            unk8->unknown_0x120 = APPLY_RELOC(unk8->unknown_0x120, unk8);
            return;
        }

        case FILETYPE_9:
        {
            UnkFileData9 * unk9 = (UnkFileData9 *) data;
            if (IS_RELOCATED(unk9->unknown_0x58, unk9))
                return;

            unk9->unknown_0x58 = APPLY_RELOC(unk9->unknown_0x58, unk9);
            unk9->unknown_0x5c = APPLY_RELOC(unk9->unknown_0x5c, unk9);
            unk9->unknown_0x60 = APPLY_RELOC(unk9->unknown_0x60, unk9);
            unk9->unknown_0x64 = APPLY_RELOC(unk9->unknown_0x64, unk9);
            return;
        }

        case FILETYPE_10:
        {
            UnkFileData10 * unk10 = (UnkFileData10 *) data;
            if (IS_RELOCATED(unk10->unknown_0x48, unk10))
                return;

            unk10->unknown_0x48 = APPLY_RELOC(unk10->unknown_0x48, unk10);
            return;
        }

        case FILETYPE_EFFDAT:
        {
            EffDataHeader * eff = (EffDataHeader *) data;
            if (IS_RELOCATED(eff->offsets[0], eff))
                return;

            for (int i = 0; i < 16; i++)
            {
                void * temp = eff->offsets[i];
                eff->offsets[i] = (void *) APPLY_RELOC(temp, eff);
            }
            return;
        }
    }
}

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

static FileEntry * _fileAlloc(const char * path, s32 fileType, s32 unused);

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

static FileEntry * _fileAlloc(const char * path, s32 fileType, s32 unused)
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
    if (new_lp == NULL)
    {
        _fileGarbage(1);
        new_lp = afp->freeStart;
    }
    if (new_lp == NULL)
    {
        _fileGarbage(0);
        new_lp = afp->freeStart;
    }
    // "File list is missing"
    assert(917, new_lp, "ファイルリストが足りない\n");

    // Backup next pointer
    FileEntry * newFreeStart = new_lp->next;

    // Check file exists
    DVDEntry * dvdEntry = DVDMgrOpen(path, 2, 0);
    if (dvdEntry == NULL)
        return NULL;

    // Check file isn't empty
    s32 length = DVDMgrGetLength(dvdEntry);
    u32 roundedLength = (length + 0x1f) & 0xffffffe0;
    if (roundedLength == 0)
    {
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
    if (afp->freeStart == NULL)
        afp->freeEnd = NULL;

    // Insert into allocated list
    if (afp->allocatedStart == NULL)
    {
        afp->allocatedStart = new_lp;
        afp->allocatedEnd = new_lp;
    }
    else
    {
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
    if (lp->state == FILE_ALLOC_CALLED)
    {
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

static void dvdReadDoneCallback(s32 result, DVDFileInfo * fp)
{
    // Find the entry for this file
    int i;
    FileEntry *lp;
    for (i = 0, lp = afp->allocatedStart; lp != NULL; i++, lp = lp->next)
    {
        if (&lp->dvdEntry->fileInfo == fp)
            break;
    }
    DVDEntry * dvdEntry = lp->dvdEntry;
    assertf(1076, lp, "listが見つからなかった\ni=%d fp=%x useBegin=%x\n", i, fp, afp->allocatedStart);

    // Check read was successful
    if (result == DVD_RESULT_FATAL_ERROR)
        _assert(1080, !"致命的なエラー DVD_RESULT_FATAL_ERROR \n");

    if (result == DVD_RESULT_CANCELED)
    {
        lp->state = FILE_WAITING_GARBAGE;
        lp->fileType = FILETYPE_12; // FILETYPE_WAITING_GARBAGE?
        lp->dvdEntry = NULL;
        DVDMgrClose(dvdEntry);
    }
    else
    {
        lp->dvdEntry = NULL;
        fileGarbageDataAdrSet(lp->sp->data, lp->fileType);
        DVDMgrClose(dvdEntry);
        if (lp->readDoneCb != NULL)
            lp->readDoneCb(lp);
    }
}

FileEntry * fileAsyncf(s32 fileType, FilemgrCallback * readDoneCb, const char * format, ...)
{
    va_list args;

    va_start(args, format);
    vsprintf(PathBuffer, format, args);
    va_end(args);

    return fileAsync(PathBuffer, fileType, readDoneCb);
}

FileEntry * fileAsync(const char * path, s32 fileType, FilemgrCallback * readDoneCb)
{
    if (DVDConvertPathToEntrynum(path) == -1)
        return (FileEntry *)-1;

    // Try find existing entry
    for (FileEntry * lp = afp->allocatedStart; lp != NULL; lp = lp->next)
    {
        // Check if name matches
        if (lp->state == FILE_EMPTY || strcmp(lp->path, path) != 0)
            continue;

        // "The file type is different from before"
        assertf(1158, lp->fileType == fileType, "ファイルタイプが以前と違います[%s]\n", path);

        // Wait for reading to finish
        if (lp->dvdEntry != NULL)
            return NULL;

        if (lp->state == FILE_ASYNC_CALLED)
            return lp;
    
        if (lp->state == FILE_WAITING_GARBAGE)
            lp->state = FILE_ASYNC_CALLED;

        return lp;
    }

    // Try get a free entry
    FileEntry * new_lp = afp->freeStart; // r31
    if (new_lp == NULL)
    {
        _fileGarbage(1);
        new_lp = afp->freeStart;
    }
    if (new_lp == NULL)
    {
        _fileGarbage(0);
        new_lp = afp->freeStart;
    }
    // "File list is missing"
    assert(1196, new_lp, "ファイルリストが足りない\n");

    // Backup next pointer
    FileEntry * newFreeStart = new_lp->next;
    
    // Check file exists
    DVDEntry * dvdEntry = DVDMgrOpen(path, 2, 0);
    if (dvdEntry == NULL)
        return (FileEntry *)-1;

    // Check file isn't empty
    s32 length = DVDMgrGetLength(dvdEntry);
    u32 roundedLength = (length + 0x1f) & 0xffffffe0;
    if (roundedLength == 0)
    {
        DVDMgrClose(dvdEntry);
        return NULL;
    }
    DVDMgrClose(dvdEntry);

    // No effect, length is already aligned
    roundedLength = (roundedLength + 0x1f) & 0xffffffe0;

    // Create space on smart heap
    SmartAllocation * new_sp = smartAlloc(roundedLength, 0);
    memset(new_sp->data, 0, new_sp->size);
    // Memory allocation failure
    assert(1251, new_sp, "メモリ確保に失敗\n");
    new_sp->fileEntry = new_lp;

    // Update file entry
    new_lp->sp = new_sp;
    new_lp->state = FILE_ASYNC_CALLED;
    new_lp->touchCnt = 0;
    new_lp->fileType = (s8) fileType;
    new_lp->next = NULL;
    new_lp->readDoneCb = readDoneCb;
    new_lp->length = (u32) length;
    strcpy(new_lp->path, path);

    // Remove from free list
    afp->freeStart = newFreeStart;
    if (afp->freeStart == NULL)
        afp->freeEnd = NULL;

    // Insert into allocated list
    if (afp->allocatedStart == NULL)
    {
        afp->allocatedStart = new_lp;
        afp->allocatedEnd = new_lp;
    }
    else
    {
        afp->allocatedEnd->next = new_lp;
        afp->allocatedEnd = new_lp;
    }

    // Start async read
    dvdEntry = DVDMgrOpen(path, 2, 0);
    new_lp->dvdEntry = dvdEntry;
    DVDMgrReadAsync(dvdEntry, new_sp->data, (s32)roundedLength,  0,  dvdReadDoneCallback);
    return NULL;
}

// To add string to pool
void unkStriped();
void unkStriped()
{
    __assert(NULL, -1, "lp->sp");
}

}
