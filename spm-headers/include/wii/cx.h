/*
    Functions to handle compressed data
*/

#pragma once

#include <common.h>

CPP_WRAPPER(wii::cx)

typedef struct
{
/* 0x0 */ u32 flags;
/* 0x4 */ u32 decompSize;
} CXParsedHeader;
SIZE_ASSERT(CXParsedHeader, 0x8)

/*
    Reads the header of compressed data and formats it into the struct
*/
CXParsedHeader CXGetCompressionHeader(const void * file);

/*
    Reads the decompressed size from the header of compressed data
*/
size_t CXGetUncompressedSize(const void * file);

/*
    Decompresses lzss10 compressed data into a buffer
*/
void CXUncompressLZ(const void * file, void * dest);

CPP_WRAPPER_END()
