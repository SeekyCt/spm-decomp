#include <common.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/fadedrv.h>
#include <spm/filemgr.h>
#include <spm/hud.h>
#include <spm/mario.h>
#include <spm/memory.h>
#include <spm/seqdrv.h>
#include <spm/seq_title.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/system.h>
#include <spm/wpadmgr.h>
#include <wii/cx.h>
#include <wii/mtx.h>
#include <msl/math.h>
#include <msl/new.h>
#include <nw4r/db/panic.h>

extern "C" {

static SeqTitleWork work;
static SeqTitleWork * wp = &work;

void seq_titleInit(SeqWork * seqWork)
{
    // Reset sounds
    spsndBGMOff_f_d_alt(1, 500);
    spsndBGMOff_f_d_alt(2, 500);
    func_8023dc90(0, 500);
    func_8023dc90(1, 500);
    func_8023dc90(2, 500);
    for (s32 i = 0; i < 0x20; i++)
        func_8023b38c(i | 0xffff0000, 500);
    spsndSetSFXReverbMode(0);
    spsndClearFlag(0x7f0);

    // Configure hud
    func_8019be84();

    gp->flags &= ~SPMARIO_FLAG_1;
    gp->flags &= ~SPMARIO_FLAG_2;

    seqWork->stage = SEQ_TITLE_WAIT_INPUT;

    // Start transition
    fadeEntry(1, 300, (GXColor) {0x00, 0x00, 0x00, 0xff});

    marioReset();

    // Setup display
    wp->openingReplayTimer = 0;
    wp->unknown_0x4 = 0;
    seqTitleInitLayout();

    // Play music
    spsndBGMOn(0, "BGM_EVT_TITLE1");
}

void seq_titleMain(SeqWork * seqWork)
{
    if (seqWork->stage != SEQ_TITLE_WAIT_INPUT)
        wp->openingReplayTimer = 0;

    switch (seqWork->stage)
    {
        case SEQ_TITLE_WAIT_INPUT:
            // Wait for transition to finish
            if (!fadeIsFinish())
                break;

            // Check for input or timeout
            // TODO: flag macro
            if (((wpadGetButtonsPressed(0) >> 8) & 1) != 0)
            {
                seqWork->stage = SEQ_TITLE_FILE_SELECT_PRESS;
            }
            else
            {
                if (++wp->openingReplayTimer > 1800)
                    seqWork->stage = SEQ_TITLE_REPLAY_TIMEOUT;
            }

            break;

        case SEQ_TITLE_FILE_SELECT_PRESS:
            // Start transition
            fadeEntry(2, 300, (GXColor) {0x00, 0x00, 0x00, 0xff});

            // Play sound effect
            spsndSFXOn("SFX_SYS_PRESS_START1");
            func_8023ce20(0, 70, 1000);

            seqWork->stage = SEQ_TITLE_FILE_SELECT_FADE;

            break;

        case SEQ_TITLE_FILE_SELECT_FADE:
            // Open file select after transition
            if (fadeIsFinish())
                seqSetSeq(SEQ_LOAD, NULL, NULL);

            break;

        case SEQ_TITLE_REPLAY_TIMEOUT:
            // Stop music
            spsndBGMOff_f_d_alt(0, 2000);

            // Start transition
            fadeEntry(2, 300, (GXColor) {0x00, 0x00, 0x00, 0xff});

            seqWork->stage = 3;

            break;

        case SEQ_TITLE_REPLAY_FADE:
            // Replay intro after transition
            if (fadeIsFinish())
                seqSetSeq(SEQ_MAPCHANGE,"aa4_01",NULL);

            break;
    }

    dispEntry(CAM_ID_2D, 4, 1000.0, seqTitleDispCb, seqWork);
}

void seq_titleExit(SeqWork * seqWork)
{
    (void) seqWork;

    delete wp->layout;
    delete wp->arcResAccessor;

    __memFree(HEAP_MAP, wp->arc);
    __memFree(HEAP_MAP, wp->heap);
    memClear(HEAP_MAP);
}

void seqTitleDispCb(s32 cameraId, void * param)
{
    (void) cameraId;
    (void) param;

    seqTitleDisp();
}

static const char * languageNames[] = {
    "jp",
    "us",
    "uk",
    "ge",
    "fr",
    "sp",
    "it",
    "uk"
};

// Not matching: try-catches are wrong
void seqTitleInitLayout()
{
    // Load archive
    const char * dvdRoot = getSpmarioDVDRoot();
    FileEntry * file = fileAllocf(0, "%s/lyt/title.bin.%s", dvdRoot, languageNames[gp->language]);
    u32 size = CXGetUncompressedSize(file->sp->data);
    wp->arc = __memAlloc(HEAP_MAP, size);
    CXUncompressLZ(file->sp->data, wp->arc);
    fileFree(file);

    // Init heap
    wp->heapSize = 0x100000;
    wp->heap = __memAlloc(HEAP_MAP, wp->heapSize);
    wp->heapHandle = MEMCreateExpHeapEx(wp->heap, wp->heapSize, 0);
    MEMInitAllocatorForExpHeap(&wp->allocator, wp->heapHandle, 0x20);

    nw4r::lyt::Layout::SetAllocator(&wp->allocator);

    void * layoutMem = MEMAllocFromAllocator(&wp->allocator, sizeof(*wp->layout));
    nw4r::lyt::Layout * layout;
    try {
        layout = new (layoutMem) nw4r::lyt::Layout();
    }
    catch (...) {
        throw;
    }
    wp->layout = layout;

    void * accessorMem = MEMAllocFromAllocator(&wp->allocator, sizeof(*wp->arcResAccessor));
    nw4r::lyt::ArcResourceAccessor * arcResAccessor;
    try {
        arcResAccessor = new (accessorMem) nw4r::lyt::ArcResourceAccessor();
    }
    catch (...) {
        throw;
    }
    wp->arcResAccessor = arcResAccessor;

    wp->arcResAccessor->Attach(wp->arc, "arc");

    void * lytRes = wp->arcResAccessor->GetResource(0, "title.brlyt", 0);
    NW4R_ASSERT_PTR(3076, lytRes);
    wp->layout->Build(lytRes, wp->arcResAccessor);

    void * lpaRes = wp->arcResAccessor->GetResource(0, "title_Start.brlan", 0);
    NW4R_ASSERT_PTR(3081, lpaRes);
    wp->animations[0] = wp->layout->CreateAnimTransform(lpaRes, wp->arcResAccessor);

    lpaRes = wp->arcResAccessor->GetResource(0, "title_Loop.brlan", 0);
    NW4R_ASSERT_PTR(3085, lpaRes);
    wp->animations[1] = wp->layout->CreateAnimTransform(lpaRes, wp->arcResAccessor);

    wp->animNum = 0;
    wp->animFrame = 0.0f;
    wp->layout->BindAnimation(wp->animations[wp->animNum]);

    wp->pushu2Pane = wp->layout->GetRootPane()->FindPaneByName("pushu_2", 1);
    wp->pushuBotanPane = wp->layout->GetRootPane()->FindPaneByName("pushu_botan", 1);

    spsndSFXOn("SFX_SYS_TITLE_APPEAR1");
}

void seqTitleDisp()
{
    // TODO: nw4r inlines
    nw4r::math::MTX34 viewMtx;
    PSMTXCopy(camGetCurPtr()->viewMtx, viewMtx);
    nw4r::math::VEC3 scale(0.95f, 1.0f, 1.0f);
    PSMTXScaleApply(viewMtx, viewMtx, scale.x, scale.y, scale.z);
    wp->drawInfo.mViewMtx = viewMtx;
    wp->drawInfo.mViewRect = wp->layout->GetLayoutRect();
    wp->layout->Animate(0);
    wp->layout->CalculateMtx(wp->drawInfo);
    f32 temp = (wp->animFrame / wp->animations[wp->animNum]->GetFrameSize());
    f32 unk = cosf(temp * PIx2 * 8.0f);
    s32 unk2 = (s32) (255.0f - ((unk * 256.0f) + 128.0f));
    if (unk2 > 0xff)
        unk2 = 0xff;
    if (unk2 < 0)
        unk2 = 0;
    if (wp->animNum == 0)
        unk2 = 0;
    wp->pushu2Pane->mAlpha = (u8) unk2;
    GXColorS10 col = {0, 0, 0, (s16) unk2};
    nw4r::lyt::Material * material = wp->pushuBotanPane->GetMaterial();
    material->mTevCols[1] = col;
    wp->layout->Draw(wp->drawInfo);
    wp->animFrame += 60.0f / gp->fps;

    while (wp->animFrame >= wp->animations[wp->animNum]->GetFrameSize())
    {
        if (wp->animNum == 0)
        {
            wp->layout->UnbindAnimation(wp->animations[wp->animNum]);
            wp->animNum += 1;
            wp->layout->BindAnimation(wp->animations[wp->animNum]);
            wp->animFrame = 0.0f;
            break;
        }

        wp->animFrame -= (float) wp->animations[wp->animNum]->GetFrameSize();
    }

    wp->animations[wp->animNum]->mFrame = wp->animFrame;
    CamEntry * cam = camGetCurPtr();
    GXSetProjection(cam->projMtx, cam->projectionType);
}

const char * getNextDanMapname(s32 dungeonNo)
{
    if (dungeonNo < 100)
    {
        switch (dungeonNo)
        {
            case 9:
            case 19:
                return "dan_21";
            case 29:
            case 39:
                return "dan_22";
            case 49:
            case 59:
                return "dan_23";
            case 69:
            case 79:
            case 89:
                return "dan_24";
            case 99:
                return "dan_30";
        }

        if (dungeonNo < 25)
            return "dan_01";
        if (dungeonNo < 49)
            return "dan_02";
        if (dungeonNo < 75)
            return "dan_03";
        return "dan_04";
    }
    else
    {
        switch (dungeonNo - 100)
        {
            case 9:
            case 19:
                return "dan_61";
            case 29:
            case 39:
                return "dan_62";
            case 49:
            case 59:
                return "dan_63";
            case 69:
            case 79:
            case 89:
                return "dan_64";
            case 99:
                return "dan_70";
        }

        if (dungeonNo - 100 < 25)
            return "dan_41";
        if (dungeonNo - 100 < 49)
            return "dan_42";
        if (dungeonNo - 100 < 75)
            return "dan_43";
        return "dan_44";
    }
}

}
