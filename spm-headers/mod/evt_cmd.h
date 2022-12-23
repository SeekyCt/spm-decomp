// Based off of https://github.com/PistonMiner/ttyd-tools/blob/master/ttyd-tools/rel/include/evt_cmd.h

#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/evtmgr_cmd.h>

#define EVT_DECLARE_USER_FUNC(name, parameter_count) \
    constexpr s32 name##_parameter_count = (parameter_count); \
    s32 name(spm::evtmgr::EvtEntry * evt, bool isFirstCall);

#define EVT_UNKNOWN_USER_FUNC(name) \
    EVT_DECLARE_USER_FUNC(name, -1)

#define EVT_DEFINE_USER_FUNC(name) \
    s32 name(spm::evtmgr::EvtEntry * evt, bool isFirstCall)

#define EVT_DECLARE(name) \
    extern const spm::evtmgr::EvtScriptCode name[];

#define EVT_BEGIN(name) \
    const spm::evtmgr::EvtScriptCode name[] = {
#define EVT_BEGIN_EDITABLE(name) \
    s32 name[] = {
#define EVT_END() \
    0x1 };

#define EVT_HELPER_OP(op) \
    reinterpret_cast<s32>((op))

// Expression types
#define EVT_HELPER_EXPR(base, offset) \
    EVT_HELPER_OP((base) + (offset))

#define EVT_HELPER_LW_BASE -30000000
#define EVT_HELPER_GW_BASE -50000000
#define EVT_HELPER_LF_BASE -70000000
#define EVT_HELPER_GF_BASE -90000000
#define EVT_HELPER_LSWF_BASE -110000000
#define EVT_HELPER_GSWF_BASE -130000000
#define EVT_HELPER_LSW_BASE -150000000
#define EVT_HELPER_GSW_BASE -170000000
#define EVT_HELPER_UW_BASE -190000000
#define EVT_HELPER_UF_BASE -210000000
#define EVT_HELPER_FLOAT_BASE -240000000
#define EVT_HELPER_POINTER_BASE -270000000

#define EVT_NULLPTR EVT_HELPER_POINTER_BASE

#define LW(id) \
    EVT_HELPER_EXPR(EVT_HELPER_LW_BASE, id)
#define GW(id) \
    EVT_HELPER_EXPR(EVT_HELPER_GW_BASE, id)
#define LF(id) \
    EVT_HELPER_EXPR(EVT_HELPER_LF_BASE, id)
#define GF(id) \
    EVT_HELPER_EXPR(EVT_HELPER_GF_BASE, id)
#define LSWF(id) \
    EVT_HELPER_EXPR(EVT_HELPER_LSWF_BASE, id)
#define GSWF(id) \
    EVT_HELPER_EXPR(EVT_HELPER_GSWF_BASE, id)
#define LSW(id) \
    EVT_HELPER_EXPR(EVT_HELPER_LSW_BASE, id)
#define GSW(id) \
    EVT_HELPER_EXPR(EVT_HELPER_GSW_BASE, id)
#define UW(id) \
    EVT_HELPER_EXPR(EVT_HELPER_UW_BASE, id)
#define UF(id) \
    EVT_HELPER_EXPR(EVT_HELPER_UF_BASE, id)

#define FLOAT(value) \
    EVT_HELPER_EXPR( \
        EVT_HELPER_FLOAT_BASE, static_cast<s32>((value) * 1024.f) \
    )
#define PTR(value) \
    reinterpret_cast<s32>(value)

// Commands
#define EVT_HELPER_CMD(parameter_count, opcode) \
    static_cast<s32>( \
        static_cast<u32>((parameter_count) << 16 | (opcode)) \
    )

#define RETURN() \
    EVT_HELPER_CMD(0, 2),

#define LBL(id) \
    EVT_HELPER_CMD(1, 3), EVT_HELPER_OP(id),
#define GOTO(id) \
    EVT_HELPER_CMD(1, 4), EVT_HELPER_OP(id),

#define DO(iteration_count) \
    EVT_HELPER_CMD(1, 5), EVT_HELPER_OP(iteration_count),
#define WHILE() \
    EVT_HELPER_CMD(0, 6),
#define DO_BREAK() \
    EVT_HELPER_CMD(0, 7),
#define DO_CONTINUE() \
    EVT_HELPER_CMD(0, 8),

#define WAIT_FRM(frm) \
    EVT_HELPER_CMD(1, 9), EVT_HELPER_OP(frm),
#define WAIT_MSEC(msec) \
    EVT_HELPER_CMD(1, 10), EVT_HELPER_OP(msec),
#define HALT(until) \
    EVT_HELPER_CMD(1, 11), EVT_HELPER_OP(until),

#define IF_STR_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 12), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IF_STR_NOT_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 13), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IF_STR_SMALL(lhs, rhs) \
    EVT_HELPER_CMD(2, 14), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IF_STR_LARGE(lhs, rhs) \
    EVT_HELPER_CMD(2, 15), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IF_STR_SMALL_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 16), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IF_STR_LARGE_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 17), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),

#define IFF_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 18), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IFF_NOT_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 19), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IFF_SMALL(lhs, rhs) \
    EVT_HELPER_CMD(2, 20), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IFF_LARGE(lhs, rhs) \
    EVT_HELPER_CMD(2, 21), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IFF_SMALL_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 22), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IFF_LARGE_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 23), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),

#define IF_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 24), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IF_NOT_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 25), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IF_SMALL(lhs, rhs) \
    EVT_HELPER_CMD(2, 26), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IF_LARGE(lhs, rhs) \
    EVT_HELPER_CMD(2, 27), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IF_SMALL_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 28), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define IF_LARGE_EQUAL(lhs, rhs) \
    EVT_HELPER_CMD(2, 29), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),

#define IF_FLAG(val, mask) \
    EVT_HELPER_CMD(2, 30), EVT_HELPER_OP(val), reinterpret_cast<s32>(mask),
#define IF_NOT_FLAG(val, mask) \
    EVT_HELPER_CMD(2, 31), EVT_HELPER_OP(val), reinterpret_cast<s32>(mask),

#define ELSE() \
    EVT_HELPER_CMD(0, 32),
#define END_IF() \
    EVT_HELPER_CMD(0, 33),

#define SWITCH(val) \
    EVT_HELPER_CMD(1, 34), EVT_HELPER_OP(val),
#define SWITCHI(val) \
    EVT_HELPER_CMD(1, 35), EVT_HELPER_OP(val),

#define CASE_EQUAL(val) \
    EVT_HELPER_CMD(1, 36), EVT_HELPER_OP(val),
#define CASE_NOT_EQUAL(val) \
    EVT_HELPER_CMD(1, 37), EVT_HELPER_OP(val),
#define CASE_SMALL(val) \
    EVT_HELPER_CMD(1, 38), EVT_HELPER_OP(val),
#define CASE_LARGE(val) \
    EVT_HELPER_CMD(1, 39), EVT_HELPER_OP(val),
#define CASE_SMALL_EQUAL(val) \
    EVT_HELPER_CMD(1, 40), EVT_HELPER_OP(val),
#define CASE_LARGE_EQUAL(val) \
    EVT_HELPER_CMD(1, 41), EVT_HELPER_OP(val),

#define CASE_ETC() \
    EVT_HELPER_CMD(0, 42),
#define CASE_OR(val) \
    EVT_HELPER_CMD(1, 43), EVT_HELPER_OP(val),
#define CASE_AND(val) \
    EVT_HELPER_CMD(1, 44), EVT_HELPER_OP(val),
#define CASE_FLAG(mask) \
    EVT_HELPER_CMD(1, 45), reinterpret_cast<s32>(mask),
#define CASE_END() \
    EVT_HELPER_CMD(0, 46),
#define CASE_BETWEEN(low, high) \
    EVT_HELPER_CMD(2, 47), EVT_HELPER_OP(low), EVT_HELPER_OP(high),

#define SWITCH_BREAK() \
    EVT_HELPER_CMD(0, 48),
#define END_SWITCH() \
    EVT_HELPER_CMD(0, 49),

#define SET(lhs, rhs) \
    EVT_HELPER_CMD(2, 50), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define SETI(lhs, rhs) \
    EVT_HELPER_CMD(2, 51), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define SETF(lhs, rhs) \
    EVT_HELPER_CMD(2, 52), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),

#define ADD(lhs, rhs) \
    EVT_HELPER_CMD(2, 53), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define SUB(lhs, rhs) \
    EVT_HELPER_CMD(2, 54), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define MUL(lhs, rhs) \
    EVT_HELPER_CMD(2, 55), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define DIV(lhs, rhs) \
    EVT_HELPER_CMD(2, 56), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define MOD(lhs, rhs) \
    EVT_HELPER_CMD(2, 57), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),

#define ADDF(lhs, rhs) \
    EVT_HELPER_CMD(2, 58), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define SUBF(lhs, rhs) \
    EVT_HELPER_CMD(2, 59), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define MULF(lhs, rhs) \
    EVT_HELPER_CMD(2, 60), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),
#define DIVF(lhs, rhs) \
    EVT_HELPER_CMD(2, 61), EVT_HELPER_OP(lhs), EVT_HELPER_OP(rhs),

#define SET_READ(val) \
    EVT_HELPER_CMD(1, 62), EVT_HELPER_OP(val),
#define READ(out) \
    EVT_HELPER_CMD(1, 63), EVT_HELPER_OP(out),
#define READ2(out1, out2) \
    EVT_HELPER_CMD(2, 64), EVT_HELPER_OP(out1), EVT_HELPER_OP(out2),
#define READ3(out1, out2, out3) \
    EVT_HELPER_CMD(3, 65), EVT_HELPER_OP(out1), EVT_HELPER_OP(out2), \
    EVT_HELPER_OP(out3),
#define READ4(out1, out2, out3, out4) \
    EVT_HELPER_CMD(4, 66), EVT_HELPER_OP(out1), EVT_HELPER_OP(out2), \
    EVT_HELPER_OP(out3), EVT_HELPER_OP(out4),
#define READ_N(out, index) \
    EVT_HELPER_CMD(2, 67), EVT_HELPER_OP(out), EVT_HELPER_OP(index),

#define SET_READF(val) \
    EVT_HELPER_CMD(1, 68), EVT_HELPER_OP(val),
#define READF(out) \
    EVT_HELPER_CMD(1, 69), EVT_HELPER_OP(out),
#define READF2(out1, out2) \
    EVT_HELPER_CMD(2, 70), EVT_HELPER_OP(out1), EVT_HELPER_OP(out2),
#define READF3(out1, out2, out3) \
    EVT_HELPER_CMD(3, 71), EVT_HELPER_OP(out1), EVT_HELPER_OP(out2), \
    EVT_HELPER_OP(out3),
#define READF4(out1, out2, out3, out4) \
    EVT_HELPER_CMD(4, 72), EVT_HELPER_OP(out1), EVT_HELPER_OP(out2), \
    EVT_HELPER_OP(out3), EVT_HELPER_OP(out4),
#define READF_N(out, index) \
    EVT_HELPER_CMD(2, 73), EVT_HELPER_OP(out), EVT_HELPER_OP(index),

#define CLAMP_INT(var, min, max) \
    EVT_HELPER_CMD(3, 74), EVT_HELPER_OP(var), EVT_HELPER_OP(min), \
    EVT_HELPER_OP(max)

#define SET_USER_WRK(val) \
    EVT_HELPER_CMD(1, 75), EVT_HELPER_OP(val),
#define SET_USER_FLG(val) \
    EVT_HELPER_CMD(1, 76), EVT_HELPER_OP(val),
#define ALLOC_USER_WRK(count) \
    EVT_HELPER_CMD(1, 77), EVT_HELPER_OP(count),

#define AND(out, lhs, rhs) \
    EVT_HELPER_CMD(3, 78), EVT_HELPER_OP(out), EVT_HELPER_OP(lhs), \
    EVT_HELPER_OP(rhs),
#define ANDI(out, lhs, rhs) \
    EVT_HELPER_CMD(3, 79), EVT_HELPER_OP(out), EVT_HELPER_OP(lhs), \
    EVT_HELPER_OP(rhs),
#define OR(out, lhs, rhs) \
    EVT_HELPER_CMD(3, 80), EVT_HELPER_OP(out), EVT_HELPER_OP(lhs), \
    EVT_HELPER_OP(rhs),
#define ORI(out, lhs, rhs) \
    EVT_HELPER_CMD(3, 81), EVT_HELPER_OP(out), EVT_HELPER_OP(lhs), \
    EVT_HELPER_OP(rhs),

#define SET_FRAME_FROM_MSEC(out, in) \
    EVT_HELPER_CMD(2, 82), EVT_HELPER_OP(out), EVT_HELPER_OP(in),
#define SET_MSEC_FROM_FRAME(out, in) \
    EVT_HELPER_CMD(2, 83), EVT_HELPER_OP(out), EVT_HELPER_OP(in),
#define SET_RAM(val, ptr) \
    EVT_HELPER_CMD(2, 84), EVT_HELPER_OP(val), EVT_HELPER_OP(ptr),
#define SET_RAMF(val, ptr) \
    EVT_HELPER_CMD(2, 85), EVT_HELPER_OP(val), EVT_HELPER_OP(ptr),
#define GET_RAM(out, ptr) \
    EVT_HELPER_CMD(2, 86), EVT_HELPER_OP(out), EVT_HELPER_OP(ptr),
#define GET_RAMF(out, ptr) \
    EVT_HELPER_CMD(2, 87), EVT_HELPER_OP(out), EVT_HELPER_OP(ptr),

// R is short for Reg
#define SETR(indirect, val) \
    EVT_HELPER_CMD(2, 88), EVT_HELPER_OP(indirect), EVT_HELPER_OP(val),
#define SETRF(indirect, val) \
    EVT_HELPER_CMD(2, 89), EVT_HELPER_OP(indirect), EVT_HELPER_OP(val),
#define GETR(indirect, out) \
    EVT_HELPER_CMD(2, 90), EVT_HELPER_OP(indirect), EVT_HELPER_OP(out),
#define GETRF(indirect, out) \
    EVT_HELPER_CMD(2, 91), EVT_HELPER_OP(indirect), EVT_HELPER_OP(out),

// User function calls with validated parameter counts
template<bool expression>
class expression_assert
{
    static_assert(expression);
};
using evt_helper_int_array = s32[];
#define EVT_HELPER_NUM_ARGS(...) \
    (sizeof(evt_helper_int_array{ __VA_ARGS__ }) / sizeof(s32))
#define USER_FUNC(function, ...) \
    ( \
        expression_assert< \
            function##_parameter_count == -1 \
            || function##_parameter_count == EVT_HELPER_NUM_ARGS(__VA_ARGS__) \
        >(), \
        EVT_HELPER_CMD(1 + EVT_HELPER_NUM_ARGS(__VA_ARGS__), 92) \
    ), \
    reinterpret_cast<s32>(function), \
    ##__VA_ARGS__ ,

#define RUN_EVT(evt) \
    EVT_HELPER_CMD(1, 93), EVT_HELPER_OP(evt),
#define RUN_EVT_ID(evt, out_id) \
    EVT_HELPER_CMD(2, 94), EVT_HELPER_OP(evt), EVT_HELPER_OP(out_id),
#define RUN_CHILD_EVT(evt) \
    EVT_HELPER_CMD(1, 95), EVT_HELPER_OP(evt),
#define DELETE_EVT(evt_id) \
    EVT_HELPER_CMD(1, 96), EVT_HELPER_OP(evt_id),
#define RESTART_EVT(evt) \
    EVT_HELPER_CMD(1, 97), EVT_HELPER_OP(evt),

#define SET_PRI(pri) \
    EVT_HELPER_CMD(1, 98), EVT_HELPER_OP(pri),
#define SET_SPD(spd) \
    EVT_HELPER_CMD(1, 99), EVT_HELPER_OP(spd),
#define SET_TYPE(type_mask) \
    EVT_HELPER_CMD(1, 100), EVT_HELPER_OP(type_mask),

#define STOP_ALL(type_mask) \
    EVT_HELPER_CMD(1, 101), EVT_HELPER_OP(type_mask),
#define START_ALL(type_mask) \
    EVT_HELPER_CMD(1, 102), EVT_HELPER_OP(type_mask),
#define STOP_OTHER(type_mask) \
    EVT_HELPER_CMD(1, 103), EVT_HELPER_OP(type_mask),
#define START_OTHER(type_mask) \
    EVT_HELPER_CMD(1, 104), EVT_HELPER_OP(type_mask),
#define STOP_ID(evt_id) \
    EVT_HELPER_CMD(1, 105), EVT_HELPER_OP(evt_id),
#define START_ID(evt_id) \
    EVT_HELPER_CMD(1, 106), EVT_HELPER_OP(evt_id),
#define CHK_EVT(evt_id, is_running) \
    EVT_HELPER_CMD(2, 107), EVT_HELPER_OP(evt_id), EVT_HELPER_OP(is_running)

#define INLINE_EVT() \
    EVT_HELPER_CMD(0, 108),
#define INLINE_EVT_ID(out_id) \
    EVT_HELPER_CMD(1, 109), EVT_HELPER_OP(out_id),
#define END_INLINE() \
    EVT_HELPER_CMD(0, 110),

#define BROTHER_EVT() \
    EVT_HELPER_CMD(0, 111),
#define BROTHER_EVT_ID(out_id) \
    EVT_HELPER_CMD(1, 112), EVT_HELPER_OP(out_id),
#define END_BROTHER() \
    EVT_HELPER_CMD(0, 113),

#define DEBUG_PUT_MSG(msg) \
    EVT_HELPER_CMD(1, 114), EVT_HELPER_OP(msg),
#define DEBUG_MSG_CLEAR(msg) \
    EVT_HELPER_CMD(0, 115),
#define DEBUG_PUT_REG(reg) \
    EVT_HELPER_CMD(1, 116), EVT_HELPER_OP(reg),
#define DEBUG_NAME(name) \
    EVT_HELPER_CMD(1, 117), EVT_HELPER_OP(name),
#define DEBUG_REM(text) \
    EVT_HELPER_CMD(1, 118), EVT_HELPER_OP(text),
#define DEBUG_BP(text) \
    EVT_HELPER_CMD(0, 119),
