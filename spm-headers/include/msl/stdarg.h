#pragma once

#include <common.h>

#ifdef DECOMP
    #error "Not implemented stdarg"
#else
    // It's safe to just use GCC's header for this, all function calls are builtins
    #include <cstdarg>
#endif
