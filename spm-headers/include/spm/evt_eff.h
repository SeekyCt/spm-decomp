#pragma once

#include <common.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::evt_eff)

enum EffSpmExplosion
{
/* 0x0 */ SPM_EXPLOSION_BOMB,
/* 0x1 */ SPM_EXPLOSION_KITTY_LASER,
/* 0x2 */ SPM_EXPLOSION_NINJOE_BOMB
};

/*
    Several variables in evt_eff are casted as multiple types and used as one or the other, depending on which effType is named.
        Args 11-14 are *never* used meaningfully.

    effType is passed through effGetSet to search for EffSet ids, which are searched casewise to run an effentry function
        Different variables are used for different purposes for each one.

    spm_explosion:
        evt_eff(const char * instanceName, const char * effType, u32 variant, f32 x, f32 y, f32 z, f32 scale)

    spm_recovery:
        evt_eff(const char * instanceName, const char * effType, f32 x, f32 y, f32 z, s32 hp)

    kemuri_test:
        evt_eff(const char * instanceName, const char * effType, u32 variant, f32 x, f32 y, f32 z, f32 scale)
*/
EVT_DECLARE_USER_FUNC(evt_eff, 14)

// evt_eff_softdelete(const char * name)
EVT_DECLARE_USER_FUNC(evt_eff_softdelete, 1)

// evt_eff_delete(const * char name)
EVT_DECLARE_USER_FUNC(evt_eff_delete, 1)

UNKNOWN_FUNCTION(func_800e617c)
UNKNOWN_FUNCTION(func_800e61b0)
UNKNOWN_FUNCTION(func_800e61e0)
UNKNOWN_FUNCTION(func_800e6210)
EVT_UNKNOWN_USER_FUNC(func_800e6250)
EVT_DECLARE_USER_FUNC(evt_eff_exclamation_question, 11)

CPP_WRAPPER_END()
