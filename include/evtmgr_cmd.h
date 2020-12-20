#ifndef EVTMGR_CMD_H
#define EVTMGR_CMD_H

#include "evtmgr.h"

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

#define EVT_CMD_FN(name) int evt_##name(EvtEntry * entry)

// Opcodes
enum {
  EVT_OPC_NEXT,
  EVT_OPC_ENDSCRIPT,
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

// Script instruction functions
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

/*
  0002e0ac 0000c0 8003368c  4 evt_if_flag evtmgr_cmd.o 
  0002dfec 0000c0 800335cc  4 evt_if_not_flag evtmgr_cmd.o 
  UNUSED   00006c ........ evt_else evtmgr_cmd.o 
  UNUSED   000008 ........ evt_end_if evtmgr_cmd.o 
  UNUSED   000064 ........ evt_switch evtmgr_cmd.o 
  UNUSED   00003c ........ evt_switchi evtmgr_cmd.o 
  0002de90 00015c 80033470  4 evt_case_equal evtmgr_cmd.o 
  0002dd34 00015c 80033314  4 evt_case_not_equal evtmgr_cmd.o 
  0002dbd8 00015c 800331b8  4 evt_case_small evtmgr_cmd.o 
  0002da7c 00015c 8003305c  4 evt_case_small_equal evtmgr_cmd.o 
  0002d920 00015c 80032f00  4 evt_case_large evtmgr_cmd.o 
  0002d7c4 00015c 80032da4  4 evt_case_large_equal evtmgr_cmd.o 
  0002d648 00017c 80032c28  4 evt_case_between evtmgr_cmd.o 
  UNUSED   00008c ........ evt_case_etc evtmgr_cmd.o 
  0002d51c 00012c 80032afc  4 evt_case_flag evtmgr_cmd.o 
  0002d3b4 000168 80032994  4 evt_case_or evtmgr_cmd.o 
  0002d1bc 0001f8 8003279c  4 evt_case_and evtmgr_cmd.o 
  0002d03c 000180 8003261c  4 evt_case_end evtmgr_cmd.o 
  UNUSED   000064 ........ evt_switch_break evtmgr_cmd.o 
  UNUSED   000028 ........ evt_end_switch evtmgr_cmd.o 
  UNUSED   000058 ........ evt_set evtmgr_cmd.o 
  UNUSED   000030 ........ evt_seti evtmgr_cmd.o 
  UNUSED   000050 ........ evt_setf evtmgr_cmd.o 
  UNUSED   00006c ........ evt_add evtmgr_cmd.o 
  UNUSED   00006c ........ evt_sub evtmgr_cmd.o 
  UNUSED   00006c ........ evt_mul evtmgr_cmd.o 
  UNUSED   00006c ........ evt_div evtmgr_cmd.o 
  UNUSED   0000d8 ........ evt_mod evtmgr_cmd.o 
  UNUSED   000074 ........ evt_addf evtmgr_cmd.o 
  UNUSED   000074 ........ evt_subf evtmgr_cmd.o 
  UNUSED   000074 ........ evt_mulf evtmgr_cmd.o 
  UNUSED   000074 ........ evt_divf evtmgr_cmd.o 
  UNUSED   00003c ........ evt_set_read evtmgr_cmd.o 
  UNUSED   00003c ........ evt_set_readf evtmgr_cmd.o 
  UNUSED   00003c ........ evt_read evtmgr_cmd.o 
  UNUSED   00006c ........ evt_read2 evtmgr_cmd.o 
  UNUSED   000088 ........ evt_read3 evtmgr_cmd.o 
  UNUSED   0000a4 ........ evt_read4 evtmgr_cmd.o 
  UNUSED   00005c ........ evt_read_n evtmgr_cmd.o 
  UNUSED   00003c ........ evt_readf evtmgr_cmd.o 
  UNUSED   00006c ........ evt_readf2 evtmgr_cmd.o 
  UNUSED   000088 ........ evt_readf3 evtmgr_cmd.o 
  UNUSED   0000a4 ........ evt_readf4 evtmgr_cmd.o 
  UNUSED   00007c ........ evt_readf_n evtmgr_cmd.o 
  UNUSED   00003c ........ evt_set_user_wrk evtmgr_cmd.o 
  UNUSED   00003c ........ evt_set_user_flg evtmgr_cmd.o 
  UNUSED   000060 ........ evt_alloc_user_wrk evtmgr_cmd.o 
  UNUSED   00006c ........ evt_and evtmgr_cmd.o 
  UNUSED   000064 ........ evt_andi evtmgr_cmd.o 
  UNUSED   00006c ........ evt_or evtmgr_cmd.o 
  UNUSED   000064 ........ evt_ori evtmgr_cmd.o 
  UNUSED   00006c ........ evt_set_frame_from_msec evtmgr_cmd.o 
  UNUSED   000070 ........ evt_set_msec_from_frame evtmgr_cmd.o 
  UNUSED   00003c ........ evt_set_ram evtmgr_cmd.o 
  UNUSED   00003c ........ evt_set_ramf evtmgr_cmd.o 
  UNUSED   000034 ........ evt_get_ram evtmgr_cmd.o 
  UNUSED   000034 ........ evt_get_ramf evtmgr_cmd.o 
  UNUSED   000064 ........ evt_setr evtmgr_cmd.o 
  UNUSED   00005c ........ evt_setrf evtmgr_cmd.o 
  UNUSED   000060 ........ evt_getr evtmgr_cmd.o 
  UNUSED   00005c ........ evt_getrf evtmgr_cmd.o 
  UNUSED   000094 ........ evt_user_func evtmgr_cmd.o 
  0002cf38 000104 80032518  4 evt_run_evt evtmgr_cmd.o 
  0002ce18 000120 800323f8  4 evt_run_evt_id evtmgr_cmd.o 
  UNUSED   000054 ........ evt_run_child_evt evtmgr_cmd.o 
  UNUSED   000044 ........ evt_restart_evt evtmgr_cmd.o 
  UNUSED   000030 ........ evt_delete_evt evtmgr_cmd.o 
  UNUSED   000048 ........ evt_set_pri evtmgr_cmd.o 
  UNUSED   000040 ........ evt_set_spd evtmgr_cmd.o 
  UNUSED   000048 ........ evt_set_type evtmgr_cmd.o 
  UNUSED   000030 ........ evt_stop_all evtmgr_cmd.o 
  UNUSED   000030 ........ evt_start_all evtmgr_cmd.o 
  UNUSED   000048 ........ evt_stop_other evtmgr_cmd.o 
  UNUSED   000048 ........ evt_start_other evtmgr_cmd.o 
  UNUSED   000030 ........ evt_stop_id evtmgr_cmd.o 
  UNUSED   000030 ........ evt_start_id evtmgr_cmd.o 
  UNUSED   000058 ........ evt_chk_evt evtmgr_cmd.o 
  0002ccbc 00015c 8003229c  4 evt_inline_evt evtmgr_cmd.o 
  0002cb40 00017c 80032120  4 evt_inline_evt_id evtmgr_cmd.o 
  UNUSED   000024 ........ evt_end_inline evtmgr_cmd.o 
  0002caac 000094 8003208c  4 evt_brother_evt evtmgr_cmd.o 
  0002c9fc 0000b0 80031fdc  4 evt_brother_evt_id evtmgr_cmd.o 
  UNUSED   000024 ........ evt_end_brother evtmgr_cmd.o 
*/
EVT_CMD_FN(debug_put_msg);
EVT_CMD_FN(debug_msg_clear);
EVT_CMD_FN(debug_put_reg);
EVT_CMD_FN(debug_name);
EVT_CMD_FN(debug_rem);
EVT_CMD_FN(debug_bp);

int evtmgrCmd(EvtEntry * entry);
int evtGetValue(EvtEntry * entry, int data);
// evtGetNumber
int evtSetValue(EvtEntry * entry, int, int);
float evtGetFloat(EvtEntry * entry, int data);
// evtSetFloat
int * evtSearchLabel(EvtEntry * entry);
int * evtSearchElse(EvtEntry * entry); 
// evtSearchEndIf
// evtSearchEndSwitch
// evtSearchCase
int * evtSearchWhile(EvtEntry * entry);
int * evtSearchJustBeforeWhile(EvtEntry * entry);

#endif