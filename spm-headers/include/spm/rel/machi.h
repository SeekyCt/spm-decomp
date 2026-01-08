#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::machi)

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x4 - 0x0];
/* 0x04 */ const char * name;
/* 0x08 */ u8 unknown_0x8[0x24 - 0x8];
/* 0x24 */ const char * destMapName;
/* 0x28 */ const char * unkChar;
} ElvDesc;
SIZE_ASSERT(ElvDesc, 0x2c)

// ...

// evt_machi_set_elv_descs(ElvDesc * descs, s32 count)
EVT_DECLARE_USER_FUNC(evt_machi_set_elv_descs, 2)

// ...

EVT_UNKNOWN_USER_FUNC(func_80c4d444)

// ...

EVT_DECLARE(mac_06_pure_heart_return_evt)

CPP_WRAPPER_END()
