/*
    Functions to access files on the game's disc
*/

#ifndef FILEMGR_H
#define FILEMGR_H

#include <common.h>
#include <tpl.h>

#define FILE_RECORD_MAX 1024

struct _SmartAllocation; // from memory.h (would be a cyclic include)
typedef struct _FileRecord {
    u8 unknown_0x0;
    u8 fileType;
    u8 unknown_0x2[0xa4 - 0x2];
    struct _SmartAllocation * sp; // smart pointer to file data
    struct _FileRecord * next; // next record in free or allocated list
    u8 unknown_0xac[0xb0 - 0xac];
    void * unknown_0xb0;
} FileRecord;

typedef struct {
    FileRecord * records; // array of 1024 (FILE_RECORD_MAX)
    FileRecord * allocatedStart; // first item in the allocated linked list
    FileRecord * allocatedEnd; // last item in the allocated linked list
    FileRecord * freeStart; // first item in the free linked list
    FileRecord * freeEnd; // last item in the allocated linked list
} FileWork;

/*
    Initialise records
*/
void filemgrInit(); // 8017e288

/*
    Takes a TPL file and converts all offsets in it to pointers
*/
void UnpackTexPalette(TPLHeader * palette); // 8019e5a4

/*
    Takes a TPL file and converts all pointers in it back to offsets
*/
void PackTexPalette(TPLHeader * palette); // 8019e6c0

// fileGarbageDataAdrClear 8019e7e0
// fileGarbageDataAdrSet 8019ee2c
void fileGarbageMoveMem(void * dest, FileRecord * record); // 8019f498
void _fileGarbage(s32); // 8019f560
FileRecord * fileAllocf(s32, const char * format, ...); // 8019f724
// fileAlloc 8019f7dc
// _fileAlloc 8019f7e4
void fileFree(FileRecord * record); // 8019fa8c
// dvdReadDoneCallback 8019fb38
FileRecord * fileAsyncf(s32, s32, const char * format, ...); // 8019fc5c
// fileAsync 8019fd24

#endif