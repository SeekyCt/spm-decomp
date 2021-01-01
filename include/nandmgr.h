#ifndef NANDMGR_H
#define NANDMGR_H

#include <common.h>
#include <nand.h>
#include <spmario.h>

#define NAND_FLAG_Exec 1

enum {
    NANDMGR_TASK_NONE,
    NANDMGR_TASK_1,
    NANDMGR_TASK_2,
    NANDMGR_TASK_3,
    NANDMGR_TASK_4,
    NANDMGR_TASK_5,
    NANDMGR_TASK_6
};

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
    s32 unknown_0x1b4;
    u32 unknown_0x1b8;
} NandWork;

// File is new to SPM so no symbols are official here, asserts gave nandmgr.c though

void nandmgrInit(); // 8023e60c
void nandmgrMain(); // 8023e97c
bool nandmgrCheckFlag1Or2(); // 8023e9c0
s32 nandmgrGet1b4(); // 8023e9e0
SaveFile * nandmgrGetSaveFiles(); // 8023e9ec, pointer to all 4
void nandmgrStartTask1(); // 8023e9f8
void nandmgrStartTask2(); // 8023ea70
void nandmgrStartTask3(); // 8023eaec
void nandmgrStartTask4(u32 unknown_1b8); // 8023eb68
void nandmgrStartTask5(); // 8023ebf0
void nandmgrStartTask6(); // 8023ec6c
void nandmgrCopySave(s32 sourceId, s32 destId); // 8023ece8
void nandmgrInitSave(s32 saveId); // 8023ed08
void nandmgrUpdateSave(s32 saveId); // 8023edcc
void nandmgrLoadSave(s32 saveId); // 8023efe0
void nandmgrSetFlag0x1000(); // 8023f0a0
void nandmgrClearFlag0x1000(); // 8023f0b4
bool nandmgrCheckFlag0x1000(); // 8023f0c8
void nandmgrTask1(); // 8023f0d8
void nandmgrTask2(); // 8023f200
void nandmgrTask3(); // 8023f570
void nandmgrTask4(); // 8023fa24
void nandmgrTask5(); // 8023fd64
void nandmgrTask6(); // 80240414
// 80240620
// 80240648

#endif