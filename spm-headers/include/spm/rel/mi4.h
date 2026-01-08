#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <wii/gx.h>

CPP_WRAPPER(spm::mi4)

USING(wii::gx::GXTexObj)
USING(spm::evtmgr::EvtEntry)

void mi4MimiHolographicEffect(GXTexObj * tex, EvtEntry * evtEntry);
// more

CPP_WRAPPER_END()
