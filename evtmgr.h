#ifndef EVTMGR_H
#define EVTMGR_H

#include <stdint.h>
#include <stdbool.h>

#define EVT_max 0x78
#define EVT_ENTRY_MAX 0x80
#define MAX_EVT_JMPTBL 16

struct _EvtEntry;
typedef int (*user_func)(struct _EvtEntry * entry, bool firstRun);

typedef struct _EvtEntry {
  uint32_t unknown_0x0;
  uint32_t unknown_0x4;
  uint8_t flags; // bit flags, 1 seems to be the script being active?
  uint8_t curDataLength; // number of pieces of data (4 bytes each) belonging to the current instruction
  uint8_t curOpcode; // see links above for opcodes
  uint8_t priority;
  uint8_t type;
  char unknown_0xd;
  int8_t dowhileDepth; // number of do-while loops the current instruction is nested into
  int8_t unknown_0xf;
  int * pCurInstruction;
  int * pCurData;
  int8_t labelIds[MAX_EVT_JMPTBL]; // each correspond to an address in the jump table
  void * jumptable[MAX_EVT_JMPTBL]; // addresses for each label
  struct _EvtEntry * parent;
  struct _EvtEntry * childEntry;
  struct _EvtEntry * brotherEntry;
  char unknown_0x74[0x94 - 0x74];
  user_func * userFunc; // Function set & called by the user_func opcode
  int lw[16];
  uint32_t lf[3];
  void * dowhileStartPtrs[8]; // pointer to do opcodes
  int dowhileIds[8];
  char unknown_0x124[0x150 - 0x124];
  int * uw;
  unsigned int * uf;
  int id;
  float speed;
  float unknown_0x160;
  int unknown_0x164;
  uint32_t unknown_0x168;
  char unknown_0x16c[0x170 - 0x16c];
  int unknown_0x170;
  int unknown_0x174;
  int unknown_0x178;
  float unknown_0x17c;
  float unknown_0x180;
  float unknown_0x184;
  int unknown_0x188;
  char unknown_0x18c[0x198 - 0x18c];
  int * scriptStart;
  uint32_t unknown_0x19c;  
  void * pPrevInstruction;
  char unknown_0x1a4[0x1a8 - 0x1a4];
} EvtEntry;

typedef struct {
  int entryCount;
  int gw[32];
  uint32_t gf[3];
  EvtEntry * entries;
  char unknown_0x94[0x98 - 0x94];
  int64_t time;
} EvtWork;

extern int evtMax; // 805ae8d8

EvtWork * evtGetWork(); // 800d87e4
void evtmgrInit(); // 800d8a88
void evtmgrReInit(); // 800d8b2c
EvtEntry * evtEntry(int * script, uint8_t priority, uint8_t flags); // 800d8b88
EvtEntry * evtEntryType(int * script, int param_2, int param_3, int param_4); // 800d8df4
EvtEntry * evtChildEntry(EvtEntry * entry, int * script, uint8_t flags); // 800d9060
EvtEntry * evtBrotherEntry(EvtEntry * entry, int * script, uint8_t flags); // 800d9370
EvtEntry * evtRestart(EvtEntry * entry); // 800d9634
void evtmgrMain(); // 800d9764
void evtDelete(EvtEntry * entry); // 800d9944
void evtDeleteID(int id); // 800d9b00
bool evtCheckID(int id); // 800d9b88
void evtSetPri(EvtEntry * entry, uint8_t pri); // 800d9bd4
void evtSetSpeed(EvtEntry * entry, float speed); // 800d9bdc
void evtSetType(EvtEntry * entry, uint8_t type); // 800d9bec
void evtStop(EvtEntry * entry, uint8_t mask); // 800d9bf4
void evtStart(EvtEntry * entry, uint8_t mask); // 800d9c98
void evtStopID(int id); // 800d9d3c
void evtStartID(int id); // 800d9dc8
void evtStopAll(uint8_t mask); // 800d9e54
void evtStartAll(uint8_t mask); // 800d9ed4
void evtStopOther(EvtEntry * entry, uint8_t mask); // 800d9f54
void evtStartOther(EvtEntry * entry, uint8_t mask); // 800d9fc8
EvtEntry * evtGetPtr(int index); // 800da03c
EvtEntry * evtGetPtrID(int id); // 800da064

#endif