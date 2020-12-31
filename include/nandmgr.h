#ifndef NANDMGR_H
#define NANDMGR_H

#include <common.h>
#include <nand.h>
#include <spmario.h>

typedef struct {
    u16 flags;
    u8 unknown_0x2[0x8 - 0x2];
    SpmarioGlobals spmarioGlobals;
    u8 unknown_0x1b10[0x25b0 - 0x1b10];
    u32 checksum;
    u32 checksumNOT;
} SaveFile;

typedef struct {
    u32 flag;
    void * unknown_0x4;
    size_t unknown_0x4_size;
    u8 unknown_0xc[0x10 - 0xc];
    SaveFile * saves; // array of 4
    void * unknown_0x14;
    u8 unknown_0x18[0x19c - 0x18];
    NANDBanner * banner;
    u32 unknown_0x1a0;
    u8 unknown_0x1a4[0x1a8 - 0x1a4];
    u32 unknown_0x1a8;
    u32 task;
    u32 unknown_0x1b0;
    u32 unknown_0x1b4;
    u32 unknown_0x1b8;
} NandWork;

// File is new to SPM so no symbols are official here, asserts gave nandmgr.c though

void nandmgrInit(); // 8023e60c
// more

#endif