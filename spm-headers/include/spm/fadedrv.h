#pragma once

#include <common.h>
#include <wii/mtx.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::fadedrv)

USING(wii::mtx::Vec3)
USING(wii::gx::GXColor)

typedef struct {
/* 0x00 */ u16 flags;
/* 0x02 */ u8 unknown_0x2[0x4 - 0x2];
/* 0x04 */ s32 transitionType;
/* 0x08 */ u8 unknown_0x8[0x10 - 0x8];
/* 0x10 */ s32 entryP2Sign;
/* 0x14 */ s32 entryP2;
/* 0x18 */ u8 unknown_0x18[0x20 - 0x18];
/* 0x20 */ Vec3 fadeCenterPosition;
/* 0x2C */ u8 unknown_0x2c[0x68 - 0x2c];
/* 0x68 */ const char * animPoseName;
/* 0x6C */ u8 unknown_0x6c[0x70 - 0x6c];
/* 0x70 */ s32 animPoseAnimName;
/* 0x74 */ s32 unkAnimposeId;
/* 0x78 */ s32 unkAnimPaperPoseId;
/* 0x7c */ u8 unknown_0x7c[0x80 - 0x7c];
/* 0x80 */ void * ip;
/* 0x84 */ u8 unknown_0x84[0x90 - 0x84];
} FadeEntry;
SIZE_ASSERT(FadeEntry, 0x90)

typedef struct {
/* 0x000 */ u8 unknown_0x0[0x8];
/* 0x008 */ FadeEntry entries[4];
/* 0x248 */ u8 unknown_0x248[0x24c - 0x248];
/* 0x24C */ s32 mapChangeInTransition;
/* 0x250 */ s32 mapChangeOutTransition;
/* 0x254 */ s32 fadeOutTime;
/* 0x258 */ s32 fadeInTime;
/* 0x25c */ u8 unknown_025c[0x260 - 0x25c];
} FadeWork;
SIZE_ASSERT(FadeWork, 0x260)

DECOMP_STATIC(FadeWork * fadedrv_wp)

void fadeInit();
void fadeEntry(s32 transitionType, s32 lengthMsec, GXColor colour);
UNKNOWN_FUNCTION(func_80066558)
void fadeMain();
UNKNOWN_FUNCTION(func_80066e4c)
UNKNOWN_FUNCTION(func_80067588)
bool fadeIsFinish();
UNKNOWN_FUNCTION(func_80067824)
UNKNOWN_FUNCTION(func_8006783c)
s32 fadeGetMapChangeInTransition();
s32 fadeGetMapChangeOutTransition();

/*
    Sets the transition to be used on the next map change
    Doesn't affect anything internally in fadedrv, it's just stored with a getter
    for callers (in map change and minigames) of fadeEntry to use
*/
void fadeSetMapChangeTransition(s32 in, s32 out);

s32 fadeGetFadeOutLength();
s32 fadeGetFadeInLength();
void fadeSetTransitionLengths(s32 out, s32 in);
UNKNOWN_FUNCTION(func_80067928)
UNKNOWN_FUNCTION(func_800679d0)
UNKNOWN_FUNCTION(func_800679d8)

CPP_WRAPPER_END()
