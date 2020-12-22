/*
  Functions to execute interpreted evt script code and handle evt variables
*/

#ifndef EVTMGR_CMD_H
#define EVTMGR_CMD_H

#include <common.h>
#include <evtmgr.h>

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

// Return values
#define EVT_END 0xff
#define EVT_CONTINUE 2

// Opcodes
enum {
  EVT_OPC_NEXT,
  EVT_OPC_END_SCRIPT,
  EVT_OPC_END_EVT,
  EVT_OPC_LBL,
  EVT_OPC_GOTO,
  EVT_OPC_DO,
  EVT_OPC_WHILE,
  EVT_OPC_DO_BREAK,
  EVT_OPC_DO_CONTINUE,
  EVT_OPC_WAIT_FRM,
  EVT_OPC_WAIT_MSEC,
  EVT_OPC_HALT,
  EVT_OPC_IF_STR_EQUAL,
  EVT_OPC_IF_STR_NOT_EQUAL,
  EVT_OPC_IF_STR_SMALL,
  EVT_OPC_IF_STR_LARGE,
  EVT_OPC_IF_STR_SMALL_EQUAL,
  EVT_OPC_IF_STR_LARGE_EQUAL,
  EVT_OPC_IFF_EQUAL,
  EVT_OPC_IFF_NOT_EQUAL,
  EVT_OPC_IFF_SMALL,
  EVT_OPC_IFF_LARGE,
  EVT_OPC_IFF_SMALL_EQUAL,
  EVT_OPC_IFF_LARGE_EQUAL,
  EVT_OPC_IF_EQUAL,
  EVT_OPC_IF_NOT_EQUAL,
  EVT_OPC_IF_SMALL,
  EVT_OPC_IF_LARGE,
  EVT_OPC_IF_SMALL_EQUAL,
  EVT_OPC_IF_LARGE_EQUAL,
  EVT_OPC_IF_FLAG,
  EVT_OPC_IF_NOT_FLAG,
  EVT_OPC_ELSE,
  EVT_OPC_END_IF,
  EVT_OPC_SWITCH,
  EVT_OPC_SWITCHI,
  EVT_OPC_CASE_EQUAL,
  EVT_OPC_CASE_NOT_EQUAL,
  EVT_OPC_CASE_SMALL,
  EVT_OPC_CASE_LARGE,
  EVT_OPC_CASE_SMALL_EQUAL,
  EVT_OPC_CASE_LARGE_EQUAL,
  EVT_OPC_CASE_ETC,
  EVT_OPC_CASE_OR,
  EVT_OPC_CASE_AND,
  EVT_OPC_CASE_FLAG,
  EVT_OPC_CASE_END,
  EVT_OPC_CASE_BETWEEN,
  EVT_OPC_SWITCH_BREAK,
  EVT_OPC_END_SWITCH,
  EVT_OPC_SET,
  EVT_OPC_SETI,
  EVT_OPC_SETF,
  EVT_OPC_ADD,
  EVT_OPC_SUB,
  EVT_OPC_MUL,
  EVT_OPC_DIV,
  EVT_OPC_MOD,
  EVT_OPC_ADDF,
  EVT_OPC_SUBF,
  EVT_OPC_MULF,
  EVT_OPC_DIVF,
  EVT_OPC_SET_READ,
  EVT_OPC_READ,
  EVT_OPC_READ2,
  EVT_OPC_READ3,
  EVT_OPC_READ4,
  EVT_OPC_READ_N,
  EVT_OPC_SET_READF,
  EVT_OPC_READF,
  EVT_OPC_READF2,
  EVT_OPC_READF3,
  EVT_OPC_READF4,
  EVT_OPC_READF_N,
  EVT_OPC_CLAMP_INT,
  EVT_OPC_SET_USER_WRK,
  EVT_OPC_SET_USER_FLG,
  EVT_OPC_ALLOC_USER_WRK,
  EVT_OPC_AND,
  EVT_OPC_ANDI,
  EVT_OPC_OR,
  EVT_OPC_ORI,
  EVT_OPC_SET_FRAME_FROM_MSEC,
  EVT_OPC_SET_MSEC_FROM_FRAME,
  EVT_OPC_SET_RAM,
  EVT_OPC_SET_RAMF,
  EVT_OPC_GET_RAM,
  EVT_OPC_GET_RAMF,
  EVT_OPC_SETR,
  EVT_OPC_SETRF,
  EVT_OPC_GETR,
  EVT_OPC_GETRF,
  EVT_OPC_USER_FUNC,
  EVT_OPC_RUN_EVT,
  EVT_OPC_RUN_EVT_ID,
  EVT_OPC_RUN_CHILD_EVT,
  EVT_OPC_DELETE_EVT,
  EVT_OPC_RESTART_EVT,
  EVT_OPC_SET_PRI,
  EVT_OPC_SET_SPD,
  EVT_OPC_SET_TYPE,
  EVT_OPC_STOP_ALL,
  EVT_OPC_START_ALL,
  EVT_OPC_STOP_OTHER,
  EVT_OPC_START_OTHER,
  EVT_OPC_STOP_ID,
  EVT_OPC_START_ID,
  EVT_OPC_CHK_EVT,
  EVT_OPC_INLINE_EVT,
  EVT_OPC_INLINE_EVT_ID,
  EVT_OPC_END_INLINE,
  EVT_OPC_BROTHER_EVT,
  EVT_OPC_BROTHER_EVT_ID,
  EVT_OPC_END_BROTHER,
  EVT_OPC_DEBUG_PUT_MSG,
  EVT_OPC_DEBUG_MSG_CLEAR,
  EVT_OPC_DEBUG_PUT_REG,
  EVT_OPC_DEBUG_NAME,
  EVT_OPC_DEBUG_REM,
  EVT_OPC_DEBUG_BP
};

// Script instruction functions, 800da0b0 - 800ddd5b
#define EVT_CMD_FN(name) s32 evt_##name(EvtEntry * entry)
EVT_CMD_FN(end_evt);
EVT_CMD_FN(lbl);
EVT_CMD_FN(goto);
EVT_CMD_FN(do);
EVT_CMD_FN(while);
EVT_CMD_FN(do_break);
EVT_CMD_FN(do_continue);
EVT_CMD_FN(wait_frm);
EVT_CMD_FN(wait_msec);
EVT_CMD_FN(halt);
EVT_CMD_FN(if_str_equal);
EVT_CMD_FN(if_str_not_equal);
EVT_CMD_FN(if_str_small);
EVT_CMD_FN(if_str_large); 
EVT_CMD_FN(if_str_small_equal);
EVT_CMD_FN(if_str_large_equal); 
EVT_CMD_FN(iff_equal);
EVT_CMD_FN(iff_not_equal);
EVT_CMD_FN(iff_small);
EVT_CMD_FN(iff_large);
EVT_CMD_FN(iff_small_equal);
EVT_CMD_FN(iff_large_equal);
EVT_CMD_FN(if_equal);
EVT_CMD_FN(if_not_equal);
EVT_CMD_FN(_if_small);
EVT_CMD_FN(if_large);
EVT_CMD_FN(if_small_equal);
EVT_CMD_FN(if_large_equal);
EVT_CMD_FN(if_flag);
EVT_CMD_FN(if_not_flag);
EVT_CMD_FN(else);
EVT_CMD_FN(end_if);
// EVT_CMD_FN(switch)
// EVT_CMD_FN(switchi)
// EVT_CMD_FN(case_equal)
// EVT_CMD_FN(case_not_equal)
// EVT_CMD_FN(case_small)
// EVT_CMD_FN(case_small_equal)
// EVT_CMD_FN(case_large)
// EVT_CMD_FN(case_large_equal)
// EVT_CMD_FN(case_between)
// EVT_CMD_FN(case_etc)
// EVT_CMD_FN(case_flag)
// EVT_CMD_FN(case_or)
// EVT_CMD_FN(case_and)
// EVT_CMD_FN(case_end)
// EVT_CMD_FN(switch_break)
// EVT_CMD_FN(end_switch)
// EVT_CMD_FN(set)
// EVT_CMD_FN(seti)
// EVT_CMD_FN(setf)
// EVT_CMD_FN(add)
// EVT_CMD_FN(sub)
// EVT_CMD_FN(mul)
// EVT_CMD_FN(div)
// EVT_CMD_FN(mod)
// EVT_CMD_FN(addf)
// EVT_CMD_FN(subf)
// EVT_CMD_FN(mulf)
// EVT_CMD_FN(divf)
// EVT_CMD_FN(set_read)
// EVT_CMD_FN(set_readf)
// EVT_CMD_FN(read)
// EVT_CMD_FN(read2)
// EVT_CMD_FN(read3)
// EVT_CMD_FN(read4)
// EVT_CMD_FN(read_n)
// EVT_CMD_FN(readf)
// EVT_CMD_FN(readf2)
// EVT_CMD_FN(readf3)
// EVT_CMD_FN(readf4)
// EVT_CMD_FN(readf_n)
// EVT_CMD_FN(set_user_wrk)
// EVT_CMD_FN(set_user_flg)
// EVT_CMD_FN(alloc_user_wrk)
// EVT_CMD_FN(and)
// EVT_CMD_FN(andi)
// EVT_CMD_FN(or)
// EVT_CMD_FN(ori)
// EVT_CMD_FN(set_frame_from_msec)
// EVT_CMD_FN(set_msec_from_frame)
// EVT_CMD_FN(set_ram)
// EVT_CMD_FN(set_ramf)
// EVT_CMD_FN(get_ram)
// EVT_CMD_FN(get_ramf)
// EVT_CMD_FN(setr)
// EVT_CMD_FN(setrf)
// EVT_CMD_FN(getr)
// EVT_CMD_FN(getrf)
// EVT_CMD_FN(user_func)
// EVT_CMD_FN(run_evt)
// EVT_CMD_FN(run_evt_id)
// EVT_CMD_FN(run_child_evt)
// EVT_CMD_FN(restart_evt)
// EVT_CMD_FN(delete_evt)
// EVT_CMD_FN(set_pri)
// EVT_CMD_FN(set_spd)
// EVT_CMD_FN(set_type)
// EVT_CMD_FN(stop_all)
// EVT_CMD_FN(start_all)
// EVT_CMD_FN(stop_other)
// EVT_CMD_FN(start_other)
// EVT_CMD_FN(stop_id)
// EVT_CMD_FN(start_id)
// EVT_CMD_FN(chk_evt)
// EVT_CMD_FN(inline_evt)
// EVT_CMD_FN(inline_evt_id)
// EVT_CMD_FN(end_inline)
// EVT_CMD_FN(brother_evt)
// EVT_CMD_FN(brother_evt_id)
// EVT_CMD_FN(end_brother)
EVT_CMD_FN(debug_put_msg);
EVT_CMD_FN(debug_msg_clear);
EVT_CMD_FN(debug_put_reg);
EVT_CMD_FN(debug_name);
EVT_CMD_FN(debug_rem);
EVT_CMD_FN(debug_bp);

/*
  Executes an entry's script until it pauses or finishes
*/
s32 evtmgrCmd(EvtEntry * entry); // 800ddd5c

/*
  Gets the value of an evt variable
*/
s32 evtGetValue(EvtEntry * entry, s32 data); // 800de594

// evtGetNumber (inlined / unused)

/*
  Sets the value of an evt variable
*/
s32 evtSetValue(EvtEntry * entry, s32, s32); // 800de9b8

/*
  Gets the value of a float evt variable
*/
f32 evtGetFloat(EvtEntry * entry, s32 data); // 800dedb8

/*
  Sets the value of a float evt variable
*/
f32 evtSetFloat(EvtEntry * entry, s32, f32); // 800df1fc

// TODO: search functions probably static?

/*
  Returns a pointer to the instruction after the specified label
  in an entry's script
*/
EvtScriptCode * evtSearchLabel(EvtEntry * entry, int id); // inlined

/*
  Returns a pointer to the instruction after the next else on the
  current if else depth in an entry's script
*/
EvtScriptCode * evtSearchElse(EvtEntry * entry); // 800df55c

/*
  Returns a pointer to the instruction after the next end if on the
  current if else depth in an entry's script
*/
EvtScriptCode * evtSearchEndIf(EvtEntry * entry); // 800df620

/*
  Returns a pointer to the the next end switch on the current switch
  depth in an entry's script
*/
EvtScriptCode * evtSearchEndSwitch(EvtEntry * entry); // 800df6d8

/*
  Returns a pointer to the next case or end switch on the current
  switch depth in an entry's script
*/
EvtScriptCode * evtSearchCase(EvtEntry * entry); // 800df780

/*
  Returns a pointer to the instruction after the next while on the
  current do while depth in an entry's script
*/
EvtScriptCode * evtSearchWhile(EvtEntry * entry); // 800df84c

/*
  Returns a pointer to the next while instruction on the current do
  while depth in an entry's script
*/
EvtScriptCode * evtSearchJustBeforeWhile(EvtEntry * entry); // 800df8f8

#endif