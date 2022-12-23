/*
    Functions to execute interpreted evt script code and handle evt variables
*/

#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/evtmgr.h>

CPP_WRAPPER(spm::evtmgr_cmd)

USING(spm::evtmgr::EvtEntry)
USING(spm::evtmgr::EvtScriptCode)
USING(spm::evtmgr::EvtVar)

// Data types
#define EVTDAT_ADDR_MAX -290000000
#define EVTDAT_FLOAT_MAX -220000000
#define EVTDAT_UF_MAX -200000000
#define EVTDAT_UW_MAX -180000000
#define EVTDAT_GSW_MAX -160000000
#define EVTDAT_LSW_MAX -140000000
#define EVTDAT_GSWF_MAX -120000000
#define EVTDAT_LSWF_MAX -100000000
#define EVTDAT_GF_MAX -80000000
#define EVTDAT_LF_MAX -60000000
#define EVTDAT_GW_MAX -40000000
#define EVTDAT_LW_MAX -20000000

#define EVTDAT_ADDR_BASE 270000000
#define EVTDAT_FLOAT_BASE 240000000
#define EVTDAT_UF_BASE 210000000
#define EVTDAT_UW_BASE 190000000
#define EVTDAT_GSW_BASE 170000000
#define EVTDAT_LSW_BASE 150000000
#define EVTDAT_GSWF_BASE 130000000
#define EVTDAT_LSWF_BASE 110000000
#define EVTDAT_GF_BASE 90000000
#define EVTDAT_LF_BASE 70000000
#define EVTDAT_GW_BASE 50000000
#define EVTDAT_LW_BASE 30000000

/*
    Instruction / User Func Return values
*/

// Stop execution of all scripts this frame
#define EVT_RET_END_FRAME -1

// Keep re-executing current instruction, continue next frame if too much time passed
#define EVT_RET_BLOCK_WEAK 0

// Move to next instruction, continue next frame if too much time passed
#define EVT_RET_CONTINUE_WEAK 1

// Move to next instruction
#define EVT_RET_CONTINUE 2

// Keep re-executing current instruction
#define EVT_RET_BLOCK 3

// Stop running current script
#define EVT_RET_END 0xff

/*
    evtmgrCmd return values
*/

#define EVTCMD_RET_END -1
#define EVTCMD_RET_CONTINUE 0
#define EVTCMD_RET_END_ALL 1

/*
    Opcodes
*/
enum EvtOpcode
{
/* 0x00 */ EVT_OPC_NEXT,
/* 0x01 */ EVT_OPC_END_SCRIPT,
/* 0x02 */ EVT_OPC_END_EVT,
/* 0x03 */ EVT_OPC_LBL,
/* 0x04 */ EVT_OPC_GOTO,
/* 0x05 */ EVT_OPC_DO,
/* 0x06 */ EVT_OPC_WHILE,
/* 0x07 */ EVT_OPC_DO_BREAK,
/* 0x08 */ EVT_OPC_DO_CONTINUE,
/* 0x09 */ EVT_OPC_WAIT_FRM,
/* 0x0A */ EVT_OPC_WAIT_MSEC,
/* 0x0B */ EVT_OPC_HALT,
/* 0x0C */ EVT_OPC_IF_STR_EQUAL,
/* 0x0D */ EVT_OPC_IF_STR_NOT_EQUAL,
/* 0x0E */ EVT_OPC_IF_STR_SMALL,
/* 0x0F */ EVT_OPC_IF_STR_LARGE,
/* 0x10 */ EVT_OPC_IF_STR_SMALL_EQUAL,
/* 0x11 */ EVT_OPC_IF_STR_LARGE_EQUAL,
/* 0x12 */ EVT_OPC_IFF_EQUAL,
/* 0x13 */ EVT_OPC_IFF_NOT_EQUAL,
/* 0x14 */ EVT_OPC_IFF_SMALL,
/* 0x15 */ EVT_OPC_IFF_LARGE,
/* 0x16 */ EVT_OPC_IFF_SMALL_EQUAL,
/* 0x17 */ EVT_OPC_IFF_LARGE_EQUAL,
/* 0x18 */ EVT_OPC_IF_EQUAL,
/* 0x19 */ EVT_OPC_IF_NOT_EQUAL,
/* 0x1A */ EVT_OPC_IF_SMALL,
/* 0x1B */ EVT_OPC_IF_LARGE,
/* 0x1C */ EVT_OPC_IF_SMALL_EQUAL,
/* 0x1D */ EVT_OPC_IF_LARGE_EQUAL,
/* 0x1E */ EVT_OPC_IF_FLAG,
/* 0x1F */ EVT_OPC_IF_NOT_FLAG,
/* 0x20 */ EVT_OPC_ELSE,
/* 0x21 */ EVT_OPC_END_IF,
/* 0x22 */ EVT_OPC_SWITCH,
/* 0x23 */ EVT_OPC_SWITCHI,
/* 0x24 */ EVT_OPC_CASE_EQUAL,
/* 0x25 */ EVT_OPC_CASE_NOT_EQUAL,
/* 0x26 */ EVT_OPC_CASE_SMALL,
/* 0x27 */ EVT_OPC_CASE_LARGE,
/* 0x28 */ EVT_OPC_CASE_SMALL_EQUAL,
/* 0x29 */ EVT_OPC_CASE_LARGE_EQUAL,
/* 0x2A */ EVT_OPC_CASE_ETC,
/* 0x2B */ EVT_OPC_CASE_OR,
/* 0x2C */ EVT_OPC_CASE_AND,
/* 0x2D */ EVT_OPC_CASE_FLAG,
/* 0x2E */ EVT_OPC_CASE_END,
/* 0x2F */ EVT_OPC_CASE_BETWEEN,
/* 0x30 */ EVT_OPC_SWITCH_BREAK,
/* 0x31 */ EVT_OPC_END_SWITCH,
/* 0x32 */ EVT_OPC_SET,
/* 0x33 */ EVT_OPC_SETI,
/* 0x34 */ EVT_OPC_SETF,
/* 0x35 */ EVT_OPC_ADD,
/* 0x36 */ EVT_OPC_SUB,
/* 0x37 */ EVT_OPC_MUL,
/* 0x38 */ EVT_OPC_DIV,
/* 0x39 */ EVT_OPC_MOD,
/* 0x3A */ EVT_OPC_ADDF,
/* 0x3B */ EVT_OPC_SUBF,
/* 0x3C */ EVT_OPC_MULF,
/* 0x3D */ EVT_OPC_DIVF,
/* 0x3E */ EVT_OPC_SET_READ,
/* 0x3F */ EVT_OPC_READ,
/* 0x40 */ EVT_OPC_READ2,
/* 0x41 */ EVT_OPC_READ3,
/* 0x42 */ EVT_OPC_READ4,
/* 0x43 */ EVT_OPC_READ_N,
/* 0x44 */ EVT_OPC_SET_READF,
/* 0x45 */ EVT_OPC_READF,
/* 0x46 */ EVT_OPC_READF2,
/* 0x47 */ EVT_OPC_READF3,
/* 0x48 */ EVT_OPC_READF4,
/* 0x49 */ EVT_OPC_READF_N,
/* 0x4A */ EVT_OPC_CLAMP_INT,
/* 0x4B */ EVT_OPC_SET_USER_WRK,
/* 0x4C */ EVT_OPC_SET_USER_FLG,
/* 0x4D */ EVT_OPC_ALLOC_USER_WRK,
/* 0x4E */ EVT_OPC_AND,
/* 0x4F */ EVT_OPC_ANDI,
/* 0x50 */ EVT_OPC_OR,
/* 0x51 */ EVT_OPC_ORI,
/* 0x52 */ EVT_OPC_SET_FRAME_FROM_MSEC,
/* 0x53 */ EVT_OPC_SET_MSEC_FROM_FRAME,
/* 0x54 */ EVT_OPC_SET_RAM,
/* 0x55 */ EVT_OPC_SET_RAMF,
/* 0x56 */ EVT_OPC_GET_RAM,
/* 0x57 */ EVT_OPC_GET_RAMF,
/* 0x58 */ EVT_OPC_SETR,
/* 0x59 */ EVT_OPC_SETRF,
/* 0x5A */ EVT_OPC_GETR,
/* 0x5B */ EVT_OPC_GETRF,
/* 0x5C */ EVT_OPC_USER_FUNC,
/* 0x5D */ EVT_OPC_RUN_EVT,
/* 0x5E */ EVT_OPC_RUN_EVT_ID,
/* 0x5F */ EVT_OPC_RUN_CHILD_EVT,
/* 0x60 */ EVT_OPC_DELETE_EVT,
/* 0x61 */ EVT_OPC_RESTART_EVT,
/* 0x62 */ EVT_OPC_SET_PRI,
/* 0x63 */ EVT_OPC_SET_SPD,
/* 0x64 */ EVT_OPC_SET_TYPE,
/* 0x65 */ EVT_OPC_STOP_ALL,
/* 0x66 */ EVT_OPC_START_ALL,
/* 0x67 */ EVT_OPC_STOP_OTHER,
/* 0x68 */ EVT_OPC_START_OTHER,
/* 0x69 */ EVT_OPC_STOP_ID,
/* 0x6A */ EVT_OPC_START_ID,
/* 0x6B */ EVT_OPC_CHK_EVT,
/* 0x6C */ EVT_OPC_INLINE_EVT,
/* 0x6D */ EVT_OPC_INLINE_EVT_ID,
/* 0x6E */ EVT_OPC_END_INLINE,
/* 0x6F */ EVT_OPC_BROTHER_EVT,
/* 0x70 */ EVT_OPC_BROTHER_EVT_ID,
/* 0x71 */ EVT_OPC_END_BROTHER,
/* 0x72 */ EVT_OPC_DEBUG_PUT_MSG,
/* 0x73 */ EVT_OPC_DEBUG_MSG_CLEAR,
/* 0x74 */ EVT_OPC_DEBUG_PUT_REG,
/* 0x75 */ EVT_OPC_DEBUG_NAME,
/* 0x76 */ EVT_OPC_DEBUG_REM,
/* 0x77 */ EVT_OPC_DEBUG_BP
};

// Script instruction functions
s32 evt_end_evt(EvtEntry * entry);
s32 evt_lbl(EvtEntry * entry);
s32 evt_goto(EvtEntry * entry);
s32 evt_do(EvtEntry * entry);
s32 evt_while(EvtEntry * entry);
s32 evt_do_break(EvtEntry * entry);
s32 evt_do_continue(EvtEntry * entry);
s32 evt_wait_frm(EvtEntry * entry);
s32 evt_wait_msec(EvtEntry * entry);
s32 evt_halt(EvtEntry * entry);
s32 evt_if_str_equal(EvtEntry * entry);
s32 evt_if_str_not_equal(EvtEntry * entry);
s32 evt_if_str_small(EvtEntry * entry);
s32 evt_if_str_large(EvtEntry * entry); 
s32 evt_if_str_small_equal(EvtEntry * entry);
s32 evt_if_str_large_equal(EvtEntry * entry); 
s32 evt_iff_equal(EvtEntry * entry);
s32 evt_iff_not_equal(EvtEntry * entry);
s32 evt_iff_small(EvtEntry * entry);
s32 evt_iff_large(EvtEntry * entry);
s32 evt_iff_small_equal(EvtEntry * entry);
s32 evt_iff_large_equal(EvtEntry * entry);
s32 evt_if_equal(EvtEntry * entry);
s32 evt_if_not_equal(EvtEntry * entry);
s32 evt_if_small(EvtEntry * entry);
s32 evt_if_large(EvtEntry * entry);
s32 evt_if_small_equal(EvtEntry * entry);
s32 evt_if_large_equal(EvtEntry * entry);
s32 evt_if_flag(EvtEntry * entry);
s32 evt_if_not_flag(EvtEntry * entry);
s32 evt_else(EvtEntry * entry);
s32 evt_end_if(EvtEntry * entry);
s32 evt_switch(EvtEntry * entry);
s32 evt_switchi(EvtEntry * entry);
s32 evt_case_equal(EvtEntry * entry);
s32 evt_case_not_equal(EvtEntry * entry);
s32 evt_case_small(EvtEntry * entry);
s32 evt_case_small_equal(EvtEntry * entry);
s32 evt_case_large(EvtEntry * entry);
s32 evt_case_large_equal(EvtEntry * entry);
s32 evt_case_between(EvtEntry * entry);
s32 evt_case_etc(EvtEntry * entry);
s32 evt_case_flag(EvtEntry * entry);
s32 evt_case_or(EvtEntry * entry);
s32 evt_case_and(EvtEntry * entry);
s32 evt_case_end(EvtEntry * entry);
s32 evt_switch_break(EvtEntry * entry);
s32 evt_end_switch(EvtEntry * entry);
s32 evt_set(EvtEntry * entry);
s32 evt_seti(EvtEntry * entry);
s32 evt_setf(EvtEntry * entry);
s32 evt_add(EvtEntry * entry);
s32 evt_sub(EvtEntry * entry);
s32 evt_mul(EvtEntry * entry);
s32 evt_div(EvtEntry * entry);
s32 evt_mod(EvtEntry * entry);
s32 evt_addf(EvtEntry * entry);
s32 evt_subf(EvtEntry * entry);
s32 evt_mulf(EvtEntry * entry);
s32 evt_divf(EvtEntry * entry);
s32 evt_set_read(EvtEntry * entry);
s32 evt_set_readf(EvtEntry * entry);
s32 evt_read(EvtEntry * entry);
s32 evt_read2(EvtEntry * entry);
s32 evt_read3(EvtEntry * entry);
s32 evt_read4(EvtEntry * entry);
s32 evt_read_n(EvtEntry * entry);
s32 evt_readf(EvtEntry * entry);
s32 evt_readf2(EvtEntry * entry);
s32 evt_readf3(EvtEntry * entry);
s32 evt_readf4(EvtEntry * entry);
s32 evt_readf_n(EvtEntry * entry);
s32 evt_clamp_int(EvtEntry * entry);
s32 evt_set_user_wrk(EvtEntry * entry);
s32 evt_set_user_flg(EvtEntry * entry);
s32 evt_alloc_user_wrk(EvtEntry * entry);
s32 evt_and(EvtEntry * entry);
s32 evt_andi(EvtEntry * entry);
s32 evt_or(EvtEntry * entry);
s32 evt_ori(EvtEntry * entry);
s32 evt_set_frame_from_msec(EvtEntry * entry);
s32 evt_set_msec_from_frame(EvtEntry * entry);
s32 evt_set_ram(EvtEntry * entry);
s32 evt_set_ramf(EvtEntry * entry);
s32 evt_get_ram(EvtEntry * entry);
s32 evt_get_ramf(EvtEntry * entry);
s32 evt_setr(EvtEntry * entry);
s32 evt_setrf(EvtEntry * entry);
s32 evt_getr(EvtEntry * entry);
s32 evt_getrf(EvtEntry * entry);
s32 evt_user_func(EvtEntry * entry);
s32 evt_run_evt(EvtEntry * entry);
s32 evt_run_evt_id(EvtEntry * entry);
s32 evt_run_child_evt(EvtEntry * entry);
s32 evt_restart_evt(EvtEntry * entry);
s32 evt_delete_evt(EvtEntry * entry);
s32 evt_set_pri(EvtEntry * entry);
s32 evt_set_spd(EvtEntry * entry);
s32 evt_set_type(EvtEntry * entry);
s32 evt_stop_all(EvtEntry * entry);
s32 evt_start_all(EvtEntry * entry);
s32 evt_stop_other(EvtEntry * entry);
s32 evt_start_other(EvtEntry * entry);
s32 evt_stop_id(EvtEntry * entry);
s32 evt_start_id(EvtEntry * entry);
s32 evt_chk_evt(EvtEntry * entry);
s32 evt_inline_evt(EvtEntry * entry);
s32 evt_inline_evt_id(EvtEntry * entry);
s32 evt_end_inline(EvtEntry * entry);
s32 evt_brother_evt(EvtEntry * entry);
s32 evt_brother_evt_id(EvtEntry * entry);
s32 evt_end_brother(EvtEntry * entry);
s32 evt_debug_put_msg(EvtEntry * entry);
s32 evt_debug_msg_clear(EvtEntry * entry);
s32 evt_debug_put_reg(EvtEntry * entry);
s32 evt_debug_name(EvtEntry * entry);
s32 evt_debug_rem(EvtEntry * entry);
s32 evt_debug_bp(EvtEntry * entry);

/*
    Executes an entry's script until it pauses or finishes
*/
s32 evtmgrCmd(EvtEntry * entry);

/*
    Gets the value of an evt variable
*/
s32 evtGetValue(EvtEntry * entry, EvtVar variable);

// evtGetNumber (unused)

/*
    Sets the value of an evt variable
*/
s32 evtSetValue(EvtEntry * entry, EvtVar variable, s32 value);

/*
    Gets the value of a float evt variable
*/
f32 evtGetFloat(EvtEntry * entry, EvtVar variable);

/*
    Sets the value of a float evt variable
*/
f32 evtSetFloat(EvtEntry * entry, EvtVar variable, f32 value);

// TODO: static?

/*
    Returns a pointer to the instruction after the specified label
    in an entry's script
*/
EvtScriptCode * evtSearchLabel(EvtEntry * entry, s32 id); // inlined

/*
    Returns a pointer to the instruction after the next else on the
    current if else depth in an entry's script
*/
EvtScriptCode * evtSearchElse(EvtEntry * entry);

/*
    Returns a pointer to the instruction after the next end if on the
    current if else depth in an entry's script
*/
EvtScriptCode * evtSearchEndIf(EvtEntry * entry);

/*
    Returns a pointer to the the next end switch on the current switch
    depth in an entry's script
*/
EvtScriptCode * evtSearchEndSwitch(EvtEntry * entry);

/*
    Returns a pointer to the next case or end switch on the current
    switch depth in an entry's script
*/
EvtScriptCode * evtSearchCase(EvtEntry * entry);

/*
    Returns a pointer to the instruction after the next while on the
    current do while depth in an entry's script
*/
EvtScriptCode * evtSearchWhile(EvtEntry * entry);

/*
    Returns a pointer to the next while instruction on the current do
    while depth in an entry's script
*/
EvtScriptCode * evtSearchJustBeforeWhile(EvtEntry * entry);

DECOMP_STATIC(char evt_debug_put_reg_str[256])

CPP_WRAPPER_END()
