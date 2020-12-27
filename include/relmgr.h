/*
    Functions to handle the loading of the game's rel file
*/

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

/*
    Sets the rel loaded bool to false
*/
void relInit(); // 8023e434

/*
    Attempts to continue the process of loading the rel, called every frame
*/
void loadRel(); // 8023e444

/*
    Returns whether the rel has finished loading and had its prolog run
*/
bool isRelLoaded(); // 8023e600

#endif
