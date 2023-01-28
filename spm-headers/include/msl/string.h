/*
    Codewarrior's standard library string functions
*/
#pragma once

#include <common.h>

CPP_WRAPPER(msl::string)

void * memcpy(void * dest, const void * src, size_t num);
void * memset(void * ptr, int value, size_t num);
char * strcpy(char * dest, const char * src);
char * strncpy(char * dest, const char * src, size_t n);
char * strcat(char * dest, const char * src);
void * memmove(void * dest, const void * src, size_t num);
int memcmp(const void * p1, const void * p2, size_t num);
int strcmp(const char * s1, const char * s2);
int strncmp(const char * s1, const char * s2, size_t num);
const char * strchr(const char * str, char c);
size_t strlen(const char * str);
const char * strstr(const char * str1, const char * str2);

CPP_WRAPPER_END()
