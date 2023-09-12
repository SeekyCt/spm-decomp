/*
    npcdrv handles the spawning, updating and deleting of NPCs ('actors') in the game
    NPCS include enemies, friendly characters and some projectiles, but not players or MOBJ
*/

#pragma once

#include <common.h>
#include <spm/evtmgr.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::npcdrv)

USING(spm::evtmgr::EvtScriptCode)
USING(wii::mtx::Vec3)

#define NPCTRIBE_MAX 535
#define NPCTEMPLATE_MAX 435

typedef struct
{
/* 0x0 */ u8 type;
/* 0x1 */ u8 defense;
/* 0x2 */ u16 flags;
} NPCDefense;
SIZE_ASSERT(NPCDefense, 0x4)

struct _NPCPart;
typedef void (NPCPartUpdateFunc)(struct _NPCPart * part, Unk);

typedef struct
{
/* 0x0 */ s32 id;
/* 0x4 */ const char * animName;
} NPCTribeAnimDef;
SIZE_ASSERT(NPCTribeAnimDef, 0x8)

typedef struct
{
/* 0x00 */ u16 id;
/* 0x02 */ u8 unknown_0x2[0x28 - 0x2];
/* 0x28 */ NPCDefense * defenses;
/* 0x2C */ Unk * unknown_0x2c;
/* 0x30 */ NPCPartUpdateFunc * updateFunc;
/* 0x34 */ Unk * unknown_0x34;
/* 0x38 */ NPCTribeAnimDef * animDefs;
/* 0x3C */ u8 unknown_0x3c[0x48 - 0x3c];
} NPCPartDef;
SIZE_ASSERT(NPCPartDef, 0x48)

typedef struct
{
/* 0x0 */ s32 itemId;
/* 0x4 */ s32 chance;
} NPCDropItem;
SIZE_ASSERT(NPCDropItem, 0x8)

typedef struct
{
/* 0x00 */ const char * animPoseName;
/* 0x04 */ NPCTribeAnimDef * animDefs; // list terminated by one with id -1
/* 0x08 */ s32 catchCardItemId;
/* 0x0C */ s16 catchCardDefense;
/* 0x0E */ u8 unknown_0xe[0x18 - 0xe];
/* 0x18 */ u8 maxHp;
/* 0x19 */ u8 partsCount;
/* 0x1A */ // padding 0x1a-1b
/* 0x1C */ NPCPartDef * partsList; // partsCount length
/* 0x20 */ u8 unknown_0x20[0x38 - 0x20];
/* 0x38 */ s16 killXp;
/* 0x40 */ u8 unknown_0x3a[0x46 - 0x3a];
/* 0x46 */ u16 coinDropChance; // chance of dropping any coins at all, percentage
/* 0x48 */ u16 coinDropBaseCount; // minimum amount of coins to drop, if any are dropping
/* 0x4A */ u16 coinDropExtraChance; // chance for each extra coin to drop, percentage
/* 0x4C */ u16 coinDropExtraMax; // maximum amount of extra coins to drop on top of base count
/* 0x4E */ u16 dropItemChance; // chance of dropping any item, percentage
/* 0x50 */ NPCDropItem * dropItemList; // terminated by an entry with id 0
/* 0x54 */ u8 unknown_0x54[0x64 - 0x54];
/* 0x64 */ u8 attackStrength; // only used for the tattle and turn-based combat, doesn't affect normal damage
/* 0x65 */ u8 unknown_0x65[0x68 - 0x65];
} NPCTribe;
SIZE_ASSERT(NPCTribe, 0x68)

typedef struct
{
/* 0x00 */ s32 m_nPoseId;
/* 0x04 */ char animPoseName[32];
/* 0x24 */ u8 unknown_0x24[0x48 - 0x24];
/* 0x48 */ NPCTribeAnimDef * tribeAnims;
/* 0x4C */ // unknown 0x4c+
} NPCAnim; // unknown size
OFFSET_ASSERT(NPCAnim, tribeAnims, 0x48)

typedef struct _NPCPart
{
/* 0x000 */ u16 id;
/* 0x002 */ u8 unknown_0x2[0x2c - 0x2];
/* 0x02C */ u32 flag2c;
/* 0x030 */ u32 flag30;
/* 0x034 */ u8 unknown_0x34[0x378 - 0x34];
/* 0x378 */ s32 attackPower; // initialised as 1, changed by onSpawnScript if needed
/* 0x37C */ u8 unknown_0x37c[0x388 - 0x37c];
/* 0x388 */ struct _NPCEntry * owner;
/* 0x38C */ struct _NPCPart * prevPart;
/* 0x390 */ struct _NPCPart * nextPart;
/* 0x394 */ u8 unknown_0x394[0x398 - 0x394];
} NPCPart;
SIZE_ASSERT(NPCPart, 0x398)

typedef struct _NPCEntry
{
/* 0x000 */ s32 id;
/* 0x004 */ s32 setupFileIndex; // 1-based index, 0 if not spawned from a setup file
    /*
        0x1 is active
        Others unknown
    */
/* 0x008 */ u32 flag8;
/* 0x00C */ u32 flagC;
/* 0x010 */ u32 flag10;
/* 0x014 */ u8 unknown_0x14[0x24 - 0x14];
/* 0x024 */ char name[32]; // name of this instance, npc_XXXXXXXX for template-spawned ones
                           // where XXXXXXXX is id in hex
/* 0x044 */ NPCAnim m_Anim; // unknown size
/* ????? */ u8 unknown_unk[0x2a0 - 0x44 - sizeof(NPCAnim)];
/* 0x2A0 */ Vec3 position;
/* 0x2AC */ u8 unknown_0x2ac[0x348 - 0x2ac];
/* 0x348 */ EvtScriptCode * templateUnkScript1; // unkScript1 from spawning SetupEnemyTemplate
                                                // (unknown for non-templated NPCs)
/* 0x34C */ u8 unknown_0x34c[0x360 - 0x34c];
/* 0x360 */ EvtScriptCode * templateUnkScript2; // unkScript2 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x364 */ EvtScriptCode * templateUnkScript3; // unkScript3 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x368 */ EvtScriptCode * templateUnkScript4; // unkScript4 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x36C */ EvtScriptCode * templateUnkScript5; // unkScript5 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x370 */ EvtScriptCode * templateUnkScript9; // unkScript9 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x374 */ EvtScriptCode * templateUnkScript6; // unkScript6 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x378 */ EvtScriptCode * templateUnkScript7; // unkScript7 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x37C */ EvtScriptCode * templateUnkScript8; // unkScript8 from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x380 */ u8 unknown_0x380[0x390 - 0x380];
/* 0x390 */ s32 onSpawnEvtId; // id of the EvtEntry running a templated npc's onSpawn scripts
                              // (unknown for non-templated NPCs)
/* 0x394 */ u8 unknown_0x394[0x39c - 0x394];
/* 0x39C */ f32 tribeField0xE; // field 0xe of spawning NPCTribe cast to float
/* 0x3A0 */ f32 tribeField0x10; // field 0x10 of spawning NPCTribe cast to float
/* 0x3A4 */ f32 tribeField0x12; // field 0x12 of spawning NPCTribe cast to float
/* 0x3A8 */ u8 unknown_0x3a8[0x3ac - 0x3a8];
/* 0x3AC */ f32 unknown_0x3ac;
/* 0x3B0 */ u8 unknown_0x3b0[0x46c - 0x3b0];
    /*
        0x80000000 is frozen
        0x40000 is on different pane to Mario
        0x20000 is hidden & frozen
    */
/* 0x46C */ u32 flag46C;
/* 0x470 */ u8 unknown_0x470[0x478 - 0x470];
/* 0x478 */ u32 tribeField0x54; // field 0x54 of spawning NPCTribe
/* 0x47C */ u8 unknown_0x47c[0x49c - 0x47c];
/* 0x49C */ s32 tribeId; // id of the NPCTribe this NPC was spawned with
/* 0x4A0 */ s32 tribeId2; // seemingly just a copy of tribeId
/* 0x4A4 */ u8 unknown_0x4a4[0x4ec - 0x4a4];
/* 0x4EC */ u32 maxHp; // copied from spawning NPCTribe, 1 for NPCs not spawned by tribe
/* 0x4F0 */ u32 hp; // copied from spawning NPCTribe, 1 for NPCs not spawned by tribe
/* 0x4F4 */ u8 unknown_0x4f4[0x508 - 0x4f4];
/* 0x508 */ u32 axisMovementUnit;
/* 0x4F4 */ u8 unknown_0x50c[0x510 - 0x50c];
/* 0x510 */ s32 unitWork[16];
/* 0x550 */ u8 unknown_0x550[0x574 - 0x550];
/* 0x574 */ s32 dropItemId; // id of the item this npc would drop if killed (determined on spawn)
/* 0x578 */ u8 unknown_0x578[0x57c - 0x578];
/* 0x57C */ u32 templateField0x5C; // field 0x5c of spawning SetupEnemyTemplate
                                   // (unknown for non-templated NPCs)
/* 0x580 */ u32 templateField0x60; // field 0x60 of spawning SetupEnemyTemplate
                                   // (unknown for non-templated NPCs)
/* 0x584 */ u32 templateField0x64; // field 0x64 of spawning SetupEnemyTemplate
                                   // (unknown for non-templated NPCs)
/* 0x588 */ u8 unknown_0x588[0x714 - 0x588];
/* 0x714 */ NPCPart * parts; // made from tribe's NPCPartDef list, linked list
/* 0x718 */ EvtScriptCode * templateField0x58; // field 0x58 from spawning SetupEnemyTemplate
                                               // g(unknown for non-templated NPCs)
/* 0x71C */ u8 unknown_0x71c[0x748 - 0x71c];
} NPCEntry;
SIZE_ASSERT(NPCEntry, 0x748)

typedef struct
{
    /*
        0x1 is freeze all NPCs
    */
/* 0x000 */ u32 flag;
/* 0x004 */ s32 num;
/* 0x008 */ NPCEntry * entries;
/* 0x00C */ u8 unknown_0xc[0x14 - 0xc];
/* 0x014 */ s32 nextTemplateNpcId;
/* 0x018 */ Unk * setupFile; // always updated to be v6 on load
/* 0x01C */ u8 unknown_0x1c[0x728 - 0x1c];
/* 0x728 */ u8 unknown_0x728[0x400];
/* 0xB28 */ u8 unknown_0xb28[0xb48 - 0xb28];
} NPCWork;
SIZE_ASSERT(NPCWork, 0xb48)

DECOMP_STATIC(NPCWork npcdrv_work)
DECOMP_STATIC(NPCWork * npcdrv_wp)

typedef bool (EnemyCanSpawnFunction)();

typedef struct
{
/* 0x00 */ u8 unknown_0x0;
/* 0x01 */ u8 unknown_0x1;
/* 0x02 */ u8 unknown_0x2;
/* 0x03 */ u8 unknown_0x3;
/* 0x04 */ s32 instanceId; // left blank to be copied from SetupEnemy
/* 0x08 */ u32 unknown_0x8;
/* 0x0C */ EnemyCanSpawnFunction * canSpawnFunction;
/* 0x10 */ u8 unknown_0x10[0x14 - 0x10];
/* 0x14 */ s32 tribeId;
/* 0x18 */ const char * instanceName; // overrides instanceName of NPC spawned if not null
/* 0x1C */ const char * japaneseName; // unused debug information?
/* 0x20 */ Vec3 pos; // left blank to be copied from SetupEnemy
/* 0x2C */ u32 flags; // ORd with NPCEntry's flags after spawning
/* 0x30 */ EvtScriptCode * onSpawnScript;
/* 0x34 */ EvtScriptCode * unkScript1;
/* 0x38 */ EvtScriptCode * unkScript2;
/* 0x3C */ EvtScriptCode * unkScript3;
/* 0x40 */ EvtScriptCode * unkScript4;
/* 0x44 */ EvtScriptCode * unkScript5;
/* 0x48 */ EvtScriptCode * unkScript6;
/* 0x4C */ EvtScriptCode * unkScript7;
/* 0x50 */ EvtScriptCode * unkScript8;
/* 0x54 */ EvtScriptCode * unkScript9;
/* 0x58 */ void * unknown_0x58;
/* 0x5C */ u8 unknown_0x5c[0x68 - 0x5c]; // all left blank to be copied from SetupEnemy
} NPCEnemyTemplate;
SIZE_ASSERT(NPCEnemyTemplate, 0x68)

extern NPCTribe npcTribes[535];
extern NPCEnemyTemplate npcEnemyTemplates[NPCTEMPLATE_MAX];

UNKNOWN_FUNCTION(npcSfxOn);
UNKNOWN_FUNCTION(func_801a96a4);
void npcInit();
void npcReset();
UNKNOWN_FUNCTION(npcFlag2On);
void npcMain();
UNKNOWN_FUNCTION(func_801aabe4);
UNKNOWN_FUNCTION(func_801aaca4);
UNKNOWN_FUNCTION(func_801aada8);
UNKNOWN_FUNCTION(npcHitMain);
UNKNOWN_FUNCTION(func_801ad058);
UNKNOWN_FUNCTION(func_801adda4);
void npcDispMain();
UNKNOWN_FUNCTION(func_801aef10);
UNKNOWN_FUNCTION(func_801aefd0);
UNKNOWN_FUNCTION(func_801af4fc);
UNKNOWN_FUNCTION(func_801afefc);
UNKNOWN_FUNCTION(func_801b000c);
UNKNOWN_FUNCTION(func_801b0154);
UNKNOWN_FUNCTION(func_801b0264);
UNKNOWN_FUNCTION(func_801b0480);
UNKNOWN_FUNCTION(func_801b0d10);
UNKNOWN_FUNCTION(func_801b0ed4);
UNKNOWN_FUNCTION(func_801b19e4);
UNKNOWN_FUNCTION(func_801b20dc);
UNKNOWN_FUNCTION(func_801b2460);
UNKNOWN_FUNCTION(func_801b24c0);
UNKNOWN_FUNCTION(func_801b2574);
UNKNOWN_FUNCTION(func_801b2640);
UNKNOWN_FUNCTION(func_801b285c);
UNKNOWN_FUNCTION(func_801b2928);
UNKNOWN_FUNCTION(func_801b33a0);
UNKNOWN_FUNCTION(npcDispSub);
UNKNOWN_FUNCTION(func_801b3860);
UNKNOWN_FUNCTION(mtx_setup);
UNKNOWN_FUNCTION(func_801b4514);
UNKNOWN_FUNCTION(func_801b4fcc);
UNKNOWN_FUNCTION(npcDisp);
UNKNOWN_FUNCTION(npcPartDisp);
UNKNOWN_FUNCTION(npcDisp_xlu);
UNKNOWN_FUNCTION(npcPartDisp_xlu);
UNKNOWN_FUNCTION(func_801b5118);
UNKNOWN_FUNCTION(func_801b5214);
UNKNOWN_FUNCTION(npcDisp_general);
UNKNOWN_FUNCTION(npcPartDisp_general);
UNKNOWN_FUNCTION(npcHitboxDisp);
UNKNOWN_FUNCTION(npcDisp_offscreen);
UNKNOWN_FUNCTION(npcDisp_offscreen_xlu);
NPCEntry * npcEntry(const char * instanceName, const char * animPoseName, s32 instanceId,
                    bool param_4);
NPCEntry * npcIdToPtr(s32 id);
NPCEntry * npcNameToPtr(const char * instanceName);
NPCEntry * npcNameToPtr_NoAssert(const char * instanceName);
void npcDelete(char *name);
UNKNOWN_FUNCTION(func_801b73cc);
UNKNOWN_FUNCTION(func_801b7a20);
UNKNOWN_FUNCTION(func_801b8064);
UNKNOWN_FUNCTION(func_801b84d4);
UNKNOWN_FUNCTION(func_801ba124);
UNKNOWN_FUNCTION(func_801bb6fc);
UNKNOWN_FUNCTION(func_801bba3c);
UNKNOWN_FUNCTION(func_801bc5d0);
UNKNOWN_FUNCTION(func_801bc7cc);
UNKNOWN_FUNCTION(npcDropFromCatch);
UNKNOWN_FUNCTION(func_801bcc00);
UNKNOWN_FUNCTION(func_801bcc8c);
UNKNOWN_FUNCTION(func_801bce2c);
UNKNOWN_FUNCTION(func_801bd4ac);
UNKNOWN_FUNCTION(func_801bd584);
UNKNOWN_FUNCTION(func_801bd7dc);
UNKNOWN_FUNCTION(func_801bd86c);
UNKNOWN_FUNCTION(func_801bde20);
UNKNOWN_FUNCTION(func_801be14c);
NPCEntry * npcEntryFromTemplate(NPCEnemyTemplate * enemyTemplate);
bool npcAreOnSpawnEvtsDone();
UNKNOWN_FUNCTION(func_801be6cc);
UNKNOWN_FUNCTION(func_801be7c8);
UNKNOWN_FUNCTION(func_801be9c4);
UNKNOWN_FUNCTION(func_801beed4);
UNKNOWN_FUNCTION(func_801bf554);
UNKNOWN_FUNCTION(func_801bf5cc);
UNKNOWN_FUNCTION(func_801bf60c);
UNKNOWN_FUNCTION(func_801bf6a4);
NPCEntry * npcEntryFromSetupEnemy(s32 setupFileIndex, Vec3 * pos, s32 nTemplateNo,
                                  Unk * miscSetupData);
s32 npcGetNextTemplateNpcId();
UNKNOWN_FUNCTION(func_801bfc14);
const char * npcSearchAnimDefs(NPCTribeAnimDef * defs, s32 id);
UNKNOWN_FUNCTION(npcSetStayPose);
UNKNOWN_FUNCTION(npcSetTalkPose);
UNKNOWN_FUNCTION(npcSetAnimSub);
UNKNOWN_FUNCTION(_npcSetAnim);
UNKNOWN_FUNCTION(npcSetAnim);
UNKNOWN_FUNCTION(func_801c018c);
UNKNOWN_FUNCTION(func_801c01a8);
UNKNOWN_FUNCTION(func_801c01c0);
UNKNOWN_FUNCTION(func_801c01dc);
UNKNOWN_FUNCTION(func_801c0284);
UNKNOWN_FUNCTION(npcGetDefense);
UNKNOWN_FUNCTION(npcGetScript);
UNKNOWN_FUNCTION(func_801c0454);
bool npcReadSetupFile(const char * mapName, bool reAllocate);
void npcUpdateSetupFile0To1(Unk * file);
void npcUpdateSetupFile1To2(Unk * file);
void npcUpdateSetupFile2To3(Unk * file);
void npcUpdateSetupFile3To4(Unk * file);
void npcUpdateSetupFile4To5(Unk * file);
void npcUpdateSetupFile5To6(Unk * file);
void npcUpdateSetupFile(void * file);
NPCEntry * npcGetEntries();
s32 npcGetMaxEntries();
UNKNOWN_FUNCTION(func_801c213c);
NPCPart * npcAddPart(NPCEntry * entry, NPCPartDef * partDef);
UNKNOWN_FUNCTION(func_801c23b0);
UNKNOWN_FUNCTION(npcGetPartById);
UNKNOWN_FUNCTION(func_801c2480);
UNKNOWN_FUNCTION(func_801c24cc);
UNKNOWN_FUNCTION(func_801c2558);
UNKNOWN_FUNCTION(func_801c25d8);
UNKNOWN_FUNCTION(func_801c3050);
UNKNOWN_FUNCTION(func_801c35fc);
UNKNOWN_FUNCTION(func_801c360c);
void func_801c3694(const char *, const char *);
UNKNOWN_FUNCTION(func_801c3720);
void func_801c37e4(const char *);
void func_801c3848(const char *);
UNKNOWN_FUNCTION(func_801c38a4);
UNKNOWN_FUNCTION(func_801c38b4);
UNKNOWN_FUNCTION(func_801c38c4);
UNKNOWN_FUNCTION(func_801c38d4);
UNKNOWN_FUNCTION(func_801c3930);
UNKNOWN_FUNCTION(func_801c3968);
UNKNOWN_FUNCTION(func_801c3a74);
UNKNOWN_FUNCTION(func_801c3a84);
UNKNOWN_FUNCTION(func_801c3b6c);
UNKNOWN_FUNCTION(func_801c3b80);
UNKNOWN_FUNCTION(func_801c3b94);
UNKNOWN_FUNCTION(func_801c3ba8);
UNKNOWN_FUNCTION(func_801c3bb8);
UNKNOWN_FUNCTION(func_801c3bc4);
UNKNOWN_FUNCTION(func_801c3cb4);
UNKNOWN_FUNCTION(func_801c3d94);
UNKNOWN_FUNCTION(func_801c45a4);
UNKNOWN_FUNCTION(func_801c498c);
UNKNOWN_FUNCTION(func_801c4afc);
UNKNOWN_FUNCTION(func_801c5140);
UNKNOWN_FUNCTION(func_801c65e8);
UNKNOWN_FUNCTION(func_801c7f68);
UNKNOWN_FUNCTION(func_801c7fe0);
UNKNOWN_FUNCTION(func_801c7ff0);
UNKNOWN_FUNCTION(func_801c8000);
UNKNOWN_FUNCTION(npcTimerDisp);
UNKNOWN_FUNCTION(npcHandleHitXp);
UNKNOWN_FUNCTION(npcTimerMain);
UNKNOWN_FUNCTION(func_801c85e4);
UNKNOWN_FUNCTION(func_801c85ec);
UNKNOWN_FUNCTION(func_801c8760);
UNKNOWN_FUNCTION(func_801c8b44);
UNKNOWN_FUNCTION(func_801c8b7c);
UNKNOWN_FUNCTION(func_801c8b98);
UNKNOWN_FUNCTION(func_801c8d48);
UNKNOWN_FUNCTION(func_801c8d54);
UNKNOWN_FUNCTION(func_801c8d70);
UNKNOWN_FUNCTION(npcCheckFlipFinished);
UNKNOWN_FUNCTION(func_801c9874);
UNKNOWN_FUNCTION(func_801c98f0);
UNKNOWN_FUNCTION(npcFinishFlipInstant);
UNKNOWN_FUNCTION(func_801c99b0);
UNKNOWN_FUNCTION(func_801c9a24);
NPCTribe * npcGetTribe(s32 tribeId);
NPCWork * npcGetWorkPtr();
UNKNOWN_FUNCTION(func_801c9ae4);
UNKNOWN_FUNCTION(func_801ca150);
UNKNOWN_FUNCTION(func_801ca198);
UNKNOWN_FUNCTION(func_801ca1a4);
UNKNOWN_FUNCTION(func_801ca238);
UNKNOWN_FUNCTION(func_801ca2d8);
UNKNOWN_FUNCTION(func_801ca300);
UNKNOWN_FUNCTION(func_801ca310);
UNKNOWN_FUNCTION(func_801ca3b8);
UNKNOWN_FUNCTION(func_801ca3e0);
UNKNOWN_FUNCTION(func_801ca448);
UNKNOWN_FUNCTION(func_801ca748);
UNKNOWN_FUNCTION(func_801ca768);
UNKNOWN_FUNCTION(func_801caab4);
UNKNOWN_FUNCTION(func_801cab08);
UNKNOWN_FUNCTION(func_801cab20);
UNKNOWN_FUNCTION(func_801cab2c);
UNKNOWN_FUNCTION(func_801cb268);
UNKNOWN_FUNCTION(func_801cb274);
UNKNOWN_FUNCTION(func_801cb754);
UNKNOWN_FUNCTION(func_801cbf84);
UNKNOWN_FUNCTION(func_801cbf9c);
UNKNOWN_FUNCTION(func_801cbfe8);
UNKNOWN_FUNCTION(func_801cc0a0);
UNKNOWN_FUNCTION(func_801cc0cc);
UNKNOWN_FUNCTION(npcGetKillXp);
UNKNOWN_FUNCTION(func_801cc134);
UNKNOWN_FUNCTION(func_801cc150);
UNKNOWN_FUNCTION(func_801cc644);
UNKNOWN_FUNCTION(func_801cc8d0);
UNKNOWN_FUNCTION(func_801cc9dc);
UNKNOWN_FUNCTION(func_801ccb68);
UNKNOWN_FUNCTION(func_801ccce0);
UNKNOWN_FUNCTION(func_801cce10);
UNKNOWN_FUNCTION(func_801ccf74);
UNKNOWN_FUNCTION(func_801cd124);
UNKNOWN_FUNCTION(func_801cd210);
UNKNOWN_FUNCTION(func_801cd3bc);
UNKNOWN_FUNCTION(func_801cd404);
UNKNOWN_FUNCTION(func_801cd5e8);
UNKNOWN_FUNCTION(func_801cd6d4);
UNKNOWN_FUNCTION(func_801cd82c);
UNKNOWN_FUNCTION(func_801cd834);
UNKNOWN_FUNCTION(func_801cd95c);
UNKNOWN_FUNCTION(func_801cdb84);
UNKNOWN_FUNCTION(func_801ceb08);
UNKNOWN_FUNCTION(func_801ceeac);
UNKNOWN_FUNCTION(func_801cf0a0);
UNKNOWN_FUNCTION(func_801cf16c);
UNKNOWN_FUNCTION(func_801d0278);
UNKNOWN_FUNCTION(func_801d029c);
UNKNOWN_FUNCTION(func_801d02ac);
UNKNOWN_FUNCTION(func_801d0610);
UNKNOWN_FUNCTION(func_801d0624);
UNKNOWN_FUNCTION(func_801d0640);
UNKNOWN_FUNCTION(func_801d1058);
UNKNOWN_FUNCTION(func_801d1064);
UNKNOWN_FUNCTION(func_801d1074);
UNKNOWN_FUNCTION(func_801d1084);
UNKNOWN_FUNCTION(func_801d1094);
UNKNOWN_FUNCTION(func_801d10a4);
UNKNOWN_FUNCTION(func_801d10cc);
UNKNOWN_FUNCTION(func_801d1240);
UNKNOWN_FUNCTION(func_801d1424);
UNKNOWN_FUNCTION(func_801d1580);
UNKNOWN_FUNCTION(func_801d1b9c);
UNKNOWN_FUNCTION(npcTryCatchCard);
UNKNOWN_FUNCTION(func_801d1d7c);
UNKNOWN_FUNCTION(func_801d2028);
UNKNOWN_FUNCTION(func_801d2208);
UNKNOWN_FUNCTION(func_801d2304);
UNKNOWN_FUNCTION(npcFreezeAll);
UNKNOWN_FUNCTION(func_801d2384);
UNKNOWN_FUNCTION(func_801d23cc);
UNKNOWN_FUNCTION(func_801d23d4);
UNKNOWN_FUNCTION(func_801d2554);
UNKNOWN_FUNCTION(func_801d25b8);
UNKNOWN_FUNCTION(func_801d25f4);

CPP_WRAPPER_END()
