#ifndef EVT_DOOR_H
#define EVT_DOOR_H

typedef struct
{
    u32 flags;
    const char * name_l;
    const char * name_r;
    const char * hitName2d;
    const char * hitName3d;
    const char * destMapName;
    const char * unknown_0x18;
    int unknown_0x1c;
} MapDoorDesc;

typedef struct
{
    int scriptNumber;
    u16 flags;
    u16 _pad_6;
    const char * name;
    const char * mapName;
    const char * hitName2d;
    const char * hitName3d;
    const char * destMapName;
    const char * unknown_0x1c;
} DokanDesc;

#endif
