/*
    msgdrv handles the reading of message text files and some of the on-screen display of dialogue
*/

#pragma once

#include <common.h>
#include <spm/memory.h>
#include <spm/windowdrv.h>
#include <wii/dvd.h>

CPP_WRAPPER(spm::msgdrv)

USING(spm::memory::SmartAllocation)
USING(spm::windowdrv::WindowEntry)
USING(wii::dvd::DVDFileInfo)

typedef void (MsgSpeakerFunc)(s32 state, SmartAllocation * speakerSp);

typedef struct
{
/* 0x0000 */ const char * msgName;
/* 0x0004 */ u32 flags;
/* 0x0008 */ u8 unknown_0x8[0x18 - 0x8];
/* 0x0018 */ s64 openTime; // when the window was opened
/* 0x0020 */ u8 unknown_0x20[0x30 - 0x20];
/* 0x0030 */ s64 messageStartTime; // last time text was added
/* 0x0038 */ s32 cellCount;
/* 0x003C */ u8 unknown_0x3c[0xf1fc - 0x3c];
/* 0xF1FC */ MsgSpeakerFunc * mainFunc; // Called multiple times in msgMain with different states
/* 0xF200 */ u8 unknown_0xf200[0xf204 - 0xf200];
/* 0xF204 */ char npcName[32];
/* 0xF224 */ u8 unknown_0xf224[0xf228 - 0xf224];
    /*
        0 normal
        1 typewriter (used in opening cutscene)
        2 writing
        3 Francis dating
        4-15 duplicates of normal
    */
/* 0xF228 */ s32 talkSfxGroup;
/* 0xF22C */ u8 talkVolume;
/* 0xF22D */ u8 unknown_0xf22d[0xf24b - 0xf22d];
/* 0xF24C */ WindowEntry * window;
/* 0xF250 */ s32 talkPoseTime; // frames remaining to be in the talking animation
/* 0xF254 */ s32 isInTalkPose; // 1 if in talking animation, 0 otherwise
} MsgSpeaker;
SIZE_ASSERT(MsgSpeaker, 0xf258)

typedef struct
{
/* 0x0 */ u32 size;
/* 0x4 */ const char * contents;
/* 0x8 */ s32 messageCount;
/* 0xC */ u8 unknown_0xc[0x10 - 0xc];
} MsgFile;
SIZE_ASSERT(MsgFile, 0x10)

typedef struct
{
/* 0x00 */ MsgFile files[9]; // 0 is global.txt
/* 0x90 */ s32 hoshi_2AnimPoseId;
/* 0x94 */ bool discReadFinished;
/* 0x95 */ bool parseFinished;
/* 0x96 */ u8 unknown_0x96[0x98 - 0x96];
} MsgWork;
SIZE_ASSERT(MsgWork, 0x98)

DECOMP_STATIC(MsgWork * msgdrv_msgw)
DECOMP_STATIC(MsgWork msgdrv_work)

typedef struct
{
/* 0x0 */ const char * name;
/* 0x4 */ s32 iconid;
} IconNameIdDef;
SIZE_ASSERT(IconNameIdDef, 0x8)

DECOMP_STATIC(IconNameIdDef msgdrv_msgIcon[24])

typedef struct
{
/* 0x0 */ const char * name1;
/* 0x4 */ const char * name2;
/* 0x8 */ const char * name3;
/* 0xC */ const char * name4;
} MsgTalkSfxGroup;
SIZE_ASSERT(MsgTalkSfxGroup, 0x10)

DECOMP_STATIC(MsgTalkSfxGroup * msgdrv_talkSfxGroups[14]) // index MsgSpeaker.talkSfxGroup
DECOMP_STATIC(const char * msgdrv_skipSfx[16]) //  index MsgSpeaker.talkSfxGroup

typedef struct
{
/* 0x0 */ const char * openName;
/* 0x4 */ const char * closeName;
} MsgOpenCloseSfxGroup;
SIZE_ASSERT(MsgOpenCloseSfxGroup, 0x8)
DECOMP_STATIC(MsgOpenCloseSfxGroup msgdrv_openCloseSfxGroups[32]) // index WindowEntry.type

// "msg/XX"
DECOMP_STATIC(const char * msgdrv_language_dir[8]) // index gp->language

/*
    Closes file and sets discReadFinished
*/
DECOMP_STATIC(void msgdrv_dvdCb(s32 result, DVDFileInfo * fileInfo))

/*
    Initialises data and starts the async load of global.txt
*/
void msgInit();

/*
    Parses un-parsed message files and loads hoshi_2 model
*/
void msgdrvMain();

/*
    Loads the specified text file into the specified file slot
    Language folder path is generated automatically, not included in textFileName
    .txt is appended automatically, not included in textFileName
*/
bool msgLoad(const char * textFileName, s32 slot);

/*
    Unloads the text file in the specified slot
*/
void msgUnLoad(s32 slot);

/*
    Calls fileAsyncf on the specified message file to start it being loaded from disc
    Language folder path is generated automatically, not included in textFileName
    .txt is appended automatically, not included in textFileName
*/
void msgPreLoad(const char * filename);

/*
    Creates a smart pointer to a MsgSpeaker struct
*/
SmartAllocation * msgSpeakerInit(const char * msg, MsgSpeakerFunc * mainFunc, WindowEntry * window);

/*
    Updates a MsgSpeaker
*/
void msgMain(SmartAllocation * speakerSp);

/*
    Renders the window & text for a MsgSpeaker
*/
void msgDisp();

/*
    Draws the star that appears when waiting for input
*/
void msgDispKeyWait();

/*
    Configures GX settings for
*/
UNKNOWN_FUNCTION(msgDispKeyWait_render);

/*
    Adds message text to a MsgSpeaker
*/
void msgAnalize(SmartAllocation * speakerSp, const char * msg);

DECOMP_STATIC(s32 msgdrv_msg_compare(void * param_1, void * param_2))

/*
    Returns the contents of a message by name
    Returns "anna_no_message" when no message with that name is loaded
*/
const char * msgSearch(const char * msgName);

/*
    Returns the contents of a message by name
    Returns a null pointer when no message with that name is loaded
*/
const char * msgSearchNoFallback(const char * msgName);

DECOMP_STATIC(bool msgdrv__ismbblead(char param_1))

/*
    Splits a message command into tag and value
    For example, command "<col ffffff80>" writes "col" to tag and "ffffff80" to value
*/
const char * msgGetCommand(const char * command, char * tag, char * value);

/*
    Takes an icon name (like "STICK") and returns the icon id for it
*/
s32 msgIconStr2ID(const char * iconStr);

/*
    Sets a MsgSpeaker's npc/fairy/player/item to their talking/standing animation
*/
void msgSetTalkPose(SmartAllocation * speakerSp);
void msgSetStayPose(SmartAllocation * speakerSp);

/*
    Creates a message WindowEntry and its MsgSpeaker
*/
s32 msgWindow_Entry(const char * msg, MsgSpeakerFunc * mainFunc, u16 priority);

/*
    Delete callback for WindowEntry, frees MsgSpeaker
*/
s32 msgWindow_Delete(WindowEntry * entry);

/*
    Adds text on to the end of a window
*/
void msgWindow_Add(const char * msg, s32 windowId);

/*
    Moves on from the window
*/
void msgWindow_Continue(s32 windowId);

/*
    Restarts text in a window
*/
void msgWindow_Repeat(s32 windowId);

/*
    Closes a window
*/
void msgWindow_ForceClose(const char * messageName);

/*
    Closes all text windows
*/
void msgWindow_ForceCloseAll();

/*
    Main callback for WindowEntry, updates text position, handles inputs
    and schedules rendering to run this frame
*/
void msgWindow_Main(WindowEntry * window);

void msgWindow_Clear_Main(WindowEntry * window);

/*
    Renders window background, text and input waiting icon
*/
void msgWindow_Disp(s8 camId, WindowEntry * window);

/*
    Main callback for WindowEntry, updates text position, handles inputs
    and schedules rendering to run this frame
*/
void selectWindow_Main(WindowEntry * window);

/*
    Renders window background, text and cursor
*/
void selectWindow_Disp(s8 camId, WindowEntry * window);

CPP_WRAPPER_END()
