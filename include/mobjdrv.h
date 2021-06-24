#ifndef MOBJDRV_H
#define MOBJDRV_H

#include <common.h>

typedef struct {
    u8 unknown_0x0[0x18 - 0x0];
    Vec3 pos;
    u8 unknown_0x24[0x2a8 - 0x24];
} MOBJEntry;

MOBJEntry * mobjNameToPtr(const char * name);

#endif
