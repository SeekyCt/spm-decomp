#pragma once

#include <common.h>
#include <wii/types.h>

#define URA_DAN_KEY 44
#define DAN_KEY 48

typedef struct
{
    u32 flags;
    u8 unknown_0x4[0x10 - 0x4];
    s16 type;
    s16 behaviour;
    Vec3 position;
    u8 unknown_0x20[0x88 - 0x20];
} ItemEntry;

typedef struct
{
    s32 num;
    ItemEntry * entries;
    u8 unknown_0x8[0x80 - 0x8];
} ItemWork;

ItemWork * itemGetWork();
void itemInit();
void itemMain();
ItemEntry * itemEntry(float x, float y, float z, const char * instanceName, s32 itemId,
                      s32 behaviour, void * pickupScript, s32 switchNumber);
s32 itemTypeNameToId(const char * itemName);
bool itemCheckForId(s32 itemId);
