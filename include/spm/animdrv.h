#pragma once

#include <common.h>

typedef struct {
    u8 unknown_0x0[0x110 - 0x0];
} AnimWork; // Uncertain size;

AnimWork * animGetPtr();

void animInit();
void animMain();

typedef void (AnimPoseDisplayCb)(void * param, s32 animGroupIdx, s32 param_3);
void animPoseSetDispCallback(s32 animPoseId, AnimPoseDisplayCb * cb, void * param);

s32 animPoseGetGroupIdx(s32 id, const char * name);
