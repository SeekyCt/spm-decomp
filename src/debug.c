#include <common.h>
#include <spm/debug.h>
#include <wii/base.h>
#include <wii/os.h>
#include <msl/stdarg.h>
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

void OSPanic(const char * filename, s32 line, const char * msg, ...)
{
    va_list args;
    char buf[2048];
    u32 stackDepth;
    u32 * stack;
    int pos;

    // Format header text
    va_start(args, msg);
    pos = vsprintf(buf, msg, args);
    va_end(args);
    pos += sprintf(buf + pos, "\n  in \"%s\" on line %d.\n", filename, line);
    pos += sprintf(buf + pos, "\nAddress:    Back Chain LR Save\n");

    // Traverse stack
    stackDepth = 0;
    stack = (u32 *) OSGetStackPointer();
    while (stack != NULL && stack != (u32 *)0xffffffff && stackDepth++ < 16)
    {
        pos += sprintf(buf + pos, "0x%08x: 0x%08x 0x%08x\n", stack, stack[0], stack[1]);
        stack = (u32 *) *stack;
    }

    // Halt execution
    OSDisableInterrupts();
    PPCHalt();
    while (1) { }
}
