#include <common.h>
#include <spm/debug.h>
#include <wii/base.h>
#include <wii/os.h>
#include <msl/stdio.h>
#include <msl/string.h>

DebugWork work;
DebugWork * wp = &work;

#include "orderstrings/803377f8_80337848.inc"

void debugInit()
{
    memset(wp, 0, sizeof(work));
}

void debugReInit()
{

}

asm void OSPanic(const char * filename, s32 line, const char * msg, ...)
{
    #include "asm/8019e33c.s"
}
