#ifndef OS_H
#define OS_H

#include <common.h>

#define OSClockSpeed ((*(u32 *) 0x800000F8) / 4)
#define OSTicksToMilliseconds(ticks)  ((ticks) / (OSClockSpeed / 1000))

void * OSGetMem1ArenaHi(); // 8026fac8
void * OSGetMem2ArenaHi(); // 8026fad0
void * OSGetArenaHi(); // 8026fad8
void * OSGetMem1ArenaLo(); // 8026fae0
void * OSGetMem2ArenaLo(); // 8026fae8 
void * OSGetArenaLo(); // 8026faf0
void OSSetMem1ArenaHi(void * arenaHi); // 8026faf8
void OSSetMem2ArenaHi(void * arenaHi); // 8026fb00
void OSSetArenaHi(void * arenaHi); // 8026fb08
void OSSetMem1ArenaLo(void * arenaLo); // 8026fb10
void OSSetMem2ArenaLo(void * arenaLo); // 8026fb18
void OSSetArenaLo(void * arenaLo); // 8026fb20

#endif