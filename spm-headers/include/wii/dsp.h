#pragma once

#include <common.h>

CPP_WRAPPER(wii::dsp)

#define DSP_DSPMBOX_H 0
#define DSP_DSPMBOX_L 1
#define DSP_CPUMBOX_H 2
#define DSP_CPUMBOX_L 3
#define DSP_REG_0x8 4
#define DSP_CSR 5
#define DSP_REG_0xC 6
#define DSP_REG_0xE 7
#define DSP_REG_0x10 8
#define DSP_AR_SIZE 9
#define DSP_REG_0x14 10
#define DSP_AR_MODE 11
#define DSP_REG_0x18 12
#define DSP_AR_REFRESH 13
#define DSP_REG_0x1C 14
#define DSP_REG_0x1E 15
#define DSP_AR_DMA_MMADDR_H 16
#define DSP_AR_DMA_MMADDR_L 17
#define DSP_AR_DMA_ARADDR_H 18
#define DSP_AR_DMA_ARADDR_L 19
#define DSP_AR_DMA_CNT_H 20
#define DSP_AR_DMA_CNT_L 21
#define DSP_REG_0x2C 22
#define DSP_REG_0x2E 23
#define DSP_AI_DMA_START_H 24
#define DSP_AI_DMA_START_L 25
#define DSP_REG_0x34 26
#define DSP_AI_DMA_CSR 27
#define DSP_REG_0x38 28
#define DSP_AI_DMA_BYTES_LEFT 29

FIXED_ADDR(volatile u16, DSP_HW_REGS[], 0xCC005000);

CPP_WRAPPER_END()
