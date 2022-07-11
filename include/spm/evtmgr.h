/*
  Functions to manage execution of the game's evt bytecode scripts
*/

#pragma once

#include <common.h>
#include <wii/os.h>

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

typedef struct _EvtEntry
{
    OSTime lifetime; // Time this script has been running & not paused
    u8 flags; // Bit flags, see defines above
    u8 curDataLength; // Number of pieces of data (4 bytes each) belonging to the current instruction
    u8 curOpcode; // Opcode of the current instruction, see evtmgr_cmd.h for opcodes
    u8 priority; // Priority for script to run (higher value -> earlier)
    u8 type; // Type mask for stopping/starting
    s8 blocked; // Keep re-exucuting the current instruction if true (decomp doesn't match as bool)
    s8 doWhileDepth; // Number of do-while loops the current instruction is nested into
    s8 switchDepth; // Number of switches the current instruction is nested into
    EvtScriptCode * pCurInstruction; // Current instruction being executed
    EvtScriptCode * pCurData; // Args for the current instruction being executed
    s8 labelIds[MAX_EVT_JMPTBL]; // Each correspond to an address in the jump table
    void * jumptable[MAX_EVT_JMPTBL]; // Addresses for each label
    struct _EvtEntry * parent; // Script that started this one as a child
    struct _EvtEntry * childEntry; // Script started by this one, pausing this until it's finished
    struct _EvtEntry * brotherEntry; // Script started by this one which will be stopped if this one is
    union
    {
      s32 tempS[3];
      u32 tempU[3];
    }; // Temp values with meanings varying by instruction / user func
    u8 unknown_0x80[0x94 - 0x80];
    UserFunc * userFunc; // Function set & called by the user_func opcode
    s32 lw[16]; // Local Work variables
    u32 lf[3]; // Local Flag variables
    void * doWhileStartPtrs[8]; // Pointer to do opcodes for active while loops
    s32 doWhileCounters[8]; // Counters for active while loops
    s8 switchStates[8]; // Sates of active switch statements
    s32 switchValues[8]; // Values passed into active switch statements
    union
    {
        s32 * readAddr;
        float * readfAddr;
    }; // Current addr for read/readf instructions
    s32 * uw; // User Work variables pointer
    u32 * uf; // User Flag variables pointer
    s32 id; // Script id
    float speed; // timeToRun given per frame
    float timeToRun; // Number of instructions to execute this frame
    s32 casedrvId;
    void * ownerNPC;
    void * ownerMOBJ;
    s32 unknown_0x170;
    s32 msgWindowId;
    s32 unknown_0x178;
    f32 unknown_0x17c;
    f32 unknown_0x180;
    f32 unknown_0x184;
    s32 msgPri;
    u8 unknown_0x18c[0x198 - 0x18c];
    EvtScriptCode * scriptStart; // Pointer to the start of this script
    char * name; // debug thing, unused?
    void * pPrevInstruction; // Pointer to last instruction executed
    u8 unknown_0x1a4[0x1a8 - 0x1a4];
} EvtEntry;

typedef struct
{
    s32 entryCount; // Number of EvtEntry structs in the entries array
    s32 gw[32]; // Global Work variables
    u32 gf[3]; // Global Flag variables
    EvtEntry * entries; // Entry array
    u8 unknown_0x94[0x98 - 0x94];
    OSTime time; // Time evtmgrMain last ran
} EvtWork;

extern s32 evtMax;

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
EvtEntry * evtEntry(EvtScriptCode * script, u32 priority, u8 flags);
EvtEntry * evtEntryType(EvtScriptCode * script, u32 priority, u8 flags, u8 type);
EvtEntry * evtChildEntry(EvtEntry * entry, EvtScriptCode * script, u8 flags);
EvtEntry * evtBrotherEntry(EvtEntry * entry, EvtScriptCode * script, u8 flags);

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
