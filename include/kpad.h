/*
    Revolution SDK KPAD library
*/

#ifndef KPAD_H
#define KPAD_H

#include <common.h>

typedef struct {
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

void KPADSetFSStickClamp(s8 min, s8 max); // 802df1b4
void KPADSetPosParam(s32 controller, float radius, float sensitivity); // 802df258
void KPADSetHoriParam(s32 controller, float radius, float sensitivity); // 802df274
void KPADSetDistParam(s32 controller, float radius, float sensitivity); // 802df290
void KPADSetAccParam(s32 controller, float radius, float sensitivity); // 802df2ac
void KPADSetBtnRepeat(s32 controller, float delay, float pulse); // 802df1c8
void KPADCalibrateDPD(s32 controller); // 802df49c
void KPADInit(); // 802e1cb8

#endif