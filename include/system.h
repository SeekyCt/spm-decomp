#ifndef SYSTEM_H
#define SYSTEM_H

#include <common.h>

#define assert(condition, message) \
    if (!(condition)) __assert2(__FILE__, __LINE__, #condition, message)

#define assertf(condition, message, ...) \
    if (!(condition)) __assert2(__FILE__, __LINE__, #condition, message, __VA_ARGS__)

s32 __assert2(char * filename, s32 line, char * assertion, char * message, ...); // 8019c54c
void fsort(char ** table, size_t size); // 8019c9d4
void qqsort(char * list, size_t nel, size_t size, void * compare); // 8019cf84
void sysWaitDrawSync(); // 8019d4b0

#endif