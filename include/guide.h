#ifndef GUIDE_H
#define GUIDE_H

typedef struct
{
    u16 flag0;
    u8 flag2;
    u8 unknown_0x3[0xb4 - 0x3];
} GuideWork;

GuideWork * guideGetWork(); // 8011fa2c

#endif
