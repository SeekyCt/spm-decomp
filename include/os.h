#ifndef OS_H
#define OS_H

#include <common.h>

#define OSClockSpeed ((*(u32 *) 0x800000F8) / 4)
#define OSTicksToMilliseconds(ticks)  ((ticks) / (OSClockSpeed / 1000))

void * OSGetMem1ArenaHi(); // 8026fac8
void * OSGetMem2ArenaHi(); // 8026fad0
void * OSGetArenaHi(); // 8026fad8
void * OSGetMem1ArenaLo(); // 8026fae0
void * OSGetMem2ArenaLo(); // 8026fae8 
void * OSGetArenaLo(); // 8026faf0
void OSSetMem1ArenaHi(void * arenaHi); // 8026faf8
void OSSetMem2ArenaHi(void * arenaHi); // 8026fb00
void OSSetArenaHi(void * arenaHi); // 8026fb08
void OSSetMem1ArenaLo(void * arenaLo); // 8026fb10
void OSSetMem2ArenaLo(void * arenaLo); // 8026fb18
void OSSetArenaLo(void * arenaLo); // 8026fb20

typedef void (RelExportFunc)();

// Unions are offsets in file then pointers after OSLink
struct _RelHeader;
typedef struct _RelHeader {
    u32 id;
    struct _RelHeader * next;
    struct _RelHeader * prev;
    u32 numSections;
    union {
        u32 sectionInfoOffset;
        void * sectionInfo;
    };
    union {
        u32 nameOffset;
        void * name;
    };
    u32 nameSize;
    u32 version;
    u32 bssSize;
    union {
        u32 relOffset;
        void * relocationTable;
    };
    union {
        u32 impOffset;
        void * impTable;
    };
    u32 impSize;
    u8 prologSection;
    u8 epilogSection;
    u8 unresolvedSection;
    u8 bssSection;
    union {
        u32 prologOffset;
        RelExportFunc * prolog;
    };
    union {
        u32 epilogOffset;
        RelExportFunc * epilog;
    };
    union {
        u32 unresolvedOffset;
        RelExportFunc * unresolved;
    };
    u32 align;
    u32 bssAlign;
    u32 fixSize;
} RelHeader;

void OSLink(RelHeader * relFile, void * bss);

#endif