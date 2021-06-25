/*
    Functions to handle the loading of the game's rel file
*/

#ifndef RELMGR_H
#define RELMGR_H

#include <common.h>
#include <os.h>

// File is new to SPM and with no asserts, so no names are official here

typedef struct
{
    RelHeader * relFile;
    void * bss;
    bool loaded;
} RelWork;

/*
    Sets the rel loaded bool to false
*/
void relInit(); // 8023e434

/*
    Attempts to continue the process of loading the rel, if it hasn't already finished
*/
void relMain(); // 8023e444

/*
    Returns whether the rel has finished loading and had its prolog run
*/
bool isRelLoaded(); // 8023e600

#endif
