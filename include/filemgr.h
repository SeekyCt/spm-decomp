#ifndef FILEMGR_H
#define FILEMGR_H

#include <common.h>

// Unsure about a lot of this

typedef struct {
    void * data;
    u32 size;
} DataHolder; // A more general smartalloc thing, maybe?

typedef struct {
    u8 unknown_0x0[0xa4 - 0x0];
    DataHolder * dataHolder;
    u8 unknown_0xa8[0xb4 - 0xa8];
} FileRecord;

FileRecord * fileAllocf(s32, const char * format, ...); // 8019f724
void fileFree(FileRecord * record); // 8019fa8c
FileRecord * fileAsyncf(s32, s32, const char * format, ...); // 8019fc5c
void _fileGarbage(s32); // 8019f560

#endif