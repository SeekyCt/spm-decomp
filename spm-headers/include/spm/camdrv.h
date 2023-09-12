/*
    camdrv handles the game's cameras
*/

#pragma once

#include <common.h>
#include <wii/os.h>
#include <wii/gx.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::camdrv)

USING(wii::mtx::Vec3)
USING(wii::mtx::Mtx34)
USING(wii::mtx::Mtx44)
USING(wii::os::OSTime)

struct _CamEntry;
typedef void (CamDispFunc)(struct _CamEntry * entry);

#define CAMFLAG_16_9 0x10000000
#define CAMFLAG_LoadBin 0x1

enum CameraId
{
/* 0x0 */ CAM_ID_0,
/* 0x1 */ CAM_ID_1,
/* 0x2 */ CAM_ID_2,
/* 0x3 */ CAM_ID_3,
/* 0x4 */ CAM_ID_4,
/* 0x5 */ CAM_ID_3D,
/* 0x6 */ CAM_ID_6,
/* 0x7 */ CAM_ID_7,
/* 0x8 */ CAM_ID_8,
/* 0x9 */ CAM_ID_9,
/* 0xA */ CAM_ID_10,
/* 0xB */ CAM_ID_2D,
/* 0xC */ CAM_ID_12,
/* 0xD */ CAM_ID_DEBUG,
/* 0xE */ CAM_ID_DEBUG_3D,
/* 0xF */ CAM_ID_MAX
};

typedef struct _CamEntry
{
/* 0x000 */ u32 flag; // CAMFLAG defines
/* 0x004 */ s32 cam_no; // CAM_ID defines
/* 0x008 */ u16 cameraMode;
/* 0x00A */ u8 unknown_0xa[0x18 - 0xa];
/* 0x018 */ u16 scissorX;
/* 0x01A */ u16 scissorY;
/* 0x01C */ u16 scissorWidth;
/* 0x01E */ u16 scissorHeight;
/* 0x020 */ f32 viewport[6];
/* 0x038 */ f32 projection[7];
/* 0x054 */ Mtx34 viewMtx;
/* 0x084 */ u8 unknown_0x84[0x94-0x84];
/* 0x094 */ Mtx44 projMtx;
/* 0x0D4 */ u8 unknown_0xd4[0xf0 - 0xd4];
/* 0x0F0 */ f32 top;
/* 0x0F4 */ f32 bottom;
/* 0x0F8 */ f32 left;
/* 0x0FC */ f32 right;
/* 0x100 */ u8 unknown_0x100[0x158 - 0x100];
/* 0x158 */ s32 isOrtho;
/* 0x15C */ Vec3 pos;
/* 0x168 */ Vec3 target;
/* 0x174 */ Vec3 up;
/* 0x180 */ f32 targetAngle;
/* 0x184 */ f32 targetDistance;
/* 0x188 */ f32 fovY;
/* 0x18C */ f32 aspect;
/* 0x190 */ f32 near;
/* 0x194 */ f32 far;
/* 0x198 */ u8 unknown_0x198[0x20c - 0x198];
/* 0x20C */ Vec3 zoomStartPos;
/* 0x218 */ Vec3 zoomStartTarget;
/* 0x224 */ Vec3 zoomDestPos;
/* 0x230 */ Vec3 zoomDestTarget;
/* 0x23C */ u8 unknown_0x23c[0x240 - 0x23c];
/* 0x240 */ OSTime zoomStartTime;
/* 0x248 */ u8 unknown_0x248[0x254 - 0x248];
/* 0x254 */ void * cameraRoad;
/* 0x258 */ CamDispFunc * dispFunc;
/* 0x25C */ u8 unknown_0x25c[0x2f0 - 0x25c];
} CamEntry;
SIZE_ASSERT(CamEntry, 0x2f0)

/*
    Cameras:
        0-4 unknown
        5 3d
        6-10 unknown
        11 2d
        12 fade
        13 dbg
        14 dbg3d
*/
extern CamEntry * camPtrTbl[15];

/*
    Creates all camera entries and initialises other data used by camdrv
*/
void camInit();

/*
    Creates a perspective camera
*/
CamEntry * camEntryPersp(f32 fovY, f32 aspect, f32 near, f32 far, f32 param_5,
                         f32 param_6, s32 cam_no, CamDispFunc * callback);

/*
    Creates an orthographic camera
*/
CamEntry * camEntryOrtho(f32 top, f32 bottom, f32 left, f32 right, f32 near,
                         f32 far, s32 cam_no, CamDispFunc * callback);

UNKNOWN_FUNCTION(func_800550f8);

/*
    Updates all cameras
*/
void camMain();

/*
    Calls dispDraw for each camera
*/
void camDraw();

/*
    Loads a camera road file from disc asynchronously, returns whether it's done
*/
bool camLoadRoadAsync(const char * mapName);

/*
    Loads a camera road file that's already been read from disc
*/
void camLoadRoadNoRead(void * file);

/*
    Unloads a camera road file
*/
void camUnloadRoad();

/*
    Returns a camera by cam_no
*/
CamEntry * camGetPtr(s32 id);

/*
    Returns the current camera
*/
CamEntry * camGetCurPtr();

/*
    Returns the current cam_no
*/
int camGetCurNo();

UNKNOWN_FUNCTION(func_80055b58);
UNKNOWN_FUNCTION(func_80055bb4);
UNKNOWN_FUNCTION(func_80055c10);
UNKNOWN_FUNCTION(func_80055ce0);
UNKNOWN_FUNCTION(func_80056948);

/*
    CamDispFunc for the 3d camera
*/
void cam3dMain(CamEntry * cp);

UNKNOWN_FUNCTION(func_80057cb0);
UNKNOWN_FUNCTION(func_80057cc8);
UNKNOWN_FUNCTION(func_80057ce0);
UNKNOWN_FUNCTION(func_80057da0);
UNKNOWN_FUNCTION(func_80057e4c);
UNKNOWN_FUNCTION(func_80057ec4);
UNKNOWN_FUNCTION(func_80057ee8);
UNKNOWN_FUNCTION(func_80058128);

/*
    Converts coordinates in the world to where they appear on screen
*/
void getScreenPoint(Vec3 * worldPos, Vec3 * screenPosOut);

/*
    Checks the 3d flag of a camera
*/
bool camCheck3d(s32 camId);

UNKNOWN_FUNCTION(func_80058388);
UNKNOWN_FUNCTION(func_800583b4);
UNKNOWN_FUNCTION(func_80058404);
UNKNOWN_FUNCTION(func_8005869c);
UNKNOWN_FUNCTION(func_800586c8);
UNKNOWN_FUNCTION(func_80058700);
UNKNOWN_FUNCTION(func_800587a0);
UNKNOWN_FUNCTION(func_80058800);
UNKNOWN_FUNCTION(func_8005881c);
UNKNOWN_FUNCTION(func_80058840);

/*
    Just returns false, unclear purpose
*/
bool camUnusedCheck();

UNKNOWN_FUNCTION(func_8005885c);
UNKNOWN_FUNCTION(func_80058b14);
UNKNOWN_FUNCTION(func_80058c80);

CPP_WRAPPER_END()
