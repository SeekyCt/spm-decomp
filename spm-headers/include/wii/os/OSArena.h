#pragma once

#include <common.h>

CPP_WRAPPER(wii::os)

void * OSGetMEM1ArenaHi();
void * OSGetMEM2ArenaHi();
void * OSGetArenaHi();
void * OSGetMEM1ArenaLo();
void * OSGetMEM2ArenaLo();
void * OSGetArenaLo();
void OSSetMEM1ArenaHi(void * arenaHi);
void OSSetMEM2ArenaHi(void * arenaHi);
void OSSetArenaHi(void * arenaHi);
void OSSetMEM1ArenaLo(void * arenaLo);
void OSSetMEM2ArenaLo(void * arenaLo);
void OSSetArenaLo(void * arenaLo);
UNKNOWN_FUNCTION(OSAllocFromMEM1ArenaLo);

CPP_WRAPPER_END()
