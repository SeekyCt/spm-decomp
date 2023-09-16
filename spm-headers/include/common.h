#pragma once

#if (defined DECOMP) && !(defined SKIP_PPCDIS)
// Decomp should include ppcdis.h in all files
#include <ppcdis.h>
#else
// Unknown function is useful outside of decomp too
#define UNKNOWN_FUNCTION(name) void name(void)
#endif

// Intellisense doesn't like asm compiler extensions
#ifdef __INTELLISENSE__ 
    #define asm
#endif

// Helpers for compiler feature checking
#ifdef __has_builtin
    #define HAS_BUILTIN(x) __has_builtin(x)
#else
    #define HAS_BUILTIN(x) 0
#endif
#ifdef __has_attribute
    #define HAS_ATTRIBUTE(x) __has_attribute(x)
#else
    #define HAS_ATTRIBUTE(x) 0
#endif

// Basic types

// Decomp needs long for matching, int is slightly more convenient for casting in mods
#ifdef DECOMP
    #define INT_TYPE long
#else
    #define INT_TYPE int
#endif

typedef unsigned long long u64;
typedef unsigned INT_TYPE u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef signed long long s64;
typedef signed INT_TYPE s32;
typedef signed short s16;
typedef signed char s8;

#undef INT_TYPE

typedef float f32;
typedef double f64;

#ifdef USE_STL
    #include <cstddef>
    static_assert(sizeof(size_t) == 4, "Expected 32-bit size_t");
#else
    typedef u32 size_t;
    #define NULL 0
    #if HAS_BUILTIN(__builtin_offsetof)
        #define offsetof __builtin_offsetof
    #else
        #define offsetof(type, member) ((u32)&((type *)0)->member)
    #endif
#endif

typedef s32 BOOL;

#ifndef __cplusplus
    #define bool char

    #define true 1
    #define false 0
#endif

#ifndef __cplusplus
    #define wchar_t s16
#endif

#ifdef DECOMP
    typedef wchar_t wchar16_t;
#else
    typedef s16 wchar16_t;
#endif

// Unknown type
typedef u32 Unk;
typedef u32 Unk32;
typedef u16 Unk16;
typedef u8 unk8;

// Use CW special static assert
#ifdef __MWERKS__
    #define static_assert(cond, msg) __static_assert(cond, msg) 
#endif

// Macro for quick size static assert
#ifndef M2C
    #define SIZE_ASSERT(type, size) static_assert(sizeof(type) == size, "Size wrong for "#type);
    #define OFFSET_ASSERT(type, member, offset) \
        static_assert(offsetof(type, member) == offset, "Offset wrong for "#member" in "#type);
#else
    // Pycparser workaround
    #define SIZE_ASSERT(type, size)
    #define OFFSET_ASSERT(type, member, offset)
#endif

// Macro for something that should only be exposed outside of decomp
#ifdef DECOMP
    #define DECOMP_STATIC(expr)
#else
    #define DECOMP_STATIC(expr) extern expr;
#endif

// Use extern "C" in C++, use namespacing in mods
#ifdef __cplusplus
    #ifndef DECOMP
        #define CPP_WRAPPER(ns) \
            namespace ns { \
            extern "C" {
        #define CPP_WRAPPER_END() }}
    #else
        #define CPP_WRAPPER(ns) \
            extern "C" {
        #define CPP_WRAPPER_END() }
    #endif
#else
    #define CPP_WRAPPER(ns)
    #define CPP_WRAPPER_END()
#endif

// Macro for potential using statements
// Should go inside a CPP_WRAPPER
#if (defined __cplusplus) && !(defined DECOMP)
    #define USING(name) using name;
#else
    #define USING(name)
#endif

// For GCC these have to be defined in the linker script
#if (defined __MWERKS__) && !(defined M2C)
    #define FIXED_ADDR(type, name, addr) \
        type name : addr
#else
    #define FIXED_ADDR(type, name, addr) \
        extern type name
#endif

#if !(defined __INTELLISENSE__) && !(defined M2C)
    #define ATTRIBUTE(x) __attribute__((x))
#else
    #define ATTRIBUTE(x)
#endif

#if HAS_ATTRIBUTE(noreturn)
    #define NORETURN ATTRIBUTE(noreturn)
#else
    #define NORETURN
#endif

#define ALIGNED(x) ATTRIBUTE(aligned(x))

#if HAS_ATTRIBUTE(format)
    #define ATTRIBUTE_FORMAT(...) __attribute__((format(__VA_ARGS__)))
#else
    #define ATTRIBUTE_FORMAT(...)
#endif

#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))
#define QUART(x) ((x) * (x) * (x) * (x))
