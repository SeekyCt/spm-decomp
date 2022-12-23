#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>

#define EVT_DECLARE_USER_FUNC(symbol, argCount) \
    s32 symbol(EvtEntry * script, bool isFirstCall);

#define EVT_UNKNOWN_USER_FUNC(symbol) \
    s32 symbol(EvtEntry * script, bool isFirstCall);

#define EVT_DEFINE_USER_FUNC(name) \
    s32 name(EvtEntry * evt, bool isFirstCall)

#define EVT_DECLARE(symbol) \
    extern EvtScriptCode symbol[];

#define EVT_CMD_(opcode) \
    (0 << 16) | opcode

#define EVT_CMD(opcode, ...) \
    ((sizeof((EvtScriptCode[]){__VA_ARGS__})/sizeof(EvtScriptCode)) << 16) | (opcode), \
    __VA_ARGS__

#define UF(id) ((id) - EVTDAT_UF_BASE)
#define UW(id) ((id) - EVTDAT_UW_BASE)
#define GSW(id) ((id) - EVTDAT_GSW_BASE)
#define LSW(id) ((id) - EVTDAT_LSW_BASE)
#define GSWF(id) ((id) - EVTDAT_GSWF_BASE)
#define LSWF(id) ((id) - EVTDAT_LSWF_BASE)
#define GF(id) ((id) - EVTDAT_GF_BASE)
#define LF(id) ((id) - EVTDAT_LF_BASE)
#define GW(id) ((id) - EVTDAT_GW_BASE)
#define LW(id) ((id) - EVTDAT_LW_BASE)
#define FLOAT(value) (((s32) ((value) * 1024.f)) - EVTDAT_FLOAT_BASE)

#define PTR(addr) \
    ((s32)(addr))
    
#define EVT_NULLPTR (-EVTDAT_ADDR_BASE)

#define EVT_BEGIN(symbol) \
    EvtScriptCode symbol[] = {

#define EVT_END() \
    EVT_CMD_(EVT_OPC_END_SCRIPT) \
    };

/*
    Instructions
*/

#define RETURN() \
    EVT_CMD_(EVT_OPC_END_EVT),

#define LBL(id) \
    EVT_CMD(EVT_OPC_LBL, id),
#define GOTO(id) \
    EVT_CMD(EVT_OPC_GOTO, id),

#define DO(n) \
    EVT_CMD(EVT_OPC_DO, n),
#define WHILE() \
    EVT_CMD_(EVT_OPC_WHILE),
#define DO_BREAK() \
    EVT_CMD_(EVT_OPC_DO_BREAK),
#define DO_CONTINUE() \
    EVT_CMD_(EVT_OPC_DO_CONTINUE),

#define WAIT_FRM(frames) \
    EVT_CMD(EVT_OPC_WAIT_FRM, frames),
#define WAIT_MSEC(msecs) \
    EVT_CMD(EVT_OPC_WAIT_MSEC, msecs),
#define HALT(condition) \
    EVT_CMD(EVT_OPC_HALT, condition),

#define IF_STR_EQUAL(s1, s2) \
    EVT_CMD(EVT_OPC_IF_STR_EQUAL, s1, s2),
#define IF_STR_NOT_EQUAL(s1, s2) \
    EVT_CMD(EVT_OPC_IF_STR_NOT_EQUAL, s1, s2),
#define IF_STR_SMALL(s1, s2) \
    EVT_CMD(EVT_OPC_IF_STR_SMALL, s1, s2),
#define IF_STR_LARGE(s1, s2) \
    EVT_CMD(EVT_OPC_IF_STR_LARGE, s1, s2),
#define IF_STR_SMALL_EQUAL(s1, s2) \
    EVT_CMD(EVT_OPC_IF_STR_SMALL_EQUAL, s1, s2),
#define IF_STR_LARGE_EQUAL(s1, s2) \
    EVT_CMD(EVT_OPC_IF_STR_LARGE_EQUAL, s1, s2),

#define IFF_EQUAL(f1, f2) \
    EVT_CMD(EVT_OPC_IFF_EQUAL, f1, f2),
#define IFF_NOT_EQUAL(f1, f2) \
    EVT_CMD(EVT_OPC_IFF_NOT_EQUAL, f1, f2),
#define IFF_SMALL(f1, f2) \
    EVT_CMD(EVT_OPC_IFF_SMALL, f1, f2),
#define IFF_LARGE(f1, f2) \
    EVT_CMD(EVT_OPC_IFF_LARGE, f1, f2),
#define IFF_SMALL_EQUAL(f1, f2) \
    EVT_CMD(EVT_OPC_IFF_SMALL_EQUAL, f1, f2),
#define IFF_LARGE_EQUAL(f1, f2) \
    EVT_CMD(EVT_OPC_IFF_LARGE_EQUAL, f1, f2),

#define IF_EQUAL(v1, v2) \
    EVT_CMD(EVT_OPC_IF_EQUAL, v1, v2),
#define IF_NOT_EQUAL(v1, v2) \
    EVT_CMD(EVT_OPC_IF_NOT_EQUAL, v1, v2),
#define IF_SMALL(v1, v2) \
    EVT_CMD(EVT_OPC_IF_SMALL, v1, v2),
#define IF_LARGE(v1, v2) \
    EVT_CMD(EVT_OPC_IF_LARGE, v1, v2),
#define IF_SMALL_EQUAL(v1, v2) \
    EVT_CMD(EVT_OPC_IF_SMALL_EQUAL, v1, v2),
#define IF_LARGE_EQUAL(v1, v2) \
    EVT_CMD(EVT_OPC_IF_LARGE_EQUAL, v1, v2),

#define IF_FLAG(value, mask) \
    EVT_CMD(EVT_OPC_IF_FLAG, value, mask),
#define IF_NOT_FLAG(value, mask) \
    EVT_CMD(EVT_OPC_IF_NOT_FLAG, value, mask),

#define ELSE() \
    EVT_CMD_(EVT_OPC_ELSE),
#define END_IF() \
    EVT_CMD_(EVT_OPC_END_IF),

#define SWITCH(value) \
    EVT_CMD(EVT_OPC_SWITCH, value),
#define SWITCHI(value) \
    EVT_CMD(EVT_OPC_SWITCHI, value),

#define CASE_EQUAL(value) \
    EVT_CMD(EVT_OPC_CASE_EQUAL, value),
#define CASE_NOT_EQUAL(value) \
    EVT_CMD(EVT_OPC_CASE_NOT_EQUAL, value),
#define CASE_SMALL(value) \
    EVT_CMD(EVT_OPC_CASE_SMALL, value),
#define CASE_LARGE(value) \
    EVT_CMD(EVT_OPC_CASE_LARGE, value),
#define CASE_SMALL_EQUAL(value) \
    EVT_CMD(EVT_OPC_CASE_SMALL_EQUAL, value),
#define CASE_LARGE_EQUAL(value) \
    EVT_CMD(EVT_OPC_CASE_LARGE_EQUAL, value),
#define CASE_ETC() \
    EVT_CMD_(EVT_OPC_CASE_ETC),
#define CASE_OR(value) \
    EVT_CMD(EVT_OPC_CASE_OR, value),
#define CASE_AND(value) \
    EVT_CMD(EVT_OPC_CASE_AND, value),
#define CASE_FLAG(value) \
    EVT_CMD(EVT_OPC_CASE_FLAG, value),
#define CASE_END() \
    EVT_CMD_(EVT_OPC_CASE_END),
#define EVT_CASE_BETWEEN(min, max) \
    EVT_CMD(EVT_OPC_CASE_BETWEEN, min, max),
#define SWITCH_BREAK() \
    EVT_CMD_(EVT_OPC_SWITCH_BREAK),
#define END_SWITCH() \
    EVT_CMD_(EVT_OPC_END_SWITCH),

#define SET(var, value) \
    EVT_CMD(EVT_OPC_SET, var, value),
#define SETI(var, value) \
    EVT_CMD(EVT_OPC_SETI, var, value),
#define SETF(var, value) \
    EVT_CMD(EVT_OPC_SETF, var, value),

#define ADD(var, value) \
    EVT_CMD(EVT_OPC_ADD, var, value),
#define SUB(var, value) \
    EVT_CMD(EVT_OPC_SUB, var, value),
#define MUL(var, value) \
    EVT_CMD(EVT_OPC_MUL, var, value),
#define DIV(var, value) \
    EVT_CMD(EVT_OPC_DIV, var, value),
#define MOD(var, value) \
    EVT_CMD(EVT_OPC_MOD, var, value),

#define ADDF(var, value) \
    EVT_CMD(EVT_OPC_ADDF, var, value),
#define SUBF(var, value) \
    EVT_CMD(EVT_OPC_SUBF, var, value),
#define MULF(var, value) \
    EVT_CMD(EVT_OPC_MULF, var, value),
#define DIVF(var, value) \
    EVT_CMD(EVT_OPC_DIVF, var, value),

#define SET_READ(addr) \
    EVT_CMD(EVT_OPC_SET_READ, addr),
#define READ(dest) \
    EVT_CMD(EVT_OPC_READ, dest),
#define READ2(dest1, dest2) \
    EVT_CMD(EVT_OPC_READ2, dest1, dest2),
#define READ3(dest1, dest2, dest3) \
    EVT_CMD(EVT_OPC_READ3, dest1, dest2, dest3),
#define READ4(dest1, dest2, dest3, dest4) \
    EVT_CMD(EVT_OPC_READ4, dest1, dest2, dest3, dest4),
#define READ_N(dest, idx) \
    EVT_CMD(EVT_OPC_READ_N, dest, idx),

#define SET_READF(addr) \
    EVT_CMD(EVT_OPC_SET_READF, addr),
#define READF(dest) \
    EVT_CMD(EVT_OPC_READF, dest),
#define READF2(dest1, dest2) \
    EVT_CMD(EVT_OPC_READF2, dest1, dest2),
#define READF3(dest1, dest2, dest3) \
    EVT_CMD(EVT_OPC_READF3, dest1, dest2, dest3),
#define READF4(dest1, dest2, dest3, dest4) \
    EVT_CMD(EVT_OPC_READF4, dest1, dest2, dest3, dest4),
#define READF_N(dest, idx) \
    EVT_CMD(EVT_OPC_READF_N, dest, idx),

#define CLAMP_INT(var, min, max) \
    EVT_CMD(EVT_OPC_CLAMP_INT, var, min, max),

#define SET_USER_WRK(addr) \
    EVT_CMD(EVT_OPC_SET_USER_WRK, addr),
#define SET_USER_FLG(addr) \
    EVT_CMD(EVT_OPC_SET_USER_FLG, addr),
#define ALLOC_USER_WRK(count) \
    EVT_CMD(EVT_OPC_ALLOC_USER_WRK, count),

#define AND(dest, mask) \
    EVT_CMD(EVT_OPC_AND, dest, mask),
#define ANDI(dest, mask) \
    EVT_CMD(EVT_OPC_ANDI, dest, mask),
#define OR(dest, mask) \
    EVT_CMD(EVT_OPC_OR, dest, mask),
#define ORI(dest, mask) \
    EVT_CMD(EVT_OPC_ORI, dest, mask),

#define SET_FRAME_FROM_MSEC(dest, msecs) \
    EVT_CMD(EVT_OPC_SET_FRAME_FROM_MSEC, dest, msecs),
#define SET_MSEC_FROM_FRAMES(dest, frames) \
    EVT_CMD(EVT_OPC_SET_MSEC_FROM_FRAME, dest, frames),

#define SET_RAM(value, addr) \
    EVT_CMD(EVT_OPC_SET_RAM, value, addr),
#define SET_RAMF(value, addr) \
    EVT_CMD(EVT_OPC_SET_RAMF, value, addr),
#define GET_RAM(dest, addr) \
    EVT_CMD(EVT_OPC_GET_RAM, dest, addr),
#define GET_RAMF(dese, addr) \
    EVT_CMD(EVT_OPC_GET_RAMF, dest, addr),

#define SETR(dest, value) \
    EVT_CMD(EVT_OPC_SETR, dest, value),
#define SETRF(dest, value) \
    EVT_CMD(EVT_OPC_SETRF, dest, value),
#define GETR(value, dest) \
    EVT_CMD(EVT_OPC_GETR, value, dest),
#define GETRF(value, dest) \
    EVT_CMD(EVT_OPC_GETRF, value, dest),

#define USER_FUNC(...) \
    EVT_CMD(EVT_OPC_USER_FUNC, (s32) __VA_ARGS__),

#define RUN_EVT(evt) \
    EVT_CMD(EVT_OPC_RUN_EVT, evt),
#define RUN_EVT_ID(evt, dest) \
    EVT_CMD(EVT_OPC_RUN_EVT_ID, evt, dest),
#define RUN_CHILD_EVT(evt) \
    EVT_CMD(EVT_OPC_RUN_CHILD_EVT, evt),
#define DELETE_EVT(id) \
    EVT_CMD(EVT_OPC_DELETE_EVT, id),
#define RESTART_EVT(id) \
    EVT_CMD(EVT_OPC_RESTART_EVT, id),

#define SET_PRI(pri) \
    EVT_CMD(EVT_OPC_SET_PRI, pri),
#define SET_SPD(spd) \
    EVT_CMD(EVT_OPC_SET_SPD, spd),
#define SET_TYPE(type) \
    EVT_CMD(EVT_OPC_SET_TYPE, type),

#define STOP_ALL(type) \
    EVT_CMD(EVT_OPC_STOP_ALL, type),
#define START_ALL(type) \
    EVT_CMD(EVT_OPC_START_ALL, type),
#define STOP_OTHER(type) \
    EVT_CMD(EVT_OPC_STOP_OTHER, type),
#define START_OTHER(type) \
    EVT_CMD(EVT_OPC_START_OTHER, type),
#define STOP_ID(id) \
    EVT_CMD(EVT_OPC_STOP_ID, id),
#define START_ID(id) \
    EVT_CMD(EVT_OPC_START_ID, id),
#define CHK_EVT(id, dest) \
    EVT_CMD(EVT_OPC_CHK_EVT, id, dest),

#define INLINE_EVT() \
    EVT_CMD_(EVT_OPC_INLINE_EVT),
#define INLINE_EVT_ID(dest) \
    EVT_CMD(EVT_OPC_INLINE_EVT_ID, dest),
#define END_INLINE() \
    EVT_CMD_(EVT_OPC_END_INLINE),
#define BROTHER_EVT() \
    EVT_CMD_(EVT_OPC_BROTHER_EVT),
#define BROTHER_EVT_ID(dest) \
    EVT_CMD(EVT_OPC_BROTHER_EVT_ID, dest),
#define END_BROTHER() \
    EVT_CMD_(EVT_OPC_END_BROTHER),

#define DEBUG_PUT_MSG(msg) \
    EVT_CMD(EVT_OPC_DEBUG_PUT_MSG, msg),
#define DEBUG_MSG_CLEAR() \
    EVT_CMD(EVT_OPC_DEBUG_MSG_CLEAR),
#define DEBUG_PUT_REG(reg) \
    EVT_CMD(EVT_OPC_DEBUG_PUT_REG, reg),
#define DEBUG_NAME(name) \
    EVT_CMD(EVT_OPC_DEBUG_NAME, name),
#define DEBUG_REM() \
    EVT_CMD_(EVT_OPC_DEBUG_REM),
#define DEBUG_BP() \
    EVT_CMD_(EVT_OPC_DEBUG_BP),
