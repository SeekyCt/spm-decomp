#pragma once

#include <common.h>
#include <wii/types.h>

enum PlayerCharacter
{
    PLAYER_MARIO,
    PLAYER_PEACH,
    PLAYER_BOWSER,
    PLAYER_LUIGI
};

typedef struct
{
    u32 flag0;
    u32 flag4;
    u32 flag8;
    u8 unknown_0xc[0x38 - 0xc];
    s8 character;
    u8 unknown_0x39[0x5c - 0x39];
    Vec3 position;
    u8 unknown_0x68[0x156c - 0x68];
} MarioWork;


void marioInit();
void marioMain();

MarioWork * marioGetPtr();
s32 marioTakeDamage(Vec3 * pos, u32 flags, s32 damage);
bool marioCtrlOffChk();
bool marioKeyOffChk();
bool func_8012dab0();
