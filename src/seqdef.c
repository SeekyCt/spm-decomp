#include <common.h>
#include <seq_game.h>
#include <seq_gameover.h>
#include <seq_load.h>
#include <seq_logo.h>
#include <seq_mapchange.h>
#include <seq_title.h>
#include <seqdef.h>
#include <seqdrv.h>

SeqDef seq_data[SEQ_COUNT] = {
    {
        .init = &seq_logoInit,
        .main = &seq_logoMain,
        .exit = &seq_logoExit
    },
    {
        .init = &seq_titleInit,
        .main = &seq_titleMain,
        .exit = &seq_titleExit
    },
    {
        .init = &seq_gameInit,
        .main = &seq_gameMain,
        .exit = &seq_gameExit
    },
    {
        .init = &seq_mapChangeInit,
        .main = &seq_mapChangeMain,
        .exit = &seq_mapChangeExit
    },
    {
        .init = &seq_gameOverInit,
        .main = &seq_gameOverMain,
        .exit = &seq_gameOverExit
    },
    {
        .init = &seq_loadInit,
        .main = &seq_loadMain,
        .exit = &seq_loadExit
    }
};
