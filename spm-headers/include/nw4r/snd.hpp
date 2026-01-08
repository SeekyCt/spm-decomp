#pragma once

#include <common.h>

CPP_WRAPPER(nw4r::snd)

 enum OffsetType { OFFSET_TYPE_TICK, OFFSET_TYPE_MILLISEC };

typedef struct { /* Size is uncertain */
/* 0x000 */ int field0_0x0;
/* 0x004 */ u8 unknown_0x04[0xc8 - 0x04];
/* 0x0C8 */ u8 loopCounter;
/* 0x0C9 */ u8 unknown_0xc9[0xe4 - 0xc9];
/* 0x0E4 */ s32 bufferCheck2;
/* 0x0E8 */ s32 progress;
/* 0x0EC */ u8 unknown_0xec[0xf0 - 0xec];
/* 0x0F0 */ s32 bufferCheck1;
/* 0x0F4 */ s32 progressLoop;
/* 0x0F8 */ u8 unknown_0xf8[0x11c - 0xf8];
} BasicPlayer;
SIZE_ASSERT(BasicPlayer, 0x11c)

typedef struct __vt_BasicSound {
    u8 unknown_0x0[0x8 - 0x0];
    void* GetRuntimeTypeInfo;
    void* _dt;
    void* Update;
    void* StartPrepared;
    void* Stop;
    void* Pause;
    void* SetAutoStopCounter;
    void* FadeIn;
    void* Shutdown;
    void* IsPrepared;
    void* IsPause;
    void* SetInitialVolume;
    void* SetVolume;
    void* SetPitch;
    void* SetPan;
    void* SetSurroundPan;
    void* SetLpfFreq;
    void* SetPlayerPriority;
    void* SetRemoteFilter;
    void* SetPanMode;
    void* SetPanCurve;
    void* IsAttachedTempSpecialHandle;
    void* DetachTempSpecialHandle;
    void* InitParam;
    BasicPlayer* GetBasicPlayer;
    BasicPlayer* GetBasicPlayer2;
} __vt_BasicSound;
SIZE_ASSERT(__vt_BasicSound, 0x70)

typedef struct { /* Size is uncertain */
    __vt_BasicSound *__vt;
    u8 unknown_0x4[0x67 - 0x4];
    bool isPaused; 
    u8 unknown_0x68[0x70 - 0x68];
    s32 progress;
    u8 unknown_0x74[0xd7 - 0x74];
} Snd_BasicSound;
SIZE_ASSERT(Snd_BasicSound, 0xd8)

// TODO: these are c++ functions used in spmario.c

extern UNKNOWN_FUNCTION(func_802f4d0c) // nw4r::snd::detail::AxManager::getInstance
extern UNKNOWN_FUNCTION(func_802f54a0) // nw4r::snd::detail::AxManager::SetMasterVolume

void UpdateLoadingBlockIndex(BasicPlayer *player);
void Skip(BasicPlayer *player, OffsetType type, s32 offset);
CPP_WRAPPER_END()
