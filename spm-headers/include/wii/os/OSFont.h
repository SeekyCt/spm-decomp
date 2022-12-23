#pragma once

#include <common.h>

CPP_WRAPPER(wii::os)

UNKNOWN_FUNCTION(GetFontCode);
UNKNOWN_FUNCTION(Decode);
s32 OSGetFontEncode();
UNKNOWN_FUNCTION(OSSetFontEncode);
UNKNOWN_FUNCTION(ReadFont);
void OSLoadFont(void * fontHeader, void * temp);
UNKNOWN_FUNCTION(ParseStringS);
UNKNOWN_FUNCTION(ParseStringW);
char * OSGetFontTexel(const char * str, void * image, s32 pos, s32 stride, s32 * width);

CPP_WRAPPER_END()
