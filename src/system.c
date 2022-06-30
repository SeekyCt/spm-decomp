#include <common.h>
#include <math.h>
#include <string.h>
#include <system.h>

#define RAND_MULT_MAGIC 0x5d588b65

static int RANDOM_SEED = 1;

static s32 _rand(s32 max) // always inlined
{
    s32 seed;
    u32 divisor;
    u32 res;

    divisor = 0xffffffff;
    divisor /= (max + 1);
    if (divisor < 1)
        divisor = 1;

    seed = RANDOM_SEED;
    do 
    {
        seed = seed * RAND_MULT_MAGIC + 1;
        res = seed / divisor;
    }
    while (res >= max + 1);
    RANDOM_SEED = seed;
    
    return (s32) res;
}

s32 irand(s32 max)
{
    // This isn't converted back to being negative on return
    s32 abs = __abs(max);

    if (abs == 0)
    {
        // 0 will always result in zero
        return 0;
    }
    else if (abs == 1)
    {
        // Special case for 1
        // 0-500 is 0, 501-1000 is 1
        return _rand(1000) > 500;
    }
    else if (abs == 100)
    {
        // Special case for 100
        // Selected from 0-1009 and divided by 10 (rounding down)
        return _rand(1009) / 10;
    }
    else
    {
        // Calculate value in requested range
        return _rand(abs);
    }
}

float reviseAngle(float angle) {
    angle = (float) fmod(angle, 360.0);

    // Seems impossible, maybe true if NaN returned?
    if (angle != angle)
        angle = 0.0f;
    
    // Convert negative angles to positive
    if (angle < 0.0f)
    {
        angle += 360.0f;

        // Seems impossible?
        if (angle >= 360.0f)
            angle = 0.0f;
    }

    return angle;
}
