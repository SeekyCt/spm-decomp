#pragma once

#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_spm_spindash)

USING(spm::effdrv::EffEntry)

// ...

EffEntry * effSpmSpindashEntry(f32, f32, f32, f32, s32);

void effSpmSpindashUpdateUnkEff(EffEntry * eff);

// ...

CPP_WRAPPER_END()
