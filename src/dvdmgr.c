#include <common.h>
#include <spm/dvdmgr.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <msl/string.h>

#include "orderstrings/803377a8_803377f8.inc"

static DVDWork work;
static DVDWork * wp = &work;

asm void DVDMgrInit()
{
    #include "asm/8019e044.s"
}

asm void DVDMgrDelete()
{
    #include "asm/8019e088.s"
}

asm DVDEntry* DVDMgrOpen(const char * path, s32 param_2, s32 param_3)
{
    #include "asm/8019e08c.s"
}

asm s32 DVDMgrRead(DVDEntry * entry, void * dest, size_t length, u32 offset)
{
    #include "asm/8019e1e4.s"
}

static asm void _cb(s32 result, DVDFileInfo * fileInfo)
{
    #include "asm/8019e29c.s"
}

asm s32 DVDMgrReadAsync(DVDEntry * entry, void * dest, size_t length, u32 offset,
                        DVDMgrCallback * callback)
{
    #include "asm/8019e2ac.s"
}

asm void DVDMgrClose(DVDEntry * entry)
{
    #include "asm/8019e2e0.s"
}

asm size_t DVDMgrGetLength(DVDEntry * entry)
{
    #include "asm/8019e320.s"
}
