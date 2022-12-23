#pragma once

#include <common.h>

CPP_WRAPPER(wii::dvd)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x3c - 0x0];
} DVDFileInfo;
SIZE_ASSERT(DVDFileInfo, 0x3c);

// Just a normal string literal, but useful for riivo detection
extern char devDiStr[]; // "/dev/di"

UNKNOWN_FUNCTION(__DVDFSInit);
s32 DVDConvertPathToEntrynum(const char * path);
UNKNOWN_FUNCTION(DVDFastOpen);
UNKNOWN_FUNCTION(DVDClose);
UNKNOWN_FUNCTION(DVDReadAsyncPrio);
UNKNOWN_FUNCTION(cbForReadAsync);
UNKNOWN_FUNCTION(DVDReadPrio);
UNKNOWN_FUNCTION(cbForReadSync);
UNKNOWN_FUNCTION(StampCommand);
UNKNOWN_FUNCTION(defaultOptionalCommandChecker);
UNKNOWN_FUNCTION(DVDInit);
UNKNOWN_FUNCTION(stateReadingFST);
UNKNOWN_FUNCTION(cbForStateReadingFST);
UNKNOWN_FUNCTION(FatalAlarmHandler);
UNKNOWN_FUNCTION(cbForStateError);
UNKNOWN_FUNCTION(cbForStoreErrorCode1);
UNKNOWN_FUNCTION(cbForStoreErrorCode2);
UNKNOWN_FUNCTION(CategorizeError);
UNKNOWN_FUNCTION(cbForStoreErrorCode3);
UNKNOWN_FUNCTION(cbForStateGettingError);
UNKNOWN_FUNCTION(cbForUnrecoveredError);
UNKNOWN_FUNCTION(cbForUnrecoveredErrorRetry);
UNKNOWN_FUNCTION(cbForStateGoToRetry);
UNKNOWN_FUNCTION(stateCheckID);
UNKNOWN_FUNCTION(cbForStateReadingTOC);
UNKNOWN_FUNCTION(cbForStateReadingPartitionInfo);
UNKNOWN_FUNCTION(cbForStateOpenPartition);
UNKNOWN_FUNCTION(cbForStateOpenPartition2);
UNKNOWN_FUNCTION(cbForStateCheckID1);
UNKNOWN_FUNCTION(cbForStateCheckID2);
UNKNOWN_FUNCTION(stateCoverClosed);
UNKNOWN_FUNCTION(ResetAlarmHandler);
UNKNOWN_FUNCTION(cbForStateReset);
UNKNOWN_FUNCTION(stateDownRotation);
UNKNOWN_FUNCTION(cbForStateDownRotation);
UNKNOWN_FUNCTION(stateCoverClosed_CMD);
UNKNOWN_FUNCTION(cbForStateCoverClosed);
UNKNOWN_FUNCTION(cbForPrepareCoverRegister);
UNKNOWN_FUNCTION(CoverAlarmHandler);
UNKNOWN_FUNCTION(stateReady);
UNKNOWN_FUNCTION(stateBusy);
UNKNOWN_FUNCTION(cbForStateBusy);
UNKNOWN_FUNCTION(DVDReadAbsAsyncPrio);
UNKNOWN_FUNCTION(DVDInquiryAsync);
UNKNOWN_FUNCTION(DVDGetCommandBlockStatus);
UNKNOWN_FUNCTION(DVDGetDriveStatus);
UNKNOWN_FUNCTION(DVDSetAutoInvalidation);
UNKNOWN_FUNCTION(DVDResume);
UNKNOWN_FUNCTION(DVDCancelAsync);
UNKNOWN_FUNCTION(DVDCancel);
UNKNOWN_FUNCTION(cbForCancelSync);
UNKNOWN_FUNCTION(__BS2DVDLowCallback);
UNKNOWN_FUNCTION(__DVDGetCoverStatus);
UNKNOWN_FUNCTION(__DVDPrepareResetAsync);
UNKNOWN_FUNCTION(Callback);
UNKNOWN_FUNCTION(__DVDPrepareReset);
UNKNOWN_FUNCTION(__DVDTestAlarm);
UNKNOWN_FUNCTION(__DVDStopMotorAsync);
UNKNOWN_FUNCTION(__DVDRestartMotor);
UNKNOWN_FUNCTION(__DVDClearWaitingQueue);
UNKNOWN_FUNCTION(__DVDPushWaitingQueue);
UNKNOWN_FUNCTION(__DVDPopWaitingQueue);
UNKNOWN_FUNCTION(__DVDCheckWaitingQueue);
UNKNOWN_FUNCTION(__DVDGetNextWaitingQueue);
UNKNOWN_FUNCTION(__DVDDequeueWaitingQueue);
UNKNOWN_FUNCTION(cbForNandClose);
UNKNOWN_FUNCTION(cbForNandWrite);
UNKNOWN_FUNCTION(cbForNandSeek);
UNKNOWN_FUNCTION(cbForNandCreate);
UNKNOWN_FUNCTION(cbForNandCreateDir);
UNKNOWN_FUNCTION(__DVDStoreErrorCode);
UNKNOWN_FUNCTION(DVDCompareDiskID);
UNKNOWN_FUNCTION(__DVDShowFatalMessage);
UNKNOWN_FUNCTION(DVDSetAutoFatalMessaging);
UNKNOWN_FUNCTION(__DVDGetAutoFatalMessaging);
UNKNOWN_FUNCTION(__DVDPrintFatalMessage);
UNKNOWN_FUNCTION(doTransactionCallback);
UNKNOWN_FUNCTION(doCoverCallback);
UNKNOWN_FUNCTION(DVDLowInit);
UNKNOWN_FUNCTION(DVDLowReadDiskId);
UNKNOWN_FUNCTION(DVDLowOpenPartition);
UNKNOWN_FUNCTION(DVDLowClosePartition);
UNKNOWN_FUNCTION(DVDLowUnencryptedRead);
UNKNOWN_FUNCTION(DVDLowStopMotor);
UNKNOWN_FUNCTION(DVDLowInquiry);
UNKNOWN_FUNCTION(DVDLowRequestError);
UNKNOWN_FUNCTION(DVDLowSetSpinupFlag);
UNKNOWN_FUNCTION(DVDLowReset);
UNKNOWN_FUNCTION(DVDLowAudioBufferConfig);
UNKNOWN_FUNCTION(DVDLowSetMaxRotation);
UNKNOWN_FUNCTION(DVDLowRead);
UNKNOWN_FUNCTION(DVDLowSeek);
UNKNOWN_FUNCTION(DVDLowGetCoverRegister);
UNKNOWN_FUNCTION(DVDLowPrepareCoverRegister);
UNKNOWN_FUNCTION(DVDLowGetImmBufferReg);
UNKNOWN_FUNCTION(DVDLowUnmaskStatusInterrupts);
UNKNOWN_FUNCTION(DVDLowMaskCoverInterrupt);
UNKNOWN_FUNCTION(DVDLowClearCoverInterrupt);
UNKNOWN_FUNCTION(__DVDLowTestAlarm);

CPP_WRAPPER_END()
