#include <string.h>

#include "seqdrv.h"
#include "system.h"

SeqWork seqWork;
s32 now_seq = -1;
s32 next_seq = -1;
s32 prev_seq = -1;
char * next_p0;
char * next_p1;

#ifdef GAME_SPM
void seqInit_SPMARIO() {
#else
void seqInit_MARIOSTORY() {
#endif
	memset(&seqWork, 0, sizeof(seqWork));
	now_seq = -1;
	next_seq = -1;
	prev_seq = -1;
}

void seqMain() {
	if (next_seq != now_seq) {
		if (now_seq != -1) {
			sysWaitDrawSync();
			(*seq_data[now_seq].exit)(&seqWork);
		}
		prev_seq = now_seq;
		memset(&seqWork, 0, sizeof(seqWork));
		now_seq = next_seq;
		seqWork.seq = next_seq;	
		seqWork.stage = 0;
		seqWork.p0 = next_p0;
		seqWork.p1 = next_p1;
#if GAME_SPM
		seqWork.afterFunc = NULL;
#endif
		(*seq_data[now_seq].init)(&seqWork);
	}
	(*seq_data[now_seq].main)(&seqWork);
#if GAME_SPM
	if (seqWork.afterFunc != NULL) {
		(*seqWork.afterFunc)(&seqWork);
	}
#endif
}

void seqSetSeq(s32 seq, char * p0, char * p1) {
	next_seq = seq;
	next_p0 = p0;
	next_p1 = p1;
}

s32 seqGetSeq() {
	#ifdef GAME_SPM
	return now_seq == -1 ? 0 : now_seq;
	#else
	return now_seq;
	#endif
}

#ifndef GAME_SPM

s32 seqGetPrevSeq() {
	return prev_seq;
}

s32 seqGetNextSeq() {
	return next_seq;
}

bool seqCheckSeq() {
	return next_seq != now_seq;
}

#endif
