#ifndef EVTMGR_H
#define EVTMGR_H

#include <stdint.h>
#include <stdbool.h>

/*
  Functions:
  ==========
  evtGetWork      yes
  ...             no
  evtmgrInit      yes
  evtmgrReInit    yes
  ...             no

  Other:
  ======

*/

#define EVT_ENTRY_MAX 0x80

struct _EvtEntry;
typedef int (*user_func)(struct _EvtEntry * entry, bool firstRun);

typedef struct _EvtEntry {
  uint32_t unknown_0x0;
  uint32_t unknown_0x4;
  uint8_t flags; // bit flags, 1 seems to be the script being active?
  uint8_t curDataLength; // number of pieces of data (4 bytes each) belonging to the current instruction
  uint8_t curOpcode; // see links above for opcodes
  uint8_t priority;
  int8_t type;
  char unknown_0xd;
  int8_t dowhileDepth; // number of do-while loops the current instruction is nested into
  int8_t unknown_0xf;
  void * curInstructionPtr;
  void * curDataPtr;
  uint8_t labelIds[16]; // each correspond to an address in the jump table
  void * jumptable[16]; // addresses for each label
  struct _EvtEntry * parent;
  struct _EvtEntry * childEntry;
  struct _EvtEntry * brotherEntry;
  char unknown_0x74[0x94 - 0x74];
  user_func * userFunc; // Function set & called by the user_func opcode
  int lw[16];
  int lf[3];
  void * dowhileStartPtrs[8]; // pointer to do opcodes
  char unknown_0x104[0x150 - 0x104];
  int * uw;
  int * uf;
  int id;
  float speed;
  float unknown_0x160;
  int unknown_0x164;
  uint32_t unknown_0x168;
  char unknown_0x16c[0x198 - 0x16c];
  void * scriptStart;
  uint32_t unknown_0x19c;  
  void * prevInstructionPtr;
  char unknown_0x1a4[0x1a8 - 0x1a4];
} EvtEntry;

typedef struct {
  int entryCount;
  uint32_t gw[32];
  uint32_t gf[3];
  EvtEntry * entries;
  char unknown_0x94[0x98 - 0x94];
  int64_t time;
} EvtWork;
extern EvtWork evtWork; // 8050c990

extern int evtActiveEntryCount; // 805ae8d8
//extern int priTblNum; // 805ae8dc
extern int evt_unk805ae8e0; // 805ae8e0
extern int evtId; // 805ae018

EvtWork * evtGetWork(); // 800d87e4
void make_pri_table(); // 800d87f0
void make_jump_table(EvtEntry * entry); // 800d890c
void evtmgrInit(); // 800d8a88
void evtmgrReInit(); // 800d8b2c
EvtEntry * evtEntry(void * script, uint8_t priority, int8_t flags); // 800d8b88
EvtEntry * evtEntryType(void * script, int param_2, int param_3, int param_4); // 800d8df4
EvtEntry * evtChildEntry(EvtEntry * entry, void * script, uint8_t flags); // 800d9060
EvtEntry * evtBrotherEntry(EvtEntry * entry, void * script, uint8_t flags); // 800d9370
EvtEntry * evtRestart(EvtEntry * entry); // 800d9634
void evtmgrMain(); // 800d9764
void evtDelete(EvtEntry * entry); // 800d9944
void evtDeleteId(int id); // 800d9b00
bool evtCheckID(int id); // 800d9b88
EvtEntry * evtSetPri(EvtEntry * entry, uint8_t pri); // 800d9bd4
void evtSetSpeed(EvtEntry * entry, float speed); // 800d9bdc
void evtSetType(EvtEntry * entry, uint8_t type); // 800d9bec
void evtStop(EvtEntry * entry, int param_2); // 800d9bf4
void evtStart(EvtEntry * entry, int param_2); // 800d9c98
void evtStopId(int id); // 800d9d3c
void evtStartId(int id); // 800d9dc8
void evtStopAll(int param_1); // 800d9e54
void evtStartAll(int param_1); // 800d9ed4
void evtStopOther(EvtEntry * entry, int param_2); // 800d9f54
void evtStartOther(EvtEntry * entry, int param_2); // 800d9fc8
EvtEntry * evtGetPtr(int index); // 800da03c
EvtEntry * evtGetPtrId(int id); // 800da064

#endif