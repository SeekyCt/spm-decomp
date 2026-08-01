#pragma once

#include <common.h>

CPP_WRAPPER(spm::mot_walk)

f32 func_80118e88(f32 speed, bool useMotionMultiplier);
f32 func_80118f40();
f32 marioGetDashSpd();
f32 func_80118fc0();
f32 func_80119000();
void func_80119040(MarioWork * mp);
void func_80119584(MarioWork * mp);

CPP_WRAPPER_END()
