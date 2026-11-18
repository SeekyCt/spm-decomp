/*
    WARNING: Not fully decompiled
    This file is currently not linked into the final dol
*/

#include <common.h>
#include <spm/animdrv.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/lz_embedded.h>
#include <spm/memory.h>
#include <spm/spmario.h>
#include <spm/system.h>
#include <wii/cx.h>
#include <wii/mtx.h>
#include <wii/os/OSTime.h>
#include <msl/math.h>
#include <msl/stdio.h>
#include <msl/string.h>

extern "C" {

static AnimWork work;
static AnimWork * wp = &work;

static f32 tanfTbl[178];

static int _dummy = -1;

#define TESTHEAP_SIZE 0x238000

AnimWork * animGetPtr()
{
	return wp;
}

//   UNUSED   000088 ........ timeGetTime animdrv.o 

OSTime animTimeGetTime()
{
	return OSTicksToMilliseconds(gp->time);
}

// Inlined + deadstripped
static void initTestHeap() {
	if (!wp->testHeap)
		wp->testHeap = __memAlloc(0, TESTHEAP_SIZE);

	wp->testHeapNextFree = wp->testHeap;
}

static void * testAlloc(u32 size)
{
	AnimWork * wp = animGetPtr();
	void * alloc = wp->testHeapNextFree;

	if (size & 31)
		size += 32 - (size & 31);

    wp->testHeapNextFree = (void*) ((u32) wp->testHeapNextFree + size);

	wp = animGetPtr(); // ???

	if ((u32) wp->testHeapNextFree >= (u32) wp->testHeap + TESTHEAP_SIZE)
    {
		animPoseRefresh();
		return testAlloc(size);
	}

	return alloc;
}

void animInit()
{
    wp->animGroupNum = 64;
    wp->textureGroupNum = 64;
    wp->animPoseNum = 384;
    wp->floatScratchRP = 1024;
    wp->floatScratchWP = 0;
    wp->animGroups = (AnimGroup *) __memAlloc(HEAP_MAIN, sizeof(AnimGroup) * wp->animGroupNum);
    wp->textureGroups = (TextureGroup *) __memAlloc(HEAP_MAIN, sizeof(TextureGroup) * wp->textureGroupNum);
    wp->animPose = (AnimPose *) __memAlloc(HEAP_MAIN, sizeof(AnimPose) * wp->animPoseNum);
    wp->useFloatScratch = false;
    wp->floatScratch = (Vec3 *) __memAlloc(HEAP_MAIN, sizeof(Vec3) * wp->floatScratchRP);

    wp->texObjs[0] = NULL;
    wp->texObjs[1] = NULL;
    wp->curTexObj = NULL;

    wp->texDatas[0] = NULL;
    wp->texDatas[1] = NULL;
    wp->curTexData = NULL;

    wp->paperMtxPtr[0] = NULL;
    wp->paperMtxPtr[1] = NULL;
    wp->paperMtxPtr[2] = NULL;

    wp->unknown_0xd8 = 1;
    wp->unknown_0xdc = 1;

    wp->testHeap = NULL;
    wp->inBattle = false;
    initTestHeap(); // inlined

    for (int i = 0; i < 0xB2; i++)
		// NON_MATCHING: stfs instead of stfsx
        tanfTbl[i] = tanf(DEG_TO_RAD(i - 89));

	u32 size = CXGetUncompressedSize(ag2tg);
	wp->ag2tg = smartAlloc(size,0);
	CXUncompressLZ(ag2tg, wp->ag2tg->data);
}

void animMain()
{
	dispEntry(CAM_ID_0, 7, 0.0f, animPaperPoseDisp, NULL);
}


/* textureGroupNew? */

int textureGroupEntry(char * filename)
{
	int textureGroupId;
	int i;
	AnimWork * wp;
	char path [256];

	sprintf(path, "%s/%s-", "a", filename);
	wp = animGetPtr();
	
	TextureGroup * tg;
	int tn1 = wp->textureGroupNum;
	for (i = 0; i < tn1; i++)
	{
		tg = wp->textureGroups + i;
		if (tg->inUse && tg->file != 0 && strcmp(tg->file->path, path) == 0)
		{
			tg->refCnt += 1;
			return i;
		}
	}

	wp = animGetPtr();	
	for (textureGroupId = 0; textureGroupId < wp->textureGroupNum; textureGroupId++)
	{
		tg = wp->textureGroups + textureGroupId;
		if (tg->inUse != 0)
			continue;
		goto found;
	}
	tg = NULL;
	textureGroupId = -1;
found:

	/* "The texture group is full and cannot be registered." */
	SPM_ASSERT(0x126, textureGroupId != -1, "テクスチャグループががいっぱいで登録できません。");

	tg->inUse = 1;
	tg->refCnt += 1;
	tg->file = fileAlloc(path, FILETYPE_TPL);
	return textureGroupId;
}



//   UNUSED   000058 ........ animPoseNew animdrv.o 
//   UNUSED   000058 ........ animGroupNew animdrv.o 
//   UNUSED   000058 ........ textureGroupNew animdrv.o 
//   UNUSED   00013c ........ textureGroupEntry animdrv.o 
//   UNUSED   00026c ........ animGroupEntry animdrv.o 


// Inlined + deadstripped
static void animPose_AllocBuffer(AnimPose * pose) {
	pose->bufferVtxPos = (Vec3 *) testAlloc(sizeof(Vec3) * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->vertexPositionCount);
	pose->vtxArrayPos = (Vec3 *) testAlloc(sizeof(Vec3) * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->vertexPositionCount);

	pose->bufferVtxNorm = (Vec3 *) testAlloc(sizeof(Vec3) * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->vertexNormalCount);
	pose->vtxArrayNorm = (Vec3 *) testAlloc(sizeof(Vec3) * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->vertexNormalCount);

	pose->bufferGroupVisibility = testAlloc(sizeof(u8) * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->visibilityGroupCount);
	pose->groupVisibility = pose->bufferGroupVisibility;

	pose->bufferNodes = testAlloc(4 * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->groupTransformDataCount);
	pose->nodes = testAlloc(4 * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->groupTransformDataCount);

	pose->bufferTexAnimEntries = testAlloc(24 * ((AnimPoseData *) wp->animGroups[pose->animGroupId].file->sp->data)->textureCoordinateTransformCount);
	pose->texAnimEntries = pose->bufferTexAnimEntries;

	pose->lastAnimFrame0 = -1;
}

//   UNUSED   0000bc ........ animPose_ResetTime animdrv.o 

void animPoseRefresh(void) {
	int i;
	AnimPose * pose;

	// Anti-inline hack
	(void) 0; (void) 0;

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

//   0003aeb0 00002c 80040490  4 animPoseBattleInit        animdrv.o 
//   0003a9b4 0004fc 8003ff94  4 animPoseEntry     animdrv.o 
//   0003a868 00014c 8003fe48  4 animPaperPoseEntry        animdrv.o 
//   0003a72c 00013c 8003fd0c  4 animEffectAsync   animdrv.o 
//   UNUSED   000058 ........ animEffectEntry animdrv.o 
//   UNUSED   000014 ........ getAnimPose animdrv.o 
//   UNUSED   0000a4 ........ getAnimDataId animdrv.o 
//   0003a710 00001c 8003fcf0  4 animPosePeraOn    animdrv.o 
//   0003a6f4 00001c 8003fcd4  4 animPosePeraOff   animdrv.o 
//   0003a6b0 000044 8003fc90  4 animPosePaperPeraOn       animdrv.o 
//   0003a688 000028 8003fc68  4 animPosePaperPeraOff      animdrv.o 
//   0003a670 000018 8003fc50  4 animPoseSetLocalTimeRate  animdrv.o 
//   0003a550 000120 8003fb30  4 animPoseSetLocalTime      animdrv.o 
//   0003a530 000020 8003fb10  4 animPoseSetStartTime      animdrv.o 
//   0003a3b4 00017c 8003f994  4 animPoseSetAnim   animdrv.o 
//   0003a2d0 0000e4 8003f8b0  4 animPaperPoseGetId        animdrv.o 
//   0003a048 000288 8003f628  4 animPoseSetPaperAnimGroup         animdrv.o 
//   00039ee8 000160 8003f4c8  4 animPoseSetPaperAnim      animdrv.o 
//   00039c6c 00027c 8003f24c  4 animPoseSetEffect         animdrv.o 
//   00039adc 000190 8003f0bc  4 animPoseSetEffectAnim     animdrv.o 
//   00039ac0 00001c 8003f0a0  4 animPoseSetGXFunc         animdrv.o 
//   00039a5c 000064 8003f03c  4 animPoseGetRadius         animdrv.o 
//   000399f8 000064 8003efd8  4 animPoseGetHeight         animdrv.o 
//   000399e0 000018 8003efc0  4 animPoseGetLoopTimes      animdrv.o 
//   UNUSED   000018 ........ animPoseGetStatus animdrv.o 
//   00039994 00004c 8003ef74  4 animPoseGetPeraEnd        animdrv.o 
//   00039974 000020 8003ef54  4 animPoseSetMaterialFlagOn         animdrv.o 
//   00039954 000020 8003ef34  4 animPoseSetMaterialFlagOff        animdrv.o 
//   00039934 000020 8003ef14  4 animPoseSetMaterialLightFlagOn    animdrv.o 
//   00039914 000020 8003eef4  4 animPoseSetMaterialLightFlagOff   animdrv.o 
//   000398f8 00001c 8003eed8  4 animPoseSetMaterialEvtColor       animdrv.o 
//   UNUSED   00001c ........ animPoseSetMaterialAnmColor animdrv.o 
//   000398e0 000018 8003eec0  4 animPoseGetMaterialFlag   animdrv.o 
//   000398c8 000018 8003eea8  4 animPoseGetMaterialLightFlag      animdrv.o 
//   000398b0 000018 8003ee90  4 animPoseGetMaterialEvtColor       animdrv.o 
//   UNUSED   000018 ........ animPoseGetMaterialAnmColor animdrv.o 
//   UNUSED   000038 ........ searchDFrameIdx animdrv.o 
//   UNUSED   000038 ........ searchDStatusIdx animdrv.o 
//   UNUSED   0000c8 ........ getLocalFrame animdrv.o 
//   UNUSED   00003c ........ mod360 animdrv.o 
//   UNUSED   000098 ........ camGetRy animdrv.o 
//   UNUSED   000048 ........ animPoseMain_ForPaper animdrv.o 
//   00037ee8 0019c8 8003d4c8  4 animPoseMain      animdrv.o 
//   UNUSED   000034 ........ setGXModelMtx animdrv.o 
//   UNUSED   0003c8 ........ pushGXModelMtx_TransformNode animdrv.o 
//   UNUSED   000344 ........ pushGXModelMtx_JointNode animdrv.o 
//   00037c3c 0002ac 8003d21c  4 pushGXModelMtx_TransformNode__    animdrv.o 
//   00037994 0002a8 8003cf74  4 pushGXModelMtx_JointNode__        animdrv.o 
//   UNUSED   000024 ........ popGXModelMtx animdrv.o 
//   UNUSED   000008 ........ getGXModelMtx animdrv.o 
//   UNUSED   000114 ........ _TEXGetGXTexObjFromPalette animdrv.o 
//   UNUSED   000138 ........ _TEXGetGXTexObjFromPaletteCI animdrv.o 
//   UNUSED   00012c ........ dispSetRenderMode animdrv.o 
//   UNUSED   00000c ........ dispGetRenderMode animdrv.o 
//   UNUSED   000030 ........ dispSetCullingMode animdrv.o 
//   0003747c 000518 8003ca5c  4 animSetMaterial_Texture   animdrv.o 
//   UNUSED   00013c ........ animSetMaterial_ChangeTexture animdrv.o 
//   00036dcc 0006b0 8003c3ac  4 materialProc      animdrv.o 
//   00036030 000d9c 8003b610  4 renderProc        animdrv.o 
//   00035c64 0003cc 8003b244  4 dispProc  animdrv.o 
//   00035bf8 00006c 8003b1d8  4 animPoseDraw      animdrv.o 
//   UNUSED   0001a8 ........ animPoseDrawDebug animdrv.o 
//   UNUSED   000118 ........ checkMain animdrv.o 
//   00035410 0007e8 8003a9f0  4 _animPoseDrawMtx  animdrv.o 
//   UNUSED   0000b0 ........ animSetCommonBuf animdrv.o 
//   UNUSED   000010 ........ animClearCommonBuf animdrv.o 
//   00034c74 00079c 8003a254  4 animPoseDrawMtx   animdrv.o 
//   00034c00 000074 8003a1e0  4 animSetPaperTexObj        animdrv.o 
//   UNUSED   0000a0 ........ animGroupRelease animdrv.o 
//   00034a6c 000194 8003a04c  4 animPoseRelease   animdrv.o 
//   00034990 0000dc 80039f70  4 animPaperPoseRelease      animdrv.o 
//   000347c4 0001cc 80039da4  4 animPoseAutoRelease       animdrv.o 
//   0003474c 000078 80039d2c  4 animPaperPoseDisp         animdrv.o 
//   00033e2c 000920 8003940c  4 animPaperPoseDispSub      animdrv.o 
//   UNUSED   0000e0 ........ animPoseGetId animdrv.o 
//   000336a8 000784 80038c88  4 animPoseDisp_MakeExtTexture       animdrv.o 
//   000335dc 0000cc 80038bbc  4 animSetPaperTexMtx        animdrv.o 
//   000334a4 000138 80038a84  4 animGroupBaseAsync        animdrv.o 
//   UNUSED   00008c ........ ag2tg animdrv.o 
//   UNUSED   000008 ........ animGroupSubAsync animdrv.o 
//   00033490 000014 80038a70  4 animPoseGetAnimPosePtr    animdrv.o 
//   0003344c 000044 80038a2c  4 animPoseGetAnimDataPtr    animdrv.o 
//   0003341c 000030 800389fc  4 animPoseGetAnimBaseDataPtr        animdrv.o 
//   UNUSED   000014 ........ animPoseGetAnimGroupPtr animdrv.o 
//   000333dc 000040 800389bc  4 animPoseGetCurrentAnim    animdrv.o 
//   UNUSED   000038 ........ animPoseTestOPA animdrv.o 
//   UNUSED   000048 ........ animPoseTestEDG animdrv.o 
//   000333a4 000038 80038984  4 animPoseTestXLU   animdrv.o 
//   UNUSED   000250 ........ animdrvtest_loadTPL animdrv.o 
//   UNUSED   000120 ........ _animPoseEvalMtx animdrv.o 
//   000331bc 0001e8 8003879c  4 evalProc  animdrv.o 
//   00033104 0000b8 800386e4  4 animPoseWorldPositionEvalOn       animdrv.o 
//   UNUSED   000058 ........ animPoseWorldPositionEvalOff animdrv.o 
//   UNUSED   00006c ........ animPoseWorldPositionPrint animdrv.o 
//   0003304c 0000b8 8003862c  4 animPoseWorldMatrixEvalOn         animdrv.o 
//   UNUSED   000058 ........ animPoseWorldMatrixEvalOff animdrv.o 
//   00033014 000038 800385f4  4 animPoseVivianMain        animdrv.o 
//   UNUSED   000038 ........ animPoseVivianMainDbg animdrv.o 
//   00032ffc 000018 800385dc  4 animPoseGetVivianType     animdrv.o 
//   00032fe0 00001c 800385c0  4 animPoseSetDispCallBack   animdrv.o 
//   00032d48 000298 80038328  4 animPoseDrawShape         animdrv.o 
//   00032c9c 0000ac 8003827c  4 animPoseGetShapeIdx       animdrv.o 
//   00032bf0 0000ac 800381d0  4 animPoseGetGroupIdx       animdrv.o 
//   UNUSED   00003c ........ animPoseGetShapeName animdrv.o 
//   00032bb4 00003c 80038194  4 animPoseGetGroupName      animdrv.o 

}
