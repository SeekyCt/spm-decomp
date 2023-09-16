/*
    Game's custom system library
*/

#pragma once

#include <common.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::system)

USING(wii::mtx::Mtx34)

enum IntplMode
{
    INTPL_MODE_LINEAR = 0,
    INTPL_MODE_QUADRATIC_IN = 1,
    INTPL_MODE_CUBIC_IN = 2,
    INTPL_MODE_QUARTIC_IN = 3,
    INTPL_MODE_QUADRATIC_OUT = 4,
    INTPL_MODE_CUBIC_OUT = 5,
    INTPL_MODE_QUARTIC_OUT = 6,
    INTPL_MODE_COS_SLOW_OVERSHOOT = 7,
    INTPL_MODE_COS_FAST_OVERSHOOT = 8,
    INTPL_MODE_9 = 9,
    INTPL_MODE_COS_BOUNCE = 10,

    INTPL_MODE_COS_IN_OUT,
    INTPL_MODE_SIN_OUT,
    INTPL_MODE_COS_IN
};

typedef f32 (IntplUserFunc)(s32, s32, f32, f32);

#ifdef DECOMP

// TODO: figure out what's going on with the assert if statement in decomp
// assert in evtmgr.c line 161 is incompatible with (condition) == false
// assertf dan.c line 144 is incompatible with !(condition)
// assertf dvdmgr.c line 51 is incompatible with bool check = (condition); !check

#define assert(line, condition, message) \
    do \
    { \
        bool check = (condition); \
        if (!check) __assert2(__FILE__, line, #condition, message); \
    } while (0)

#define assertf(line, condition, message, ...) \
    do \
    { \
        if ((condition) == false) __assert2(__FILE__, line, #condition, message, __VA_ARGS__); \
    } while (0)

#else

#define assert(condition, message) \
    if (!(condition)) spm::system::__assert2(__FILE__, __LINE__, #condition, message)

#define assertf(condition, message, ...) \
    if (!(condition)) spm::system::__assert2(__FILE__, __LINE__, #condition, message, __VA_ARGS__)

#endif

#define RAND_MAX 0x7fff


/*
    Initialises an unused mutex
*/
void sysInitMutex();

/*
    Gets the root path for the DVD filesystem
*/
const char * getSpmarioDVDRoot();

/*
    Gets the root path for the mapdata folder in the DVD filesystem
*/
const char * getMapdataDvdRoot();

/*
    Assertion failure handlers
*/
s32 NORETURN __assert(const char * filename, s32 line, const char * assertion);
s32 NORETURN ATTRIBUTE_FORMAT(printf, 4, 5) __assert2(
    const char * filename, s32 line, const char * assertion, const char * message, ...);

/*
    Rounds a float to an int
    Deadstripped, always inlined
*/
s32 roundi(f32 x);

/*
    Adjusts an angle to be 0 <= x < 360
*/
f32 reviseAngle(f32 angle);

/*
    Gets the straight line distance between two 2D points
*/
f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);

/*
    Calculates b - a in the range -180 <= x < 180 for 0 <= a, b < 360
*/
f32 compAngle(f32 a, f32 b);

/*
    Gets the angle between two 2D points (atan2)
*/
f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);

/*
    Gets the sine and cosine of an angle
*/
void sincosf(f32 x, f32 * sinx, f32 * cosx);

/*
    Translates a point in along a straight line at an angle
*/
void movePos(f32 distance, f32 angle, f32 * x, f32 * z);

DECOMP_STATIC(void fsort(char ** table, size_t size))
void qqsort(char * list, size_t nel, size_t size, void * compare);

/*
    Gets a random number 0 <= x <= RAND_MAX
*/
s32 rand();

/*
    Gets a random number 0 <= x <= limit
*/
s32 irand(s32 limit);
f32 frand(f32 limit);

/*
    Initialises the RNG seed
*/
void sysRandInit();

/*
    Gets the current screen draw token
    Deadstripped, always inlined
*/
u16 sysGetToken();

/*
    Waits until the next screen draw, or max 100ms
*/
void sysWaitDrawSync();

/*
    memcmp/memcpy, done word-by-word rather than byte-by-byte
*/
s32 memcmp_as4(const void * a, const void * b, u32 n);
void memcpy_as4(void * dest, const void * source, u32 n);

/*
    Extracts the rotation of a matrix
*/
void mtxGetRotationElement(Mtx34 mtx, Mtx34 out, char axis1, char axis2);

/*
    Extracts the scale factor of a matrix
*/
void mtxGetScaleElement(Mtx34 mtx, Mtx34 out);

/*
    Converts a millisecond count to frame count
*/
s32 sysMsec2Frame(s32 msec);

/*
    Interpolates a value, see IntplMode above
    Mode can also be an IntplUserFunc pointer
*/
f32 intplGetValue(s32 mode, f32 min, f32 max, s32 progress, s32 progressMax);

CPP_WRAPPER_END()
