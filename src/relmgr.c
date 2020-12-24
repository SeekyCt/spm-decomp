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

#pragma pool_strings off

static RelHolder relFHolder;
static RelHolder * relHolder = &relFHolder;
static const char * relDecompName = "relF.rel";
static const char * relCompName = "relF.bin";

void relInit() {
    relHolder->loaded = false;
}

void loadRel() {
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
    if ((u32) file == 0xffff0000) {
        relHolder->loaded = true;
        return;
    }

    if (!isCompressed) {
        relHolder->relFile = __memAlloc(0, file->dataHolder->size);
        memcpy(relHolder->relFile, file->dataHolder->data, file->dataHolder->size);
    }
    else {
        relHolder->relFile = __memAlloc(0, lzss10ParseHeader(file->dataHolder->data).decompSize);
        lzss10Decompress(file->dataHolder->data, relHolder->relFile);
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

#pragma pool_strings reset
