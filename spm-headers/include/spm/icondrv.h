/*
    icondrv handles the display of icons such as items and buttons
    Icon entries are handled by names, which must be unique, instead of pointers to entries
*/

#pragma once

#include <common.h>
#include <spm/filemgr.h>
#include <wii/gx.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::icondrv)

USING(spm::filemgr::FileEntry)
USING(wii::gx::GXTexObj)
USING(wii::mtx::Vec3)
USING(wii::mtx::Mtx34)

enum IconId
{
/* 0x00 */ ICON_BTN_WII_REMOTE = 0,
/* 0x01 */ ICON_BTN_1_UP,
/* 0x02 */ ICON_BTN_1_DOWN,
/* 0x03 */ ICON_BTN_2_UP,
/* 0x04 */ ICON_BTN_2_DOWN,
/* 0x05 */ ICON_BTN_A_UP,
/* 0x06 */ ICON_BTN_A_DOWN,
    /// ...
/* 0x09 */ ICON_DPAD_NEUTRAL = 9,
/* 0x0A */ ICON_BTN_A,
/* 0x0B */ ICON_BTN_POWER,
/* 0x0C */ ICON_BTN_HOME,
/* 0x0D */ ICON_BTN_MINUS,
/* 0x0E */ ICON_BTN_PLUS,
/* 0x0F */ ICON_BTN_1,
/* 0x10 */ ICON_BTN_2,
    /// ...
/* 0x25 */ ICON_DPAD_UP_PROMPT = 37,
/* 0x26 */ ICON_DPAD_DOWN_PROMPT,
/* 0x27 */ ICON_DPAD_LEFT_PROMPT,
/* 0x28 */ ICON_DPAD_RIGHT_PROMPT,
/* 0x29 */ ICON_DPAD_NEUTRAL_PROMPT,
/* 0x2A */ ICON_DPAD_UP_PROMPT_FLASH,
/* 0x2B */ ICON_DPAD_DOWN_PROMPT_FLASH,
/* 0x2C */ ICON_DPAD_LEFT_PROMPT_FLASH,
/* 0x2D */ ICON_DPAD_RIGHT_PROMPT_FLASH,
/* 0x2E */ ICON_BTN_1_PRESSING,
/* 0x2F */ ICON_BTN_2_PRESSING,
/* 0x30 */ ICON_BTN_A_PRESSING
    /// ...
};

#define MAX_ICONOBJ_NAME 16

typedef struct
{
/* 0x00 */ u32 flags;
/* 0x04 */ Vec3 position;
/* 0x10 */ f32 scale;
/* 0x14 */ u8 unknown_0x14[0x24 - 0x14];
/* 0x24 */ char name[MAX_ICONOBJ_NAME];
/* 0x34 */ s16 iconId;
/* 0x36 */ u8 unknown_0x36[0x38 - 0x36];
/* 0x38 */ s32 offscreenId;
/* 0x3C */ u8 unknown_0x3c[0x3f - 0x3c];
/* 0x3F */ u8 alpha;
/* 0x40 */ u8 unknown_0x40[0x4c - 0x40];
} IconEntry;
SIZE_ASSERT(IconEntry, 0x4c)

typedef struct
{
/* 0x00 */ s32 num; // 0x100
/* 0x04 */ IconEntry * entries; // array of num
/* 0x08 */ FileEntry * wiconTpl; // wicon.tpl from dvd root
/* 0x0C */ FileEntry * wiconbin; // wicon.bin from dvd root
} IconWork;
SIZE_ASSERT(IconWork, 0x10)

DECOMP_STATIC(IconWork icondrv_work)
DECOMP_STATIC(IconWork * icondrv_wp)

/*
    Initialise data used by icon functions
*/
void iconInit();
void iconReInit();

/*
    Loads icon files if not loaded and displays icons
*/
void iconMain();

/*
    Set up an icon to be displayed
*/
void iconEntry(const char * name, s32 iconId);
void iconEntry2D(const char * name, s32 iconId);

/*
    Stops an icon being displayed
*/
void iconDelete(const char * name);

/*
    Renders an icon
*/
void iconDisp(s8 cameraId, IconEntry * entry);

UNKNOWN_FUNCTION(func_80072da0);

void iconDispGxAlpha(f32 scale, Vec3 * position, Unk param_3, s32 iconId, u8 alpha);

/*
    Flags:
        16: seems to be required to display
         8: flip y
         4: flip x
        Others unknown
*/
void iconDispGx(f32 scale, const Vec3 * position, u32 flags, s32 iconId);

void iconDispGx2(Mtx34 * param_1, Unk param_2, s32 iconId);
void iconDispGxCol(Mtx34 * param_1, Unk param_2, s32 iconId, void * param_4);
void iconGetTexObj(GXTexObj * dest, s32 iconId);
void iconGetTexObj2(GXTexObj * dest, s32 iconId, Unk param_3, Unk param_4);
void iconGetWidthHeight(s16 * width, s16 * height, s32 iconId);
void iconGX(Mtx34 * param_1, IconEntry * entry);
IconEntry * iconNameToPtr(const char * name);
void iconSetPos(const char * name, f32 x, f32 y, f32 z);
void iconFlagOn(const char * name, u32 flag);
void iconFlagOn(const char * name, u32 flag);
void iconSetScale(const char * name, f32 scale);
void iconSetAlpha(const char * name, u8 alpha);
void iconNumberDispGx(Mtx34 * param_1, s32 number, s32 keta, Unk param_4, void * param_5,
                      Unk param_6);

UNKNOWN_FUNCTION(func_80074f80);
UNKNOWN_FUNCTION(func_80074fe8);

CPP_WRAPPER_END()
