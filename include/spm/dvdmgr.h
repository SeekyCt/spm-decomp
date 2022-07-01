#pragma once

#include <common.h>

typedef void (DVDMgrCallback)(s32 result, void * fileInfo);

typedef struct
{
    u8 unknown_0x0[0x7c - 0x0];
    void * unknown_0x7c;
    u8 unknown_0x80[0x9c - 0x80];
} DVDEntry;

void DVDMgrInit();
void DVDMgrDelete();

DVDEntry * DVDMgrOpen(const char * path, s32 param_2, s32 param_3);
s32 DVDMgrRead(DVDEntry * entry, void * dest, size_t length, u32 offset);
s32 DVDMgrReadAsync(DVDEntry * entry, void * dest, size_t length, u32 offset, DVDMgrCallback * callback);
void DVDMgrClose(DVDEntry * entry);
size_t DVDMgrGetLength(DVDEntry * entry);

