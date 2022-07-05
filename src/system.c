#include <common.h>
#include <spm/mapdrv.h>
#include <spm/spmario.h>
#include <spm/system.h>
#include <wii/gx.h>
#include <wii/math.h>
#include <wii/mtx.h>
#include <wii/os.h>
#include <wii/stdio.h>
#include <wii/string.h>

#define RAND_MULT_MAGIC 0x5d588b65

// .bss
static OSMutex sysMutex;
static char * tmp0[0xc00];
static char tmp1[0x100];

// Should be removable once a referencing function is decompiled
ORDER_BSS_DATA
{
    ORDER_BSS(sysMutex)
    ORDER_BSS(tmp0)
    ORDER_BSS(tmp1)
}

// .sdata
static OSMutex * sysMutexP = &sysMutex;
static s32 randomSeed = 1;

// .sbss
static u32 token;
static u32 p;
static u32 tail;
static u32 gt;
static u32 hi;
static u32 lo;
static u32 comp;

// .rodata
static f32 angleABTBL[] = {
    1.000000, 1.273187, 1.273030, 1.272768, 1.272402, 1.271932, 1.271358, 1.270681,
    1.269902, 1.269021, 1.268038, 1.266956, 1.265774, 1.264494, 1.263116, 1.261643,
    1.260075, 1.258413, 1.256659, 1.254815, 1.252881, 1.250859, 1.248752, 1.246560,
    1.244285, 1.241929, 1.239494, 1.236981, 1.234393, 1.231731, 1.228997, 1.226192,
    1.223320, 1.220382, 1.217379, 1.214315, 1.211189, 1.208006, 1.204766, 1.201471,
    1.198124, 1.194727, 1.191281, 1.187787, 1.184250, 1.180669, 1.177047, 1.173386,
    1.169687, 1.165952, 1.162184, 1.158384, 1.154553, 1.150693, 1.146806, 1.142893,
    1.138957, 1.134998, 1.131018, 1.127019, 1.123002, 1.118969, 1.114920, 1.110858,
    1.106783, 1.102697, 1.098601, 1.094496, 1.090384, 1.086266, 1.082142, 1.078014,
    1.073883, 1.069750, 1.065616, 1.061482, 1.057348, 1.053216, 1.049087, 1.044960,
    1.040838, 1.036721, 1.032610, 1.028504, 1.024406, 1.020316, 1.016234, 1.012160,
    1.008097, 1.004043, 1.000000
};

#include "orderdoubles/803376d0_803376e8.inc"
#include "orderstrings/803376e8_803377a5.inc"

// .sdata2
#include "orderfloats/805b3c38_805b3c88.inc"

void sysInitMutex()
{
    OSInitMutex(sysMutexP);
}

const char * getSpmarioDVDRoot()
{
    return ".";
}


const char * getMapdataDvdRoot()
{
    return "./map";
}

s32 __assert(const char * filename, s32 line, const char * assertion)
{
    OSPanic(filename, line, assertion);

    return 1;
}

asm s32 __assert2(const char * filename, s32 line, const char * assertion, const char * message, ...)
{
    #include "asm/8019c54c.s"
}

float reviseAngle(float angle)
{
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

f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2)
{
    f32 xDiff;
    f32 zDiff;
    
    xDiff = x2 - x1;
    zDiff = z2 - z1;
    
    return (f32) sqrt(xDiff * xDiff + zDiff * zDiff);
}

asm f32 compAngle(f32 a, f32 b)
{
    #include "asm/8019c6bc.s"
}

asm f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2)
{
    #include "asm/8019c6f8.s"
}

asm void sincosf(f32 x, f32 * sinx, f32 * cosx)
{
    #include "asm/8019c8ac.s"
}

asm UNKNOWN_FUNCTION(func_8019c930)
{
    #include "asm/8019c930.s"
}

asm void fsort(char ** table, size_t size)
{
    #include "asm/8019c9d4.s"
}

asm void qqsort(char * list, size_t nel, size_t size, void * compare)
{
    #include "asm/8019cf84.s"
}

// TODO: irand(0x7fff) inlined
asm s32 rand()
{
    #include "asm/8019d150.s"
}

static FORCESTRIP s32 _rand(s32 max) // always inlined
{
    s32 seed;
    u32 divisor;
    u32 res;

    divisor = 0xffffffff;
    divisor /= (max + 1);
    if (divisor < 1)
        divisor = 1;

    seed = randomSeed;
    do 
    {
        seed = seed * RAND_MULT_MAGIC + 1;
        res = seed / divisor;
    }
    while (res >= max + 1);
    randomSeed = seed;
    
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

// TODO: limit * ((float) irand(0x7fff) / 32767.0f) inlined
asm float frand(f32 limit)
{
    #include "asm/8019d354.s"
}

void sysRandInit()
{
    randomSeed = (s32) OSGetTime();
}

asm void sysWaitDrawSync()
{
    #include "asm/8019d4b0.s"
}

// Seems like handwritten asm
asm s32 memcmp_as4(const void * a, const void * b, u32 n)
{
    #include "asm/8019d570.s"
}

// Seems like handwritten asm
asm void memcpy_as4(void * dest, const void * source, u32 n)
{
    #include "asm/8019d5a4.s"
}

// Functions used only in the rel currently aren't forced automatically
FORCEACTIVE_START
asm UNKNOWN_FUNCTION(func_8019d5c4)
{
    #include "asm/8019d5c4.s"
}
FORCEACTIVE_END

asm UNKNOWN_FUNCTION(func_8019d8fc)
{
    #include "asm/8019d8fc.s"
}

asm UNKNOWN_FUNCTION(func_8019dc6c)
{
    #include "asm/8019dc6c.s"
}

#include "jumptable/8042a2cc.inc"
asm f32 intplGetValue(f32 min, f32 max, void * mode, u32 progress, u32 progressMax)
{
    #include "asm/8019dc94.s"
}
