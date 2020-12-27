#include <common.h>
#include <filemgr.h>
#include <memory.h>
#include <string.h>

static FileWork fileWork;
static FileWork * afp = &fileWork;

void filemgrInit() {
    // Allocate file record array
    afp->records = __memAlloc(0, sizeof(FileRecord[FILE_RECORD_MAX]));

    // Initialise allocated linked list
    afp->allocatedStart = NULL;
    afp->allocatedEnd = NULL;

    // Initialise free linked list
    memset(afp->records, 0, sizeof(FileRecord[FILE_RECORD_MAX]));
    FileRecord * curRecord = afp->records;
    for (s32 i = 0; i < FILE_RECORD_MAX; i++) {
        curRecord->next = curRecord + 1;
        curRecord++;
    }
    afp->freeStart = &afp->records[0];
    afp->freeEnd = &afp->records[FILE_RECORD_MAX - 1];
    afp->freeEnd->next = NULL;
}
