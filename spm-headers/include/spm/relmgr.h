/*
    relmgr controls the loading and execution of the game's rel file

    Rel files are all in the /rel/ folder at the root of the disk
    relF.bin is the actual rel, rel.bin and relD.bin are unused
    .bin rels are lzss10 compressed, the game also supports loading an
    uncompressed file named relF.rel if relF.bin is not found
*/

#pragma once

#include <common.h>
#include <wii/os.h>

CPP_WRAPPER(spm::relmgr)

USING(wii::os::RelHeader)

typedef struct
{
/* 0x0 */ RelHeader * relFile;
/* 0x4 */ void * bss;
/* 0x8 */ bool loaded;
} RelWork;
SIZE_ASSERT(RelWork, 0xc)

DECOMP_STATIC(RelWork * relmgr_wp)
DECOMP_STATIC(const char * relmgr_relDecompName)
DECOMP_STATIC(const char * relmgr_relCompName)

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

CPP_WRAPPER_END()
