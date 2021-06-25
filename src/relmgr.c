#include <common.h>
#include <dvd.h>
#include <filemgr.h>
#include <lzss10.h>
#include <memory.h>
#include <os.h>
#include <relmgr.h>
#include <stdio.h>
#include <string.h>
#include <system.h>

#pragma push
#pragma pool_strings off

static RelWork work;
static RelWork * wp = &work;
static const char * relDecompName = "relF.rel";
static const char * relCompName = "relF.bin";

void relInit()
{
    wp->loaded = false;
}

void relMain()
{
    char relPath[0x48];

    if (wp->loaded)
        return;

    bool isCompressed = 1;
    sprintf(relPath, "%s/rel/%s", getSpmarioDVDRoot(), relCompName);
    if (DVDConvertPathToEntrynum(relPath) == -1)
    {
        sprintf(relPath, "%s/rel/%s", getSpmarioDVDRoot(), relDecompName);
        isCompressed = 0;
    }

    if (fileAsyncf(0, 0, relPath) == 0) return;

    FileRecord * file = fileAllocf(0, relPath);
    if ((u32) file == 0xffffffff)
    {
        wp->loaded = true;
        return;
    }

    if (!isCompressed)
    {
        wp->relFile = __memAlloc(0, file->sp->size);
        memcpy(wp->relFile, file->sp->data, file->sp->size);
    }
    else {
        wp->relFile = __memAlloc(0, lzss10ParseHeader(file->sp->data).decompSize);
        lzss10Decompress(file->sp->data, wp->relFile);
    }

    fileFree(file);

    wp->bss = __memAlloc(0, wp->relFile->bssSize);
    memset(wp->bss, 0, wp->relFile->bssSize);
    OSLink(wp->relFile, wp->bss);
    wp->relFile->prolog();
    wp->loaded = true;
}

bool isRelLoaded()
{
    return wp->loaded;
}

#pragma pop
