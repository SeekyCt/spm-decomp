#ifndef STRING_H
#define STRING_H

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

void * memset(void * ptr, int value, size_t num); // 80004104

#endif