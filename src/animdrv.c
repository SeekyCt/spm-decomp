#include <common.h>
#include <spm/animdrv.h>
#include <spm/memory.h>

extern "C" {

static AnimWork work;
static AnimWork * wp = &work;

#define TESTHEAP_SIZE 0x238000

// void * testAlloc(u32 size);
// void animPose_AllocBuffer(AnimPoseEntry* pose);
// void initTestHeap();

static void * testAlloc(u32 size)
{
	void * alloc = wp->testHeapNextFree;

	if (size & 31)
		size += 32 - (size & 31);

    wp->testHeapNextFree = (void*) ((u32) wp->testHeapNextFree + size);
	if ((u32) wp->testHeapNextFree >= (u32) wp->testHeap + TESTHEAP_SIZE)
    {
		animPoseRefresh();
		alloc = testAlloc(size);
	}

	return alloc;
}

static void initTestHeap() {
	if (!wp->testHeap) {
		wp->testHeap = __memAlloc(0, TESTHEAP_SIZE);
	}
	wp->testHeapNextFree = wp->testHeap;
}

static void animPose_AllocBuffer(AnimPoseEntry* pose) {
	pose->bufferVtxPos = (Vec3 *) testAlloc(sizeof(Vec3) * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->vertexPositionCount);
	pose->vtxArrayPos = (Vec3 *) testAlloc(sizeof(Vec3) * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->vertexPositionCount);

	pose->bufferVtxNorm = (Vec3 *) testAlloc(sizeof(Vec3) * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->vertexNormalCount);
	pose->vtxArrayNorm = (Vec3 *) testAlloc(sizeof(Vec3) * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->vertexNormalCount);

	pose->bufferGroupVisibility = testAlloc(sizeof(u8) * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->visibilityGroupCount);
	pose->groupVisibility = pose->bufferGroupVisibility;

	pose->bufferNodes = testAlloc(4 * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->groupTransformDataCount);
	pose->nodes = testAlloc(4 * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->groupTransformDataCount);

	pose->bufferTexAnimEntries = testAlloc(0x18U * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->textureCoordinateTransformCount);
	pose->texAnimEntries = pose->bufferTexAnimEntries;

	pose->lastAnimFrame0 = -1;
}

void animPoseRefresh(void) {
	int i;
	AnimPoseEntry* pose;

	initTestHeap();
	for (i = 0; i < wp->animPoseNum; i++)
    {
		pose = wp->animPose + i;
		if (pose->flag)
        {
			if ((!wp->inBattle || pose->releaseType != 0) && (wp->inBattle || pose->releaseType != 1))
            {
				animPose_AllocBuffer(pose);
				pose->lastAnimFrame0 = -1;
			}
		}
	}
}

}
