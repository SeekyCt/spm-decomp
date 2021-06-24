/*
    Codewarrior's standard library string functions
*/
#ifndef STRING_H
#define STRING_H

#include <common.h>

void * memcpy(void * dest, const void * src, size_t num); // 80004000
void * memset(void * ptr, s32 value, size_t num); // 80004104
char * strcpy(char * dest, const char * src); // 8025e8fc
void * memmove(void * dest, const void * src, size_t num); // 8025a874
s32 strcmp(const char * s1, const char * s2); // 8025ea78
s32 strncmp(const char * s1, const char * s2, size_t num); // 8025eb94

#endif