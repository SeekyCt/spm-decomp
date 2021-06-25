/*
  Functions to manage execution of the game's evt bytecode scripts
*/

#ifndef EVTMGR_H
#define EVTMGR_H

#include <common.h>

#define EVT_max 0x78
#define EVT_ENTRY_MAX 0x80
#define MAX_EVT_JMPTBL 16

struct _EvtEntry;
typedef s32 (*user_func)(struct _EvtEntry * entry, bool firstRun);
typedef s32 EvtScriptCode;

#define EVT_FLAG_IN_USE (1 << 0) 
#define EVT_FLAG_PAUSED (1 << 1)

typedef struct _EvtEntry
{
    s64 lifetime;
    u8 flags; // bit flags, see defines above
    u8 curDataLength; // number of pieces of data (4 bytes each) belonging to the current instruction
    u8 curOpcode; // see evtmgr_cmd.h for opcodes
    u8 priority;
    u8 type;
    s8 blocked;
    s8 dowhileDepth; // number of do-while loops the current instruction is nested into
    s8 switchDepth; // number of switches the current instruction is nested into
    EvtScriptCode * pCurInstruction;
    EvtScriptCode * pCurData;
    s8 labelIds[MAX_EVT_JMPTBL]; // each correspond to an address in the jump table
    void * jumptable[MAX_EVT_JMPTBL]; // addresses for each label
    struct _EvtEntry * parent;
    struct _EvtEntry * childEntry;
    struct _EvtEntry * brotherEntry;
    union {
      s32 tempS[3];
      u32 tempU[3];
    };
    u8 unknown_0x80[0x94 - 0x80];
    user_func * userFunc; // Function set & called by the user_func opcode
    s32 lw[16];
    u32 lf[3];
    void * dowhileStartPtrs[8]; // pointer to do opcodes
    s32 dowhileCounters[8];
    u8 unknown_0x124[0x14c - 0x124];
    s32 * readAddr;
    s32 * uw;
    u32 * uf;
    s32 id;
    float speed;
    float unknown_0x160;
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
    EvtScriptCode * scriptStart;
    char * name; // debug thing, unused?
    void * pPrevInstruction;
    u8 unknown_0x1a4[0x1a8 - 0x1a4];
} EvtEntry;

typedef struct
{
    s32 entryCount;
    s32 gw[32];
    u32 gf[3];
    EvtEntry * entries;
    u8 unknown_0x94[0x98 - 0x94];
    s64 time;
} EvtWork;

extern s32 evtMax; // 805ae8d8

/*
    Returns pointer to the EvtWork struct 
*/
EvtWork * evtGetWork(); // 800d87e4

/*
    Allocates memories for entries and zeros global evt variables
*/
void evtmgrInit(); // 800d8a88

/*
    Zeros all entries
*/
void evtmgrReInit(); // 800d8b2c

/*
    Starts execution of a script
*/
EvtEntry * evtEntry(EvtScriptCode * script, u8 priority, u8 flags); // 800d8b88
EvtEntry * evtEntryType(EvtScriptCode * script, s32 param_2, s32 param_3, s32 param_4); // 800d8df4
EvtEntry * evtChildEntry(EvtEntry * entry, EvtScriptCode * script, u8 flags); // 800d9060
EvtEntry * evtBrotherEntry(EvtEntry * entry, EvtScriptCode * script, u8 flags); // 800d9370

/*
    Restarts a entry
*/
EvtEntry * evtRestart(EvtEntry * entry); // 800d9634

/*
    Sets up entry executions, called every frame
*/
void evtmgrMain(); // 800d9764

/*
    Terminates a entry and all its children & brothers
*/
void evtDelete(EvtEntry * entry); // 800d9944

/*
    Terminates a entry by id and all its children & brothers
*/
void evtDeleteID(s32 id); // 800d9b00

/*
    Checks if a entry is running by id
*/
bool evtCheckID(s32 id); // 800d9b88

/*
    Sets the execution priority of a entry
*/
void evtSetPri(EvtEntry * entry, u8 pri); // 800d9bd4

/*
    Sets the execution speed of a entry
*/
void evtSetSpeed(EvtEntry * entry, f32 multiplier); // 800d9bdc

/*
    Sets the type mask of a entry
*/
void evtSetType(EvtEntry * entry, u8 type); // 800d9bec

/*
    Pauses execution a entry and all its children & brothers with types included in the mask
*/
void evtStop(EvtEntry * entry, u8 mask); // 800d9bf4

/*
    Resumes execution of a entry and all its children & brothers with types included in the mask
*/
void evtStart(EvtEntry * entry, u8 mask); // 800d9c98

/*
    Pauses execution of a entry by id and all its children & brothers
*/
void evtStopID(s32 id); // 800d9d3c

/*
    Resumes execution of a entry by id and all its children & brothers
*/
void evtStartID(s32 id); // 800d9dc8

/*
    Pauses execution of all entries with types included in the mask
*/
void evtStopAll(u8 mask); // 800d9e54

/*
    Resumes execution of all entries with types included in the mask
*/
void evtStartAll(u8 mask); // 800d9ed4

/*
    Pauses execution of all entries with types included in the mask other than one specific entry
*/
void evtStopOther(EvtEntry * entry, u8 mask); // 800d9f54

/*
    Resumes execution of all entries with types included in the mask other than one specific entry
*/
void evtStartOther(EvtEntry * entry, u8 mask); // 800d9fc8

/*
    Gets an entry by its index into the array of entries
*/
EvtEntry * evtGetPtr(s32 index); // 800da03c

/*
    Gets an entry by its id
*/
EvtEntry * evtGetPtrID(s32 id); // 800da064

#endif