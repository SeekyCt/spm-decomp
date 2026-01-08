#pragma once

#include <common.h>
#include <spm/seqdrv.h>
#include <wii/mem.h>

#ifdef __cplusplus
#include <nw4r/lyt/arcResourceAccessor.hpp>
#include <nw4r/lyt/animation.hpp>
#include <nw4r/lyt/drawInfo.hpp>
#include <nw4r/lyt/layout.hpp>
#endif

CPP_WRAPPER(spm::seq_title)

USING(spm::seqdrv::SeqWork)
USING(wii::mem::MEMAllocator)

/*
    seqWork stage values
*/

// Wait for button press, or to timeout and replay intro
#define SEQ_TITLE_WAIT_INPUT -1

// Button pressed, start file select transition
#define SEQ_TITLE_FILE_SELECT_PRESS 0

// Wait for file select transition
#define SEQ_TITLE_FILE_SELECT_FADE 1

// Timeout, replay intro
#define SEQ_TITLE_REPLAY_TIMEOUT 2

// Wait for intro replay transition
#define SEQ_TITLE_REPLAY_FADE 3

typedef struct
{
/* 0x00 */ s32 openingReplayTimer;
/* 0x04 */ u32 unknown_0x4; // unused?
/* 0x08 */ void * arc;
/* 0x0C */ void * heap;
/* 0x10 */ u32 heapSize;
/* 0x14 */ void * heapHandle;
/* 0x18 */ MEMAllocator allocator;
#ifdef __cplusplus
/* 0x28 */ nw4r::lyt::ArcResourceAccessor * arcResAccessor;
/* 0x2C */ nw4r::lyt::Layout  * layout;
/* 0x30 */ nw4r::lyt::Pane * pushu2Pane;
/* 0x30 */ nw4r::lyt::Pane * pushuBotanPane;
/* 0x38 */ nw4r::lyt::AnimTransform * animations[2]; // 0 start, 1 loop
/* 0x40 */ nw4r::lyt::DrawInfo drawInfo;
#else
/* 0x28 */ u8 cpp_pad[0x94 - 0x28];
#endif
/* 0x94 */ s32 animNum;
/* 0x98 */ f32 animFrame;
} SeqTitleWork;
SIZE_ASSERT(SeqTitleWork, 0x9c)

void seq_titleInit(SeqWork * work);
void seq_titleMain(SeqWork * work);
void seq_titleExit(SeqWork * work);
void seqTitleDispCb(s32 cameraId, void * param);
void seqTitleInitLayout();
STRIPPED(void seqTitleDeleteLayout())
void seqTitleDisp();

// No idea why this is in this file of all places, but it is
const char * getDanMapName(s32 dungeonNo);

CPP_WRAPPER_END()
