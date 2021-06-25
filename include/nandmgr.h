/*
    Functions to handle NAND interactions, including banner + save files
    The save games are in title/00010000/52385050/data/wiimario0X (0-3)
    The files are the SaveFile struct
*/

#ifndef NANDMGR_H
#define NANDMGR_H

#include <common.h>
#include <mario_pouch.h>
#include <nand.h>
#include <spmario.h>

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
void nandInit(); // 8023e60c

/*
    Continues the current asynchronous task
*/
void nandMain(); // 8023e97c

/*
    Checks whether an asynchronous task is still running
*/
bool nandIsExec(); // 8023e9c0

/*
    Gets the latest NAND library function return
*/
s32 nandGetCode(); // 8023e9e0

/*
    Returns a pointer to the array of all 4 save files
*/
SaveFile * nandGetSaveFiles(); // 8023e9ec

/*
    Starts NANDCheck being called to see that the game's files can all be created asynchronously
*/
void nandCheck(); // 8023e9f8

/*
    Starts banner.bin being written to NAND asynchronously 
*/
void nandWriteBanner(); // 8023ea70

/*
    Starts all save files being written to NAND asynchronously 
*/
void nandWriteAllSaves(); // 8023eaec

/*
    Starts a save file being written to NAND asynchronously 
*/
void nandWriteSave(s32 saveId); // 8023eb68

/*
    Starts writing banner.bin to and reading all save files from NAND asynchronously 
*/
void nandWriteBannerLoadAllSaves(); // 8023ebf0

/*
    Starts a save file being deleted from NAND asynchronously
    Bugged to always delete the first save file
*/
void nandDeleteSave(s32 saveId); // 8023ec6c

/*
    Copies the contents of one save file to another
    Changes copies in memory only, NAND is not written
*/
void nandCopySave(s32 sourceId, s32 destId); // 8023ece8

/*
    Clears the contents of a save file
    Changes the copy in memory only, NAND is not written
*/
void nandClearSave(s32 saveId); // 8023ed08

/*
    Updates the contents of a save file
    Changes the copy in memory only, NAND is not written
*/
void nandUpdateSave(s32 saveId); // 8023edcc

/*
    Loads the contents of a save file into the game's systems
    For example, SPMarioGlobals are copied to gp.
    Uses the copy in memory, NAND is not read
*/
void nandLoadSave(s32 saveId); // 8023efe0

/*
    Disables saving to NAND, all saves are cleared on game over.
*/
void nandDisableSaving(); // 8023f0a0

/*
    Re-enables saving to NAND
*/
void nandEnableSaving(); // 8023f0b4

/*
    Returns whether saving to NAND is enabled
*/
bool nandCheckSaving(); // 8023f0c8

/*
    Carries out the asynchronous process for nandCheck
*/
void nandCheckMain(); // 8023f0d8

/*
    Carries out the asynchronous process for nandWriteBanner
*/
void nandWriteBannerMain(); // 8023f200

/*
    Carries out the asynchronous process for nandWriteAllSaves
*/
void nandWriteAllSavesMain(); // 8023f570

/*
    Carries out the asynchronous process for nandWriteSave
*/
void nandWriteSaveMain(); // 8023fa24

/*
    Carries out the asynchronous process for nandWriteBannerLoadAllSaves
*/
void nandWriteBannerLoadAllSavesMain(); // 8023fd64

/*
    Carries out the asynchronous process for nandDeleteSave
*/
void nandDeleteSaveMain(); // 80240414

/*
    Callback for NAND library functions
*/
void genericCallback(s32 result, NANDCommandBlock * commandBlock); // 80240620

/*
    Unique callback for NANDCheck
*/
void checkCallback(s32 result, NANDCommandBlock * commandBlock); // 80240648

#endif
