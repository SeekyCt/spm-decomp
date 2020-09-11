#include <string.h>

#include "seqdrv.h"
#include "system.h"

/*
8017bf2c 94  21  ff  f0    stwu       r1,-16(r1)
8017bf30 7c  08  02  a6    mfspr      r0,LR
8017bf34 3c  60  80  51    lis        r3,-0x7faf
8017bf38 38  80  00  00    li         r4,0x0
8017bf3c 90  01  00  14    stw        r0,20(r1)
8017bf40 38  63  23  60    addi       r3=>seqWork ,r3,0x2360
8017bf44 38  a0  00  24    li         r5,0x24
8017bf48 4b  e8  81  bd    bl         memset
8017bf4c 38  00  ff  ff    li         r0,-0x1
8017bf50 90  0d  81  b8    stw        r0,-0x7e48 (r13 )=>now_seq 
8017bf54 90  0d  81  bc    stw        r0,-0x7e44 (r13 )=>next_seq
8017bf58 90  0d  81  c0    stw        r0,-0x7e40 (r13 )=>prev_seq
8017bf5c 80  01  00  14    lwz        r0,20(r1)
8017bf60 7c  08  03  a6    mtspr      LR,r0
8017bf64 38  21  00  10    addi       r1,r1,0x10
8017bf68 4e  80  00  20    blr
*/
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

/*
8017bf6c 94  21  ff  f0    stwu       r1,local_10 (r1)
8017bf70 7c  08  02  a6    mfspr      r0,LR
8017bf74 90  01  00  14    stw        r0,local_res4 (r1)
8017bf78 93  e1  00  0c    stw        r31 ,local_4 (r1)
8017bf7c 80  6d  81  b8    lwz        r3,-0x7e48 (r13 )=>now_seq
8017bf80 80  0d  81  bc    lwz        r0,-0x7e44 (r13 )=>next_seq
8017bf84 7c  00  18  00    cmpw       r0,r3
8017bf88 41  82  00  98    beq        LAB_8017c020
8017bf8c 2c  03  ff  ff    cmpwi      r3,-0x1
8017bf90 41  82  00  30    beq        LAB_8017bfc0
8017bf94 48  02  15  1d    bl         system::sysWaitDrawSync
8017bf98 80  0d  81  b8    lwz        r0,-0x7e48 (r13 )=>now_seq
8017bf9c 3c  80  80  43    lis        r4,-0x7fbd
8017bfa0 3c  60  80  51    lis        r3,-0x7faf
8017bfa4 1c  00  00  0c    mulli      r0,r0,0xc
8017bfa8 38  84  87  a8    subi       r4,r4,0x7858
8017bfac 38  63  23  60    addi       r3=>seqWork ,r3,0x2360 
8017bfb0 7c  84  02  14    add        r4=>PTR_s_sp_8042879c ,r4,r0
8017bfb4 81  84  00  08    lwz        r12 ,0x8 (r4)=>seqFuncs[0].exit
8017bfb8 7d  89  03  a6    mtspr      CTR ,r12
8017bfbc 4e  80  04  21    bctrl
						LAB_8017bfc0                                    XREF[1]:     8017bf90 (j)   
8017bfc0 80  0d  81  b8    lwz        r0,-0x7e48 (r13 )=>now_seq
8017bfc4 3f  e0  80  51    lis        r31 ,-0x7faf
8017bfc8 38  7f  23  60    addi       r3=>seqWork ,r31 ,0x2360
8017bfcc 38  80  00  00    li         r4,0x0
8017bfd0 90  0d  81  c0    stw        r0,-0x7e40 (r13 )=>prev_seq
8017bfd4 38  a0  00  24    li         r5,0x24
8017bfd8 4b  e8  81  2d    bl         memset
8017bfdc 81  0d  81  bc    lwz        r8,-0x7e44 (r13 )=>next_seq
8017bfe0 3c  80  80  43    lis        r4,-0x7fbd
8017bfe4 38  84  87  a8    subi       r4=>seqFuncs ,r4,0x7858
8017bfe8 80  cd  8a  18    lwz        r6,-0x75e8 (r13 )=>seqMapName
8017bfec 1c  08  00  0c    mulli      r0,r8,0xc
8017bff0 38  7f  23  60    addi       r3=>seqWork ,r31 ,0x2360
8017bff4 38  e0  00  00    li         r7,0x0
8017bff8 80  ad  8a  1c    lwz        r5,-0x75e4 (r13 )=>seqDoorName
8017bffc 91  0d  81  b8    stw        r8,-0x7e48 (r13 )=>now_seq
8017c000 7d  84  00  2e    lwzx       r12 ,r4=>->seq_logo::seq_logoInit ,r0=>PTR_s_sp_  = 
																						= 805b3094
8017c004 91  1f  23  60    stw        r8,offset  seqWork (r31 )
8017c008 90  e3  00  04    stw        r7,0x4 (r3)=>seqWork.stage
8017c00c 90  c3  00  08    stw        r6,0x8 (r3)=>seqWork.mapName
8017c010 90  a3  00  0c    stw        r5,0xc (r3)=>seqWork.doorName
8017c014 90  e3  00  20    stw        r7,0x20 (r3)=>seqWork.afterFunc
8017c018 7d  89  03  a6    mtspr      CTR ,r12
8017c01c 4e  80  04  21    bctrl
						LAB_8017c020                                    XREF[1]:     8017bf88 (j)   
8017c020 80  0d  81  b8    lwz        r0,-0x7e48 (r13 )=>now_seq
8017c024 3c  80  80  43    lis        r4,-0x7fbd
8017c028 3f  e0  80  51    lis        r31 ,-0x7faf
8017c02c 1c  00  00  0c    mulli      r0,r0,0xc
8017c030 38  84  87  a8    subi       r4,r4,0x7858
8017c034 38  7f  23  60    addi       r3=>seqWork ,r31 ,0x2360
8017c038 7c  84  02  14    add        r4=>PTR_s_sp_8042879c ,r4,r0
8017c03c 81  84  00  04    lwz        r12 ,0x4 (r4)=>seqFuncs[0].main                   = null
																						= 805b3098
8017c040 7d  89  03  a6    mtspr      CTR ,r12
8017c044 4e  80  04  21    bctrl
8017c048 38  7f  23  60    addi       r3,r31 ,0x2360
8017c04c 81  83  00  20    lwz        r12 ,0x20 (r3)=>seqWork.afterFunc
8017c050 2c  0c  00  00    cmpwi      r12 ,0x0
8017c054 41  82  00  0c    beq        LAB_8017c060
8017c058 7d  89  03  a6    mtspr      CTR ,r12
8017c05c 4e  80  04  21    bctrl
						LAB_8017c060                                    XREF[1]:     8017c054 (j)   
8017c060 80  01  00  14    lwz        r0,local_res4 (r1)
8017c064 83  e1  00  0c    lwz        r31 ,local_4 (r1)
8017c068 7c  08  03  a6    mtspr      LR,r0
8017c06c 38  21  00  10    addi       r1,r1,0x10
8017c070 4e  80  00  20    blr
*/
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

/*
8017c074 90  6d  81  bc    stw        r3,-0x7e44 (r13 )=>next_seq
8017c078 90  8d  8a  18    stw        r4,-0x75e8 (r13 )=>next_p0
8017c07c 90  ad  8a  1c    stw        r5,-0x75e4 (r13 )=>next_p1
8017c080 4e  80  00  20    blr
*/

void seqSetSeq(int seq, char * p0, char * p1) {
	next_seq = seq;
	next_p0 = p0;
	next_p1 = p1;
}

/*
8017c084 80  8d  81  b8    lwz        r4,-0x7e48 (r13 )=>now_seq
8017c088 38  64  00  01    addi       r3,r4,0x1
8017c08c 20  04  ff  ff    subfic     r0,r4,-0x1
8017c090 7c  60  00  f8    nor        r0,r3,r0
8017c094 7c  00  fe  70    srawi      r0,r0,0x1f
8017c098 7c  83  00  78    andc       r3,r4,r0
8017c09c 4e  80  00  20    blr
*/
int seqGetSeq() {
	#ifdef GAME_SPM
	return now_seq == -1 ? 0 : now_seq;
	#else
	return now_seq;
	#endif
}

#ifndef GAME_SPM

/*
8002e2f0 80  6d  82  78    lwz        r3,-0x7d88 (r13 )=>prev_seq
8002e2f4 4e  80  00  20    blr
*/
int seqGetPrevSeq() {
	return prev_seq;
}

/*
8002e2e8 80  6d  82  74    lwz        r3,-0x7d8c (r13 )=>next_seq
8002e2ec 4e  80  00  20    blr
*/
int seqGetNextSeq() {
	return next_seq;
}

/*
8002e2cc 80  8d  82  70    lwz        r4,-0x7d90 (r13 )=>now_seq
8002e2d0 80  0d  82  74    lwz        r0,-0x7d8c (r13 )=>next_seq
8002e2d4 7c  64  00  50    subf       r3,r4,r0
8002e2d8 7c  00  20  50    subf       r0,r0,r4
8002e2dc 7c  60  03  78    or         r0,r3,r0
8002e2e0 54  03  0f  fe    rlwinm     r3,r0,0x1 ,0x1f ,0x1f
8002e2e4 4e  80  00  20    blr
*/
bool seqCheckSeq() {
	return next_seq != now_seq;
}

#endif