#ifndef EVTMGR_H
#define EVTMGR_H

#include <common.h>

#define EVT_max 0x78
#define EVT_ENTRY_MAX 0x80
#define MAX_EVT_JMPTBL 16

struct _EvtEntry;
typedef s32 (*user_func)(struct _EvtEntry * entry, bool firstRun);
typedef s32 EvtScriptCode;

typedef struct _EvtEntry {
  s64 lifetime;
  u8 flags; // bit flags, 1 seems to be the script being active?
  u8 curDataLength; // number of pieces of data (4 bytes each) belonging to the current instruction
  u8 curOpcode; // see evtmgr_cmd.h for opcodes
  u8 priority;
  u8 type;
  s8 blocked;
  s8 dowhileDepth; // number of do-while loops the current instruction is nested into
  s8 unknown_0xf;
  EvtScriptCode * pCurInstruction;
  EvtScriptCode * pCurData;
  s8 labelIds[MAX_EVT_JMPTBL]; // each correspond to an address in the jump table
  void * jumptable[MAX_EVT_JMPTBL]; // addresses for each label
  struct _EvtEntry * parent;
  struct _EvtEntry * childEntry;
  struct _EvtEntry * brotherEntry;
  s32 unknown_0x74;
  s64 unknown_0x78;
  u8 unknown_0x80[0x94 - 0x80];
  user_func * userFunc; // Function set & called by the user_func opcode
  s32 lw[16];
  u32 lf[3];
  void * dowhileStartPtrs[8]; // pointer to do opcodes
  s32 dowhileCounters[8];
  u8 unknown_0x124[0x150 - 0x124];
  s32 * uw;
  u32 * uf;
  s32 id;
  float speed;
  float unknown_0x160;
  s32 unknown_0x164;
  u32 unknown_0x168;
  u8 unknown_0x16c[0x170 - 0x16c];
  s32 unknown_0x170;
  s32 unknown_0x174;
  s32 unknown_0x178;
  f32 unknown_0x17c;
  f32 unknown_0x180;
  f32 unknown_0x184;
  s32 unknown_0x188;
  u8 unknown_0x18c[0x198 - 0x18c];
  EvtScriptCode * scriptStart;
  char * name; // debug thing, unused?
  void * pPrevInstruction;
  u8 unknown_0x1a4[0x1a8 - 0x1a4];
} EvtEntry;

typedef struct {
  s32 entryCount;
  s32 gw[32];
  u32 gf[3];
  EvtEntry * entries;
  u8 unknown_0x94[0x98 - 0x94];
  s64 time;
} EvtWork;

extern s32 evtMax; // 805ae8d8

EvtWork * evtGetWork(); // 800d87e4
void evtmgrInit(); // 800d8a88
void evtmgrReInit(); // 800d8b2c
EvtEntry * evtEntry(EvtScriptCode * script, u8 priority, u8 flags); // 800d8b88
EvtEntry * evtEntryType(EvtScriptCode * script, s32 param_2, s32 param_3, s32 param_4); // 800d8df4
EvtEntry * evtChildEntry(EvtEntry * entry, EvtScriptCode * script, u8 flags); // 800d9060
EvtEntry * evtBrotherEntry(EvtEntry * entry, EvtScriptCode * script, u8 flags); // 800d9370
EvtEntry * evtRestart(EvtEntry * entry); // 800d9634
void evtmgrMain(); // 800d9764
void evtDelete(EvtEntry * entry); // 800d9944
void evtDeleteID(s32 id); // 800d9b00
bool evtCheckID(s32 id); // 800d9b88
void evtSetPri(EvtEntry * entry, u8 pri); // 800d9bd4
void evtSetSpeed(EvtEntry * entry, f32 multiplier); // 800d9bdc
void evtSetType(EvtEntry * entry, u8 type); // 800d9bec
void evtStop(EvtEntry * entry, u8 mask); // 800d9bf4
void evtStart(EvtEntry * entry, u8 mask); // 800d9c98
void evtStopID(s32 id); // 800d9d3c
void evtStartID(s32 id); // 800d9dc8
void evtStopAll(u8 mask); // 800d9e54
void evtStartAll(u8 mask); // 800d9ed4
void evtStopOther(EvtEntry * entry, u8 mask); // 800d9f54
void evtStartOther(EvtEntry * entry, u8 mask); // 800d9fc8
EvtEntry * evtGetPtr(s32 index); // 800da03c
EvtEntry * evtGetPtrID(s32 id); // 800da064

#endif