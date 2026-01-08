#include <common.h>
#include <spm/effdrv.h>
#include <wii/mtx.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::eff_fire)

USING(spm::effdrv::EffEntry)
USING(wii::mtx::Vec3)
USING(wii::gx::GXColor)

// ...

typedef struct
{
/* 0x00 */ s32 type;
/* 0x04 */ Vec3 pos;
/* 0x10 */ GXColor color; // idk man it's probably an rgba field
/* 0x14 */ s32 duration;
/* 0x18 */ u8 unknown_0x18[0x50 - 0x18];
} EffFire;
SIZE_ASSERT(EffFire, 0x50)

EffEntry * effFireEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 duration);

// ...

CPP_WRAPPER_END()
