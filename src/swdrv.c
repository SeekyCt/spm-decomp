#include <common.h> 
#include <evtmgr_cmd.h>
#include <somewhere.h>
#include <spmario.h>
#include <string.h>
#include <swdrv.h>
#include <system.h>

void swInit() {
    memset(gp->gswf, 0, sizeof(gp->gswf));
    memset(gp->gsw, 0, sizeof(gp->gsw));
    memset(gp->lswf, 0, sizeof(gp->lswf));
    memset(gp->lsw, 0, sizeof(gp->lsw));
    memset(gp->unknown_0x1184, 0, sizeof(gp->unknown_0x1184));
    unknown_805adf40[0] = 0;
    unknown_805adf40[1] = 0;
    gp->gsw0 = 0;
}

void swReInit() {
    memset(gp->lswf, 0, sizeof(gp->lswf));
    memset(gp->lsw, 0, sizeof(gp->lsw));
    memset(gp->unknown_0x1184, 0, sizeof(gp->unknown_0x1184));
    unknown_805adf40[0] = 0;
}

void swSet(s32 num) {
    gp->gswf[num / 32] |= 1U << (num % 32);
}

bool swGet(s32 num) {
    u32 mask = 1U << (num % 32);
    u32 dat = gp->gswf[num / 32];
    return (bool) (mask & dat);
}

void swClear(s32 num) {
    gp->gswf[num / 32] &= ~(1U << (num % 32));
}

void swByteSet(s32 num, s32 value) {
    if (num == 0) {
        gp->gsw0 = value;
    }
    else {
        // "The value is strange"
        assertf(num < 256, "値がおかしい sw_byte[%d] = %d", num + EVTDAT_LSW_BASE, value);
        gp->gsw[num] = (s8) value;
    }
}

s32 swByteGet(s32 num) {
    if (num == 0) return gp->gsw0;
    else return gp->gsw[num];
}

void _swSet(s32 num) {
    gp->lswf[num / 32] |= 1U << (num % 32);
}

bool _swGet(s32 num) {
    u32 mask = 1U << (num % 32);
    u32 dat = gp->lswf[num / 32];
    return (bool) (mask & dat);
}

void _swClear(s32 num) {
    gp->lswf[num / 32] &= ~(1U << (num % 32));
}

void _swByteSet(s32 num, s8 value) {
    gp->lsw[num] = (s8) value;
}

s32 _swByteGet(s32 num) {
    return gp->gsw[num];
}

// a lot
