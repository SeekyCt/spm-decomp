/*
    /dev/fs
*/

#pragma once

#include <common.h>

CPP_WRAPPER(ios::fs)

enum FsErr
{
    ERR_FS_EINVAL = -101,
    ERR_FS_ENOENT = -106
};

enum FsIoctl
{
    IOCTL_FS_FORMAT = 1,
    IOCTL_FS_GET_STATS = 2,
    IOCTL_FS_CREATE_DIR = 3,
    IOCTL_FS_READ_DIR = 4,
    IOCTL_FS_SET_ATTR = 5,
    IOCTL_FS_GET_ATTR = 6,
    IOCTL_FS_DELETE = 7,
    IOCTL_FS_RENAME = 8,
    IOCTL_FS_CREATE_FILE = 9,
    IOCTL_FS_GET_FILE_STATS = 11,
    IOCTL_FS_SHUTDOWN = 14
};

/*
    GetFileStats output
*/
typedef struct
{
/* 0x0 */ u32 length;
/* 0x4 */ u32 position;
} FsFileStats;
SIZE_ASSERT(FsFileStats, 8)

CPP_WRAPPER_END()
