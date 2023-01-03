#pragma once

#include <common.h>

CPP_WRAPPER(wii::os)

typedef void (RelExportFunc)();

// Unions are offsets in file then pointers after OSLink
typedef struct _RelHeader
{
    u32 id;
    struct _RelHeader * next;
    struct _RelHeader * prev;
    u32 numSections;
    union
    {
        u32 sectionInfoOffset;
        void * sectionInfo;
    };
    union
    {
        u32 nameOffset;
        void * name;
    };
    u32 nameSize;
    u32 version;
    u32 bssSize;
    union
    {
        u32 relOffset;
        void * relocationTable;
    };
    union
    {
        u32 impOffset;
        void * impTable;
    };
    u32 impSize;
    u8 prologSection;
    u8 epilogSection;
    u8 unresolvedSection;
    u8 bssSection;
    union
    {
        u32 prologOffset;
        RelExportFunc * prolog;
    };
    union
    {
        u32 epilogOffset;
        RelExportFunc * epilog;
    };
    union
    {
        u32 unresolvedOffset;
        RelExportFunc * unresolved;
    };
    u32 align;
    u32 bssAlign;
    u32 fixSize;
} RelHeader;
SIZE_ASSERT(RelHeader, 0x4c)

FIXED_ADDR(RelHeader *, firstRel, 0x800030c8);
FIXED_ADDR(RelHeader *, lastRel, 0x800030cc);

UNKNOWN_FUNCTION(OSNotifyLink);
UNKNOWN_FUNCTION(OSNotifyUnlink);
UNKNOWN_FUNCTION(OSNotifyPreLink);
UNKNOWN_FUNCTION(OSNotifyPostLink);
UNKNOWN_FUNCTION(Relocate);
UNKNOWN_FUNCTION(Link);
bool OSLink(RelHeader * relFile, void * bss);
UNKNOWN_FUNCTION(Undo);
UNKNOWN_FUNCTION(OSUnlink);

CPP_WRAPPER_END()
