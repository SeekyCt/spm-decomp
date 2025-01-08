#pragma once

#include <common.h>

CPP_WRAPPER(msl::stdarg)

// Based on open_rvl

typedef enum _va_arg_type {
    arg_ARGPOINTER,
    arg_WORD,
    arg_DOUBLEWORD,
    arg_ARGREAL
} _va_arg_type;

typedef struct __va_list_struct {
    char gpr;
    char fpr;
    char* input_arg_area;
    char* reg_save_area;
} va_list[1];

void* __va_arg(va_list, int);

#if (defined __INTELLISENSE__) || (defined __CLANGD__)
#define va_start(ap, param)
#define va_end(ap)
#define va_arg(ap, type)
#else
#define va_start(VA_LIST, ARG) ((void)ARG, __builtin_va_info(&VA_LIST))
#define va_end(VA_LIST) ((void)VA_LIST)
#define va_arg(VA_LIST, ARG_TYPE)                                              \
    (*(ARG_TYPE*)__va_arg(VA_LIST, _var_arg_typeof(ARG_TYPE)))
#endif

CPP_WRAPPER_END()
