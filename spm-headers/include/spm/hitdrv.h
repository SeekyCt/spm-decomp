/*
    Hitdrv handles collision with MOBJs or the map
*/

#pragma once

#include <common.h>
#include <spm/mapdrv.h>
#include <spm/mobjdrv.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::hitdrv)

USING(spm::mapdrv::MapObj)
USING(spm::mapdrv::MapFileJoint)
USING(spm::mobjdrv::MobjEntry)
USING(wii::mtx::Vec3)
USING(wii::mtx::Mtx34)

#define HITOBJ_FLAG_DELETED 0x80

// Matrix recalculated
#define HITOBJ_FLAG_MTX_RECALC 0x40

#define HITOBJ_FLAG_DISABLE 0x1

typedef struct _HitObj
{
/* 0x000 */ u16 flags;
/* 0x002 */ u8 unknown_0x2[0x4 - 0x2];
/* 0x004 */ u32 attr; // unknown bitflags
/* 0x008 */ MapFileJoint * joint;
/* 0x00C */ u8 unknown_0xc[0xcc - 0xc];
/* 0x0CC */ Vec3 position; 
/* 0x0D8 */ s16 totalTri;
/* 0x0DA */ s16 mapEntryIdx;
/* 0x0DC */ Unk * tris; // array of totalTri length
/* 0x0E0 */ u8 unknown_0xe0[0x100 - 0xe0];
/* 0x100 */ union
            {
                MobjEntry * mobj;
                MapObj * mapObj;
            } owner;
/* 0x104 */ struct _HitObj * parent;
/* 0x108 */ struct _HitObj * child;
/* 0x10C */ struct _HitObj * nextSibling;
/* 0x110 */ u8 unknown_0x110[0x114 - 0x110];
} HitObj;
SIZE_ASSERT(HitObj, 0x114)

/*
    Stub
*/
void hitInit();

/*
    Deletes all map HitObjs & their tris
*/
void hitReinit();

/*
    Creates a HitObj for a joint
*/
HitObj * _hitEnt(MapFileJoint * joint, HitObj * parent, Mtx34 * parentMtx, s32 mapEntryIdx);

/*
    Recursively creates HitObjs for a joint and its children
*/
HitObj * hitEntrySub(MapFileJoint * joint, HitObj * parent, Mtx34 * parentMtx, bool ignoreSiblings,
                     s32 mapEntryIdx);

/*
    Create HitObjs for a joint and its children
*/
HitObj * hitEntry(MapFileJoint * joint, Mtx34 * mtx, s32 mapEntryIdx);
HitObj * hitEntryMOBJ(MapFileJoint * joint, Mtx34 * mtx);

/*
    Deletes a HitObj by name
*/
void hitDelete(const char * name);

/*
    Updates matrices for HitObjs
*/
void hitMain();

void hitRecalcMatrices();

/*
    Turns on/off flags for a HitObj by name
    allowSubName matches HitObjs with the string anywhere in their name (strstr instead of strcmp)
*/
void hitObjFlagOn(bool allowSubName, const char * name, u16 mask);
void hitObjFlagOff(bool allowSubName, const char * name, u16 mask);

/*
    Recursively turn on/off flags for a HitObj and its children by pointer/name
    allowSubName matches HitObjs with the string anywhere in their name (strstr instead of strcmp)
*/
void hitFlgOn(HitObj * hitObj, u16 mask, bool allowSiblings);
void hitFlgOff(HitObj * hitObj, u16 mask, bool allowSiblings);
void hitGrpFlagOn(bool allowSubName, const char * name, u16 mask);
void hitGrpFlagOff(bool allowSubName, const char * name, u16 mask);

/*
    Turns on/off attr bitflags for a HitObj by name
    allowSubName matches HitObjs with the string anywhere in their name (strstr instead of strcmp)
*/
void hitObjAttrOn(bool allowSubName, const char * name, u32 mask);
void hitObjAttrOff(bool allowSubName, const char * name, u32 mask);

/*
    Recursively turn on/off attr bitflags for a HitObj and its children by pointer/name
    allowSubName matches HitObjs with the string anywhere in their name (strstr instead of strcmp)
*/
void hitAtrOn(HitObj * hitObj, u32 mask, bool allowSiblings);
void hitAtrOff(HitObj * hitObj, u32 mask, bool allowSiblings);
void hitGrpAttrOn(bool allowSubName, const char * name, u32 mask);
void hitGrpAttrOff(bool allowSubName, const char * name, u32 mask);

void hitReCalcMatrix(HitObj *, Unk);
void hitReCalcMatrix2(HitObj *, Mtx34 *, bool);
void hitCalcVtxPosition(HitObj *);

UNKNOWN_FUNCTION(func_8006da4c);
UNKNOWN_FUNCTION(func_8006debc);
UNKNOWN_FUNCTION(func_8006e160);
UNKNOWN_FUNCTION(func_8006e400);

UNKNOWN_FUNCTION(hitCheckVecFilter);
bool hitCheckFilter(f32, f32, f32, f32, f32, f32, void *, f32 *, f32 *,
                    f32 *, f32 *, f32 *, f32 *, f32 *);

UNKNOWN_FUNCTION(func_8006ef1c);

UNKNOWN_FUNCTION(chkFilterAttr);
UNKNOWN_FUNCTION(hitCheckAttr);

UNKNOWN_FUNCTION(func_8006f128);
UNKNOWN_FUNCTION(func_8006f47c);
UNKNOWN_FUNCTION(func_8006f710);
UNKNOWN_FUNCTION(func_8006f7cc);
UNKNOWN_FUNCTION(func_8006f884);

/*
    Gets a HitObj by name / a string anwyhere in its name
*/
HitObj * hitNameToPtr(const char * name);
HitObj * hitSubNameToPtr(const char * subname);

/*
    Gets the position vector of a HitObj
*/
void hitObjGetPosSub(HitObj *, Vec3 *, s32 *, bool);
void hitObjGetPos(const char * name, Vec3 * posOut);

/*
    Gets the normal vector of a HitObj
*/
void hitObjGetNormal(const char * name, Vec3 * normalOut);

/*
    Returns the bounding box of the specified map entry
*/
void hitGetMapEntryBbox(s32 mapEntryIdx, Vec3 * minOut, Vec3 * maxOut);

UNKNOWN_FUNCTION(func_8007058c);
UNKNOWN_FUNCTION(func_80070790);

/*
    Gets the name of a HitObj's joint
*/
const char * hitGetName(HitObj * hitObj);

/*
    Returns the attr of a HitObj
*/
u32 hitGetAttr(HitObj * hitGetAttr);

/*
    Sets the owner of a HitObj
*/
void hitBindMapObj(const char * hitName, const char * mapObjName);

void hitBindUpdate();
void hitCheckVecFilter2();

UNKNOWN_FUNCTION(func_800710b4);
UNKNOWN_FUNCTION(func_800717d8);
UNKNOWN_FUNCTION(func_80071f44);

CPP_WRAPPER_END()

