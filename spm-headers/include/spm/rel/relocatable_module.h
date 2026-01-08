#pragma once

#include <common.h>

CPP_WRAPPER(spm::relocatable_module)

// Mods define functions with the same names
// TODO: some kind of prefix to allow patching the game's
#ifdef DECOMP
void _prolog();
// more
#endif

CPP_WRAPPER_END()
