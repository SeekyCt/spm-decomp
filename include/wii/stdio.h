/*
    Codewarrior's standard library stdio functions
*/
#pragma once

#include <common.h>

s32 sprintf(char *str, const char *format, ...);
s32 vsprintf(char * str, const char * format, void * arg);
