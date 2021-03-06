#ifndef ITEMDRV_H
#define ITEMDRV_H

#include <common.h>

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
    int num;
    ItemEntry * entries;
    u8 unknown_0x8[0x80 - 0x8];
} ItemWork;

ItemWork * itemGetWork(); // 80077660
ItemEntry * itemEntry(float x, float y, float z, const char * instanceName, int itemId,
                      int behaviour, void * pickupScript, int switchNumber); // 80078b3c
int itemTypeNameToId(const char * itemName); // 8007bccc
bool itemCheckForId(int itemId); // 8007d8b4

#endif
