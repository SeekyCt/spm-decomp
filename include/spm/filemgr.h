/*
    Functions to access files on the game's disc
*/

#pragma once

#include <common.h>
#include <spm/dvdmgr.h>
#include <wii/tpl.h>

#define FILE_ENTRY_MAX 1024

enum FileType
{
    FILETYPE_0, // home button bins, rel (comp & decomp), setup dats, win/card.dat, lyt/title.bin, camera_road.bin
    FILETYPE_1,
    FILETYPE_2,
    FILETYPE_3,
    FILETYPE_TPL, // not used for bg tpls?
    FILETYPE_5, // a/%s (no dash)
    FILETYPE_6,
    FILETYPE_7,
    FILETYPE_8,
    FILETYPE_9,
    FILETYPE_10,
    FILETYPE_11 // eff/%s/%s.dat, eff/%seffdata.dat
};

typedef struct _FileEntry
{
    u8 state;
    s8 fileType;
    s16 touchCnt;
    u8 unknown_0x4[0xa4 - 0x4];
    struct _SmartAllocation * sp; // smart pointer to file data
    struct _FileEntry * next; // next entry in free or allocated list
    u8 unknown_0xac[0xb0 - 0xac];
    DVDEntry * dvdEntry;
} FileEntry;

typedef struct
{
    FileEntry * entries; // array of 1024 (FILE_ENTRY_MAX)
    FileEntry * allocatedStart; // first item in the allocated linked list
    FileEntry * allocatedEnd; // last item in the allocated linked list
    FileEntry * freeStart; // first item in the free linked list
    FileEntry * freeEnd; // last item in the allocated linked list
} FileWork;

/*
    Initialise entries
*/
void fileInit();

/*
    Takes a TPL file and converts all offsets in it to pointers
*/
void UnpackTexPalette(TPLHeader * palette);

/*
    Takes a TPL file and converts all pointers in it back to offsets
*/
void PackTexPalette(TPLHeader * palette);

/*
    Converts self-pointers in file data to offsets to allow safe moving
*/
void fileGarbageDataAdrClear(FileEntry * entry);

/*
    Converts offsets in file data back to self-pointers
*/
void fileGarbageDataAdrSet(void * data, s32 fileType);

/*
    Safely moves memory containing file data, preserving any self-pointers
*/
void fileGarbageMoveMem(void * dest, FileEntry * sourceEntry);

void _fileGarbage(s32);
FileEntry * fileAllocf(s32 fileType, const char * format, ...);
FileEntry * fileAlloc(const char * path, s32 fileType);
FileEntry * _fileAlloc(const char * path, s32 fileType, s32 p3);
void fileFree(FileEntry * entry);
// dvdReadDoneCallback 8019fb38
FileEntry * fileAsyncf(s32 fileType, s32 p2, const char * format, ...);
FileEntry * fileAsync(const char * path, s32 fileType, s32 p3);
