/*
    Functions to handle the loading of the game's rel file
*/

#pragma once

#include <common.h>
#include <wii/os.h>

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
void relInit();

/*
    Attempts to continue the process of loading the rel, if it hasn't already finished
*/
void relMain();

/*
    Returns whether the rel has finished loading and had its prolog run
*/
bool isRelLoaded();
