#include <common.h>
#include <spm/debug.h>
#include <wii/base.h>
#include <wii/os.h>
#include <msl/stdio.h>
#include <msl/string.h>

DebugWork work;
DebugWork * wp = &work;

#include "orderstrings/803377f8_80337848.inc"

asm UNKNOWN_FUNCTION(debugInit)
{
    #include "asm/8019e328.s"
}

asm UNKNOWN_FUNCTION(debugReInit)
{
    #include "asm/8019e338.s"
}

asm void OSPanic(const char * filename, s32 line, const char * msg, ...)
{
    #include "asm/8019e33c.s"
}
