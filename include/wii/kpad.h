/*
    Revolution SDK KPAD library
*/

#pragma once

#include <common.h>
#include <wii/types.h>

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
    u8 type;
    s8 error;
    s8 dpdStatus;
    u8 dataFormat;
    u8 extension[0x24];
} KPADStatus;

void KPADSetFSStickClamp(s8 min, s8 max);
void KPADSetPosParam(s32 controller, f32 radius, f32 sensitivity);
void KPADSetHoriParam(s32 controller, f32 radius, f32 sensitivity);
void KPADSetDistParam(s32 controller, f32 radius, f32 sensitivity);
void KPADSetAccParam(s32 controller, f32 radius, f32 sensitivity);
void KPADSetBtnRepeat(s32 controller, f32 delay, f32 pulse);
void KPADCalibrateDPD(s32 controller);
s32 KPADRead(s32 controller, KPADStatus * dest, u32 maxStatuses);// 802e15a0
void KPADInit();
void KPADDisableDPD(s32 controller);
void KPADEnableDPD(s32 controller);
