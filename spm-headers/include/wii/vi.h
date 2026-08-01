#pragma once

#include <common.h>

CPP_WRAPPER(wii::vi)

// OnShutdown
UNKNOWN_FUNCTION(__VIRetraceHandler)
UNKNOWN_FUNCTION(VISetPreRetraceCallback)
typedef void (*VIRetraceCallback)(u32 retraceCount);
VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback);
UNKNOWN_FUNCTION(getTiming)
UNKNOWN_FUNCTION(__VIInit)
void VIInit();
void VIWaitForRetrace();
UNKNOWN_FUNCTION(setFbbRegs)
UNKNOWN_FUNCTION(setVerticalRegs)
UNKNOWN_FUNCTION(VIConfigure)
UNKNOWN_FUNCTION(VIConfigurePan)
void VIFlush();
UNKNOWN_FUNCTION(VISetNextFrameBuffer)
void VISetBlack(bool enabled);
UNKNOWN_FUNCTION(VIGetRetraceCount)
UNKNOWN_FUNCTION(VIGetNextField)
UNKNOWN_FUNCTION(VIGetCurrentLine)
u32 VIGetTvFormat();
u32 VIGetScanMode();
u32 VIGetDTVStatus();
UNKNOWN_FUNCTION(__VIDisplayPositionToXY)
void VISetVSyncTimingTest();
UNKNOWN_FUNCTION(VISetTimeToDimming)
UNKNOWN_FUNCTION(__VIResetRFIdle)
UNKNOWN_FUNCTION(__VIResetSIIdle)
UNKNOWN_FUNCTION(WaitMicroTime)
UNKNOWN_FUNCTION(sendSlaveAddr)
UNKNOWN_FUNCTION(__VISendI2CData)
UNKNOWN_FUNCTION(__VISetYUVSEL)
UNKNOWN_FUNCTION(__VISetFilter4EURGB60)
UNKNOWN_FUNCTION(__VISetCGMS)
UNKNOWN_FUNCTION(__VISetWSS)
UNKNOWN_FUNCTION(__VISetClosedCaption)
UNKNOWN_FUNCTION(__VISetMacrovision)
UNKNOWN_FUNCTION(__VISetGammaImm)
UNKNOWN_FUNCTION(__VISetGamma)
UNKNOWN_FUNCTION(__VISetTrapFilter)
UNKNOWN_FUNCTION(__VISetRGBOverDrive)
UNKNOWN_FUNCTION(VISetRGBModeImm)
UNKNOWN_FUNCTION(__VISetRGBModeImm)
UNKNOWN_FUNCTION(__VISetRevolutionModeSimple)

CPP_WRAPPER_END()
