/*
    Codewarrior's standard library string functions
*/
#pragma once

#include <common.h>

void * memcpy(void * dest, const void * src, size_t num);
void * memset(void * ptr, s32 value, size_t num);
char * strcpy(char * dest, const char * src);
void * memmove(void * dest, const void * src, size_t num);
s32 memcmp(const void * p1, const void * p2, size_t num);
s32 strcmp(const char * s1, const char * s2);
s32 strncmp(const char * s1, const char * s2, size_t num);

