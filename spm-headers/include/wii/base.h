#pragma once

#include <common.h>

CPP_WRAPPER(wii::base)

#define FPSCR_FX     0x80000000
#define FPSCR_FEX    0x40000000
#define FPSCR_VX     0x20000000
#define FPSCR_OX     0x10000000
#define FPSCR_UX     0x8000000
#define FPSCR_ZX     0x4000000
#define FPSCR_XX     0x2000000
#define FPSCR_VXSNAN 0x1000000
#define FPSCR_VXISI  0x800000
#define FPSCR_VXIDI  0x400000
#define FPSCR_VXZDZ  0x200000
#define FPSCR_VXIMZ  0x100000
#define FPSCR_VXVC   0x80000
#define FPSCR_FR     0x40000
#define FPSCR_FI     0x20000
#define FPSCR_FPRF   0x1f000
#define FPSCR_UNK20  0x800
#define FPSCR_VXSOFT 0x400
#define FPSCR_VXSQRT 0x200
#define FPSCR_VXCVI  0x100
#define FPSCR_VE     0x80
#define FPSCR_OE     0x40
#define FPSCR_UE     0x20
#define FPSCR_ZE     0x10
#define FPSCR_XE     0x8
#define FPSCR_NI     0x4
#define FPSCR_RN     0x3

#define MSR_POW 0x40000
#define MSR_ILE 0x10000
#define MSR_EE  0x8000
#define MSR_PR  0x4000
#define MSR_FP  0x2000
#define MSR_ME  0x1000
#define MSR_FE0 0x800
#define MSR_SE  0x400
#define MSR_BE  0x200
#define MSR_FE1 0x100
#define MSR_IP  0x40
#define MSR_IR  0x20
#define MSR_DR  0x10
#define MSR_PM  0x4
#define MSR_RI  0x2
#define MSR_LE  0x1

u32 PPCMfmsr();
void PPCMtmsr(u32);

u32 PPCMfhid0();
void PPCMthid0(u32);

u32 PPCMfl2cr();
void PPCMtl2cr(u32);

void PPCMtdec(u32);

void PPCSync();
void PPCHalt();

void PPCMtmmcr0(u32);

u32 PPCMfmmcr1();

void PPCMtpmc1(u32);

void PPCMtpmc2(u32);

void PPCMtpmc3(u32);

void PPCMtpmc4(u32);

u32 PPCMffpscr();
void PPCMtfpscr(u32);

u32 PPCMfhid2();
void PPCMthid2(u32);

u32 PPCMfwpar();
void PPCMtwpar(u32);

void PPCDisableSpeculation();
void PPCSetFpNonIEEEMode();

void PPCMthid4(u32);

CPP_WRAPPER_END()
