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
    float accelerationMagnitude;
    float accelerationDif;
    Vec2 pointingPos;
    Vec2 pointingPosDif;
    float pointingPosDifSpeed;
    Vec2 horizontal;
    Vec2 horizontalDif;
    float horizontalDifSpeed;
    float distance;
    float distanceDif;
    float distanceDifSpeed;
    Vec2 verticalPos;
    u8 type;
    s8 error;
    s8 dpdStatus;
    u8 dataFormat;
    u8 extension[0x24];
} KPADStatus;

void KPADSetFSStickClamp(s8 min, s8 max);
void KPADSetPosParam(s32 controller, float radius, float sensitivity);
void KPADSetHoriParam(s32 controller, float radius, float sensitivity);
void KPADSetDistParam(s32 controller, float radius, float sensitivity);
void KPADSetAccParam(s32 controller, float radius, float sensitivity);
void KPADSetBtnRepeat(s32 controller, float delay, float pulse);
void KPADCalibrateDPD(s32 controller);
s32 KPADRead(s32 controller, KPADStatus * dest, u32 maxStatuses);// 802e15a0
void KPADInit();
void KPADDisableDPD(s32 controller);
void KPADEnableDPD(s32 controller);
