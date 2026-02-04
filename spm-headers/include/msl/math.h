#pragma once

#include <common.h>

CPP_WRAPPER(msl::math)

typedef float float_t;
typedef double double_t;

#define PI 3.141592653589793f
#define PIx2 (PI * 2.0f)

double cos(double x);
#define cosf(x) ((f32)cos(x))

double sin(double x);
#define sinf(x) ((f32)sin(x))

double fmod(double numer, double denom);
#define fmodf(numer, denom) ((f32)fmod(numer, denom))

double sqrt(double x);
#define sqrtf(x) ((f32)sqrt(x))

double pow(double base, double exponent);

double floor(double x);

#ifdef __MWERKS__
    #define fabs(x) __fabs(x)
    #define fabsf(x) __fabsf(x)
    #define abs(x) __abs(x)
#elif defined __GNU_C__
    #define fabs(x) __builtin_fabs(x)
    #define fabsf(x) __builtin_fabsf(x)
    #define abs(x) __builtin_abs(x)
#else
    double fabs(double x);
    float fabsf(float x);
    int abs(int x);
#endif

double ceil(double x);
#define ceilf(x) ((f32)ceil(x))

CPP_WRAPPER_END()
