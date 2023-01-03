#pragma once

#include <common.h>

// TODO: implement for decomp
// TODO: does cstdarg even work anymore
#ifndef DECOMP
    // It's safe to just use GCC's header for this, all function calls are builtins
    #include <cstdarg>
#endif
