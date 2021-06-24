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

static RelHolder relFHolder;
static RelHolder * relHolder = &relFHolder;
static const char * relDecompName = "relF.rel";
static const char * relCompName = "relF.bin";

void relInit() {
    relHolder->loaded = false;
}

void relMain() {
    char relPath[0x48];

    if (relHolder->loaded) return;

    bool isCompressed = 1;
    sprintf(relPath, "%s/rel/%s", getSpmarioDVDRoot(), relCompName);
    if (DVDConvertPathToEntrynum(relPath) == -1) {
        sprintf(relPath, "%s/rel/%s", getSpmarioDVDRoot(), relDecompName);
        isCompressed = 0;
    }

    if (fileAsyncf(0, 0, relPath) == 0) return;

    FileRecord * file = fileAllocf(0, relPath);
    if ((u32) file == 0xffffffff) {
        relHolder->loaded = true;
        return;
    }

    if (!isCompressed) {
        relHolder->relFile = __memAlloc(0, file->sp->size);
        memcpy(relHolder->relFile, file->sp->data, file->sp->size);
    }
    else {
        relHolder->relFile = __memAlloc(0, lzss10ParseHeader(file->sp->data).decompSize);
        lzss10Decompress(file->sp->data, relHolder->relFile);
    }

    fileFree(file);

    relHolder->bss = __memAlloc(0, relHolder->relFile->bssSize);
    memset(relHolder->bss, 0, relHolder->relFile->bssSize);
    OSLink(relHolder->relFile, relHolder->bss);
    relHolder->relFile->prolog();
    relHolder->loaded = true;
}

bool isRelLoaded() {
    return relHolder->loaded;
}

#pragma pop
