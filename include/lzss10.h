/*
    Functions to handle lzss10 compressed data
*/

#ifndef LZSS10_H
#define LZSS10_H

#include <common.h>

// Unsure about a lot of this

// File is new to SPM and with no asserts, so no names are official here

typedef struct
{
    u32 flags;
    u32 decompSize;
} Lzss10ParsedHeader;

/*
    Reads the header of lzss10 compressed data and formats it into the struct
*/
Lzss10ParsedHeader lzss10ParseHeader(const void * file); // 802e270c

/*
    Reads the decompressed size from the header of lzss10 compressed data
*/
size_t lzss10GetDecompSize(const void * file); // 802e2598

/*
    Decompresses lzss10 compressed data into a buffer
*/
void lzss10Decompress(const void * file, void * dest); // 802e25d8

#endif