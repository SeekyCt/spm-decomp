#include <common.h>
#include <spm/mapdrv.h>
#include <spm/spmario.h>
#include <spm/system.h>
#include <wii/gx.h>
#include <wii/mtx.h>
#include <wii/os.h>
#include <msl/math.h>
#include <msl/stdio.h>
#include <msl/string.h>

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

f32 reviseAngle(f32 angle)
{
    angle = (f32) fmod(angle, 360.0);

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

f32 compAngle(f32 a, f32 b) {
    f32 diff;
    diff = __fabsf(b - a);
    if (diff >= 180.0f) {
        if (b < a) {
            b += 360.0f;
        } else {
            b -= 360.0f;
        }
    }
    return b - a;
}

asm f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2)
{
    #include "asm/8019c6f8.s"
}

asm void sincosf(f32 x, f32 * sinx, f32 * cosx)
{
    #include "asm/8019c8ac.s"
}

void movePos(f32 distance, f32 angle, f32 * x, f32 * z)
{
    f32 c;
    f32 s;
    s = (f32) sin(DEG_TO_RAD(angle));
    c = (f32) cos(DEG_TO_RAD(angle));
    *x += distance * s;
    *z -= distance * c;
}

asm void fsort(char ** table, size_t size)
{
    #include "asm/8019c9d4.s"
}

asm void qqsort(char * list, size_t nel, size_t size, void * compare)
{
    #include "asm/8019cf84.s"
}

static s32 _rand_advance()
{
    randomSeed = randomSeed * 0x5d588b65 + 1;
    return randomSeed;
}

static f32 _frand()
{
    return irand(0x7fff) / 32767.0f;
}

static s32 _rand(s32 max) // always inlined
{
    u32 divisor;
    u32 res;

    divisor = 0xffffffff;
    divisor /= (max + 1);
    if (divisor < 1)
        divisor = 1;

    do 
    {
        res = _rand_advance() / divisor;
    }
    while (res >= max + 1);
    
    return (s32) res;
}

s32 rand()
{
    return irand(0x7fff);
}

s32 irand(s32 max)
{
    // This isn't converted back to being negative on return
    max = abs(max);

    if (max == 0)
    {
        // 0 will always result in zero
        return 0;
    }
    else if (max == 1)
    {
        // Special case for 1
        // 0-500 is 0, 501-1000 is 1
        if (_rand(1000) <= 500)
            return 0;
        else
            return 1;
    }
    else if (max == 100)
    {
        // Special case for 100
        // Selected from 0-1009 and divided by 10 (rounding down)
        return _rand(1009) / 10;
    }
    else
    {
        // Calculate value in requested range
        return _rand(max);
    }
}

f32 frand(f32 limit)
{
    return limit * _frand();
}

void sysRandInit()
{
    randomSeed = (s32) OSGetTime();
}

asm void sysWaitDrawSync()
{
    #include "asm/8019d4b0.s"
}

// The use of r11/12 and placement of the return 1 pretty much guarantee inline asm
asm s32 memcmp_as4(register const void * a, register const void * b, register u32 n)
{
    // Offset a & b for lwzu
    // Divide n by 4 and move to count register
    subi a, a, 4
    srwi n, n, 2
    subi b, b, 4
    mtctr n

    // Run n/4 times
top:
    // Compare a[i] to b[i]
    lwzu r11, 4 (a)
    lwzu r12, 4 (b)
    cmplw r11, r12
    bne diff

    bdnz top

    // Return 0
    li r3, 0
    blr

    // Return 1
diff:
    li r3, 1
    blr
}

// The use of r11/12 pretty much guarantees inline asm
asm void memcpy_as4(register void * dest, register const void * source, register u32 n)
{
    // Offset dest & source for stwu/lwzu
    // Divide n by 4 and move to count register
    subi dest, dest, 4
    srwi n, n, 2
    subi source, source, 4
    mtctr n

    // Run n/4 times
top:
    // Copy source[i] to dest[i]
    lwzu r11, 4 (source)
    stwu r11, 4 (dest)

    bdnz top

    blr
}

asm void mtxGetRotationElement(Mtx34 * mtx, Mtx34 * out, char axis1, char axis2)
{
    #include "asm/8019d5c4.s"
}

asm void mtxGetScaleElement(Mtx34 * mtx, Mtx34 * out)
{
    #include "asm/8019d8fc.s"
}

s32 sysMsec2Frame(s32 msec)
{
    return (msec * gp->fps) / 1000;
}

#include "jumptable/8042a2cc.inc"
asm f32 intplGetValue(f32 min, f32 max, void * mode, u32 progress, u32 progressMax)
{
    #include "asm/8019dc94.s"
}
