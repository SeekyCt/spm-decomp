/*
    mapdrv handles the rendering & collision of the main map model
*/

#pragma once

#include <common.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <wii/tpl.h>

CPP_WRAPPER(spm::hitdrv)

struct _HitObj;

CPP_WRAPPER_END()

CPP_WRAPPER(spm::mapdrv)

USING(spm::hitdrv::_HitObj)
USING(wii::gx::GXColor)
USING(wii::mtx::Mtx34)
USING(wii::mtx::Vec3)
USING(wii::tpl::TPLHeader)

extern BOOL __mapdrv_make_dl;

typedef struct
{
/* 0x0 */ u8 unknown_0x0;
/* 0x1 */ u8 cullMode;
/* 0x2 */ u8 flags;
/* 0x3 */ u8 unknown_0x3;
/* 0x4 */ u32 lightMask;
/* 0x8 */ u32 attr;
/* 0xC */ u8 unknown_0xc[0x10 - 0xc];
} MapFileJointDrawMode;
SIZE_ASSERT(MapFileJointDrawMode, 0x10)

typedef struct
{
/* 0x0 */ Unk * material;
/* 0x4 */ Unk * mesh;
} MapFileJointPart;
SIZE_ASSERT(MapFileJointPart, 0x8)

typedef struct _MapFileJoint
{
/* 0x00 */ const char * name;
/* 0x04 */ const char * type;
/* 0x08 */ struct _MapFileJoint * parent;
/* 0x0C */ struct _MapFileJoint * firstChild;
/* 0x10 */ struct _MapFileJoint * nextSibling;
/* 0x14 */ struct _MapFileJoint * prevSibling;
/* 0x18 */ Vec3 scale;
/* 0x24 */ Vec3 rotation;
/* 0x30 */ Vec3 translation;
/* 0x3C */ Vec3 bboxMin;
/* 0x48 */ Vec3 bboxMax;
/* 0x54 */ u8 unknown_0x54[0x58 - 0x54];
/* 0x58 */ MapFileJointDrawMode * drawMode;
/* 0x5C */ s32 partTableCount;
    // Actually a VLA, length is partTableCount
/* 0x60 */ MapFileJointPart partTable[1];
} MapFileJoint;
SIZE_ASSERT(MapFileJoint, 0x68)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x4 - 0x0];
/* 0x04 */ s32 type;
/* 0x08 */ f32 startZ;
/* 0x0C */ f32 endZ;
/* 0x10 */ GXColor colour;
} MapFileFog;
SIZE_ASSERT(MapFileFog, 0x14)

typedef struct
{
/* 0x00 */ u32 fileSize;
/* 0x04 */ u32 mainDataSize;
/* 0x08 */ u32 relTableCount;
/* 0x0C */ u32 chunkTableCount;
/* 0x10 */ u8 unknown_0x10[0x20 - 0x10];
} MapFileHeader;
SIZE_ASSERT(MapFileHeader, 0x20)

typedef struct
{
/* 0x0 */ u32 offset; // into main data
/* 0x4 */ u32 nameOffset; // into string table
} MapFileChunk;
SIZE_ASSERT(MapFileChunk, 0x8)

typedef struct
{
/* 0x00 */ MapFileHeader fileHeader;
/* 0x20 */ void * mainData;
/* 0x24 */ Unk * relTable;
/* 0x28 */ MapFileChunk * chunkTable;
/* 0x2C */ u8 unknown_0x2c[0x30 - 0x2c];
/* 0x30 */ const char * stringTable;
/* 0x34 */ u8 unknown_0x34[0x3c - 0x34];
/* 0x3C */ u32 flags;
/* 0x40 */ void * rawFile;
} MapFileContext;
SIZE_ASSERT(MapFileContext, 0x44)

typedef struct
{
/* 0x00 */ const char * name;
/* 0x04 */ const char * type;
/* 0x08 */ Vec3 position;
/* 0x14 */ Vec3 rotation;
/* 0x20 */ Vec3 scale;
/* 0x2C */ GXColor colour;
/* 0x30 */ f32 spotAngle;
/* 0x34 */ f32 angleAttenuation;
/* 0x38 */ s32 distanceAttenuationType;
/* 0x3C */ u32 flags;
/* 0x40 */ s32 version;
} MapFileLightEntry;
SIZE_ASSERT(MapFileLightEntry, 0x44)

typedef struct
{
/* 0x0 */ s32 count;
/* 0x4 */ MapFileLightEntry * entries[1]; // array of length count
} MapFileLightTable;
OFFSET_ASSERT(MapFileLightTable, entries, 0x4)

typedef struct
{
/* 0x00 */ const char * meshName;
/* 0x04 */ Vec3 translation;
/* 0x10 */ Vec3 rotation;
/* 0x1C */ Vec3 scale;
/* 0x28 */ u8 unknown_0x28[0x58 - 0x28];
/* 0x58 */ s32 frameCount;
} MapFileMeshTrack;
SIZE_ASSERT(MapFileMeshTrack, 0x5c)

typedef struct
{
/* 0x0 */ s32 count;
/* 0x4 */ MapFileMeshTrack * tracks; // array of length count
} MapFileMeshTracks;
SIZE_ASSERT(MapFileMeshTracks, 0x8)

typedef struct
{
/* 0x00 */ const char * name;
/* 0x04 */ u8 unknown_0x4[0x8 - 0x4];
/* 0x08 */ f32 duration;
/* 0x0C */ MapFileMeshTracks * meshTracks;
/* 0x10 */ Unk * materialTrack;
/* 0x14 */ u8 unknown_0x14[0x20 - 0x14];
} MapFileAnimData;
SIZE_ASSERT(MapFileAnimData, 0x20)

#define MAPOBJ_FLAG0_HIDE 1

typedef struct _MapObj
{
/* 0x000 */ u32 flag0;
/* 0x004 */ u32 flag4;
/* 0x008 */ u8 blendMode;
/* 0x009 */ u8 unknown_0x9[0xc - 0x9];
/* 0x00C */ MapFileJoint * joint;
/* 0x010 */ GXColor colour;
/* 0x014 */ u8 unknown_0x14[0x1c - 0x14];
/* 0x01C */ Vec3 translation;
/* 0x028 */ Mtx34 modelWorldMtx;
/* 0x058 */ u8 unknown_0x58[0xb8 - 0x58];
/* 0x0B8 */ Mtx34 runtimeWorldMtx;
/* 0x0E8 */ s16 offscreenId;
/* 0x0EA */ s16 mapEntryId;
/* 0x0EC */ struct _MapObj * parent;
/* 0x0F0 */ struct _MapObj * firstChild;
/* 0x0F4 */ struct _MapObj * nextSibling;
/* 0x0F8 */ u16 gxBboxLeft;
/* 0x0FA */ u16 gxBboxTop;
/* 0x0FC */ u16 gxBboxRight;
/* 0x0FE */ u16 gxBboxBottom;
/* 0x100 */ void * dl_buf[8]; // display list pointers
/* 0x120 */ u32 dl_size[8]; // display list sizes
/* 0x140 */ u8 unknown_0x140[0x144 - 0x140];
} MapObj;
SIZE_ASSERT(MapObj, 0x144)

typedef struct
{
/* 0x00 */ u16 flags;
/* 0x02 */ u8 unknown_0x2[0x10 - 0x2];
/* 0x10 */ f32 timePlayed;
/* 0x14 */ f32 playRate;
/* 0x18 */ MapFileAnimData * fileData;
/* 0x1C */ u8 unknown_0x1c[0x28 - 0x1c];
} MapAnimObj;
SIZE_ASSERT(MapAnimObj, 0x28)

typedef void (MapEntryUnloadCb)();

// Internally referred to as a "kind"
typedef struct
{
/* 0x000 */ s32 mapCount; // active MapEntry count (always 1?)
/* 0x004 */ u16 flags;
/* 0x006 */ char name[16];
/* 0x016 */ GXColor colour;
/* 0x01A */ GXColor blend1;
/* 0x01E */ GXColor blend2;
/* 0x022 */ u8 unknown_0x22[0x24 - 0x22];
/* 0x024 */ MapFileFog fogDefs[2];
/* 0x04C */ s32 loadStage; // used for mapLoad
/* 0x050 */ MapFileContext mapDatContext;
/* 0x094 */ void * mapDat;
/* 0x098 */ u32 mapDatSize;
/* 0x09C */ void * tpl;
/* 0x0A0 */ u32 tplSize;
/* 0x0A4 */ Unk * textureTable;
/* 0x0A8 */ MapFileLightTable * lightTable;
/* 0x0AC */ MapFileFog * fogData;
/* 0x0B0 */ Unk * materialNameTable;
/* 0x0B4 */ Unk * vcdTable;
/* 0x0B8 */ MapObj * rootMapObj;
/* 0x0BC */ struct _HitObj * rootHitObj;
/* 0x0C0 */ Vec3 bboxCorners[2][8];
/* 0x180 */ s32 mapObjCount;
/* 0x184 */ MapObj * mapObjs;
/* 0x188 */ s32 hitObjCount;
/* 0x18C */ struct _HitObj * hitObj;
/* 0x190 */ s32 animCount;
/* 0x194 */ MapAnimObj * animData;
/* 0x198 */ u32 dl_size; // display list size
/* 0x19C */ void * dl_buf; // display list buffer
/* 0x1A0 */ MapEntryUnloadCb * unloadCb;
} MapEntry;
SIZE_ASSERT(MapEntry, 0x1a4)

typedef struct
{
/* 0x000 */ MapEntry entries[2]; // being an array is a TTYD leftover
/* 0x348 */ u8 unknown_0x348[0x34c - 0x348];
} MapWorkGroup;
SIZE_ASSERT(MapWorkGroup, 0x34c)

typedef struct
{
/* 0x000 */ MapWorkGroup groups[2];
/* 0x698 */ s32 activeGroup;
/* 0x69C */ u8 unknown_0x69c[0x6a0 - 0x69c];
/* 0x6A0 */ s32 dispOff;
/* 0x6A4 */ u8 unknown_0x6a4[0x6a8 - 0x6a4];
/* 0x6A8 */ GXColor paperAmbColor;
} MapWork;
SIZE_ASSERT(MapWork, 0x6ac)

DECOMP_STATIC(MapWork mapdrv_work)
DECOMP_STATIC(MapWork * mapdrv_wp)

/*
    Records when a MapObj/HitObj isn't found
    Types:
        0 - MapObj
        1 - HitObj
*/
void mapErrorEntry(s32 type, char * elementName);

UNKNOWN_FUNCTION(func_8007ecc4);
UNKNOWN_FUNCTION(func_8007ee24);
UNKNOWN_FUNCTION(func_8007eff0);

/*
    Hides/unhides the map
*/
void mapDispOff();
void mapDispOn();

/*
    Returns the pointer to the active group in mapWork
*/
MapWorkGroup * mapGetWork();

/*
    Counts the children of a MapFileJoint and the joint itself
*/
void mapGetJointsSub(MapFileJoint * joint, s32 countOut);
void mapGetJoints(MapFileJoint * joint);

/*
    Returns the bounding box of all MapObjs in the specified map entry
*/
void mapGetBoundingBox(s32 entryId, Vec3 * minOut, Vec3 * maxOut);

/*
    Clears work and sets entry bounding boxes to (-)1000.0
*/
void mapInit();

/*
    Clears entries
*/
void mapReset();

/*
    Initialises textures for a MapObj
*/
void mapBuildTexture(MapObj * mapObj, TPLHeader * tpl, Unk * textureTable);

/*
    Creates a MapObj from a MapFileJoint
*/
MapObj * _mapEnt(MapFileJoint * joint, MapObj * parent, Mtx34 * mtx, s32 mapEntryId);

/*
    Creates MapObjs for a joint and its children
*/
MapObj * mapEntrySub(MapFileJoint * joint, MapObj * parent, Mtx34 * mtx, bool isRoot,
                     s32 mapEntryId);

/*
    Creates the display list for a map entry
*/
DECOMP_STATIC(void mapdrv_makeDisplayList(s32 mapEntryId))

UNKNOWN_FUNCTION(func_800808f0);

/*
    Gets a MapAnimObj by name
*/
MapAnimObj * mapSearchAnimObj(const char * animName);


/*
    Checks the progress of an animation
*/
void mapCheckAnimation(const char * animName, s32 * finishedOut, f32 * timeRemainingOut);

/*
    Plays an animation which will only progress on the specified system level or lower
*/
void mapPlayAnimationLv(const char * animName, Unk param_2, s32 level);

/*
    Creates the MapAnimObjs for a map entry
*/
void mapEntryAnimation(s32 mapEntryId);

/*
    Ends/pauses/restarts an animation
*/
void mapDeleteAnimation(const char * animName);
void mapPauseAnimation(const char * animName);
void mapReStartAnimation(const char * animName);

/*
    Pauses/restarts all animations
*/
void mapPauseAnimationAll();
void mapReStartAnimationAll();

/*
    Sets the speed of an animation
*/
void mapSetPlayRate(const char * animName, f32 playRate);

/*
    Jumps to a fraction of the way through an animation
*/
void mapSetPlayProgress(const char * animName, f32 playProgress);

/*
    Returns the length of an animation
*/
f32 mapGetPlayDuration(const char * animName);

/*
    Clears the current map, hit, env and light entries
*/
void mapUnload();

/*
    Loads a new map
*/
void mapLoad(const char * name);

UNKNOWN_FUNCTION(func_80083aa4);

void mapCalcAnimMatrix(Mtx34 * dest, Mtx34 * parentMtx, MapAnimObj * obj, MapFileMeshTrack * track);

UNKNOWN_FUNCTION(func_80083eb4);
UNKNOWN_FUNCTION(func_80084098);
UNKNOWN_FUNCTION(func_80084274);

void mapReCalcMatrix(MapObj * mapObj, Mtx34 * dest, bool includeRoot);

/*
    Updates map visuals and collision
*/
void mapMain();

/*
    Renders a MapObj (and its children)
*/
void _mapDispMapObj(s8 cameraId, MapObj * obj);
void _mapDispMapGrp(s8 cameraId, MapObj * obj);
void mapDispMapObj(s8 cameraId, void * param);
void mapDispMapGrp(s8 cameraId, void * param);

/*
    Renders an offscreen MapObj (and its children)
*/
void mapDispMapObj_off(s8 cameraId, void * param);
void mapDispMapGrp_off(s8 cameraId, void * param);

/*
    Renders a MapObj without a material (and its children)    
*/
void _mapDispMapObj_NoMaterial(s8 cameraId, MapObj * obj);
void _mapDispMapGrp_NoMaterial(s8 cameraId, MapObj * obj);

/*
    Updates the GX bounding box of a MapObj (and its children)
*/
void mapDispMapObj_bbox(s8 cameraId, void * param);
void mapDispMapGrp_bbox(s8 cameraId, void * param);

/*
    Prepares for & schedules rendering the map
*/
void mapDisp();

void mapSetLight(MapObj *, Unk);
UNKNOWN_FUNCTION(mapSetMaterialTev);
UNKNOWN_FUNCTION(mapSetMaterialLastStageBlend);
UNKNOWN_FUNCTION(mapSetMaterialFog);
UNKNOWN_FUNCTION(mapSetMaterialFogMapObj);
UNKNOWN_FUNCTION(mapSetTextureMatrix);
UNKNOWN_FUNCTION(mapSetMaterial);
void mapSetPaperAmbColor(GXColor * colour);
UNKNOWN_FUNCTION(mapResetPaperAmbColor);
UNKNOWN_FUNCTION(mapSetMaterialLight);
UNKNOWN_FUNCTION(mapSetPolygon);
UNKNOWN_FUNCTION(mapSetPolygonVtxDesc);

UNKNOWN_FUNCTION(func_8008b4c8);

MapFileJoint * mapSearchDmdJointSub(MapFileJoint * joint, const char * name);
UNKNOWN_FUNCTION(mapSearchDmdJointSub2);
Unk * mapSearchDmdMaterial(const char * name);

/*
    Gets a MapObj by name
*/
MapObj * mapGetMapObj(const char * name);

/*
    Turns on/off flag0 flags for a MapObj (and its children)
    allowSubName matches MapObjs with the string anywhere in their name (strstr instead of strcmp)
*/
void mapObjFlagOn(bool allowSubName, const char * name, u32 mask);
void mapObjFlagOff(bool allowSubName, const char * name, u32 mask);
void mapFlgOn(MapObj * obj, u32 mask, bool allowSiblings);
void mapFlgOff(MapObj * obj, u32 mask, bool allowSiblings);
void mapGrpFlagOn(bool allowSubName, const char * name, u32 mask);
void mapGrpFlagOff(bool allowSubName, const char * name, u32 mask);

/*
    Turns on/off flag4 flags for a MapObj (and its children)
    allowSubName matches MapObjs with the string anywhere in their name (strstr instead of strcmp)
*/
void mapObjFlag4On(bool allowSubName, const char * name, u32 mask);
void mapObjFlag4Off(bool allowSubName, const char * name, u32 mask);
void mapFlg4On(MapObj * obj, u32 mask, bool allowSiblings);
void mapFlg4Off(MapObj * obj, u32 mask, bool allowSiblings);
void mapGrpFlag4On(bool allowSubName, const char * name, u32 mask);
void mapGrpFlag4Off(bool allowSubName, const char * name, u32 mask);

/*
    Sets the offscreen id of a MapObj (and its children)
*/
void _setOffScrnId(MapObj * obj, s16 ofsId, bool allowSiblings);
void mapObjSetOffscreen(const char * objName, const char * ofsName);
void mapGrpSetOffscreen(const char * objName, const char * ofsName);

/*
    Removes the offscreen id of a MapObj (and its children), checking the name is correct
*/
void mapObjClearOffScreen(const char * objName, const char * ofsName);
void mapGrpClearOffScreen(const char * objName, const char * ofsName);

void func_8008f2a4(const char *);
void func_8008f324(const char *);

/*
    Rotates a MapObj in degrees
*/
void mapObjRotate(const char * objName, f32 x, f32 y, f32 z);

/*
    Moves a MapObj by the specified amount in each direction
*/
void mapObjTranslate(const char * objName, f32 x, f32 y, f32 z);

/*
    Scales a MapObj by the specified amount in each direction
*/
void mapObjScale(const char * objName, f32 x, f32 y, f32 z);

/*
    Gets the centre of a MapObj and its children
*/
void mapObjGetPosSub(MapObj * obj, Vec3 * posOut, s32 * countOut, bool allowSiblings);
void mapObjGetPos(const char * name, Vec3 * posOut);

UNKNOWN_FUNCTION(func_800900cc);
UNKNOWN_FUNCTION(func_80090270);

/*
    Sets the fog parameters for a map group
*/
void mapSetFog(s32 groupId, s32 type, GXColor * colour, f32 startZ, s32 endZ);

/*
    Enables/disables fog
*/
void mapFogOn();
void mapFogOff();

/*
    Sets/gets a blend colour of the active group's first map entry
*/
void mapSetBlend(GXColor * colour);
void mapSetBlend2(GXColor * colour);
void mapGetBlend(GXColor * colour);
void mapGetBlend2(GXColor * colour);

/*
    Ddisables a blend colour of the active group's first map entry
*/
void mapBlendOff();
void mapBlendOff2();

/*
    Sets the colour of the active group's first map entry
*/
void mapSetColor(GXColor * colour);

/*
    Sets the colour of a MapObj (and its children)
*/
void mapObjSetColor(const char * name, GXColor * colour);
DECOMP_STATIC(void mapdrv_setColor(MapObj * obj, GXColor * colour, bool allowSiblings))
void mapGrpSetColor(const char * name, GXColor * colour);

UNKNOWN_FUNCTION(spline_maketable);
UNKNOWN_FUNCTION(spline_getvalue);

/*
    Applies relocations to a map.dat file and generates its context
    Returns 0 if successful, -1 if error
*/
s32 Locate(MapFileContext * outCtx, void * mapDat, u32 mapDatSize);

UNKNOWN_FUNCTION(mapTextXLU);

CPP_WRAPPER_END()
