#pragma once

#include <common.h>
#include <wii/os/OSContext.h>

CPP_WRAPPER(wii::os)

USING(wii::os::OSContext)

#define DI_DMA_MMADDR 5

FIXED_ADDR(volatile u32, DI_HW_REGS[], 0xcd006000);

FIXED_ADDR(volatile OSContext *, OS_CURRENT_FPU_CONTEXT, 0x800000d8);

CPP_WRAPPER_END()
