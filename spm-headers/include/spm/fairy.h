/*
    fairy controls Pixl entities in game (except Tippi), as well as luvbi and squirps
*/

#pragma once

#include <common.h>
#include <wii/gx.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::fairy)

USING(wii::gx::GXColor)
USING(wii::mtx::Vec3)

#define FAIRYRunEnd 7

enum FairyExtraNo
{
/* 0x0 */ FAIRY_EXTRA_TAMARA, // Squirps
/* 0x1 */ FAIRY_EXTRA_ANGEKO // Luvbi
};

struct _FairyEntry;

// Returns an override for the name
typedef const char * (FairyAnimChangeHandler)(struct _FairyEntry * fairy, const char * newAnimName);

typedef struct _FairyEntry
{
    /*
        1 is hide & disable all fairies
        Others unknown
    */
/* 0x00 */ u16 flag0;
    /*
        2 is anim changed
        1 is run mode changed
        Others unknown
    */
/* 0x02 */ u16 flag2;
/* 0x04 */ s32 itemId; // 0 for extra entries
/* 0x08 */ u8 unknown_0x8[0xc - 0x8];
/* 0x0C */ s32 animPoseId;
/* 0x10 */ const char * curAnimName;
/* 0x14 */ FairyAnimChangeHandler * animChangeHandler;
/* 0x18 */ GXColor materialEvtColor;
/* 0x1C */ u8 unknown_0x1c[0x28 - 0x1c];
    /*
        Run modes are from 0-6
        Currently unknown what each one is
    */
/* 0x28 */ s32 runMode;
/* 0x2C */ s32 prevRunMode;
/* 0x30 */ u8 unknown_0x30[0x48 - 0x30];
/* 0x48 */ Vec3 position;
/* 0x54 */ u8 unknown_0x54[0xd0 - 0x54];
} FairyEntry;
SIZE_ASSERT(FairyEntry, 0xd0)

typedef struct
{
/* 0x000 */ u32 flags;
/* 0x004 */ FairyEntry entries[4];
/* 0x344 */ s32 max; // 4
/* 0x348 */ s32 num;
/* 0x34C */ s32 numExtra;
/* 0x350 */ s32 extraNo;
/* 0x354 */ u8 unknown_0x354[0x368 - 0x354];
} FairyWork;
SIZE_ASSERT(FairyWork, 0x368)

DECOMP_STATIC(FairyWork fairy_work)

DECOMP_STATIC(const char * fairy_extraAnimPoseNames[2])

UNKNOWN_FUNCTION(func_8011bc08);
UNKNOWN_FUNCTION(func_8011c4d8);
UNKNOWN_FUNCTION(func_8011c920);
UNKNOWN_FUNCTION(func_8011ceb4);
UNKNOWN_FUNCTION(func_8011d300);
UNKNOWN_FUNCTION(func_8011d3bc);
UNKNOWN_FUNCTION(func_8011d440);

/*
    Creates a fairy entry for the currently selected pixl
*/
void fairyEntry();

/*
    Calls _fairyReInit
*/
void fairyReset();

/*
    Clears fairy work
*/
void fairyInit();

/*
    Calls _fairyReInit
*/
void fairyReInit();

/*
    Updates a fairy entry
*/
void fairyUpdate(FairyEntry * thisFairy, FairyEntry * otherFairy);

/*
    Calls fairyUpdate for each entry, first with otherFairy as
    NULL and then with it as every other fairy entry one by one
*/
void fairyMain();

/*
    Renders a fairy
*/
void fairyDisp(s8 camId, FairyEntry * fairy);

/*
    Does unknown calculations and schedules fairyDisp to run for this fairy this frame
*/
void fairyDispMain(FairyEntry * fairy);

/*
    Re-creates all fairys
*/
void _fairyReInit();

/*
    Gets a fairy entry by id
*/
FairyEntry * fairyIdToPtr(s32 id);

/*
    Gets a fairy entry by item id
*/
FairyEntry * fairyItemIdToPtr(s32 itemId);

/*
    Returns a pointer to the 'extra' FairyEntries
*/
FairyEntry * fairyGetExtra();

/*
    Sets curAnimName to an new name, and allows animChangeHandler to override it
*/
void fairySetAnim(FairyEntry * fairy, const char * animName);

UNKNOWN_FUNCTION(func_8011ea48);

/*
    Functions to change the current run mode of a fairy
*/
void fairyEnterRunMode(FairyEntry * fairy, s32 runMode);
void fairyIdEnterRunMode(s32 id, s32 runMode);
    // Less checks & clears flag2 0x1 instead of setting
void fairyEnterRunModeAlt(FairyEntry * fairy, s32 RunMode);
void fairyIdEnterRunMode0(s32 id);
void fairyIdEnterRunMode1(s32 id);
void fairyAllEnterRunMode0();
void fairyAllEnterRunMode1();
void fairyIdEnterRunMode2();
void fairyAllEnterRunMode2();

/*
    Sets position of all fairies
*/
void fairySetAllPositions(Vec3 * position);

UNKNOWN_FUNCTION(func_8011f2d4);
UNKNOWN_FUNCTION(func_8011f320);

/*
    Returns a pointer to all FairyEntries
*/
FairyEntry * fairyGetEntries();

/*
    Returns how many normal/extra fairies there are
*/
s32 fairyGetNum();
s32 fairyGetNumExtra();

UNKNOWN_FUNCTION(func_8011f39c);
UNKNOWN_FUNCTION(func_8011f3b0);
UNKNOWN_FUNCTION(func_8011f45c);
UNKNOWN_FUNCTION(func_8011f534);

/*
    Configures the animPose for this fairy based on entry fields
*/
void fairyUpdateAnimPose(FairyEntry * entry);

/*
    Sets the alpha of materialEvtColor
*/
void fairySetMaterialEvtAlpha(FairyEntry * entry, u8 alpha);

/*
    Creates a 'extra' fairy entry for the character specified by extra no
*/
void fairyExtraEntry(s32 extraNo);

/*
    Deletes the 'extra' fairy entry
*/
void fairyFreeExtra();

/*
    Disable & hide / enable & unhide all fairies
*/
void fairyOff();
void fairyOn();

CPP_WRAPPER_END()
