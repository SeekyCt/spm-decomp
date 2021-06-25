#ifndef DVDDMGR_H
#define DVDMGR_H

#include <common.h>

typedef struct
{
    u8 unknown_0x0[0x7c - 0x0];
    void * unknown_0x7c;
    u8 unknown_0x80[0x9c - 0x80];
} DVDEntry;

#endif
