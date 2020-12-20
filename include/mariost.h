#ifndef MARIOST_H
#define MARIOST_H

#include <stdint.h>

typedef struct {
    char unknown_0x0[0x4 - 0x0];
    uint32_t unknown_0x4;
    char unknown_0x8[0xf8 - 0x8];
    int64_t time;
    char unknown_0x100[0x140 - 0x100];
    int gsw0;
    uint8_t gswf[1024];
    uint8_t gsw[2048];
    uint8_t lswf[64];
    uint8_t lsw[1024];
    char unknown_0x1184[0x1ab0 - 0x1184];
} MarioStGlobals;
extern MarioStGlobals * gp;
extern MarioStGlobals marioSt;

#endif