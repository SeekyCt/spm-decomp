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

    // TODO: flag defines
    gp->flags &= ~1;
    gp->flags &= ~2;

    seqWork->stage = -1;

    fadeEntry(1, 300, (GXColor) {0x00, 0x00, 0x00, 0xff});

    marioReset();
    wp->unknown_0x0 = 0;
    wp->unknown_0x4 = 0;
    seqTitleInitLayout();
    spsndBGMOn(0, "BGM_EVT_TITLE1");
}

void seq_titleMain(SeqWork * seqWork)
{
    if (seqWork->stage != -1)
        wp->unknown_0x0 = 0;

    switch (seqWork->stage)
    {
        case -1:
            if (fadeIsFinish())
            {
                // TODO: flag macro
                if (((wpadGetButtonsPressed(0) >> 8) & 1) != 0)
                {
                    seqWork->stage = 0;
                }
                else
                {
                    if (wp->unknown_0x0++ > 1800)
                        seqWork->stage = 2;
                }
            } 
            break;

        case 0:
            fadeEntry(2, 300, (GXColor) {0x00, 0x00, 0x00, 0xff});
            spsndSFXOn("SFX_SYS_PRESS_START1");
            func_8023ce20(0, 70, 1000);
            seqWork->stage = 1;
            break;

        case 1:
            if (fadeIsFinish())
                seqSetSeq(SEQ_LOAD, NULL, NULL);
            break;

        case 2:
            spsndBGMOff_f_d_alt(0, 2000);
            fadeEntry(2, 300, (GXColor) {0x00, 0x00, 0x00, 0xff});
            seqWork->stage = 3;
            break;

        case 3:
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

void * operator new(size_t size, MEMAllocator * allocator);
void * operator new(size_t size, MEMAllocator * allocator)
{
    return MEMAllocFromAllocator(allocator, size);
}

// Not matching: has try-catch missing
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
    wp->layout = new (&wp->allocator) nw4r::lyt::Layout();
    wp->arcResAccessor = new (&wp->allocator) nw4r::lyt::ArcResourceAccessor();
    wp->arcResAccessor->Attach(wp->arc, "arc");

    void * lytRes = wp->arcResAccessor->GetResource(0, "title.brlyt", 0);
    NW4R_ASSERT_PTR(3076, lytRes);
    wp->layout->Build(lytRes, wp->arcResAccessor);

    void * lpaRes = wp->arcResAccessor->GetResource(0, "title_start.brlan", 0);
    NW4R_ASSERT_PTR(3081, lpaRes);
    wp->animations[0] = wp->layout->CreateAnimTransform(lpaRes, wp->arcResAccessor);

    lpaRes = wp->arcResAccessor->GetResource(0, "title_Loop.brlan", 0);
    NW4R_ASSERT_PTR(3085, lpaRes);
    wp->animations[1] = wp->layout->CreateAnimTransform(lpaRes, wp->arcResAccessor);

    wp->startAnimNum = 0;
    wp->unknown_0x98 = 0.0f;
    wp->layout->BindAnimation(wp->animations[wp->startAnimNum]);

    wp->pushu2Pane = wp->layout->GetRootPane()->FindPaneByName("pushu_2", 1);
    wp->pushuBotanPane = wp->layout->GetRootPane()->FindPaneByName("pushu_botan", 1);

    spsndSFXOn("SFX_SYS_TITLE_APPEAR1");
}

// func_8017b840

// func_8017b8ac

// getNextDanMapName

// [ sinit ]

// func_8017bed0

}
