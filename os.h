#ifndef OS_H
#define OS_H

#define OSClockSpeed ((*(unsigned int *) 0x800000F8) / 4)
#define OSTicksToMilliseconds(ticks)  ((ticks) / (OSClockSpeed / 1000))

#endif