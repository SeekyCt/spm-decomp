#pragma once
#include <common.h>
#include <spm/effdrv.h>

CPP_WRAPPER(spm::eff_map_block_del)

USING(spm::effdrv::EffEntry)

// ...

EffEntry * effMapBlockDelEntry(f32 x, f32 y, f32 z, f32 scale, s32 variant);
// ...

CPP_WRAPPER_END()
