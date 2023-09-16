/*
    Functions to access files on the game's disc
*/

#pragma once

#include <common.h>
#include <spm/dvdmgr.h>
#include <wii/tpl.h>

CPP_WRAPPER(spm::memory)

struct _SmartAllocation;

CPP_WRAPPER_END()

CPP_WRAPPER(spm::filemgr)

USING(spm::dvdmgr::DVDEntry)
USING(spm::memory::_SmartAllocation)
USING(wii::dvd::DVDFileInfo)
USING(wii::tpl::TPLHeader)

#define FILE_ENTRY_MAX 1024

enum FileType
{
/* 0x0 */ FILETYPE_0, // home button bins, rel (comp & decomp), setup dats, win/card.dat,
                      // lyt/title.bin, camera_road.bin
/* 0x1 */ FILETYPE_1,
/* 0x2 */ FILETYPE_2,
/* 0x3 */ FILETYPE_3,
/* 0x4 */ FILETYPE_TPL, // not used for bg tpls?
/* 0x5 */ FILETYPE_5, // a/%s (no dash)
/* 0x6 */ FILETYPE_6,
/* 0x7 */ FILETYPE_7,
/* 0x8 */ FILETYPE_8,
/* 0x9 */ FILETYPE_9,
/* 0xA */ FILETYPE_10,
/* 0xB */ FILETYPE_11 // eff/%s/%s.dat, eff/%seffdata.dat
};

typedef struct _FileEntry
{
/* 0x00 */ u8 state;
/* 0x01 */ s8 fileType;
/* 0x02 */ s16 touchCnt;
/* 0x04 */ u8 unknown_0x4[0x20 - 0x4];
/* 0x20 */ char path[64]; // size is uncertain
/* 0x60 */ u8 unknown_0x60[0xa0 - 0x60];
/* 0xA0 */ u32 length;
/* 0xA4 */ struct _SmartAllocation * sp; // smart pointer to file data
/* 0xA8 */ struct _FileEntry * next; // next entry in free or allocated list
/* 0xAC */ void * readDoneCb;
/* 0xB0 */ DVDEntry * dvdEntry;
} FileEntry;
SIZE_ASSERT(FileEntry, 0xb4)

typedef struct
{
/* 0x00 */ FileEntry * entries; // array of 1024 (FILE_ENTRY_MAX)
/* 0x04 */ FileEntry * allocatedStart; // first item in the allocated linked list
/* 0x08 */ FileEntry * allocatedEnd; // last item in the allocated linked list
/* 0x0C */ FileEntry * freeStart; // first item in the free linked list
/* 0x10 */ FileEntry * freeEnd; // last item in the allocated linked list
} FileWork;
SIZE_ASSERT(FileWork, 0x14)

DECOMP_STATIC(FileWork filemgr_work)
DECOMP_STATIC(FileWork * filemgr_afp)

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

/*
    Reads a file into memory
    Calling on its own will be a synchronous load
    Calling after Asyncf is finished allows accessing data loaded asynchronously
    Returns -1 if there's an error
*/
FileEntry * fileAllocf(s32 fileType, const char * format, ...);
FileEntry * fileAlloc(const char * path, s32 fileType);
FileEntry * _fileAlloc(const char * path, s32 fileType, s32 p3);

/*
    Unloads a file
*/
void fileFree(FileEntry * entry);

DECOMP_STATIC(void filemgr_dvdReadDoneCallback(s32 result, DVDFileInfo * fileInfo))

/*
    Loads a file asynchronously
    Returns a null pointer if the file isn't loaded yet
*/
ATTRIBUTE_FORMAT(printf, 3, 4)
FileEntry * fileAsyncf(s32 fileType, s32 p2, const char * format, ...);
FileEntry * fileAsync(const char * path, s32 fileType, s32 p3);

CPP_WRAPPER_END()
