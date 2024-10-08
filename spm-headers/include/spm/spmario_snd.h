#pragma once

#include <common.h>

// TODO: C++

CPP_WRAPPER(spm::spmario_snd)

void spsndInit();
void spsndMain();
void spsndExit();
UNKNOWN_FUNCTION(spsndPause);
UNKNOWN_FUNCTION(spsndUnpause);
UNKNOWN_FUNCTION(func_8023872c);
UNKNOWN_FUNCTION(spsndSetPosDirListener);
void spsndSetSFXReverbMode(u8 mode);
UNKNOWN_FUNCTION(func_80238804);
UNKNOWN_FUNCTION(func_80238868);
UNKNOWN_FUNCTION(spsndSetFlag);
void spsndClearFlag(u16 flags);
UNKNOWN_FUNCTION(spsndGetFlag);
UNKNOWN_FUNCTION(func_802388f4);
UNKNOWN_FUNCTION(func_80238b04);
UNKNOWN_FUNCTION(spsndSFXMain);
UNKNOWN_FUNCTION(__spsndSFXOn);
s32 spsndSFXOn(const char * name);
UNKNOWN_FUNCTION(spsndSFXOnVol);
UNKNOWN_FUNCTION(spsndSFXOn_UnkEffect);
UNKNOWN_FUNCTION(_spsndSFXOn);
UNKNOWN_FUNCTION(spsndSFXOn_3D);
UNKNOWN_FUNCTION(spsndSFXOff);
void func_8023b38c(u32 param_1, u32 param_2);
UNKNOWN_FUNCTION(spsndSetSfxPlayerPos);
UNKNOWN_FUNCTION(spsndSFX_vol);
UNKNOWN_FUNCTION(spsndSFX_pit);
UNKNOWN_FUNCTION(spsndSFX_delay);
UNKNOWN_FUNCTION(func_8023b680);
UNKNOWN_FUNCTION(func_8023b77c);
UNKNOWN_FUNCTION(func_8023b858);
UNKNOWN_FUNCTION(func_8023b974);
UNKNOWN_FUNCTION(spsndSFX_flagOn);
UNKNOWN_FUNCTION(spsndSFX_flagOff);
UNKNOWN_FUNCTION(spsndSFX_chkName);
UNKNOWN_FUNCTION(spsndSFX_chk);
UNKNOWN_FUNCTION(spsndSFX_getIdPlayingName);
UNKNOWN_FUNCTION(spsndGetSFXEntry);
UNKNOWN_FUNCTION(spsndBGMMain);
UNKNOWN_FUNCTION(spsndBGMOn_f_d);
bool spsndBGMOn(u32 flags, const char * name);
UNKNOWN_FUNCTION(spsndBGMOn_f_d_alt);
bool spsndBGMOff_f_d(s32 player, u32 fadeoutTime);
UNKNOWN_FUNCTION(spsndBGMOff);
bool spsndBGMOff_f_d_alt(s32 player, s32 fadeoutTime);
UNKNOWN_FUNCTION(func_8023cab4);
UNKNOWN_FUNCTION(func_8023cc90);
UNKNOWN_FUNCTION(func_8023cc98);
UNKNOWN_FUNCTION(func_8023cc9c);
UNKNOWN_FUNCTION(func_8023ce1c);
void func_8023ce20(s32 player, s32 param_2, s32 param_3);
UNKNOWN_FUNCTION(func_8023cf14);
UNKNOWN_FUNCTION(func_8023cfe8);
UNKNOWN_FUNCTION(func_8023d0dc);
bool spsndCheckBgmPlaying(s32 player);
UNKNOWN_FUNCTION(spsndGetBgmName);
UNKNOWN_FUNCTION(func_8023d1f8);
UNKNOWN_FUNCTION(func_8023d218);
UNKNOWN_FUNCTION(func_8023d2b8);
UNKNOWN_FUNCTION(spsndENVMain);
UNKNOWN_FUNCTION(spsndENVOn_f_d);
UNKNOWN_FUNCTION(spsndENVOn);
bool func_8023db5c(s32 param_1, s32 param_2);
UNKNOWN_FUNCTION(func_8023dc88);
bool func_8023dc90(s32 param_1, s32 param_2);
UNKNOWN_FUNCTION(func_8023dc94);
UNKNOWN_FUNCTION(func_8023dda4);
UNKNOWN_FUNCTION(func_8023de90);
UNKNOWN_FUNCTION(func_8023dfa0);
UNKNOWN_FUNCTION(func_8023e08c);

CPP_WRAPPER_END()
