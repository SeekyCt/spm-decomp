/*
    casedrv runs scripts when the player interacts with certain HitObjs
*/

#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/hitdrv.h>

CPP_WRAPPER(spm::casedrv)

USING(spm::evtmgr::EvtScriptCode)
USING(spm::hitdrv::HitObj)

typedef struct
{
/* 0x00 */ u16 flags;
/* 0x02 */ u8 unknown_0x2[0x4 - 0x2];
    // Touching a HitObj with either name triggers the script
/* 0x04 */ const char * name;
/* 0x08 */ const char * name2;
/* 0x0C */ u8 unknown_0xc[0x10 - 0xc];
/* 0x10 */ s32 lw[16]; // copied into script's LW on run
/* 0x50 */ u8 unknown_0x50[0x54 - 0x50];
/* 0x54 */ EvtScriptCode * script;
/* 0x58 */ s32 scriptPriority;
} CaseEntDef;
SIZE_ASSERT(CaseEntDef, 0x5c)

// Script running this frame
#define CASE_FLAG_RUN 0x10
// Entry allocated for a case
#define CASE_FLAG_IN_USE 1

typedef struct
{
/* 0x00 */ u16 flags; // CASE_FLAG defines
/* 0x02 */ u8 unknown_0x2[0x4 - 0x2];
/* 0x04 */ s32 type; // controls the action & type of HitObj required
/* 0x08 */ char name[64];
/* 0x48 */ char name2[64];
/* 0x88 */ s32 id;
/* 0x8C */ u32 def_0xc; // 0xc-f from CaseEntDef
/* 0x90 */ s32 lw[16];
/* 0xD0 */ s32 evtId;
/* 0xD4 */ EvtScriptCode * script;
/* 0xD8 */ s32 scriptPriority;
} CaseEntry;
SIZE_ASSERT(CaseEntry, 0xdc)

typedef struct
{
/* 0x0 */ s32 n;
/* 0x4 */ CaseEntry * entries; // array of n length
/* 0x8 */ u8 unknown_0x8[0x10 - 0x8];
} CaseWork;
SIZE_ASSERT(CaseWork, 0x10)

DECOMP_STATIC(CaseWork casedrv_work)
DECOMP_STATIC(CaseWork * casedrv_wp)

/*
    Allocates entries
*/
void caseInit();

/*
    Clears entries
*/
void caseReInit();

/*
    Creates an entry from a CaseEntDef and returns its id
*/
s32 caseEntry(CaseEntDef * def);

/*
    Deletes an entry by id
*/
void caseDelete(s32 id);

UNKNOWN_FUNCTION(func_80059268);

/*
    Updates all entries
*/
void caseMain();

/*
    Returns an entry by its HitObj
*/
CaseEntry * caseCheckHitObj(HitObj * hitObj);
CaseEntry * caseCheckHitObj2(HitObj * hitObj);

/*
    Returns an entry by id
*/
CaseEntry * caseIdToPtr(s32 id);

UNKNOWN_FUNCTION(func_8005adec);
UNKNOWN_FUNCTION(func_8005ae08);
UNKNOWN_FUNCTION(func_8005ae24);
UNKNOWN_FUNCTION(func_8005ae64);

CPP_WRAPPER_END()
