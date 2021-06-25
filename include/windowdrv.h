#ifndef WINDOWDRV_H
#define WINDOWDRV_H

#include <common.h>

struct _WindowEntry;

typedef void (WindowFunc)(struct _WindowEntry * );

typedef struct _WindowEntry
{
    u8 unknown_0x0[0x2 - 0x0];
    u16 flags;
    u16 pri; // uncertain
    u8 unknown_0x6[0x8 - 0x6];
    s32 unknown_0x8;
    u8 unknown_0xc[0x28 - 0xc];
    u32 unknown_0x28;
    u8 unknown_0x2c[0x3c - 0x2c];
    WindowFunc * mainFunc;
    WindowFunc * deleteFunc;
    u8 unknown_0x44[0x48 - 0x44];
} WindowEntry;

#define WINDOW_MAX 7

void windowInit();
void windowReInit();
s32 windowEntry(u16 pri);
bool windowDelete(WindowEntry * entry);
bool windowDeleteID(s32 id);
void windowMain();
// unknown_80038b08
// unknown_80038cc0
// unknown_80038fb8
// unknown_80039188
// unknown_800393c8
// unknown_800396e0
// unknown_80039b80
// unknown_80039d40
// unknown_8003a440
// unknown_8003a5e4
// windowDispGX2_Waku_col
s32 windowCheckID(s32 id);
WindowEntry * windowGetPointer(s32 id);
bool unknown_8003b180();

#endif
