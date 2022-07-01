#pragma once

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
UNKNOWN_FUNCTION(func_80038b08);
UNKNOWN_FUNCTION(func_80038cc0);
UNKNOWN_FUNCTION(func_80038fb8);
UNKNOWN_FUNCTION(windowDispGX_Kanban);
UNKNOWN_FUNCTION(func_800393c8);
UNKNOWN_FUNCTION(windowDispGX_System);
UNKNOWN_FUNCTION(func_80039b80);
UNKNOWN_FUNCTION(func_80039d40);
UNKNOWN_FUNCTION(windowDispGX_Message);
UNKNOWN_FUNCTION(windowDispGX_ItemBox);
UNKNOWN_FUNCTION(windowDispGX2_Waku_col);
s32 windowCheckID(s32 id);
WindowEntry * windowGetPointer(s32 id);
bool windowCheckOpen();
