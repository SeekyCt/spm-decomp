#ifndef MARIO_H
#define MARIO_H

#include <common.h>

enum {
    PLAYER_MARIO,
    PLAYER_PEACH,
    PLAYER_BOWSER,
    PLAYER_LUIGI
};

typedef struct {
    u8 unknown_0x0[0x38 - 0x0];
    s8 character;
    u8 unknown_0x39[0x156c - 0x39];
} MarioWork;

MarioWork * marioGetPtr();

#endif
