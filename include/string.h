#ifndef STRING_H
#define STRING_H

#include <common.h>

void * memcpy(void * dest, void * src, size_t num);
void * memset(void * ptr, s32 value, size_t num); // 80004104
s32 strcmp(char * s1, char * s2); // 8025ea78

#endif