/*
    effdrv handles effect animations
*/

#pragma once

#include <common.h>
#include <spm/filemgr.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::effdrv)

USING(spm::filemgr::FileEntry)
USING(wii::gx::GXTexObj)

struct _EffEntry;
typedef void (EffFunc)(struct _EffEntry * entry);

#define EFF_FLAG_SOFT_DELETED 4
#define EFF_FLAG_IN_USE 1

typedef struct _EffEntry
{
/* 0x00 */ u16 flags; // EFF_FLAG defines
/* 0x02 */ u16 type; // 0 or 1, groups entries for limits
/* 0x04 */ s32 releaseType; // 0 or 1, groups entries for freeing
                            // (1 means 'in battle', unused TTYD leftover)
/* 0x08 */ u8 unknown_0x8[0xc - 0x8];
/* 0x0C */ void * userWork;
/* 0x10 */ EffFunc * mainFunc;
/* 0x14 */ u8 unknown_0x14[0x18 - 0x14];
/* 0x18 */ char instanceName[16];
/* 0x28 */ u8 unknown_0x28[0x2c - 0x28];
} EffEntry;
SIZE_ASSERT(EffEntry, 0x2c)

typedef struct
{
/* 0x0 */ s32 curCount; // current number of active entries of this type
/* 0x4 */ s32 maxCount; // maximum number of active entries of this type allowed
} EffTypeStats;
SIZE_ASSERT(EffTypeStats, 0x8)

typedef struct
{
/* 0x000 */ s32 entryCount;
/* 0x004 */ EffEntry * entries; // array of entryCount length
/* 0x008 */ FileEntry * effectTpl;
/* 0x00C */ FileEntry * effectN64Tpl;
/* 0x010 */ u8 unknown_0x10[0x11c - 0x10];
/* 0x11C */ EffTypeStats typeStats[2];
/* 0x12C */ u8 unknown_0x12c[0x134 - 0x12c];
} EffWork;
SIZE_ASSERT(EffWork, 0x134)

DECOMP_STATIC(EffWork effdrv_work)
DECOMP_STATIC(EffWork * effdrv_wp)

/*
    Allocates entries
*/
void effInit();

/*
    Gets a tex obj for a texture in the effect TPL
*/
void effGetTexObj(s32 id, GXTexObj * out);

UNKNOWN_FUNCTION(func_800615cc);

/*
    Gets a tex obj for a texture in the effect N64 TPL
*/
void effGetTexObjN64(s32 id, GXTexObj * out);

/*
    Frees all entries of the specified release type
*/
void effAutoRelease(s32 releaseType);

/*
    Creates an entry of type 0
*/
EffEntry * effEntry();

/*
    Creates an entry of the specified type
*/
EffEntry * effEntryType(s32 type);

/*
    Sets the name of an entry
*/
void effSetName(const char * name);

/*
    Loads TPLs if not loaded, calls effSubMain and calls the mainFunc of each entry
*/
void effMain();

UNKNOWN_FUNCTION(func_80061a34);
UNKNOWN_FUNCTION(func_80061a44);

/*
    Delete an entry
*/
void effDelete(EffEntry * entry);
void effSoftDelete(EffEntry * entry);

/*
    Gets an entry by name
*/
EffEntry * effNameToPtr(const char * name);

UNKNOWN_FUNCTION(func_80061c28);
UNKNOWN_FUNCTION(func_80061cd0);
s32 func_80061d78();
UNKNOWN_FUNCTION(func_80061e18);
UNKNOWN_FUNCTION(func_80061eb8);
UNKNOWN_FUNCTION(func_800623f8);
UNKNOWN_FUNCTION(func_80062d04);
UNKNOWN_FUNCTION(func_80062f9c);
UNKNOWN_FUNCTION(func_800630b8);
UNKNOWN_FUNCTION(func_80063130);
UNKNOWN_FUNCTION(func_80063198);
UNKNOWN_FUNCTION(func_800631a4);
UNKNOWN_FUNCTION(func_800631ac);
UNKNOWN_FUNCTION(func_800631b8);
UNKNOWN_FUNCTION(func_800632ec);
UNKNOWN_FUNCTION(func_800632fc);
UNKNOWN_FUNCTION(func_80063308);
UNKNOWN_FUNCTION(func_80063374);
UNKNOWN_FUNCTION(func_8006339c);
UNKNOWN_FUNCTION(func_800633fc);

CPP_WRAPPER_END()
