#pragma once

#include <common.h>

CPP_WRAPPER(wii::os)

void __OSReboot(Unk param_1, Unk param_2);
UNKNOWN_FUNCTION(OSGetSaveRegion);

CPP_WRAPPER_END()
