/*
    windowdrv handles some of the game's UI
*/

#pragma once

#include <common.h>
#include <spm/memory.h>
#include <wii/gx.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::windowdrv)

USING(spm::memory::SmartAllocation)
USING(wii::gx::GXColor)
USING(wii::mtx::Mtx34)
USING(wii::mtx::Vec2)
USING(wii::mtx::Vec3)

struct _WindowEntry;

typedef void (WindowFunc)(struct _WindowEntry * entry);

typedef struct _WindowEntry
{
/* 0x00 */ u8 unknown_0x0[0x2 - 0x0];
    /*
        1 active
        Others unknown
    */
/* 0x02 */ u16 flags;
/* 0x04 */ u16 priority;
/* 0x06 */ u8 unknown_0x6[0x8 - 0x6];
    /*
        0 - normal
        1 - small (used for crowds shouting)
        2 - sign
        3 - system
        4 - sign 2
        5 - selection
        6 - TTYD dragon style (unused?)
        7 - written on letter (used for intermission texts)
        8 - purple (used by spider Mimi)
        9 - computer
        10 - invisible (just text)
        11 - pixl
        12 - Tiptron
        13 - alternate system (used in underchomp battle)
        14 - invisible with alternate icon (using in Francis dating)
        15 - alternate selection (used in Francis dating)
    */
/* 0x08 */ s32 type;
/* 0x0C */ Vec2 pos;
/* 0x14 */ Vec2 scale;
/* 0x1C */ u8 unknown_0x1c[0x20 - 0x1c];
/* 0x20 */ s64 lastUpdateTime;
/* 0x28 */ SmartAllocation * speakerSp; // smart pointer to MsgSpeaker
/* 0x2C */ u8 unknown_0x2c[0x30 - 0x2c];
/* 0x30 */ Vec3 speakerPos;
/* 0x3C */ WindowFunc * mainFunc;
/* 0x40 */ WindowFunc * deleteFunc;
/* 0x44 */ u8 unknown_0x44[0x48 - 0x44];
} WindowEntry;
SIZE_ASSERT(WindowEntry, 0x48)

#define WINDOW_MAX 7
DECOMP_STATIC(WindowEntry * windowdrv_wp) // array of WINDOW_MAX

/*
    Allocates & clears entries
*/
void windowInit();

/*
    Clears entries
*/
void windowReInit();

/*
    Creates an empty window
*/
s32 windowEntry(u16 priority);

/*
    Delete window entry
*/
bool windowDelete(WindowEntry * entry);
bool windowDeleteID(s32 id);

/*
    Call mainFunc for all active entries
*/
void windowMain();

UNKNOWN_FUNCTION(func_80038b08);
UNKNOWN_FUNCTION(func_80038cc0);
UNKNOWN_FUNCTION(func_80038fb8);

/*
    Draws the background for a sign message
*/
void windowDispGX_Kanban(s32 type, GXColor * colour, f32 x, f32 y, f32 width, f32 height);

UNKNOWN_FUNCTION(func_800393c8);

/*
    Draws the background for a system message
*/
void windowDispGX_System(s32 type, u8 alpha, f32 x, f32 y, f32 width, f32 height);

UNKNOWN_FUNCTION(func_80039b80);
UNKNOWN_FUNCTION(func_80039d40);

/*
    Draws the background for a regular message
*/
void windowDispGX_Message(s32 type, Unk, u8 alpha, f32 x, f32 y, f32 width, f32 height, f32, f32);

/*
    Draws the background for an item list
*/
void windowDispGX_ItemBox(Unk, GXColor * colour, f32 x, f32 y, f32 width, f32 height);

/*
    Draws a solid colour rectangle with a black border
*/
void windowDispGX2_Waku_col(Mtx34 * mtx, u32 gxTexMapId, const GXColor * colour, f32 x, f32 y,
                            f32 width, f32 height, f32 curve);

/*
    Checks flag & 2 for a window
    Returns either 2 or 0 instead of a normalised bool
*/
s32 windowCheckID(s32 id);

/*
    Returns a window by id
*/
WindowEntry * windowGetPointer(s32 id);

/*
    Returns whether any window is open, excluding type 1
*/
bool windowCheckOpen();

CPP_WRAPPER_END()
