#ifndef STRING_H
#define STRING_H

#include <stddef.h>

void * memcpy(void * dest, void * src, size_t num);
void * memset(void * ptr, int value, size_t num); // 80004104
int strcmp(char * s1, char * s2); // 8025ea78

#endif