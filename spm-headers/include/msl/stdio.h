/*
    Codewarrior's standard library stdio functions
*/
#pragma once

#include <common.h>

CPP_WRAPPER(msl::stdio)

// TODO: va_list
// int vsprintf(char * s, const char * format, va_list arg );
UNKNOWN_FUNCTION(vsprintf);

int snprintf(char * dest, size_t n, const char * format, ...);
int sprintf(char * dest, const char * format, ...);
int sscanf(const char * str, const char * format, ...);

CPP_WRAPPER_END()
