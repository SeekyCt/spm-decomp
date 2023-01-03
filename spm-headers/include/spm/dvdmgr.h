/*
    dvdmgr provides lower level file reading than Filemgr
*/

#pragma once

#include <common.h>
#include <wii/dvd.h>

CPP_WRAPPER(spm::dvdmgr)

USING(wii::dvd::DVDFileInfo)

typedef void (DVDMgrCallback)(s32 result, DVDFileInfo * fileInfo);

typedef struct
{
/* 0x00 */ char path[64];
/* 0x40 */ DVDFileInfo fileInfo;
/* 0x7C */ void * unknown_0x7c;
/* 0x80 */ u8 unknown_0x80[0x8c - 0x80];
/* 0x8C */ DVDMgrCallback * readCallback;
/* 0x90 */ u8 unknown_0x90[0x98 - 0x90];
/* 0x98 */ s32 entrynum;
} DVDEntry;
SIZE_ASSERT(DVDEntry, 0x9c)

typedef struct
{
/* 0x0 */ DVDEntry * entries; // array of entryLimit
/* 0x4 */ s32 entryLimit; // 0x200
} DVDWork;
SIZE_ASSERT(DVDWork, 0x8)

DECOMP_STATIC(DVDWork dvdmgr_work)
DECOMP_STATIC(DVDWork * dvdmgr_wp)

/*
    Initialises data used by dvdmgr functions
*/
void DVDMgrInit();

/*
    Stub
*/
void DVDMgrDelete();

/*
    Opens a file on the disc
*/
DVDEntry * DVDMgrOpen(const char * path, s32 param_2, s32 param_3);

/*
    Read from a file synchronously
*/
s32 DVDMgrRead(DVDEntry * entry, void * dest, size_t length, u32 offset);

/*
    Calls an entry's callback
*/
DECOMP_STATIC(void dvdmgr__cb(s32 result, DVDFileInfo * fileInfo))

/*
    Read from a file asynchronously
*/
s32 DVDMgrReadAsync(DVDEntry * entry, void * dest, size_t length, u32 offset,
                    DVDMgrCallback * callback);

/*
    Close a an entry
*/
void DVDMgrClose(DVDEntry * entry);

/*
    Get the length of the file
*/
size_t DVDMgrGetLength(DVDEntry * entry);

CPP_WRAPPER_END()
