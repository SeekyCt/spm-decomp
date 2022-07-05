#pragma once

#include <common.h>

double cos(double x);
double sin(double x);
double fmod(double numer, double denom);
double sqrt(double x);

#define fabs(x) __fabs(x)
#define fabsf(x) __fabsf(x)
#define abs(x) __abs(x)
