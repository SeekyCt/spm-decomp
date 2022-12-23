/*
    Game's custom system library
*/

#pragma once

#include <common.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::system)

USING(wii::mtx::Mtx34)

#ifdef DECOMP

#define assert(line, condition, message) \
    do \
    { \
        bool check = (condition); \
        if (!check) __assert2(__FILE__, line, #condition, message); \
    } while (0)

#define assertf(line, condition, message, ...) \
    do \
    { \
        bool check = (condition); \
        if (!check) __assert2(__FILE__, line, #condition, message, __VA_ARGS__); \
    } while (0)

#else

#define assert(condition, message) \
    if (!(condition)) spm::system::__assert2(__FILE__, __LINE__, #condition, message)

#define assertf(condition, message, ...) \
    if (!(condition)) spm::system::__assert2(__FILE__, __LINE__, #condition, message, __VA_ARGS__)

#endif


void sysInitMutex();
const char * getSpmarioDVDRoot();
const char * getMapdataDvdRoot();
s32 __assert(const char * filename, s32 line, const char * assertion);
s32 __assert2(const char * filename, s32 line, const char * assertion, const char * message, ...);
f32 reviseAngle(f32 angle);
f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
f32 compAngle(f32 a, f32 b);
f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
void sincosf(f32 x, f32 * sinx, f32 * cosx);
void movePos(f32 distance, f32 angle, f32 * x, f32 * z);
void fsort(char ** table, size_t size);
void qqsort(char * list, size_t nel, size_t size, void * compare);
s32 rand();
s32 irand(s32 limit);
f32 frand(f32 limit);
void sysRandInit();
void sysWaitDrawSync();
s32 memcmp_as4(const void * a, const void * b, u32 n);
void memcpy_as4(void * dest, const void * source, u32 n);
void mtxGetRotationElement(Mtx34 * mtx, Mtx34 * out, char axis1, char axis2);
void mtxGetScaleElement(Mtx34 * mtx, Mtx34 * out);
s32 sysMsec2Frame(s32 msec);
f32 intplGetValue(f32 min, f32 max, void * mode, u32 progress, u32 progressMax);

CPP_WRAPPER_END()
