#pragma once

#include <common.h>
#include <wii/os/OSContext.h>

CPP_WRAPPER(wii::os)

USING(wii::os::OSContext)

#define OS_ERROR_SYSTEM_RESET 0
#define OS_ERROR_MACHINE_CHECK 1
#define OS_ERROR_DSI 2
#define OS_ERROR_ISI 3
#define OS_ERROR_EXT_INTERRUPT 4
#define OS_ERROR_ALIGNMENT 5
#define OS_ERROR_PROGRAM 6
#define OS_ERROR_FP_UNAVAIL 7
#define OS_ERROR_DECREMENTER 8
#define OS_ERROR_SYSTEM_CALL 9
#define OS_ERROR_TRACE 10
#define OS_ERROR_PERF_MONITOR 11
#define OS_ERROR_IABR 12
#define OS_ERROR_SMI 13
#define OS_ERROR_THERMAL 14
#define OS_ERROR_PROTECTION 15
#define OS_ERROR_FP_EXCEPTION 16
#define OS_ERROR_MAX 17

typedef void (*OSErrorHandler)(u16 error, OSContext* context, u32 dsisr, u32 dar, ...);
extern OSErrorHandler __OSErrorTable[OS_ERROR_MAX];

DECOMP_STATIC(char __OSUnhandledException_msg1[])
DECOMP_STATIC(char __OSUnhandledException_msg2[])
DECOMP_STATIC(char __OSUnhandledException_msg3[])

ATTRIBUTE_FORMAT(printf, 1, 2) void OSReport(const char * message, ...);
UNKNOWN_FUNCTION(OSVReport);

// Weak symbol, game overrides with its own
ATTRIBUTE_FORMAT(printf, 3, 4) void OSPanic(const char * filename, s32 line,
                                            const char * msg, ...);

UNKNOWN_FUNCTION(OSSetErrorHandler);
void __OSUnhandledException(u8 exception, OSContext* context, u32 dsisr, u32 dar);

CPP_WRAPPER_END()
