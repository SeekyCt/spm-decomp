/*
    Revolution SDK KPAD library
*/

#pragma once

#include <common.h>
#include <wii/mtx.h>

CPP_WRAPPER(wii::kpad)

USING(wii::mtx::Vec2)
USING(wii::mtx::Vec3)

#define KPAD_EXTENSION_FS 1

typedef struct
{
    Vec2 stickPos;
    Vec3 acceleration;
    f32 accelerationMagnitude;
    f32 accelerationDif;
} KPADFsExt;
SIZE_ASSERT(KPADFsExt, 0x1c)

typedef struct
{
    u32 buttonsHeld; // all buttons down
    u32 buttonsPressed; // buttons down this frame but not last
    u32 buttonsReleased; // buttons released this frame
    Vec3 acceleration;
    f32 accelerationMagnitude;
    f32 accelerationDif;
    Vec2 pointingPos;
    Vec2 pointingPosDif;
    f32 pointingPosDifSpeed;
    Vec2 horizontal;
    Vec2 horizontalDif;
    f32 horizontalDifSpeed;
    f32 distance;
    f32 distanceDif;
    f32 distanceDifSpeed;
    Vec2 verticalPos;
    u8 extensionType;
    s8 error;
    s8 dpdStatus;
    u8 dataFormat;
    union
    {
        KPADFsExt fs;
        u8 unk[0x24];
    } extension;
} KPADStatus;
SIZE_ASSERT(KPADStatus, 0x84)

void KPADSetFSStickClamp(s8 min, s8 max);
void KPADSetBtnRepeat(s32 controller, f32 delay, f32 pulse);
void KPADSetPosParam(s32 controller, f32 radius, f32 sensitivity);
void KPADSetHoriParam(s32 controller, f32 radius, f32 sensitivity);
void KPADSetDistParam(s32 controller, f32 radius, f32 sensitivity);
void KPADSetAccParam(s32 controller, f32 radius, f32 sensitivity);
UNKNOWN_FUNCTION(reset_kpad);
void KPADCalibrateDPD(s32 controller);
UNKNOWN_FUNCTION(calc_button_repeat);
UNKNOWN_FUNCTION(calc_acc_horizon);
UNKNOWN_FUNCTION(calc_acc_vertical);
UNKNOWN_FUNCTION(read_kpad_acc);
UNKNOWN_FUNCTION(select_2obj_first);
UNKNOWN_FUNCTION(select_2obj_continue);
UNKNOWN_FUNCTION(select_1obj_first);
UNKNOWN_FUNCTION(select_1obj_continue);
UNKNOWN_FUNCTION(calc_dpd_variable);
UNKNOWN_FUNCTION(read_kpad_dpd);
UNKNOWN_FUNCTION(clamp_stick_circle);
UNKNOWN_FUNCTION(clamp_stick_cross);
UNKNOWN_FUNCTION(read_kpad_stick);
s32 KPADRead(s32 controller, KPADStatus * dest, u32 maxStatuses);
void KPADInit();
void KPADDisableDPD(s32 controller);
void KPADEnableDPD(s32 controller);
UNKNOWN_FUNCTION(KPADiSamplingCallback);
UNKNOWN_FUNCTION(KPADiControlDpdCallback);

CPP_WRAPPER_END()
