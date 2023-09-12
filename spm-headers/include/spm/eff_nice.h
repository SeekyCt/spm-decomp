#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_nice)

USING(spm::effdrv::EffEntry)

EffEntry * effNiceEntry(double param_1, double param_2, double param_3, double param_4, int param_5);
// more

CPP_WRAPPER_END()
