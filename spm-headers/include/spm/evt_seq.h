#pragma once

#include <common.h>
#include <evt_cmd.h>
#include <spm/evtmgr.h>

CPP_WRAPPER(spm::evt_seq)

// evt_seq_set_seq(s32 seq, void * p0, void * p1)
EVT_DECLARE_USER_FUNC(evt_seq_set_seq, 3)

// evt_seq_wait(s32 seq)
EVT_DECLARE_USER_FUNC(evt_seq_wait, 1)

UNKNOWN_FUNCTION(evt_seq_mapchange);

CPP_WRAPPER_END()
