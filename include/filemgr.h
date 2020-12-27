/*
    Functions to access files on the game's disc
*/

#ifndef FILEMGR_H
#define FILEMGR_H

#include <common.h>
#include <tpl.h>

#define FILE_RECORD_MAX 1024

typedef struct {
    void * data;
    u32 size;
} DataHolder; // A more general smartalloc thing, maybe?

typedef struct _FileRecord {
    u8 unknown_0x0[0xa4 - 0x0];
    DataHolder * dataHolder;
    struct _FileRecord * next;
    u8 unknown_0xac[0xb4 - 0xac];
} FileRecord;

typedef struct {
    FileRecord * records; // array 1024
    FileRecord * allocatedStart;
    FileRecord * allocatedEnd;
    FileRecord * freeStart;
    FileRecord * freeEnd;
} FileWork;


/*
    Initialise records
*/
void filemgrInit(); // 8017e288

/*
    Takes a TPL file and converts all offsets in it to pointers
*/
void UnpackTexPalette(TPLHeader * palette); // 8019e5a4

void PackTexPalette(TPLHeader * palette); // 8019e6c0
// fileGarbageDataAdrClear 8019e7e0
// fileGarbageDataAdrSet 8019ee2c
void fileGarbageMoveMem(void *, void *); // 8019f498
void _fileGarbage(s32); // 8019f560
FileRecord * fileAllocf(s32, const char * format, ...); // 8019f724
// fileAlloc 8019f7dc
// _fileAlloc 8019f7e4
void fileFree(FileRecord * record); // 8019fa8c
// dvdReadDoneCallback 8019fb38
FileRecord * fileAsyncf(s32, s32, const char * format, ...); // 8019fc5c
// fileAsync 8019fd24

#endif