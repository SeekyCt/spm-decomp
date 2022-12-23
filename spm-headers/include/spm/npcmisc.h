#include <common.h>
#include <spm/npcdrv.h>

// TODO: this isn't a proper file, figure out what it really is

CPP_WRAPPER(spm::npcmisc)

USING(spm::npcdrv::NPCEntry)

void npcDropItem(NPCEntry * npc, s32 itemType, s32 coinCount);

CPP_WRAPPER_END()
