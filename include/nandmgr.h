/*
    Functions to handle NAND interactions, including banner + save files
    The save games are in title/00010000/52385050/data/wiimario0X (0-3)
    The files are the SaveFile struct
*/

#ifndef NANDMGR_H
#define NANDMGR_H

#include <common.h>
#include <nand.h>
#include <spmario.h>

#define NAND_FLAG_Exec 1
#define NAND_FLAG_Waiting 2

#define NAND_BLOCK_COUNT 7
#define NAND_INODE_COUNT 5
#define NAND_ATTEMPTS_MAX 0x65

enum {
    NANDMGR_TASK_NONE,
    NANDMGR_TASK_CHECK,
    NANDMGR_TASK_WRITE_BANNER,
    NANDMGR_TASK_WRITE_ALL,
    NANDMGR_TASK_WRITE,
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
    void * openingBuffer; // used by NANDSafeOpenAsync
    size_t openingBufferSize; // 0x4000
    u8 unknown_0xc[0x10 - 0xc];
    SaveFile * saves; // array of 4
    void * unknown_0x14;
    char homedir[64];
    NANDFileInfo fileInfo;
    NANDCommandBlock commandBlock;
    NANDBanner * banner;
    u32 bannerSize; // omits unused iconTextures
    u8 unknown_0x1a4[0x1a8 - 0x1a4];
    u32 answer;
    u32 task;
    u32 stage;
    s32 code;
    u32 saveId;
} NandWork;

void nandInit(); // 8023e60c
void nandMain(); // 8023e97c
bool nandIsExec(); // 8023e9c0
s32 nandGetCode(); // 8023e9e0
SaveFile * nandGetSaveFiles(); // 8023e9ec, pointer to all 4

void nandCheck(); // 8023e9f8
void nandWriteBanner(); // 8023ea70
void nandWriteAll(); // 8023eaec
void nandWrite(u32 saveId); // 8023eb68
void nandmgrStartTask5(); // 8023ebf0
void nandmgrStartTask6(); // 8023ec6c

void nandCopy(s32 sourceId, s32 destId); // 8023ece8
void nandErase(s32 saveId); // 8023ed08
void nandUpdateSave(s32 saveId); // 8023edcc
void nandLoadSave(s32 saveId); // 8023efe0

void nandmgrSetFlag0x1000(); // 8023f0a0
void nandmgrClearFlag0x1000(); // 8023f0b4
bool nandmgrCheckFlag0x1000(); // 8023f0c8

void nandCheckMain(); // 8023f0d8
void nandWriteBannerMain(); // 8023f200
void nandWriteAllMain(); // 8023f570
void nandWriteMain(); // 8023fa24
void nandmgrTask5(); // 8023fd64
void nandmgrTask6(); // 80240414

void genericCallback(s32 result, NANDCommandBlock * commandBlock); // 80240620
void checkCallback(s32 result, NANDCommandBlock * commandBlock); // 80240648

#endif