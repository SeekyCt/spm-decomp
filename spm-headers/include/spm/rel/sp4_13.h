#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <spm/npcdrv.h>
#include <evt_cmd.h>

CPP_WRAPPER(spm::sp4_13)

USING(spm::npcdrv::NPCTribeAnimDef)

char * spReturnAngryAnimPos(char *param_1);
char * func_80c5c304(char *param_1);
EVT_DECLARE_USER_FUNC(func_80c5c36c, 0)

DECOMP_STATIC(NPCTribeAnimDef spMrLStartTribeAnimDefs[1])
DECOMP_STATIC(NPCTribeAnimDef spMrLTribeAnimDefs[11])
DECOMP_STATIC(NPCTribeAnimDef spMrLTauntTribeAnimDefs[7])
DECOMP_STATIC(NPCTribeAnimDef spTheGreenThunderTribeAnimDefs[5])

EVT_DECLARE(mr_l_appear_evt)
EVT_DECLARE(brobot_appear_evt)
// more

CPP_WRAPPER_END()
