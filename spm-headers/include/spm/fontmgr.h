/*
    fontmgr allows for text to be drawn on the screen
*/

#pragma once

#include <common.h>
#include <wii/gx.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::fontmgr)

USING(wii::gx::GXColor)
USING(wii::mtx::Mtx34)
USING(wii::mtx::Vec3)

DECOMP_STATIC(GXColor fontmgr_fontColor) // Color of text, alpha ignored
DECOMP_STATIC(u8 fontmgr_fontAlpha) // Alpha of text
DECOMP_STATIC(s32 fontmgr_fontEdge) // Whether the font edge is turned on
DECOMP_STATIC(u8 fontmgr_lbl_805ae994)
DECOMP_STATIC(void * fontmgr_pfh)
DECOMP_STATIC(u32 fontmgr_lbl_805ae99c)
DECOMP_STATIC(u32 fontmgr_lbl_805ae9a0)
DECOMP_STATIC(f32 fontmgr_fontScale[3]) // Scale of text

/*
    Loads the font from disc
*/
void fontmgrInit();

/*
    Sets up drawing a new string
*/  
void FontDrawStart();

/*
    Sets up drawing a new string with custom transparency
    (setting the alpha in FontDrawColor's GXColor doesn't work correctly)
*/
void FontDrawStart_alpha(u8 alpha);

/*
    Enable / disable drawing a border around text
*/
void FontDrawEdge();
void FontDrawEdgeOff();

/*
    Control text changing colour over time
*/
void FontDrawRainbowColor();
void FontDrawRainbowColorOff();

/*
    Control noise of text
*/
void FontDrawNoise();
void FontDrawNoiseOff();

/*
    Set the text colour to a pre-defined or custom one
*/
void FontDrawColorIDX(u32 idx);
void FontDrawColor(GXColor * color); // Warning: Overwrites color.a

void FontDrawColor_();

/*
    Returns the current text colour
*/
GXColor FontGetDrawColor();

/*
    Sets the text scale
*/
void FontDrawScale(f32 scale);

void FontDrawCode(f32 x, f32 y, u16 code);
void FontDrawCodeMtx(Mtx34 matrix, u16 code);

/*
    Draws a string of text at a position
*/
void FontDrawString(f32 x, f32 y, const char * string);

void FontDrawStringVecPitch(Vec3 position, f32 pitch, const char * string);
void FontDrawStringMtx(Mtx34 matrix, const char * string);
void FontDrawStringCenterMtx(Mtx34 matrix, const char * string);
void FontDrawMessage(s32 x, s32 y, const char * message);
void FontDrawMessageMtx(Mtx34 matrix, const char * message);
u16 hankakuSearch(u8 code);
u16 kanjiSearch(u16 code);
u8 kanjiGetWidth(u16 code);
u16 FontGetMessageWidthLine(const char * message, u16 * outlines);

/*
    Returns the width a string will cover when drawn
*/
u16 FontGetMessageWidth(const char * message);

u32 HSV2RGB(u8 hsv[3]);

UNKNOWN_FUNCTION(JUTFontSetup);
UNKNOWN_FUNCTION(func_801a2ebc);
UNKNOWN_FUNCTION(func_801a2ec4);
UNKNOWN_FUNCTION(JUTFont_CodeToGlyph);
UNKNOWN_FUNCTION(JUTFont_DrawStart);
UNKNOWN_FUNCTION(_JUTFont_DrawPos);

CPP_WRAPPER_END()
