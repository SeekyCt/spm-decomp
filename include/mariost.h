#ifndef MARIOST_H
#define MARIOST_H

#include <common.h>

typedef struct {
    u8 unknown_0x0[0x4 - 0x0];
    u32 unknown_0x4;
    u8 unknown_0x8[0xf8 - 0x8];
    s64 time;
    u8 unknown_0x100[0x140 - 0x100];
    s32 gsw0;
    u8 gswf[1024];
    u8 gsw[2048];
    u8 lswf[64];
    u8 lsw[1024];
    u8 unknown_0x1184[0x1ab0 - 0x1184];
} MarioStGlobals;
extern MarioStGlobals * gp;
extern MarioStGlobals marioSt;

#endif
