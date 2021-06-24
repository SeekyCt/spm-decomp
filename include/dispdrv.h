#ifndef DISPDRV_H
#define DISPDRV_H

#include <common.h>

typedef void (DispCallback)(s8 cameraId, void * param);

void dispEntry(s8 cameraId, u8 renderMode, float z, DispCallback * cb, void * cbParam);


#endif
