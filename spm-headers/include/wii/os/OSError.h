#pragma once

#include <common.h>

CPP_WRAPPER(wii::os)

DECOMP_STATIC(char __OSUnhandledException_msg1[])
DECOMP_STATIC(char __OSUnhandledException_msg2[])
DECOMP_STATIC(char __OSUnhandledException_msg3[])

ATTRIBUTE_FORMAT(printf, 1, 2) void OSReport(const char * message, ...);
UNKNOWN_FUNCTION(OSVReport);

// Weak symbol, games overrides with its own
ATTRIBUTE_FORMAT(printf, 3, 4) void OSPanic(const char * filename, s32 line,
                                            const char * msg, ...);

UNKNOWN_FUNCTION(OSSetErrorHandler);
void __OSUnhandledException(s32 p1, s32 p2, s32 p3, s32 p4);

CPP_WRAPPER_END()
