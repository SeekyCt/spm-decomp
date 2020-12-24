#ifndef RELMGR_H
#define RELMGR_H

#include <common.h>

// File is new to SPM and with no asserts, so no names are official here

typedef struct {
    RelHeader * relFile;
    void * bss;
    bool loaded;
    u8 unknown_0x9[0x10 - 0x9]; // probably just padding between file .data sections?
} RelHolder;

void relInit(); // 8023e434
void loadRel(); // 8023e444
bool isRelLoaded(); // 8023e600

#endif
