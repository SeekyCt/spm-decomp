/*
    Revolution SDK TPL format
    Documentation used from http://wiki.tockdom.com/wiki/TPL_(File_Format)
*/

#pragma once

#include <common.h>
#include <wii/os.h>

CPP_WRAPPER(wii::tpl)

// Unions are offsets in files and pointers after loading

enum ImageFormat
{
/* 0x0 */ IMG_FMT_I4,
/* 0x1 */ IMG_FMT_I8,
/* 0x2 */ IMG_FMT_IA4,
/* 0x3 */ IMG_FMT_IA8,
/* 0x4 */ IMG_FMT_RGB565,
/* 0x5*/ IMG_FMT_RGB5A3,
/* 0x6 */ IMG_FMT_RGBA32,
/* 0x7 */ IMG_FMT_7,
/* 0x8 */ IMG_FMT_C4,
/* 0x9 */ IMG_FMT_C8,
/* 0xA */ IMG_FMT_C14X2,
/* 0xB */ IMG_FMT_11,
/* 0xC */ IMG_FMT_12,
/* 0xD */ IMG_FMT_13,
/* 0xE */ IMG_FMT_CMPR
};

typedef struct
{
    u16 height;
    u16 width;
    u32 format;
    union
    {
        u32 dataOffset;
        void * data;
    };
    u32 wrapS;
    u32 wrapT;
    u32 minFilter;
    u32 magFilter;
    f32 LODBias;
    bool edgeLODEnable;
    u8 minLOD;
    u8 maxLOD;
    bool unpacked;
} ImageHeader;
SIZE_ASSERT(ImageHeader, 0x24)

typedef struct
{
    u16 entryCount;
    bool unpacked;
    u32 format;
    union
    {
        u32 dataOffset;
        void * data;
    };
} PaletteHeader;
SIZE_ASSERT(PaletteHeader, 0xc)

typedef struct
{
    union
    {
        u32 imageOffset;
        ImageHeader * image;
    };
    union
    {
        u32 paletteOffset;
        PaletteHeader * palette;
    };
} ImageTableEntry;
SIZE_ASSERT(ImageTableEntry, 0x8)

typedef struct
{
    u32 version; // 0x20af30
    u32 imageCount;
    union
    {
        u32 imageTableOffset;
        ImageTableEntry * imageTable;
    };
} TPLHeader;
SIZE_ASSERT(TPLHeader, 0xc)

// offset will be a pointer to the table if unpacked
#define IS_TPL_PACKED(tpl) (tpl->imageTableOffset < (u32) tpl)

#define VALIDATE_TPL_VERSION(line, palette) \
    if (palette->version != 0x20af30) \
    { \
        OSPanic(__FILE__, line, "invalid version number for texture palette"); \
    }

/*
    Takes a TPL file and converts all offsets in it to pointers
*/
void TPLBind(TPLHeader * palette);

UNKNOWN_FUNCTION(TPLGet);
UNKNOWN_FUNCTION(TPLGetGXTexObjFromPalette);

CPP_WRAPPER_END()
