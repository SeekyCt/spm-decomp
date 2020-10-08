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

// Script instruction functions
EVT_CMD_FN(end_evt);
EVT_CMD_FN(lbl);
EVT_CMD_FN(goto);
EVT_CMD_FN(do);
// EVT_CMD_FN(while);
EVT_CMD_FN(do_break);
EVT_CMD_FN(do_continue);
EVT_CMD_FN(wait_frm);

/*
  0002f0d4 0000d0 800346b4  4 evt_wait_msec evtmgr_cmd.o 
  UNUSED   00003c ........ evt_halt evtmgr_cmd.o 
  0002efe4 0000f0 800345c4  4 evt_if_str_equal evtmgr_cmd.o 
  0002eef4 0000f0 800344d4  4 evt_if_str_not_equal evtmgr_cmd.o 
  0002ee04 0000f0 800343e4  4 evt_if_str_small evtmgr_cmd.o 
  0002ed14 0000f0 800342f4  4 evt_if_str_large evtmgr_cmd.o 
  0002ec24 0000f0 80034204  4 evt_if_str_small_equal evtmgr_cmd.o 
  0002eb34 0000f0 80034114  4 evt_if_str_large_equal evtmgr_cmd.o 
  0002ea60 0000d4 80034040  4 evt_iff_equal evtmgr_cmd.o 
  0002e98c 0000d4 80033f6c  4 evt_iff_not_equal evtmgr_cmd.o 
  0002e8b4 0000d8 80033e94  4 evt_iff_small evtmgr_cmd.o 
  0002e7dc 0000d8 80033dbc  4 evt_iff_large evtmgr_cmd.o 
  0002e708 0000d4 80033ce8  4 evt_iff_small_equal evtmgr_cmd.o 
  0002e634 0000d4 80033c14  4 evt_iff_large_equal evtmgr_cmd.o 
  0002e568 0000cc 80033b48  4 evt_if_equal evtmgr_cmd.o 
  0002e49c 0000cc 80033a7c  4 evt_if_not_equal evtmgr_cmd.o 
  0002e3d0 0000cc 800339b0  4 evt_if_small evtmgr_cmd.o 
  0002e304 0000cc 800338e4  4 evt_if_large evtmgr_cmd.o 
  0002e238 0000cc 80033818  4 evt_if_small_equal evtmgr_cmd.o 
  0002e16c 0000cc 8003374c  4 evt_if_large_equal evtmgr_cmd.o 
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
  UNUSED   000008 ........ evt_debug_put_msg evtmgr_cmd.o 
  UNUSED   000008 ........ evt_debug_msg_clear evtmgr_cmd.o 
*/

EVT_CMD_FN(debug_put_reg);

/*
  UNUSED   000014 ........ evt_debug_name evtmgr_cmd.o 
  UNUSED   000008 ........ evt_debug_rem evtmgr_cmd.o 
  UNUSED   000054 ........ evt_debug_bp evtmgr_cmd.o 
  0002af10 0013b0 800304f0  4 evtmgrCmd evtmgr_cmd.o 
*/

int evtGetValue(EvtEntry * entry, int data);

/*
  missed   000164 8002ff34  4 evtGetNumber evtmgr_cmd.o 
*/

int evtSetValue(EvtEntry * entry, int, int);
float evtGetFloat(EvtEntry * entry, int data);

/*
  00029d14 000384 8002f2f4  4 evtSetFloat evtmgr_cmd.o 
  UNUSED   00005c ........ evtSearchLabel evtmgr_cmd.o 
  UNUSED   00006c ........ evtSearchElse evtmgr_cmd.o 
  UNUSED   000064 ........ evtSearchEndIf evtmgr_cmd.o 
  UNUSED   00005c ........ evtSearchEndSwitch evtmgr_cmd.o 
  UNUSED   000080 ........ evtSearchCase evtmgr_cmd.o 
*/

int * evtSearchWhile(EvtEntry * entry);
int * evtSearchJustBeforeWhile(EvtEntry * entry);

#endif