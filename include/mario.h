#ifndef MARIO_H
#define MARIO_H

#include <common.h>

enum PlayerCharacter
{
    PLAYER_MARIO,
    PLAYER_PEACH,
    PLAYER_BOWSER,
    PLAYER_LUIGI
};

typedef struct {
    u32 flag0;
    u32 flag4;
    u32 flag8;
    u8 unknown_0xc[0x38 - 0xc];
    s8 character;
    u8 unknown_0x39[0x5c - 0x39];
    Vec3 position;
    u8 unknown_0x68[0x156c - 0x68];
} MarioWork;

MarioWork * marioGetPtr(); // 80121f48

#endif
