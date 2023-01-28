#pragma once

#include <common.h>
#include <wii/gx.h>

CPP_WRAPPER(wii::os)

USING(wii::gx::GXColor)

UNKNOWN_FUNCTION(ScreenReport);
UNKNOWN_FUNCTION(ConfigureVideo);
void OSFatal(const GXColor * fg, const GXColor * bg, const char * message) NORETURN;
UNKNOWN_FUNCTION(Halt);

CPP_WRAPPER_END()
