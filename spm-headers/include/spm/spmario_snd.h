#pragma once

#include <common.h>
#include <nw4r/snd.hpp>
#include <wii/mtx.h>

// TODO: C++

CPP_WRAPPER(spm::spmario_snd)

USING(nw4r::snd::Snd_BasicSound)
USING(wii::mtx::Vec3)

typedef struct Snd_SoundHandle {
    Snd_BasicSound *mSound;
} Snd_SoundHandle;
SIZE_ASSERT(Snd_SoundHandle, 0x4)

typedef struct {
    const char *name;
    short brsarIdx; /* file param 6 */
    u16 fileParam1; /* file param 1 */
    u8 volume; /* file param 2 */
    u8 pan; /* file param 3 */
    u8 flags; /* file param 4 */
    u8 otoyaId; /* file param 5 */
} BgmEntry;
SIZE_ASSERT(BgmEntry, 0xc)

typedef struct {
    BgmEntry *bgmEntry;
    Snd_SoundHandle soundHandle;
    u8 field2_0x8;
    u8 pan;
    u8 unknown_0xa[0xe - 0xa];
    u16 flags;
    s16 field8_0x10;
    s16 field9_0x12;
    s16 field10_0x14;
    s16 fadeoutFrames;
    u8 unknown_0x18[0x27 - 0x18];
} BgmPlayer;
SIZE_ASSERT(BgmPlayer, 0x28)

extern BgmPlayer spsndBgmPlayers[3];

void spsndInit();
void spsndMain();
void spsndExit();
UNKNOWN_FUNCTION(spsndPause)
UNKNOWN_FUNCTION(spsndUnpause)
UNKNOWN_FUNCTION(func_8023872c)
UNKNOWN_FUNCTION(spsndSetPosDirListener)
void spsndSetSFXReverbMode(u8 mode);
UNKNOWN_FUNCTION(func_80238804)
UNKNOWN_FUNCTION(func_80238868)
UNKNOWN_FUNCTION(spsndSetFlag)
void spsndClearFlag(u16 flags);
UNKNOWN_FUNCTION(spsndGetFlag)
UNKNOWN_FUNCTION(func_802388f4)
UNKNOWN_FUNCTION(func_80238b04)
UNKNOWN_FUNCTION(spsndSFXMain)
s32 __spsndSFXOn(const char * name, u8 volume, u8 pan, s32 delay, Vec3 * position, u32 param_6);
void spsndSFXOn(const char * name);
void spsndSFXOnVol(const char * name, u8 volume);
UNKNOWN_FUNCTION(spsndSFXOn_UnkEffect)
u32 _spsndSFXOn(const char * name, Vec3 * position, u32 param_3);
s32 spsndSFXOn_3D(const char * name, Vec3 * position); // Thunks to _spsndSFXOn, technically void on its own
void spsndSFXOff(s32 player);
void func_8023b38c(u32 param_1, u32 param_2);
void spsndSetSfxPlayerPos(u32 player, Vec3 * position);
UNKNOWN_FUNCTION(spsndSFX_vol)
UNKNOWN_FUNCTION(spsndSFX_pit)
UNKNOWN_FUNCTION(spsndSFX_delay)
UNKNOWN_FUNCTION(func_8023b680)
UNKNOWN_FUNCTION(func_8023b77c)
UNKNOWN_FUNCTION(func_8023b858)
UNKNOWN_FUNCTION(func_8023b974)
UNKNOWN_FUNCTION(spsndSFX_flagOn)
UNKNOWN_FUNCTION(spsndSFX_flagOff)
UNKNOWN_FUNCTION(spsndSFX_chkName)
UNKNOWN_FUNCTION(spsndSFX_chk)
UNKNOWN_FUNCTION(spsndSFX_getIdPlayingName)
UNKNOWN_FUNCTION(spsndGetSFXEntry)
UNKNOWN_FUNCTION(spsndBGMMain)
s32 spsndBGMOn_f_d(u32 flags, const char * name, s32 fadeMsec);
bool spsndBGMOn(u32 flags, const char * name);
UNKNOWN_FUNCTION(spsndBGMOn_f_d_alt)
bool spsndBGMOff_f_d(s32 player, u32 fadeoutTime);
UNKNOWN_FUNCTION(spsndBGMOff)
bool spsndBGMOff_f_d_alt(s32 player, s32 fadeoutTime);
s32 spsndBGMFadeout(s32 player, s32 fadeMsec);
UNKNOWN_FUNCTION(func_8023cc90)
UNKNOWN_FUNCTION(func_8023cc98)
s32 spsndBGMFadein(s32 player, s32 fadeMsec);
UNKNOWN_FUNCTION(func_8023ce1c)
void func_8023ce20(s32 player, s32 param_2, s32 param_3);
UNKNOWN_FUNCTION(func_8023cf14)
void spsndBGMSetVol(s32 player, s32 volume, u32 fadeMsec);
UNKNOWN_FUNCTION(func_8023d0dc)
bool spsndCheckBgmPlaying(s32 player);
UNKNOWN_FUNCTION(spsndGetBgmName)
UNKNOWN_FUNCTION(func_8023d1f8)
UNKNOWN_FUNCTION(func_8023d218)
UNKNOWN_FUNCTION(func_8023d2b8)
UNKNOWN_FUNCTION(spsndENVMain)
UNKNOWN_FUNCTION(spsndENVOn_f_d)
UNKNOWN_FUNCTION(spsndENVOn)
bool func_8023db5c(s32 param_1, s32 param_2);
UNKNOWN_FUNCTION(func_8023dc88)
bool func_8023dc90(s32 param_1, s32 param_2);
UNKNOWN_FUNCTION(func_8023dc94)
UNKNOWN_FUNCTION(func_8023dda4)
UNKNOWN_FUNCTION(func_8023de90)
UNKNOWN_FUNCTION(func_8023dfa0)
UNKNOWN_FUNCTION(func_8023e08c)

CPP_WRAPPER_END()
