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

void windowInit(); // 800388b4
void windowReInit(); // 8003891c
s32 windowEntry(u16 pri); // 80038960
bool windowDelete(WindowEntry * entry); // 800389e8
bool windowDeleteID(s32 id); // 80038a34
void windowMain(); // 80038aa0
// func_80038b08
// func_80038cc0
// func_80038fb8
// func_80039188
// func_800393c8
// func_800396e0
// func_80039b80
// func_80039d40
// func_8003a440
// func_8003a5e4
// windowDispGX2_Waku_col 8003ab80
s32 windowCheckID(s32 id); // 8003b158
WindowEntry * windowGetPointer(s32 id); // 8003b170
bool func_8003b180();

#endif
