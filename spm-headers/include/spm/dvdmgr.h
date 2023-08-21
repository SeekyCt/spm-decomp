/*
    dvdmgr provides lower level file reading than Filemgr
*/

#pragma once

#include <common.h>
#include <wii/dvd.h>

CPP_WRAPPER(spm::dvdmgr)

USING(wii::dvd::DVDFileInfo)

typedef void (DVDMgrCallback)(s32 result, DVDFileInfo * fileInfo);

#define MAX_DVD_ENTRY 0x200

#define DVD_FLAG_IN_USE 0x8000

typedef struct
{
/* 0x00 */ char path[64];
/* 0x40 */ DVDFileInfo fileInfo;
/* 0x7C */ void * dest;
/* 0x80 */ s32 lengthRemaining;
/* 0x84 */ s32 offset;
/* 0x88 */ s32 lengthRead;
/* 0x8C */ DVDMgrCallback * readCallback;
/* 0x90 */ u16 flags;
/* 0x92 */ u16 priority;
/* 0x94 */ s16 openP3;
/* 0x96 */ u8 unknown_0x96[0x98 - 0x96]; // possibly padding?
/* 0x98 */ s32 entrynum;
} DVDEntry;
SIZE_ASSERT(DVDEntry, 0x9c)

typedef struct
{
/* 0x0 */ DVDEntry * entries; // array of num
/* 0x4 */ s32 num; // 0x200
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
DVDEntry * DVDMgrOpen(const char * name, s32 priority, s16 param_3);

/*
    Read from a file synchronously
*/
s32 DVDMgrRead(DVDEntry * entry, void * dest, s32 length, s32 offset);

/*
    Calls an entry's callback
*/
DECOMP_STATIC(void dvdmgr__cb(s32 result, DVDFileInfo * fileInfo))

/*
    Read from a file asynchronously
*/
s32 DVDMgrReadAsync(DVDEntry * entry, void * dest, s32 length, s32 offset,
                    DVDMgrCallback * callback);

/*
    Close a an entry
*/
void DVDMgrClose(DVDEntry * entry);

/*
    Get the length of the file
*/
s32 DVDMgrGetLength(DVDEntry * entry);

CPP_WRAPPER_END()
