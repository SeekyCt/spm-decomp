/*
    guide controls Tippi & Tiptron
*/

#pragma once

#include <common.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::guide)

USING(wii::mtx::Vec3)

#define GUIDERunEnd 6

// Hide and disable tippi
#define GUIDE_FLAG0_DISABLE 0x80

// Returns an override for the name
typedef const char * (GuideAnimChangeHandler)(const char * newAnimName);

typedef struct
{
    /*
        See defines above
    */
/* 0x00 */ u16 flag0;
    /*
        All unknown
    */
/* 0x02 */ u8 flag2;
    /*
        0x2 is anim changed
        0x1 is run mode changed
        Others unknown
    */
/* 0x03 */ u8 flag3;
/* 0x04 */ u8 unknown_0x4[0x8 - 0x4];
/* 0x08 */ s32 animPoseId;
/* 0x0C */ char curAnimName[4];
/* 0x10 */ u8 unknown_0x10[0x1c - 0x10];
/* 0x1C */ GuideAnimChangeHandler * animChangeHandler;
    /*
        Run modes are from 0-5
        Currently unknown what each one is
    */
/* 0x20 */ s32 runMode;
/* 0x24 */ s32 prevRunMode;
/* 0x28 */ u8 unknown_0x28[0x40 - 0x28];
/* 0x40 */ Vec3 pos;
/* 0x4C */ u8 unknown_0x4c[0xb4 - 0x4c];
} GuideWork;
SIZE_ASSERT(GuideWork, 0xb4)

DECOMP_STATIC(GuideWork guide_work)

typedef void (GuideRunModeFunc)(GuideWork * work);

DECOMP_STATIC(GuideRunModeFunc * guide_runModeMainFuncs[6])

/*
    Returns a pointer to guideWork
*/
GuideWork * guideGetWork();

/*
    Clears guideWork and sets animPoseId to -1
*/  
void guideInit();

/*
    Loads the model and does other unknown tasks
*/
void guideReInit();

/*
    Updates and calls the main func for this runMode
*/
void guideMain();

/*
    Does unknown calculations and schedules guideDisp to run this frame
*/
void guideDispMain();

/*
    Renders Tippi/Tiptron
*/
void guideDisp(GuideWork * work);

/*
    Functions ran every frame based on the current runMode
*/
void runMode0Main(GuideWork * work);
void runMode1Main(GuideWork * work);
void runMode2Main(GuideWork * work);
void runMode3Main(GuideWork * work);
void runMode4Main(GuideWork * work);
void runMode5Main(GuideWork * work);

UNKNOWN_FUNCTION(func_801215f0);
UNKNOWN_FUNCTION(func_8012167c);
UNKNOWN_FUNCTION(func_80121900);
UNKNOWN_FUNCTION(func_80121948);

/*
    Sets curAnimName to an new name, and allows animChangeHandler to override it
*/
void guideSetAnim(const char * name);

/*
    Functions to change the current run mode
*/
void guideEnterRunMode(s32 runMode);
void guideEnterRunMode1();
void guideEnterRunMode2();
void guideEnterRunMode5(f32, f32, Vec3 *);

UNKNOWN_FUNCTION(func_80121ba4);
UNKNOWN_FUNCTION(func_80121bc8);
UNKNOWN_FUNCTION(func_80121bcc);
UNKNOWN_FUNCTION(guideCanSearch);
UNKNOWN_FUNCTION(func_80121d84);
UNKNOWN_FUNCTION(func_80121db4);
UNKNOWN_FUNCTION(func_80121e18);

CPP_WRAPPER_END()
