#include <common.h>
#include <spm/seq_game.h>
#include <spm/seq_gameover.h>
#include <spm/seq_load.h>
#include <spm/seq_logo.h>
#include <spm/seq_mapchange.h>
#include <spm/seq_title.h>
#include <spm/seqdef.h>
#include <spm/seqdrv.h>

extern "C" {

SeqDef seq_data[SEQ_COUNT] = {
    {
        &seq_logoInit,
        &seq_logoMain,
        &seq_logoExit
    },
    {
        &seq_titleInit,
        &seq_titleMain,
        &seq_titleExit
    },
    {
        &seq_gameInit,
        &seq_gameMain,
        &seq_gameExit
    },
    {
        &seq_mapChangeInit,
        &seq_mapChangeMain,
        &seq_mapChangeExit
    },
    {
        &seq_gameOverInit,
        &seq_gameOverMain,
        &seq_gameOverExit
    },
    {
        &seq_loadInit,
        &seq_loadMain,
        &seq_loadExit
    }
};

}
