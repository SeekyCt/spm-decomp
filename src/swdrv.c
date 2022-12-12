#include <common.h>
#include <evt_cmd.h>
#include <spm/evtmgr_cmd.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <spm/swdrv.h>
#include <spm/system.h>
#include <wii/string.h>

// .rodata
static const s32 lbl_80325cb8[] = {
    GSW(153), GSW(154), GSW(155), GSW(156), GSW(157), GSW(158), GSW(159), GSW(160),
    GSW(161), GSW(162), GSW(163), GSW(164), GSW(165), GSW(166), GSW(167), GSW(168),
    GSW(169), GSW(170), GSW(171), GSW(172), GSW(173), GSW(174), GSW(175), GSW(176),
    GSW(177), GSW(178), GSW(179), GSW(180), GSW(181), GSW(182), GSW(218), GSW(219),
    GSW(220), GSW(221), GSW(222), GSW(223), GSW(224), GSW(225), GSW(226), GSW(227),
    GSW(228), GSW(229), GSW(230), GSW(231), GSW(232), GSW(233), GSW(234), GSW(235),
    GSW(236), GSW(237), GSW(238), GSW(239), GSW(240), GSW(241), GSW(242), GSW(243),
    GSW(244), GSW(245), GSW(246), GSW(247), GSW(253), GSW(254), GSW(255), GSW(256),
    GSW(257), GSW(258), GSW(259), GSW(260), GSW(261), GSW(262), GSW(263), GSW(264),
    GSW(265), GSW(266), GSW(267), GSW(268), GSW(269), GSW(270), GSW(271), GSW(272),
    GSW(273), GSW(274), GSW(275), GSW(276), GSW(277), GSW(278), GSW(279), GSW(280),
    GSW(281), GSW(282), GSW(301), GSW(302), GSW(303), GSW(304), GSW(305), GSW(306),
    GSW(307), GSW(308), GSW(309), GSW(310), GSW(311), GSW(312), GSW(313), GSW(314),
    GSW(315), GSW(316), GSW(317), GSW(318), GSW(319), GSW(320), GSW(321), GSW(322),
    GSW(323), GSW(324), GSW(325), GSW(326), GSW(327), GSW(328), GSW(329), GSW(330),
    GSW(358), GSW(359), GSW(360), GSW(361), GSW(362), GSW(363), GSW(364), GSW(365),
    GSW(366), GSW(367), GSW(368), GSW(369), GSW(370), GSW(371), GSW(372), GSW(373),
    GSW(374), GSW(375), GSW(376), GSW(377), GSW(378), GSW(379), GSW(380), GSW(381),
    GSW(382), GSW(383), GSW(384), GSW(385), GSW(386), GSW(387), GSW(401), GSW(402),
    GSW(403), GSW(404), GSW(405), GSW(406), GSW(407), GSW(408), GSW(409), GSW(410),
    GSW(411), GSW(412), GSW(413), GSW(414), GSW(415), GSW(416), GSW(417), GSW(418),
    GSW(419), GSW(420), GSW(421), GSW(422), GSW(423), GSW(424), GSW(425), GSW(426),
    GSW(427), GSW(428), GSW(429), GSW(430), GSW(451), GSW(452), GSW(453), GSW(454),
    GSW(455), GSW(456), GSW(457), GSW(458), GSW(459), GSW(460), GSW(461), GSW(462),
    GSW(463), GSW(464), GSW(465), GSW(466), GSW(467), GSW(468), GSW(469), GSW(470),
    GSW(471), GSW(472), GSW(473), GSW(474), GSW(475), GSW(476), GSW(477), GSW(478),
    GSW(479), GSW(480), GSW(505), GSW(506), GSW(507), GSW(508), GSW(509), GSW(510),
    GSW(511), GSW(512), GSW(513), GSW(514), GSW(515), GSW(516), GSW(517), GSW(518),
    GSW(519), GSW(520), GSW(521), GSW(522), GSW(523), GSW(524), GSW(525), GSW(526),
    GSW(527), GSW(528), GSW(529), GSW(530), GSW(531), GSW(532), GSW(533), GSW(534)
};

static const AssignTblEntry assign_tbl[] = {
    {"an3_01", 27}, {"an3_03",  6}, {"an3_12", 24}, {"an3_14",  4},
    {"an3_15", 20}, {"an4_12", 16}, {"gn3_04", 48}, {"mi1_10", 15},
    {"sp2_09", 48}, {"sp4_15", 18}, {"ta2_02", 10}, {"an1_09", 28},
    {"gn1_05", 45}, {"gn2_06", 28}, {"gn3_10", 48}, {"gn3_16", 41},
    {"he1_01",  4}, {"he1_03", 62}, {"he2_02", 29}, {"he2_08", 63},
    {"mi1_01",  4}, {"mi1_03", 16}, {"mi2_05", 50}, {"sp1_07", 36},
    {"sp2_01",  8}, {"sp3_07", 33}, {"ta1_03", 10}, {"ta1_06", 17},
    {"ta1_07", 10}, {"ta2_01", 34}, {"ta3_02",  3}, {"ta3_05", 48}
};

#include "orderstrings/80326278_80326360.inc"

// .bss
static SwWork work;

// .sdata
static SwWork * wp = &work;

void swInit()
{
    memset(gp->gswf, 0, sizeof(gp->gswf));
    memset(gp->gsw, 0, sizeof(gp->gsw));
    memset(gp->lswf, 0, sizeof(gp->lswf));
    memset(gp->lsw, 0, sizeof(gp->lsw));

    memset(gp->coinThings, 0, sizeof(gp->coinThings));

    wp->coinId = 0;
    wp->gameCoinId = 0;

    gp->gsw0 = 0;
}

void swReInit()
{
    memset(gp->lswf, 0, sizeof(gp->lswf));
    memset(gp->lsw, 0, sizeof(gp->lsw));

    memset(gp->coinThings, 0, sizeof(gp->coinThings));

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
    else
    {
        // "The value is strange"
        assertf(0x9c, num < 256, "値がおかしい sw_byte[%d] = %d", id + EVTDAT_GSW_BASE, num);

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

void _swByteSet(s32 id, u8 num)
{
    gp->lsw[id] = num;
}

s32 _swByteGet(s32 id)
{
    return gp->lsw[id];
}

// Shoutouts to an anonymous decomp.me user (Grumpy Nighingale) for matching it from 95.34% to 100%
s32 func_80038204()
{
    CoinThing *pCoinThings;
    char *mapname;
    s32 i;
    CoinThing *pCVar3;
    CoinThing *pCVar2;
    s32 id;
    pCoinThings = gp->coinThings;

    if (seqGetSeq() != SEQ_MAPCHANGE)
      return -1;
      
    mapname = gp->mapName;
    for (i = 0; i < MAX_COIN_MAP; i++)
        if (strcmp(mapname, pCoinThings[i].mapName) == 0)
            break;
    
    if (i >= MAX_COIN_MAP)
    {
        s32 i;
        CoinThing *pCVar2;
        pCVar3 = gp->coinThings;
        pCVar2 = gp->coinThings;
        if (strncmp(mapname, pCVar2[0].mapName, 3) != 0 && strncmp(mapname, "bos", 3) != 0)
        {
            memset(pCVar3, 0, 0x900);
            wp->coinId = 0;
        }
        pCVar2 = &gp->coinThings[0];
        for (i = 0; i < MAX_COIN_MAP; i++)
        {
            if (strcmp(pCVar2->mapName, "") == 0) break;
            pCVar2++;
        }

        // Coin flag's save location not found
        assert(245, i < MAX_COIN_MAP, "コインフラグの保存場所がみつかりません");
        strcpy(pCVar2[0].mapName, gp->mapName);
        wp->coinId = 0;
    }
    id = wp->coinId++;
    
    // Coin flags have overflowed
    assert(253, wp->coinId < MAX_COIN_BIT, "コインのフラグが溢れました");
    return id;
}

asm UNKNOWN_FUNCTION(func_800383a0)
{
    #include "asm/800383a0.s"
}

asm UNKNOWN_FUNCTION(func_80038478)
{
    #include "asm/80038478.s"
}

asm UNKNOWN_FUNCTION(func_80038550)
{
    #include "asm/80038550.s"
}

void func_8003863c()
{
    wp->coinId = 0;
}

void func_8003864c()
{
    wp->gameCoinId = 0;
}

s32 func_8003865c(void)
{
    char *mapName;
    u32 i;
    s32 output = 0;

    if (seqGetSeq() != SEQ_MAPCHANGE)
        return -1;

    mapName = gp->mapName;
    
    for (i = 0; i < MAX_COIN_MAP; i++)
    {
        if (strcmp(mapName, assign_tbl[i].mapName) == 0) break;
        output += assign_tbl[i].num;
    }
    
    if (i >= MAX_COIN_MAP)
        return -1;

    output += wp->gameCoinId++;
    
    // Coin flags have overflowed
    assert(505, (wp->gameCoinId-1) < assign_tbl[i].num, "コインのフラグが溢れました");
    return output;
}

asm UNKNOWN_FUNCTION(func_8003875c)
{
    #include "asm/8003875c.s"
}

asm UNKNOWN_FUNCTION(func_800387d8)
{
    #include "asm/800387d8.s"
}
