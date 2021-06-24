/*
    Revolution SDK TPL format
    Documentation used from http://wiki.tockdom.com/wiki/TPL_(File_Format)
*/

#ifndef TPL_H
#define TPL_H

#include <common.h>
#include <os.h>

// Unions are offsets in files and pointers after loading

typedef struct {
    u16 height;
    u16 width;
    u32 format;
    union {
        u32 dataOffset;
        void * data;
    };
    u32 wrapS;
    u32 wrapT;
    u32 minFilter;
    u32 magFilter;
    float LODBias;
    bool edgeLODEnable;
    u8 minLOD;
    u8 maxLOD;
    bool unpacked;
} ImageHeader;

typedef struct {
    u16 entryCount;
    bool unpacked;
    u32 format;
    union {
        u32 dataOffset;
        void * data;
    };
} PaletteHeader;

typedef struct {
    union {
        u32 imageOffset;
        ImageHeader * image;
    };
    union {
        u32 paletteOffset;
        PaletteHeader * palette;
    };
} ImageTableEntry;

typedef struct {
    u32 version; // 0x20af30
    u32 imageCount;
    union {
        u32 imageTableOffset;
        ImageTableEntry * imageTable;
    };
} TPLHeader;

#define IS_TPL_PACKED(tpl) (tpl->imageTableOffset < (u32) tpl) // offset will be a pointer to the table if unpacked
#define VALIDATE_TPL_VERSION(line, palette) \
    if (palette->version != 0x20af30) { \
        OSPanic(__FILE__, line, "invalid version number for texture palette"); \
    }

/*
    Takes a TPL file and converts all offsets in it to pointers
*/
void UnpackTexPalette(TPLHeader * palette); // 802e30ec

#endif