#include <common.h>
#include <spm/camdrv.h>
#include <spm/dispdrv.h>
#include <spm/fadedrv.h>
#include <spm/hud.h>
#include <spm/mario.h>
#include <spm/seqdrv.h>
#include <spm/seq_title.h>
#include <spm/spmario.h>
#include <spm/spmario_snd.h>
#include <spm/wpadmgr.h>

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

    dispEntry(CAM_ID_2D, 4, 1000.0, func_8017b49c, seqWork);
}

// seq_titleExit

// func_8017b49c

// seqTitleInitLayout

// func_8017b840

// func_8017b8ac

// getNextDanMapName

// [ sinit ]

// func_8017bed0

}
