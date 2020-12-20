#ifndef SEQDEF_H
#define SEQDEF_H

struct _SeqWork;
typedef void (SeqFunc)(struct _SeqWork *);

#ifdef GAME_SPM
    #define SEQ_COUNT 6
#else
    #define SEQ_COUNT 8
#endif

typedef struct {
    SeqFunc * init;
    SeqFunc * main;
    SeqFunc * exit;
} SeqDef;
extern SeqDef seq_data[SEQ_COUNT];

#endif