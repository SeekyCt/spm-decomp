#ifndef HUD_H
#define HUD_H

#include <common.h>

typedef void (CountdownDoneCb)();

void hudStartCountdown(u32 length, CountdownDoneCb * cb);

#endif
