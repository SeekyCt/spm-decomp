/*
    Game's custom system library
*/
#ifndef SYSTEM_H
#define SYSTEM_H

#include <common.h>

#define assert(line, condition, message) \
    do { \
        bool check = (condition); \
        if (!check) __assert2(__FILE__, line, #condition, message); \
    } while (0)

#define assertf(line, condition, message, ...) \
    do { \
        bool check = (condition); \
        if (!check) __assert2(__FILE__, line, #condition, message, __VA_ARGS__); \
    } while (0)

const char * getSpmarioDVDRoot(); // 8019c508
s32 __assert2(char * filename, s32 line, char * assertion, char * message, ...); // 8019c54c
void fsort(char ** table, size_t size); // 8019c9d4
void qqsort(char * list, size_t nel, size_t size, void * compare); // 8019cf84
void sysWaitDrawSync(); // 8019d4b0

#endif