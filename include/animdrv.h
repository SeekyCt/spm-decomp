#ifndef ANIMDRV_H
#define ANIMDRV_H

typedef void (AnimPoseDisplayCb)(void * param, int animGroupIdx, int param_3);

void animPoseSetDispCallback(int animPoseId, AnimPoseDisplayCb * cb, void * param);

#endif
