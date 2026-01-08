#pragma once

#include <common.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::framedrv)

USING(wii::gx::GXColor)

enum FrameType
{
/* 0x0 */ FRAME_TYPE_OFFSCREEN,
/* 0x1 */ FRAME_TYPE_WIRE,
/* 0x2 */ FRAME_TYPE_LINE,
/* 0x3 */ FRAME_TYPE_EVT
};

typedef struct
{
/* 0x000 */ u16 flags;
/* 0x002 */ u16 type; // see FrameType enum
/* 0x004 */ const char * instanceName;
/* 0x008 */ u8 unknown_0x008[0x014 - 0x008];
/* 0x014 */ s32 animPoseId;
/* 0x018 */ const char * animPoseName;
/* 0x01C */ u8 unknown_0x01c[0x060 - 0x01C];
/* 0x060 */ f32 length;
/* 0x064 */ f32 height;
/* 0x068 */ u8 unknown_0x068[0x078 - 0x068];
/* 0x078 */ s32 startX;
/* 0x07C */ s32 startY;
/* 0x080 */ s32 destX;
/* 0x084 */ s32 destY;
/* 0x088 */ u8 unknown_0x088[0x094 - 0x088];
/* 0x094 */ GXColor color;
/* 0x098 */ u8 unknown_0x098[0x0104 - 0x098];
} FrameEntry;
SIZE_ASSERT(FrameEntry, 0x104)

typedef struct
{
/* 0x0 */ s32 num;
/* 0x4 */ FrameEntry * entries;
} FrameWork;
SIZE_ASSERT(FrameWork, 0x8)

DECOMP_STATIC(FrameWork * framedrv_wp)

void frameInit();
void frameReInit();
void frameMain();
UNKNOWN_FUNCTION(func_80067c94)
UNKNOWN_FUNCTION(func_800680a4)
UNKNOWN_FUNCTION(func_80068254)
UNKNOWN_FUNCTION(func_800682d8)
UNKNOWN_FUNCTION(func_80068358)
UNKNOWN_FUNCTION(func_80068708)
UNKNOWN_FUNCTION(func_80068b84)
UNKNOWN_FUNCTION(func_80068e34)
UNKNOWN_FUNCTION(func_80068e60)
UNKNOWN_FUNCTION(func_80069050)
UNKNOWN_FUNCTION(func_800691c0)
UNKNOWN_FUNCTION(func_80069284)
UNKNOWN_FUNCTION(func_80069334)
UNKNOWN_FUNCTION(func_80069420)
UNKNOWN_FUNCTION(func_8006958c)
UNKNOWN_FUNCTION(func_8006966c)
UNKNOWN_FUNCTION(func_8006972c)
UNKNOWN_FUNCTION(func_800697ec)
UNKNOWN_FUNCTION(func_800698ac)
UNKNOWN_FUNCTION(func_80069a48)
void frameDispCb(s32 camId, FrameEntry * frame);
UNKNOWN_FUNCTION(func_8006a1b0)
UNKNOWN_FUNCTION(func_8006a3ac)
UNKNOWN_FUNCTION(func_8006ad9c)

CPP_WRAPPER_END()
