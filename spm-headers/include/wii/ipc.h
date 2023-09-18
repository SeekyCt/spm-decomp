#pragma once

#include <common.h>

CPP_WRAPPER(wii::ipc)

/*
    IOS_Open mode param
*/
enum IosOpenMode
{
/* 0x0 */ IOS_OPEN_NONE = 0,
/* 0x1 */ IOS_OPEN_READ = 1,
/* 0x2 */ IOS_OPEN_WRITE = 2,
/* 0x3 */ IOS_OPEN_READ_WRITE = IOS_OPEN_READ | IOS_OPEN_WRITE
};

/*
    IOS_Seek mode param
*/
enum IosSeekMode
{
/* 0x0 */ IOS_SEEK_START,
/* 0x1 */ IOS_SEEK_CURRENT,
/* 0x2 */ IOS_SEEK_END
};

typedef struct
{
/* 0x0 */ void * data;
/* 0x4 */ u32 len;
} Ioctlv;
SIZE_ASSERT(Ioctlv, 0x8)

DECOMP_STATIC(s32 ipc_lbl_805ae3b4)

UNKNOWN_FUNCTION(IPCInit);
UNKNOWN_FUNCTION(IPCReadReg);
UNKNOWN_FUNCTION(IPCWriteReg);
UNKNOWN_FUNCTION(IPCGetBufferHi);
UNKNOWN_FUNCTION(IPCGetBufferLo);
UNKNOWN_FUNCTION(IPCSetBufferLo);
UNKNOWN_FUNCTION(strnlen);
UNKNOWN_FUNCTION(IpcReplyHandler);
UNKNOWN_FUNCTION(IPCInterruptHandler);
UNKNOWN_FUNCTION(IPCCltInit);
s32 __ios_Ipc2(void * param_1, s32 param_2);
UNKNOWN_FUNCTION(IOS_OpenAsync);
s32 IOS_Open(const char * path, s32 mode);
UNKNOWN_FUNCTION(IOS_CloseAsync);
s32 IOS_Close(s32 fd);
UNKNOWN_FUNCTION(IOS_ReadAsync);
s32 IOS_Read(s32 fd, void * dest, u32 length);
UNKNOWN_FUNCTION(IOS_WriteAsync);
UNKNOWN_FUNCTION(IOS_Write);
UNKNOWN_FUNCTION(IOS_SeekAsync);
UNKNOWN_FUNCTION(IOS_IoctlAsync);
s32 IOS_Ioctl(s32 fd, s32 command, void * buf, u32 bufSize, void * ioBuf, u32 ioBufSize);
UNKNOWN_FUNCTION(__ios_Ioctlv);
UNKNOWN_FUNCTION(IOS_IoctlvAsync);
s32 IOS_Ioctlv(s32 fd, s32 command, s32 inCount, s32 outCount, Ioctlv * vecs);
UNKNOWN_FUNCTION(IOS_IoctlvReboot);
UNKNOWN_FUNCTION(iosCreateHeap);
UNKNOWN_FUNCTION(__iosAlloc);
void * iosAllocAligned(s32 heapId, u32 size, u32 alignment);
UNKNOWN_FUNCTION(iosFree);
UNKNOWN_FUNCTION(IPCiProfInit);
UNKNOWN_FUNCTION(IPCiProfQueueReq);
UNKNOWN_FUNCTION(IPCiProfAck);
UNKNOWN_FUNCTION(IPCiProfReply);

CPP_WRAPPER_END()
