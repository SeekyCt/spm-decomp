#include <common.h>
#include <spm/effdrv.h>
#include <wii/mtx.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::happy_flower)

USING(spm::effdrv::EffEntry)

// ...

EffEntry * effHappyFlower(s32 variant);

// ...

CPP_WRAPPER_END()
