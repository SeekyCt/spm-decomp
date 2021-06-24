/*
    Unknown functions and data from unknown files
*/

#ifndef SOMEWHERE_H
#define SOMEWHERE_H

#include <common.h>

 // referenced in swdrv.c
extern s32 * unknown_805adf40;

// referenced in nandmgr.c
typedef struct {
    u8 unknown_0x0[0x4 - 0x0];
    u32 unknown_0x4;
    u32 unknown_0x8;
    u8 unknown_0xc[0x38 - 0xc];
    s8 unknown_0x38;
    u8 unknown_0x39[0x5c - 0x39];
    float unknown_0x5c;
    float unknown_0x60;
    float unknown_0x64;
    // unknown size
} UnkStruct80510458;
typedef struct {
    u8 unknown_0x0[0x728 - 0x0];
    u8 unknown_0x728[0x400];
} UnkStruct805283e0;
u16 * unknown_0x8011fa2c();
UnkStruct80510458 * unknown_0x80121f48();
void * unknown_0x8014c088();
UnkStruct805283e0 * unknown_0x801c9adc();

// referenced in dan.c
void func_800b426c(float, float, float, int, int);
void func_800cd554(float, float, float, float, float, float, int, int);

#endif
