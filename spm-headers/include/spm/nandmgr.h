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

CPP_WRAPPER(spm::nandmgr)

USING(spm::mario_pouch::MarioPouchWork)
USING(spm::spmario::SpmarioGlobals)
USING(wii::nand::NANDBanner)
USING(wii::nand::NANDCommandBlock)
USING(wii::nand::NANDFileInfo)

#define NAND_BLOCK_COUNT 7
#define NAND_INODE_COUNT 5
#define NAND_ATTEMPTS_MAX 100

enum NandTask
{
/* 0x0 */ NANDMGR_TASK_NONE,
/* 0x1 */ NANDMGR_TASK_CHECK,
/* 0x2 */ NANDMGR_TASK_WRITE_BANNER,
/* 0x3 */ NANDMGR_TASK_WRITE_ALL_SAVES,
/* 0x4 */ NANDMGR_TASK_WRITE_SAVE,
/* 0x5 */ NANDMGR_TASK_WRITE_BANNER_LOAD_ALL_SAVES,
/* 0x6 */ NANDMGR_TASK_DELETE_SAVE
};

#define SAVE_FLAG_1 1
#define SAVE_FLAG_CORRUPT 2

typedef struct
{
/* 0x0000 */ u16 flags;
/* 0x0002 */ u8 unknown_0x2[0x8 - 0x2];
/* 0x0008 */ SpmarioGlobals spmarioGlobals;
/* 0x1B10 */ MarioPouchWork pouch;
/* 0x21B0 */ u8 unknown_0x21b0[0x400];
    // Checksums are calculated with checksum set to 0 and checksumNOT set to 0xffff
/* 0x25B0 */ u32 checksum;
/* 0x25B4 */ u32 checksumNOT;
} SaveFile;
SIZE_ASSERT(SaveFile, 0x25b8)

#define SAVE_FILE_COUNT 4

#define NAND_FLAG_Exec 1
#define NAND_FLAG_Waiting 2
#define NAND_FLAG_Error 4
#define NAND_FLAG_NoSave 0x1000

// 32-byte alignment?
#define NAND_TEMP_SAVE_SIZE (sizeof(SaveFile) + 0x8)

typedef struct
{
/* 0x000 */ u32 flag;
/* 0x004 */ void * openingBuffer; // used by NANDSafeOpenAsync
/* 0x008 */ size_t openingBufferSize; // 0x4000
/* 0x00C */ u8 unknown_0xc[0x10 - 0xc];
/* 0x010 */ SaveFile * saves; // array of 4
/* 0x014 */ void * tempSaveFile; // NAND_TEMP_SAVE_FILE_SIZE bytes
/* 0x018 */ char homedir[64];
/* 0x058 */ NANDFileInfo fileInfo;
/* 0x0E4 */ NANDCommandBlock commandBlock;
/* 0x19C */ NANDBanner * banner;
/* 0x1A0 */ u32 bannerSize; // omits unused iconTextures
/* 0x1A4 */ void * tempBanner;
/* 0x1A8 */ u32 answer;
/* 0x1AC */ u32 task; // used by task main functions to track progress
/* 0x1B0 */ s32 stage;
/* 0x1B4 */ s32 code;
/* 0x1B8 */ s32 saveId; // slot of save file to work on
} NandWork;
#ifdef SPM_KR0
SIZE_ASSERT(NandWork, 0x1c0)
#else
SIZE_ASSERT(NandWork, 0x1bc)
#endif

DECOMP_STATIC(NandWork nandmgr_work)
DECOMP_STATIC(NandWork * nandmgr_wp)

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
    Starts NANDCheck being called asynchronously to see that the game's files can all be created
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
    Starts all save files being deleted from NAND asynchronously
    Parameter value is ignored
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
DECOMP_STATIC(void nandmgr_genericCallback(s32 result, NANDCommandBlock * commandBlock))

/*
    Unique callback for NANDCheck
*/
DECOMP_STATIC(void nandmgr_checkCallback(s32 result, NANDCommandBlock * commandBlock))

CPP_WRAPPER_END()
