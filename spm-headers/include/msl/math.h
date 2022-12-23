#pragma once

#include <common.h>

CPP_WRAPPER(msl::math)

#define PI 3.141592653589793f
#define PIx2 (PI * 2.0f)

double cos(double x);
double sin(double x);
double fmod(double numer, double denom);
double sqrt(double x);

#define fabs(x) __fabs(x)
#define fabsf(x) __fabsf(x)
#define abs(x) __abs(x)

CPP_WRAPPER_END()
