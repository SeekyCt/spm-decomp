#include <common.h>
#include <spm/mario.h>
#include <spm/mot_walk.h>

extern "C" {

#pragma dont_inline on

f32 func_80118e88(f32 speed, bool useMotionMultiplier)
{
    MarioWork * mp = marioGetPtr();

    if (mp->miscFlags & 0x4000)
        speed *= 0.75f;

    if (mp->miscFlags & MARIO_MISC_FLAG_WATER) {
        if (mp->motionId != 43)
            speed *= 0.5f;
    } else if ((mp->flags & 0x02000000) || (mp->miscFlags & 0x800)) {
        speed *= 0.5f;
    }

    if (useMotionMultiplier && mp->unknown_0x164 != 0.0f)
        speed *= mp->unknown_0x164;

    speed *= mp->unknown_0x248;
    return speed;
}

#pragma dont_inline off

f32 func_80118f40()
{
    MarioWork * mp = marioGetPtr();
    f32 speed;
    bool useMotionMultiplier;

    if (mp->miscFlags & 0x00800000)
        speed = 2.0f;
    else
        speed = mp->walkSpeed;

    useMotionMultiplier = true;
    return func_80118e88(speed, useMotionMultiplier);
}

f32 marioGetDashSpd()
{
    MarioWork * mp = marioGetPtr();
    f32 speed;
    bool useMotionMultiplier;

    if (mp->miscFlags & 0x00800000)
        speed = 4.0f;
    else
        speed = mp->dashSpeed;

    useMotionMultiplier = true;
    return func_80118e88(speed, useMotionMultiplier);
}

f32 func_80118fc0()
{
    MarioWork * mp = marioGetPtr();
    f32 speed;
    bool useMotionMultiplier;

    if (mp->miscFlags & 0x00800000)
        speed = 2.0f;
    else
        speed = mp->walkSpeed;

    useMotionMultiplier = false;
    return func_80118e88(speed, useMotionMultiplier);
}

f32 func_80119000()
{
    MarioWork * mp = marioGetPtr();
    f32 speed;
    bool useMotionMultiplier;

    if (mp->miscFlags & 0x00800000)
        speed = 4.0f;
    else
        speed = mp->dashSpeed;

    useMotionMultiplier = false;
    return func_80118e88(speed, useMotionMultiplier);
}

}

#include "src/mot_walk_main.c"
#include "src/mot_dash_main.c"
