/*
    Functions to manage execution of the game's evt bytecode scripts
*/

#pragma once

#include <common.h>
#include <wii/os.h>

CPP_WRAPPER(spm::evtmgr)

USING(wii::os::OSTime)

/*
    Defines from asserts
*/
#define EVT_max 0x78
#define EVT_ENTRY_MAX 0x80
#define MAX_EVT_JMPTBL 16

/*
    EvtEntry.flags values
*/

// Entry is being used to run a script
#define EVT_FLAG_IN_USE 0x1

// Entry's script is temporarily stopped
#define EVT_FLAG_PAUSED 0x2

// Entry's lifetime doesn't increase
#define EVT_FLAG_STOP_TIME 0x4

// Entry's script is temporarily stopped until its child script finishes
#define EVT_FLAG_WAIT_CHILD 0x10

// Entry should still execute this frame if created while evtmgrMain is running
#define EVT_FLAG_START_IMMEDIATE 0x20

// Same effect as EVT_FLAG_PAUSED?
#define EVT_FLAG_80 0x80

struct _EvtEntry;
typedef s32 (UserFunc)(struct _EvtEntry * entry, bool firstRun);
typedef s32 EvtScriptCode;
typedef s32 EvtVar;

typedef struct _EvtEntry
{
/* 0x000 */ OSTime lifetime; // Time this script has been running & not paused
/* 0x008 */ u8 flags; // Bit flags, see defines above
/* 0x009 */ u8 curDataLength; // Number of pieces of data (4 bytes each) belonging to the
                              // current instruction
/* 0x00A */ u8 curOpcode; // Opcode of the current instruction
/* 0x00B */ u8 priority; // Priority for script to run (higher value -> earlier)
/* 0x00C */ u8 type; // Type mask for stopping/starting
/* 0x00D */ s8 blocked; // Keep re-exucuting the current instruction if true
                        // (decomp doesn't match as bool)
/* 0x00E */ s8 doWhileDepth; // Number of do-while loops the current instruction is nested into
/* 0x00F */ s8 switchDepth; // Number of switches the current instruction is nested into
/* 0x010 */ EvtScriptCode * pCurInstruction; // Current instruction being executed
/* 0x014 */ EvtScriptCode * pCurData; // Args for the current instruction being executed
/* 0x018 */ s8 labelIds[MAX_EVT_JMPTBL]; // Each correspond to an address in the jump table
/* 0x028 */ EvtScriptCode * jumptable[MAX_EVT_JMPTBL]; // Addresses for each label
/* 0x068 */ struct _EvtEntry * parent; // Script that started this one as a child
/* 0x06C */ struct _EvtEntry * childEntry; // Script started by this one, pausing this until it's
                                           // finished
/* 0x070 */ struct _EvtEntry * brotherEntry; // Script started by this one which will be stopped if
                                             // this one is
/* 0x074 */ union
            {
                s32 tempS[3];
                u32 tempU[3];
            }; // Temp values with meanings varying by instruction / user func
/* 0x080 */ u8 unknown_0x80[0x94 - 0x80];
/* 0x094 */ UserFunc * userFunc; // Function set & called by the user_func opcode
/* 0x098 */ s32 lw[16]; // Local Work variables
/* 0x0D8 */ u32 lf[3]; // Local Flag variables
/* 0x0E4 */ EvtScriptCode * doWhileStartPtrs[8]; // Pointer to do opcodes for active while loops
/* 0x104 */ s32 doWhileCounters[8]; // Counters for active while loops
/* 0x124 */ s8 switchStates[8]; // Sates of active switch statements
/* 0x12C */ s32 switchValues[8]; // Values passed into active switch statements
/* 0x14C */ union
            {
                s32 * readAddr;
                f32 * readfAddr;
            }; // Current addr for read/readf instructions
/* 0x150 */ s32 * uw; // User Work variables pointer
/* 0x154 */ u32 * uf; // User Flag variables pointer
/* 0x158 */ s32 id; // Script id
/* 0x15C */ f32 speed; // timeToRun given per frame
/* 0x160 */ f32 timeToRun; // Number of instructions to execute this frame
/* 0x164 */ s32 casedrvId;
/* 0x168 */ void * ownerNPC;
/* 0x16C */ void * ownerMOBJ;
/* 0x170 */ s32 unknown_0x170;
/* 0x174 */ s32 msgWindowId;
/* 0x178 */ s32 unknown_0x178;
/* 0x17C */ f32 unknown_0x17c;
/* 0x180 */ f32 unknown_0x180;
/* 0x184 */ f32 unknown_0x184;
/* 0x188 */ s32 msgPri;
/* 0x18C */ u8 unknown_0x18c[0x198 - 0x18c];
/* 0x198 */ EvtScriptCode * scriptStart; // Pointer to the start of this script
/* 0x19C */ char * name; // debug thing, unused?
/* 0x1A0 */ void * pPrevInstruction; // Pointer to last instruction executed
/* 0x1A4 */ u8 unknown_0x1a4[0x1a8 - 0x1a4];
} EvtEntry;
SIZE_ASSERT(EvtEntry, 0x1a8)

typedef struct
{
/* 0x00 */ s32 entryCount; // Number of EvtEntry structs in the entries array
/* 0x04 */ s32 gw[32]; // Global Work variables
/* 0x84 */ u32 gf[3]; // Global Flag variables
/* 0x90 */ EvtEntry * entries; // Entry array
/* 0x04 */ // Alignment padding 0x94-97
/* 0xA0 */ OSTime time; // Time evtmgrMain last ran
} EvtWork;
SIZE_ASSERT(EvtWork, 0xa0)

// Next script id to use
DECOMP_STATIC(s32 evtmgr_evtId)

// Default speed for scripts
DECOMP_STATIC(f32 evtmgr_evtSpd)

// Number of scripts running
DECOMP_STATIC(s32 evtmgr_evtMax)

// Numbers of entries in the priority table
DECOMP_STATIC(s32 evtmgr_priTblNum)

// Whether evtmgrMain is running
DECOMP_STATIC(s32 evtmgr_runMainF)

/*
    Returns pointer to the EvtWork struct
*/
EvtWork * evtGetWork();

/*
    Allocates memories for entries and zeros global evt variables
*/
void evtmgrInit();

/*
    Zeros all entries
*/
void evtmgrReInit();

/*
    Stops scripts based on _spmarioSystemLevel
*/
void evtEntryRunCheck();

/*
    Starts execution of a script
*/
// TODO: try fix const in decomp
#ifdef DECOMP
EvtEntry * evtEntry(EvtScriptCode * script, u32 priority, u8 flags);
EvtEntry * evtEntryType(EvtScriptCode * script, u32 priority, u8 flags, u8 type);
EvtEntry * evtChildEntry(EvtEntry * parent, EvtScriptCode * script, u8 flags);
EvtEntry * evtBrotherEntry(EvtEntry * brother, EvtScriptCode * script, u8 flags);
#else
EvtEntry * evtEntry(const EvtScriptCode * script, u32 priority, u8 flags);
EvtEntry * evtEntryType(const EvtScriptCode * script, u32 priority, u8 flags, u8 type);
EvtEntry * evtChildEntry(EvtEntry * parent, const EvtScriptCode * script, u8 flags);
EvtEntry * evtBrotherEntry(EvtEntry * brother, const EvtScriptCode * script, u8 flags);
#endif

/*
    Restarts a entry
*/
EvtEntry * evtRestart(EvtEntry * entry);

/*
    Sets up entry executions, called every frame
*/
void evtmgrMain();

/*
    Terminates a entry and all its children & brothers
*/
void evtDelete(EvtEntry * entry);

/*
    Terminates a entry by id and all its children & brothers
*/
void evtDeleteID(s32 id);

/*
    Checks if a entry is running by id
*/
bool evtCheckID(s32 id);

/*
    Sets the execution priority of a entry
*/
void evtSetPri(EvtEntry * entry, u32 pri);

/*
    Sets the execution speed of a entry
*/
void evtSetSpeed(EvtEntry * entry, f32 multiplier);

/*
    Sets the type mask of a entry
*/
void evtSetType(EvtEntry * entry, u32 type);

/*
    Pauses execution a entry and all its children & brothers with types included in the mask
*/
void evtStop(EvtEntry * entry, u32 mask);

/*
    Resumes execution of a entry and all its children & brothers with types included in the mask
*/
void evtStart(EvtEntry * entry, u32 mask);

/*
    Pauses execution of a entry by id and all its children & brothers
*/
void evtStopID(s32 id);

/*
    Resumes execution of a entry by id and all its children & brothers
*/
void evtStartID(s32 id);

/*
    Pauses execution of all entries with types included in the mask
*/
void evtStopAll(u32 mask);

/*
    Resumes execution of all entries with types included in the mask
*/
void evtStartAll(u32 mask);

/*
    Pauses execution of all entries with types included in the mask other than one specific entry
*/
void evtStopOther(EvtEntry * entry, u32 mask);

/*
    Resumes execution of all entries with types included in the mask other than one specific entry
*/
void evtStartOther(EvtEntry * entry, u32 mask);

/*
    Gets an entry by its index into the array of entries
*/
EvtEntry * evtGetPtr(s32 index);

/*
    Gets an entry by its id
*/
EvtEntry * evtGetPtrID(s32 id);

CPP_WRAPPER_END()
