#pragma once

#include <common.h>
#include <spm/filemgr.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::eff_sub)

USING(spm::filemgr::FileEntry)

typedef struct
{
/* 0x000 */ FileEntry * activeTplFile;
/* 0x004 */ FileEntry * activeDatFile;
/* 0x008 */ void * datBody;
/* 0x00C */ FileEntry * effdataTplFile;
/* 0x010 */ FileEntry * effdataDatFile;
/* 0x014 */ u8 unknown_0x014[0x100 - 0x14];
} EffSubWork;
SIZE_ASSERT(EffSubWork, 0x100)

DECOMP_STATIC(EffSubWork * effsub_wp)

void effSubMain();

CPP_WRAPPER_END()
