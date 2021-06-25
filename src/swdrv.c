#include <common.h> 
#include <evtmgr_cmd.h>
#include <somewhere.h>
#include <spmario.h>
#include <string.h>
#include <swdrv.h>
#include <system.h>

static SwWork work; // 8050a118
static SwWork * wp; // 805adf40

void swInit()
{
    memset(gp->gswf, 0, sizeof(gp->gswf));
    memset(gp->gsw, 0, sizeof(gp->gsw));
    memset(gp->lswf, 0, sizeof(gp->lswf));
    memset(gp->lsw, 0, sizeof(gp->lsw));

    memset(gp->unknown_0x1184, 0, sizeof(gp->unknown_0x1184));

    wp->coinId = 0;
    wp->gameCoinId = 0;

    gp->gsw0 = 0;
}

void swReInit()
{
    memset(gp->lswf, 0, sizeof(gp->lswf));
    memset(gp->lsw, 0, sizeof(gp->lsw));

    memset(gp->unknown_0x1184, 0, sizeof(gp->unknown_0x1184));

    wp->coinId = 0;
}

void swSet(s32 id)
{
    gp->gswf[id / 32] |= 1U << (id % 32);
}

bool swGet(s32 id)
{
    u32 mask = 1U << (id % 32);
    u32 dat = gp->gswf[id / 32];
    return (bool) (mask & dat);
}

void swClear(s32 id)
{
    gp->gswf[id / 32] &= ~(1U << (id % 32));
}

void swByteSet(s32 id, s32 num)
{
    if (id == 0)
    {
        gp->gsw0 = num;
    }
    else {
        // "The value is strange"
        assertf(0x9c, num < 256, "’l‚ª‚¨‚©‚µ‚¢ sw_byte[%d] = %d", id + EVTDAT_GSW_BASE, num);

        gp->gsw[id] = (s8) num;
    }
}

s32 swByteGet(s32 id)
{
    if (id == 0)
        return gp->gsw0;
    else
        return gp->gsw[id] & 0xff;
}

void _swSet(s32 id)
{
    gp->lswf[id / 32] |= 1U << (id % 32);
}

bool _swGet(s32 id)
{
    u32 mask = 1U << (id % 32);
    u32 dat = gp->lswf[id / 32];
    return (bool) (mask & dat);
}

void _swClear(s32 id)
{
    gp->lswf[id / 32] &= ~(1U << (id % 32));
}

void _swByteSet(s32 id, s8 num)
{
    gp->lsw[id] = (s8) num;
}

s32 _swByteGet(s32 id)
{
    return gp->lsw[id] & 0xff;
}

// 80038204
// 800383a0
// 80038478
// 80038550
// 8003863c
// 8003864c
// 8003865c
// 8003875c
// 800387d8
