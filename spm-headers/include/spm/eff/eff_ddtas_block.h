#include <common.h>
#include <spm/effdrv.h>
#include <wii/mtx.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::eff_ddtas_block)

USING(spm::effdrv::EffEntry)

// ...

EffEntry * effDdtasBlockEntry(f32 x, f32 y, f32 z, f32, f32, f32, f32 scaleUnused, s32 variant);

// ...

CPP_WRAPPER_END()
