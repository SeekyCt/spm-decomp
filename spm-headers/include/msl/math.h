#pragma once

#include <common.h>

CPP_WRAPPER(msl::math)

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

#define fabs(x) __fabs(x)
#define fabsf(x) __fabsf(x)
#define abs(x) __abs(x)

CPP_WRAPPER_END()
