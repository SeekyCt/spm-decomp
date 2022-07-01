/*
    Functions to handle NAND interactions, including banner + save files
    The save games are in title/00010000/52385050/data/wiimario0X (0-3)
    The files are the SaveFile struct
*/

#pragma once

#include <common.h>
#include <spm/mario_pouch.h>
#include <spm/spmario.h>
#include <wii/nand.h>

#define NAND_FLAG_Exec 1
#define NAND_FLAG_Waiting 2
// 4 is had an error?
#define NAND_FLAG_NoSave 0x1000

#define NAND_BLOCK_COUNT 7
#define NAND_INODE_COUNT 5
#define NAND_ATTEMPTS_MAX 100

enum NandTask
{
    NANDMGR_TASK_NONE,
    NANDMGR_TASK_CHECK,
    NANDMGR_TASK_WRITE_BANNER,
    NANDMGR_TASK_WRITE_ALL_SAVES,
    NANDMGR_TASK_WRITE_SAVE,
    NANDMGR_TASK_WRITE_BANNER_LOAD_ALL_SAVES,
    NANDMGR_TASK_DELETE_SAVE
};

typedef struct
{
    u16 flags; // 2 is corrupt
    u8 unknown_0x2[0x8 - 0x2];
    SpmarioGlobals spmarioGlobals;
    MarioPouchWork pouch;
    u8 unknown_0x21b0[0x400];
    u32 checksum;
    u32 checksumNOT;
} SaveFile;

typedef struct
{
    u32 flag;
    void * openingBuffer; // used by NANDSafeOpenAsync
    size_t openingBufferSize; // 0x4000
    u8 unknown_0xc[0x10 - 0xc];
    SaveFile * saves; // array of 4
    void * tempSaveFile; // 0x25c0 allocation
    char homedir[64];
    NANDFileInfo fileInfo;
    NANDCommandBlock commandBlock;
    NANDBanner * banner;
    u32 bannerSize; // omits unused iconTextures
    void * tempBanner;
    u32 answer;
    u32 task;
    u32 stage;
    s32 code;
    s32 saveId;
} NandWork;

/*
    Initialises banner, work and saves
    Saves are zerod with hashes corrected, not loaded from NAND
*/
void nandInit();

/*
    Continues the current asynchronous task
*/
void nandMain();

/*
    Checks whether an asynchronous task is still running
*/
bool nandIsExec();

/*
    Gets the latest NAND library function return
*/
s32 nandGetCode();

/*
    Returns a pointer to the array of all 4 save files
*/
SaveFile * nandGetSaveFiles();

/*
    Starts NANDCheck being called to see that the game's files can all be created asynchronously
*/
void nandCheck();

/*
    Starts banner.bin being written to NAND asynchronously 
*/
void nandWriteBanner();

/*
    Starts all save files being written to NAND asynchronously 
*/
void nandWriteAllSaves();

/*
    Starts a save file being written to NAND asynchronously 
*/
void nandWriteSave(s32 saveId);

/*
    Starts writing banner.bin to and reading all save files from NAND asynchronously 
*/
void nandWriteBannerLoadAllSaves();

/*
    Starts a save file being deleted from NAND asynchronously
    Bugged to always delete the first save file
*/
void nandDeleteSave(s32 saveId);

/*
    Copies the contents of one save file to another
    Changes copies in memory only, NAND is not written
*/
void nandCopySave(s32 sourceId, s32 destId);

/*
    Clears the contents of a save file
    Changes the copy in memory only, NAND is not written
*/
void nandClearSave(s32 saveId);

/*
    Updates the contents of a save file
    Changes the copy in memory only, NAND is not written
*/
void nandUpdateSave(s32 saveId);

/*
    Loads the contents of a save file into the game's systems
    For example, SPMarioGlobals are copied to gp.
    Uses the copy in memory, NAND is not read
*/
void nandLoadSave(s32 saveId);

/*
    Disables saving to NAND, all saves are cleared on game over.
*/
void nandDisableSaving();

/*
    Re-enables saving to NAND
*/
void nandEnableSaving();

/*
    Returns whether saving to NAND is enabled
*/
bool nandCheckSaving();

/*
    Carries out the asynchronous process for nandCheck
*/
void nandCheckMain();

/*
    Carries out the asynchronous process for nandWriteBanner
*/
void nandWriteBannerMain();

/*
    Carries out the asynchronous process for nandWriteAllSaves
*/
void nandWriteAllSavesMain();

/*
    Carries out the asynchronous process for nandWriteSave
*/
void nandWriteSaveMain();

/*
    Carries out the asynchronous process for nandWriteBannerLoadAllSaves
*/
void nandWriteBannerLoadAllSavesMain();

/*
    Carries out the asynchronous process for nandDeleteSave
*/
void nandDeleteSaveMain();

/*
    Callback for NAND library functions
*/
void genericCallback(s32 result, NANDCommandBlock * commandBlock);

/*
    Unique callback for NANDCheck
*/
void checkCallback(s32 result, NANDCommandBlock * commandBlock);
