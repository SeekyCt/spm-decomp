#pragma once

// Unknown function declaration
#define UNKNOWN_FUNCTION(name) void name(void)

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

// Use required static_assert keyword
#ifdef __MWERKS__
    #define static_assert __static_assert 
#elif !(defined __cplusplus)
    #define static_assert _Static_assert
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

// Macro for something that is deadstripped outside of decomp
#ifdef DECOMP
    #define STRIPPED(expr) expr;
#else
    #define STRIPPED(expr)
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
#if (defined __MWERKS__) && !(defined M2C) && !(defined __INTELLISENSE__)
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

#if HAS_ATTRIBUTE(noreturn) && (defined __cplusplus) // TODO: the usage sites are probabably what should be fixed here
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

typedef int BOOL;

#ifdef M2C
    #define wchar_t u16
    #define bool char
#endif

#include <stddef.h>
#include <stdbool.h>

// wchar_t is 32-bit in GCC but 16-bit in CW
#ifdef DECOMP
    typedef wchar_t wchar16_t;
#else
    typedef u16 wchar16_t;
#endif

// Unknown type
typedef u32 Unk;
typedef u32 Unk32;
typedef u16 Unk16;
typedef u8 unk8;
