#pragma once

#include <common.h>
#include <wii/os.h>

CPP_WRAPPER(wii::dvd)

USING(wii::os::OSThreadQueue)

#define DVD_ALIGN 32

struct _DVDCommandBlock;
typedef void (DVDCBCallback)(s32 code, struct _DVDCommandBlock * block);

typedef struct _DVDCommandBlock
{
/* 0x00 */ u8 unknown_0x0[0x8 - 0x0];
/* 0x08 */ s32 command;
/* 0x0C */ s32 state;
/* 0x10 */ u32 offset;
/* 0x14 */ u8 unknown_0x14[0x28 - 0x14];
/* 0x28 */ DVDCBCallback * callback;
/* 0x2C */ void * userData;
} DVDCommandBlock;
SIZE_ASSERT(DVDCommandBlock, 0x30)

typedef struct _DVDFileInfo
{
/* 0x00 */ DVDCommandBlock commandBlock;
/* 0x30 */ u32 startAddr;
/* 0x34 */ u32 length;
/* 0x38 */ u8 unknown_0x38[0x3c - 0x38];
} DVDFileInfo;
SIZE_ASSERT(DVDFileInfo, 0x3c)

typedef void (DVDFICallback)(s32 code, struct _DVDFileInfo * fileInfo);

typedef struct
{
    u8 isDir : 8;
    u32 stringOffset : 24;
    union
    {
        struct
        {
            u32 unknown_0x4;
            u32 next;
        } dir;
        struct
        {
            u32 startAddr;
            u32 length;
        } file;
    };
} FstEntry;
SIZE_ASSERT(FstEntry, 0xc)

typedef struct
{
    u32 entrynum;
    u32 location;
    u32 next;
} DVDDir;
SIZE_ASSERT(DVDDir, 0xc)

typedef struct
{
    u32 entrynum;
    BOOL isDir;
    const char * name;
} DVDDirEntry;
SIZE_ASSERT(DVDDirEntry, 0xc)

// Just a normal string literal, but useful for riivo detection
extern char devDiStr[]; // "/dev/di"

DECOMP_STATIC(OSThreadQueue __DVDThreadQueue)
DECOMP_STATIC(u32 MaxEntryNum)
DECOMP_STATIC(char * FstStringStart)
DECOMP_STATIC(FstEntry * FstStart)
DECOMP_STATIC(u32 PauseFlag)
DECOMP_STATIC(u32 executing)


UNKNOWN_FUNCTION(__DVDFSInit);
s32 DVDConvertPathToEntrynum(const char * path);
s32 DVDFastOpen(s32 entrynum, DVDFileInfo * fileInfo);
s32 DVDClose(DVDFileInfo * fileInfo);
s32 DVDReadAsyncPrio(DVDFileInfo * fileInfo, void * addr, s32 length, s32 offset,
                     DVDFICallback * callback, s32 priority);
UNKNOWN_FUNCTION(cbForReadAsync);
s32 DVDReadPrio(DVDFileInfo * fileInfo, void * dest, s32 length, s32 offset, s32 priority);
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
s32 __DVDPushWaitingQueue(s32 priority, DVDCommandBlock * commandBlock);
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
