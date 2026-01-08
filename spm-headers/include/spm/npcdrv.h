/*
    npcdrv handles the spawning, updating and deleting of NPCs ('actors') in the game
    NPCS include enemies, friendly characters and some projectiles, but not players or MOBJ
*/

#pragma once

#include <common.h>
#include <spm/setup_data.h>
#include <spm/evtmgr.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::npcdrv)

USING(spm::evtmgr::EvtScriptCode)
USING(spm::setup_data::MiscSetupDataV6)
USING(wii::mtx::Vec3)

#define NPCTRIBE_MAX 535
#define NPCTEMPLATE_MAX 435

enum NPCTribeId
{
    NPC_GOOMBA = 0,
    NPC_FLIP_GOOMBA = 1,
    NPC_DARK_GOOMBA = 2,
    NPC_GLOOMBA = 3,
    NPC_SPIKED_GOOMBA = 4,
    NPC_UNUSED_SPIKED_GOOMBA = 5,
    NPC_DARK_SPIKED_GOOMBA = 6,
    NPC_PARAGOOMBA = 7,
    NPC_DARK_PARAGOOMBA = 8,
    NPC_HEADBONK_GOOMBA = 9,
    NPC_DARK_HEADBONK_GOOMBA = 10,
    NPC_KOOPA_TROOPA = 11,
    NPC_KOOPA_TROOPA_STG3 = 12,
    NPC_KOOPA_TROOPA_NO_BOUNCE = 13,
    NPC_RED_KOOPA = 14,
    NPC_FLIP_RED_KOOPA = 15,
    NPC_DARK_KOOPA = 16,
    NPC_MEGA_KOOPA = 17,
    NPC_KOOPATROL = 18,
    NPC_DARK_KOOPATROL = 19,
    NPC_PARATROOPA = 20,
    NPC_PARATROOPA_STG3 = 21,
    NPC_RED_PARATROOPA = 22,
    NPC_FLIP_RED_PARATROOPA = 23,
    NPC_DARK_PARATROOPA = 24,
    NPC_BUZZY_BEETLE = 25,
    NPC_CEILING_BUZZY_BEETLE = 26,
    NPC_SPIKE_TOP = 27,
    NPC_CEILING_SPIKE_TOP = 28,
    NPC_DARK_SPIKE_TOP = 29,
    NPC_PARABUZZY = 30,
    NPC_SPIKY_PARABUZZY = 31,
    NPC_STONE_BUZZY = 32,
    NPC_DARK_STONE_BUZZY = 33,
    NPC_LAKITU = 34,
    NPC_FLIP_LAKITU = 35,
    NPC_SPINY = 36,
    NPC_FLIP_SPINY = 37,
    NPC_DARK_SPINY = 38,
    NPC_DRY_BONES = 39,
    NPC_DRY_BONES_PROJ = 40,
    NPC_DULL_BONES = 41,
    NPC_DULL_BONES_PROJ = 42,
    NPC_DARK_DULL_BONES = 43,
    NPC_DARK_DULL_BONES_PROJ = 44,
    NPC_HAMMER_BRO = 45,
    NPC_HAMMER_BRO_PROJ = 46,
    NPC_FLIP_HAMMER_BRO = 47,
    NPC_FLIP_HAMMER_BRO_PROJ = 48,
    NPC_DARK_HAMMER_BRO = 49,
    NPC_DARK_HAMMER_BRO_PROJ = 50,
    NPC_BOOMERANG_BRO = 51,
    NPC_BOOMERANG_BRO_PROJ = 52,
    NPC_FLIP_BOOMERANG_BRO = 53,
    NPC_FLIP_BOOMERANG_BRO_PROJ = 54,
    NPC_DARK_BOOMERANG_BRO = 55,
    NPC_DARK_BOOMERANG_BRO_PROJ = 56,
    NPC_FIRE_BRO = 57,
    NPC_FIRE_BRO_PROJ = 58,
    NPC_FLIP_FIRE_BRO = 59,
    NPC_FLIP_FIRE_BRO_PROJ = 60,
    NPC_DARK_FIRE_BRO = 61,
    NPC_DARK_FIRE_BRO_PROJ = 62,
    NPC_MAGIKOOPA = 63,
    NPC_BROOM_MAGIKOOPA = 64,
    NPC_MAGIKOOPA_PROJ = 65,
    NPC_DARK_MAGIKOOPA = 66,
    NPC_DARK_BROOM_MAGIKOOPA = 67,
    NPC_BROOM_MAGIKOOPA_PROJ = 68,
    NPC_KOOPA_STRIKER = 69,
    NPC_KOOPA_STRIKER_PROJ = 70,
    NPC_TOOPA_STRIKER = 71,
    NPC_TOOPA_STRIKER_PROJ = 72,
    NPC_SOOPA_STRIKER = 73,
    NPC_SOOPA_STRIKER_PROJ = 74,
    NPC_DARK_STRIKER = 75,
    NPC_DARK_STRIKER_PROJ = 76,
    NPC_PIRANHA_PLANT = 77,
    NPC_POISON_PIRANHA = 78,
    NPC_POISON_PIRANHA_BREATH = 79,
    NPC_FROST_PIRANHA = 80,
    NPC_FROST_PIRANHA_BREATH = 81,
    NPC_BULLET_BILL = 82,
    NPC_BILL_BLASTER = 83,
    NPC_BOO = 84,
    NPC_DARK_BOO = 85,
    NPC_DARK_DARK_BOO = 86,
    NPC_ATOMIC_BOO = 87,
    NPC_GREEN_CHEEP_CHEEP = 88,
    NPC_RED_CHEEP_CHEEP = 89,
    NPC_FLIP_GREEN_CHEEP_CHEEP = 90,
    NPC_BLOOPER = 91,
    NPC_FLIP_BLOOPER = 92,
    NPC_BITTACUDA = 93,
    NPC_CLUBBA = 94,
    NPC_DARK_CLUBBA = 95,
    NPC_FUZZY = 96,
    NPC_PINK_FUZZY = 97,
    NPC_DARK_FUZZY = 98,
    NPC_BALD_CLEFT = 99,
    NPC_MOON_CLEFT = 100,
    NPC_DARK_CLEFT = 101,
    NPC_POKEY = 102,
    NPC_POKEY_PROJ = 103,
    NPC_POISON_POKEY = 104,
    NPC_POISON_POKEY_PROJ = 105,
    NPC_DARK_POKEY = 106,
    NPC_DARK_POKEY_PROJ = 107,
    NPC_SWOOPER = 108,
    NPC_RUFF_PUFF = 109,
    NPC_RUFF_PUFF_PROJ = 110,
    NPC_DARK_RUFF_PUFF = 111,
    NPC_DARK_RUFF_PUFF_PROJ = 112,
    NPC_SPANIA = 113,
    NPC_DARK_SPANIA = 114,
    NPC_CRAZEE_DAYZEE = 115,
    NPC_CRAZEE_DAYZEE_PROJ = 116,
    NPC_AMAZY_DAYZEE = 117,
    NPC_AMAZY_DAYZEE_PROJ = 118,
    NPC_DARK_DAYZEE = 119,
    NPC_DARK_DAYZEE_PROJ = 120,
    NPC_LAVA_BUBBLE = 121,
    NPC_CHAIN_CHOMP = 122,
    NPC_RED_CHOMP = 123,
    NPC_DARK_CHOMP = 124,
    NPC_SQUIGLET = 125,
    NPC_SQUIG = 126,
    NPC_SQUIG_PROJ = 127,
    NPC_SQUOG = 128,
    NPC_SQUOG_PROJ = 129,
    NPC_SQUOINKER = 130,
    NPC_SQUOINKER_PROJ = 131,
    NPC_DARK_SQUIGLET = 132,
    NPC_DARK_SQUIGLET_PROJ = 133,
    NPC_SPROING_OING = 134,
    NPC_MINI_SPROING = 135,
    NPC_BOING_OING = 136,
    NPC_MINI_BOING = 137,
    NPC_ZOING_OING = 138,
    NPC_MINI_ZOING = 139,
    NPC_DARK_SPROING_OING = 140,
    NPC_DARK_MINI_SPROING = 141,
    NPC_BOOMBOXER = 142,
    NPC_BOOMBOXER_PROJ = 143,
    NPC_BEEPBOXER = 144,
    NPC_BEEPBOXER_PROJ = 145,
    NPC_BLASTBOXER = 146,
    NPC_BLASTBOXER_PROJ = 147,
    NPC_DARK_BOOMBOXER = 148,
    NPC_DARK_BOOMBOXER_PROJ = 149,
    NPC_MISTER_I = 150,
    NPC_MISTER_I_PROJ = 151,
    NPC_RED_I = 152,
    NPC_RED_I_PROJ = 153,
    NPC_DARK_I = 154,
    NPC_DARK_I_PROJ = 155,
    NPC_SHLURP = 156,
    NPC_SHLORP = 157,
    NPC_DARK_SHLURP = 158,
    NPC_GROWMEBA = 159,
    NPC_BLOMEBA = 160,
    NPC_CHROMEBA = 161,
    NPC_DARK_GROWMEBA = 162,
    NPC_TILEOID_G = 163,
    NPC_TILEOID_B = 164,
    NPC_TILEOID_R = 165,
    NPC_TILEOID_Y = 166,
    NPC_DARK_TILEOID = 167,
    NPC_JAWBUS = 168,
    NPC_RAWBUS = 169,
    NPC_GAWBUS = 170,
    NPC_DARK_JAWBUS = 171,
    NPC_BIGMEOW = 172,
    NPC_PATROLMEOW = 173,
    NPC_AIRMEOW = 174,
    NPC_SURPRISEMEOW = 175,
    NPC_MEOWBOMB_FRANCIS = 176,
    NPC_MEOWBOMB_SURPRISEMEOW = 177,
    NPC_JELLIEN = 178,
    NPC_FOTON = 179,
    NPC_FOTON_PROJ = 180,
    NPC_WARPID = 181,
    NPC_EELIGON = 182,
    NPC_EELIGON_SEG = 183,
    NPC_HOOLIGON = 184,
    NPC_HOOLIGON_SEG = 185,
    NPC_DARK_EELIGON = 186,
    NPC_DARK_EELIGON_SEG = 187,
    NPC_LONGATOR = 188,
    NPC_LONGADILE = 189,
    NPC_DARK_LONGATOR = 190,
    NPC_CHOPPA = 191,
    NPC_COPTA = 192,
    NPC_DARK_CHOPPA = 193,
    NPC_BARRIBAD = 194,
    NPC_BARRIBAD_PROJ = 195,
    NPC_SOBARRIBAD = 196,
    NPC_SOBARRIBAD_PROJ = 197,
    NPC_DARK_BARRIBAD = 198,
    NPC_DARK_BARRIBAD_PROJ = 199,
    NPC_PIGARITHM = 200,
    NPC_HOGARITHM = 201,
    NPC_DARK_PIGARITHM = 202,
    NPC_FLORO_CRAGNIEN_TRIANGLE = 203,
    NPC_FLORO_CRAGNIEN_CIRCLE = 204,
    NPC_FLORO_SAPIEN_YELLOW = 205,
    NPC_FLORO_SAPIEN_YELLOW_HEAD = 206,
    NPC_FLORO_SAPIEN_PURPLE = 207,
    NPC_FLORO_SAPIEN_PURPLE_HEAD = 208,
    NPC_FLORO_SAPIEN_RED = 209,
    NPC_FLORO_SAPIEN_RED_HEAD = 210,
    NPC_NINJOE = 211,
    NPC_NINJOE_SHURIKEN = 212,
    NPC_NINJOE_BOMB = 213,
    NPC_NINJOHN = 214,
    NPC_NINJOHN_SHURIKEN = 215,
    NPC_NINJOHN_BOMB = 216,
    NPC_NINJERRY = 217,
    NPC_NINJERRY_SHURIKEN = 218,
    NPC_NINJERRY_BOMB = 219,
    NPC_DARK_NINJOE = 220,
    NPC_DARK_NINJOE_SHURIKEN = 221,
    NPC_DARK_NINJOE_BOMB = 222,
    NPC_SKELLOBIT = 223,
    NPC_FLIP_SKELLOBIT = 224,
    NPC_DARK_SKELLOBIT = 225,
    NPC_SPIKY_SKELLOBIT = 226,
    NPC_FLIP_SPIKY_SKELLOBIT = 227,
    NPC_DARK_SPIKY_SKELLOBIT = 228,
    NPC_SKELLOBOMBER_HEAD = 229,
    NPC_FLIP_SKELLOBOMBER_HEAD = 230,
    NPC_SKELLOBOMBER_HEAD_PROJ = 231,
    NPC_SKELLOBOMBER_WITH_SKELLOBAIT = 232,
    NPC_FLIP_SKELLOBOMBER_WITH_SKELLOBAIT = 233,
    NPC_SKELLOBAIT = 234,
    NPC_SPIKY_SKELLOBAIT = 235,
    NPC_SKELLOBAIT_BREATH = 236,
    NPC_MAGIBLOT_B = 237,
    NPC_MAGIBLOT_B_PROJ = 238,
    NPC_MAGIBLOT_R = 239,
    NPC_MAGIBLOT_R_RPOJ = 240,
    NPC_MAGIBLOT_Y = 241,
    NPC_MAGIBLOT_Y_PROJ = 242,
    NPC_DARK_MAGIBLOT = 243,
    NPC_DARK_MAGIBLOT_PROJ = 244,
    NPC_CURSYA = 245,
    NPC_BACK_CURSYA = 246,
    NPC_TECH_CURSYA = 247,
    NPC_HEAVY_CURSYA = 248,
    NPC_REVERSYA_CURSYA = 249,
    NPC_DARK_CURSYA = 250,
    NPC_DARK_TECH_CURSYA = 251,
    NPC_DARK_HEAVY_CURSYA = 252,
    NPC_DARK_REVERSYA_CURSYA = 253,
    NPC_MEGABITE = 254,
    NPC_GIGABITE = 255,
    NPC_DARK_MEGABITE = 256,
    NPC_MUTH = 257,
    NPC_MEGA_MUTH = 258,
    NPC_DARK_MUTH = 259,
    NPC_GNIP_HOWL = 260,
    NPC_GNAW_GROWL = 261,
    NPC_UNDERHAND_TYPE1 = 262,
    NPC_UNDERHAND_TYPE2 = 263,
    NPC_UNDERHAND_TYPE3 = 264,
    NPC_THWOMP = 265,
    NPC_SPINY_TROMP = 266,
    NPC_SPIKY_TROMP = 267,
    NPC_ZOMBIE_SHROOM = 268,
    NPC_GHOUL_SHROOM = 269,
    NPC_OCHUNKS_STG1 = 270,
    NPC_OCHUNKS_STG5_2 = 271,
    NPC_OCHUNKS_STG5_4 = 272,
    NPC_OCHUNKS_STG8 = 273,
    NPC_OCHUNKS_STG5_BLOCK_SMALL = 274,
    NPC_OCHUNKS_STG5_BLOCK_MEDIUM = 275,
    NPC_OCHUNKS_STG5_BLOCK_LARGE = 276,
    NPC_OCHUNKS_STG8_BLOCK_SMALL = 277,
    NPC_OCHUNKS_STG8_BLOCK_MEDIUM = 278,
    NPC_OCHUNKS_STG8_BLOCK_LARGE = 279,
    NPC_MIMI_STG2 = 280,
    NPC_MIMI_STG2_RUBEE = 281,
    NPC_MIMI_STG6 = 282,
    NPC_MIMI_STG6_RUBEE = 283,
    NPC_MIMI_STG8 = 284,
    NPC_MIMI_STG8_RUBEE = 285,
    NPC_DIMENTIO_STG3 = 286,
    NPC_DIMENTIO_STG3_CLONE = 287,
    NPC_DIMENTIO_STG3_MAGIC = 288,
    NPC_DIMENTIO_STG5 = 289,
    NPC_DIMENTIO_STG5_CLONE = 290,
    NPC_DIMENTIO_STG5_MAGIC = 291,
    NPC_DIMENTIO_STG8 = 292,
    NPC_DIMENTIO_STG8_CLONE = 293,
    NPC_DIMENTIO_STG8_MAGIC = 294,
    NPC_MR_L = 295,
    NPC_BROBOT = 296,
    NPC_BROBOT_BIG_MISSILE = 297,
    NPC_BROBOT_EYE_BEAM = 298,
    NPC_BROBOT_SMALL_MISSILE = 299,
    NPC_BROBOT_LTYPE = 300,
    NPC_BROBOT_LTYPE_MUSTACHE = 301,
    NPC_BROBOT_LTYPE_HAND = 302,
    NPC_BROBOT_LTYPE_MISSILE = 303,
    NPC_NASTASIA = 304,
    NPC_COUNT_BLECK = 305,
    NPC_BLECK_SMALL_PORTAL = 306,
    NPC_BLECK_LARGE_PORTAL = 307,
    NPC_BLECK_DUMMY = 308,
    NPC_SUPER_DIMENTIO = 309,
    NPC_SUPER_DIMENTIO_BLOCK = 310,
    NPC_SUPER_DIMENTIO_BLOCK_PROJ = 311,
    NPC_SUPER_DIMENTIO_8BIT_LUIGI = 312,
    NPC_FRACKTAIL = 313,
    NPC_WRACKTAIL = 314,
    NPC_BOWSER_STG3 = 315,
    NPC_BOWSER_STG7 = 316,
    NPC_BIG_BLOOPER = 317,
    NPC_FRANCIS = 318,
    NPC_KING_CROACUS = 319,
    NPC_CROACUS_SEGMENT_PINK = 320,
    NPC_CROACUS_SEGMENT_ORANGE = 321,
    NPC_CROACUS_SEGMENT_BLUE = 322,
    NPC_CROACUS_PROJ = 323,
    NPC_UNDERCHOMP_RED = 324,
    NPC_UNDERCHOMP_BLUE = 325,
    NPC_UNDERCHOMP_YELLOW = 326,
    NPC_BONECHILL = 327,
    NPC_BONECHILL_ICE_BULLET = 328,
    NPC_BONECHILL_ICICLE = 329,
    NPC_DARK_MARIO = 330,
    NPC_DARK_LUIGI = 331,
    NPC_DARK_PEACH = 332,
    NPC_DARK_BOWSER = 333,
    NPC_SHELL_SHOCK_SMALL = 334,
    NPC_SHELL_SHOCK_LARGE = 335,
    NPC_KINKAI = 336,
    NPC_KINKAI3 = 337,
    NPC_CHERBIL = 440,
    NPC_CHERBIL_PROJ = 441,
    NPC_POISON_CHERBIL = 442,
    NPC_POISON_CHERBIL_PROJ = 443,
    NPC_ICE_CHERBIL = 444,
    NPC_ICE_CHERBIL_PROJ = 445,
    NPC_DARK_CHERBIL = 446,
    NPC_DARK_CHERBIL_SLEEP_PROJ = 447,
    NPC_DARK_CHERBIL_ICE_PROJ = 448,
    NPC_DARK_CHERBIL_POISON_PROJ = 449,
    NPC_FRACKLE = 450,
    NPC_WRACKLE = 451,
    NPC_WRACKLE_PROJ = 452,
    NPC_UNUSED_KOOPA_STRIKER = 462,
    NPC_UNUSED_KOOPA_STRIKER_SHELL = 463,
    NPC_UNUSED_GLOOMBA = 464,
    NPC_SHADY_KOOPA = 465,
    NPC_FLIP_SHADY_KOOPA = 466,
    NPC_DARK_SHADY_KOOPA = 467,
    NPC_SHADY_PARATROOPA = 468,
    NPC_FLIP_SHADY_PARATROOPA = 469,
    NPC_FLIP_BUZZY_BEETLE = 470,
    NPC_FLIP_SPIKE_TOP = 471,
    NPC_SPINY_CEILING = 472,
    NPC_GREEN_MAGIKOOPA = 473,
    NPC_GREEN_BROOM_MAGIKOOPA = 474,
    NPC_GREEN_MAGIKOOPA_PROJ = 475,
    NPC_WHITE_MAGIKOOPA = 476,
    NPC_WHITE_BROOM_MAGIKOOPA = 477,
    NPC_WHITE_MAGIKOOPA_PROJ = 478,
    NPC_RED_MAGIKOOPA = 479,
    NPC_RED_BROOM_MAGIKOOPA = 480,
    NPC_RED_MAGIKOOPA_PROJ = 481,
    NPC_PIRANHA_PLANT_UPSIDE_DOWN = 482,
    NPC_POISON_PIRANHA_UPSIDE_DOWN = 483,
    NPC_FROST_PIRANHA_UPSIDE_DOWN = 484,
    NPC_BANDIT = 485,
    NPC_BADGE_BANDIT = 486,
    NPC_BADGE_BANDIT2 = 487,
    NPC_PIDER = 488,
    NPC_PIDER_PROJ = 489,
    NPC_ARANTULA = 490,
    NPC_ARANTULA_PROJ = 491,
    NPC_SWOOPULA = 492,
    NPC_RUFF_PUFF_UNUSED = 493,
    NPC_RUFF_PUFF_UNUSED_PROJ = 494,
    NPC_SPINIA = 495,
    NPC_SPUNIA = 496,
    NPC_DARK_BACK_CURSYA = 497,
    NPC_DARK_ATOMIC_BOO = 498,
    NPC_UNDERCHOMP_RED2 = 499,
    NPC_UNDERCHOMP_BLUE2 = 500,
    NPC_UNDERCHOMP_YELLOW2 = 501,
    NPC_BOMBSHELL_BILL = 502,
    NPC_BOMBSHELL_BILL_BLASTER = 503,
    NPC_GREEN_BOOMBOXER = 504,
    NPC_GREEN_BOOMBOXER_PROJ = 505,
    NPC_PURPLE_MUTH = 506,
    NPC_TEST_GAZIPIO1 = 507,
    NPC_TEST_GAZIPIO2 = 508,
    NPC_TEST_GAZIPIO3 = 509,
    NPC_SPIKE_TOP_UNUSED = 510,
    NPC_BROBOT_LTYPE_HAND2 = 511,
    NPC_BROBOT_LTYPE_HAND3 = 512,
    NPC_BROBOT_LTYPE_FOOT1 = 513,
    NPC_BROBOT_LTYPE_FOOT2 = 514,
    NPC_MONTY_MOLE = 516,
    NPC_MONTY_MOLE_PROJ = 517,
    NPC_DRY_BONES_PARTS = 518,
    NPC_GOOMBA_UNUSED1 = 519,
    NPC_GOOMBA_UNUSED2 = 520,
    NPC_LAVA_BUBBLE_UNUSED1 = 521,
    NPC_WIGGLER_SHORT = 522,
    NPC_WIGGLER_LONG = 523,
    NPC_KOOPA_TROOPA_UNUSED = 524,
    NPC_GOOMBA_UNUSED3 = 525,
    NPC_GOOMBA_UNUSED4 = 526,
    NPC_GOOMBA_UNUSED5 = 527,
    NPC_KAMIKAZE_GOOMBA = 528,
    NPC_DRY_BONES_UNUSED = 529,
    NPC_GOLD_FUZZY = 530,
    NPC_GREEN_FUZZY = 531,
    NPC_SWAMPIRE = 532,
    NPC_EMBER = 533,
    NPC_PHANTOM_EMBER = 534
};

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

typedef enum NPCMoveMode {
    NPC_MOVE_WALK_NO_HIT=0,
    NPC_MOVE_WALK=1,
    NPC_MOVE_STAY_NO_DAMAGE=2,
    NPC_MOVE_SPIN=3,
    NPC_MOVE_GRABBED=4,
    NPC_MOVE_THROWN=5,
    NPC_MOVE_DEATH=6,
    NPC_MOVE_DEATH_3D=7,
    NPC_MOVE_UNK=8,
    NPC_MOVE_BARRY=9
} NPCMoveMode;

typedef struct _NPCPart
{
/* 0x000 */ u16 id;
/* 0x002 */ s16 mode;
/* 0x004 */ s32 minimum_damage;
/* 0x008 */ Vec3 position;
/* 0x014 */ Vec3 ownerPosLastAnimChange;
/* 0x020 */ Vec3 positionDelta;
/* 0x02C */ u32 flag2c;
/* 0x030 */ u32 hitFlags;
/* 0x034 */ u8 unknown_0x34[0x3c - 0x34];
/* 0x03c */ Vec3 hitboxSize;
/* 0x034 */ u8 unknown_0x48[0x378 - 0x48];
/* 0x378 */ s32 attackPower; // initialised as 1, changed by onSpawnScript if needed
/* 0x37C */ u8 unknown_0x37c[0x388 - 0x37c];
/* 0x388 */ struct _NPCEntry * owner;
/* 0x38C */ struct _NPCPart * prevPart;
/* 0x390 */ struct _NPCPart * nextPart;
/* 0x394 */ u8 unknown_0x394[0x398 - 0x394];
} NPCPart;
SIZE_ASSERT(NPCPart, 0x398)

typedef struct
{
/* 0x00 */ u16 id;
/* 0x02 */ s16 mode;
/* 0x04 */ s32 minimum_damage;
/* 0x08 */ Vec3 position;
/* 0x14 */ u32 flag14;
/* 0x18 */ u32 hitFlags;
/* 0x1C */ Vec3 hitboxScale;
/* 0x28 */ NPCDefense * defenses;
/* 0x2C */ Unk * unknown_0x2c;
/* 0x30 */ NPCPartUpdateFunc * updateFunc;
/* 0x34 */ const char * animPoseName;
/* 0x38 */ NPCTribeAnimDef * animDefs;
/* 0x3C */ Vec3 positionDelta;
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
/* 0x20 */ const char * powBlockDeathSfx;
/* 0x24 */ const char * bowserFireDeathSfx;
/* 0x28 */ const char * boomerDeathSfx;
/* 0x2c */ const char * barryDeathSfx;
/* 0x30 */ const char * unused_sfx;
/* 0x34 */ const char * fireBurstDeathSfx;
/* 0x38 */ s16 killXp;
/* 0x3a */ s16 hitXp;
/* 0x3c */ s16 stylishXp;
/* 0x3e */ s16 stopWatchStunTime;
/* 0x40 */ s16 sleepySheepStunTime;
/* 0x42 */ s16 iceStormStunTime;
/* 0x44 */ s16 voltShroomStunTime;
/* 0x46 */ u16 coinDropChance; // chance of dropping any coins at all, percentage
/* 0x48 */ u16 coinDropBaseCount; // minimum amount of coins to drop, if any are dropping
/* 0x4A */ u16 coinDropExtraChance; // chance for each extra coin to drop, percentage
/* 0x4C */ u16 coinDropExtraMax; // maximum amount of extra coins to drop on top of base count
/* 0x4E */ u16 dropItemChance; // chance of dropping any item, percentage
/* 0x50 */ NPCDropItem * dropItemList; // terminated by an entry with id 0
/* 0x54 */ s32 unknown_0x54; // Sets a value in npcEntry which is unused
/* 0x58 */ f32 bounceEjection;
/* 0x5c */ f32 jumpEjection;
/* 0x60 */ f32 unk_float;
/* 0x64 */ u8 attackStrength; // only used for the tattle and turn-based combat, doesn't affect normal damage
/* 0x65 */ u8 padding_0x65[0x68 - 0x65]; // padding
} NPCTribe;
SIZE_ASSERT(NPCTribe, 0x68)

typedef struct
{
/* 0x000 */ s32 m_nPoseId;
/* 0x004 */ char animPoseName[32];
/* 0x024 */ u8 unknown_0x24[0x48 - 0x24];
/* 0x048 */ NPCTribeAnimDef * tribeAnims;
/* 0x04C */ u8 unknown_0x4c[0xb8 - 0x4c];
/* 0x0B8 */ u8 red;
/* 0x0B9 */ u8 green;
/* 0x0BA */ u8 blue;
/* 0x0BB */ u8 alpha;
/* 0x0BC */ u8 unknown_0xbc[0x254 - 0xbc];
} NPCAnim; // unknown size
SIZE_ASSERT(NPCAnim, 0x254)

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
/* 0x044 */ NPCAnim m_Anim;
/* 0x298 */ s32 cameraId;
/* 0x29C */ s32 offscreenId;
/* 0x2A0 */ Vec3 position;
/* 0x2AC */ u8 unknown_0x2ac[0x2ec - 0x2ac];
/* 0x2EC */ s32 flippedTo3d;
/* 0x2F0 */ u8 unknown_0x2f0[0x2f8 - 0x2f0];
/* 0x2F8 */ NPCMoveMode moveMode;
/* 0x2FC */ u8 unknown_0x2fc[0x348 - 0x2fc];
/* 0x348 */ EvtScriptCode * templateinitScript; // initScript from spawning SetupEnemyTemplate
                                                // (unknown for non-templated NPCs)
/* 0x34C */ u8 unknown_0x34c[0x360 - 0x34c];
/* 0x360 */ EvtScriptCode * templatemoveScript; // moveScript from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x364 */ EvtScriptCode * templateonHitScript; // onHitScript from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x368 */ EvtScriptCode * templatepickupScript; // pickupScript from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x36C */ EvtScriptCode * templatethrowScript; // throwScript from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x370 */ EvtScriptCode * templatekouraKickScript; // kouraKickScript from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x374 */ EvtScriptCode * templatedeathScript; // deathScript from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x378 */ EvtScriptCode * templateatkScript; // atkScript from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x37C */ EvtScriptCode * templatemiscScript; // miscScript from spawning SetupEnemyTemplate4
                                                // (unknown for non-templated NPCs)
/* 0x380 */ u8 unknown_0x380[0x390 - 0x380];
/* 0x390 */ s32 onSpawnEvtId; // id of the EvtEntry running a templated npc's onSpawn scripts
                              // (unknown for non-templated NPCs)
/* 0x394 */ s32 unkEvtId;
/* 0x398 */ u32 flags_398;
/* 0x39C */ f32 tribeField0xE; // field 0xe of spawning NPCTribe cast to float
/* 0x3A0 */ f32 tribeField0x10; // field 0x10 of spawning NPCTribe cast to float
/* 0x3A4 */ f32 tribeField0x12; // field 0x12 of spawning NPCTribe cast to float
/* 0x3A8 */ u8 unknown_0x3a8[0x3ac - 0x3a8];
/* 0x3AC */ f32 unknown_0x3ac;
/* 0x3B0 */ u8 unknown_0x3b0[0x400 - 0x3b0];
/* 0x400 */ f32 unknown_0x400;
/* 0x3B0 */ u8 unknown_0x404[0x46c - 0x404];
    /*
        0x80000000 is frozen
        0x40000 is on different pane to Mario
        0x20000 is hidden & frozen
    */
/* 0x46C */ u32 flag46C;
/* 0x470 */ f32 gravity;
/* 0x474 */ u8 unknown_0x474[0x478 - 0x474];
/* 0x478 */ u32 tribeField0x54; // field 0x54 of spawning NPCTribe, unused
/* 0x47C */ u8 unknown_0x47c[0x49c - 0x47c];
/* 0x49C */ s32 tribeId; // id of the NPCTribe this NPC was spawned with
/* 0x4A0 */ s32 tribeId2; // seemingly just a copy of tribeId
/* 0x4A4 */ u8 unknown_0x4a4[0x4d8 - 0x4a4];
/* 0x4D8 */ f32 fleepStunTime;
/* 0x4DC */ u8 unknown_0x4dc[0x4ec - 0x4dc];
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
/* 0x588 */ u8 unknown_0x588[0x624 - 0x588];
/* 0x624 */ f32 stunTime;
/* 0x628 */ u8 unknown_0x628[0x6e0 - 0x628];
/* 0x6E0 */ const char * unkShellSfx;
/* 0x6E4 */ u8 unknown_0x6e4[0x714 - 0x6e4];
/* 0x714 */ NPCPart * parts; // made from tribe's NPCPartDef list, linked list
/* 0x718 */ EvtScriptCode * templateField0x58; // field 0x58 from spawning SetupEnemyTemplate (unknown for non-templated NPCs)
/* 0x71C */ u8 unknown_0x71c[0x730 - 0x71c];
/* 0x730 */ _NPCEntry * master;
/* 0x734 */ u8 unknown_0x734[0x748 - 0x734];
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

struct NPCEntryUnkDef {
    int type;
    void *value;
};

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
/* 0x34 */ EvtScriptCode * initScript;
/* 0x38 */ EvtScriptCode * moveScript;
/* 0x3C */ EvtScriptCode * onHitScript;
/* 0x40 */ EvtScriptCode * pickupScript;
/* 0x44 */ EvtScriptCode * throwScript;
/* 0x48 */ EvtScriptCode * deathScript;
/* 0x4C */ EvtScriptCode * atkScript;
/* 0x50 */ EvtScriptCode * miscScript;
/* 0x54 */ EvtScriptCode * kouraKickScript;
/* 0x58 */ NPCEntryUnkDef * unkDefinitionTable;
/* 0x5C */ u8 unknown_0x5c[0x68 - 0x5c]; // all left blank to be copied from SetupEnemy
} NPCEnemyTemplate;
SIZE_ASSERT(NPCEnemyTemplate, 0x68)

extern NPCTribe npcTribes[535];
extern NPCEnemyTemplate npcEnemyTemplates[NPCTEMPLATE_MAX];

typedef struct
{
/* 0x00 */ u8 unknown_0x00[0x04 - 0x00];
/* 0x04 */ NPCEntry * npcDealingDamage;
/* 0x08 */ u32 collisionDefenseType;
/* 0x0C */ NPCEntry * npcTakingDamage;
/* 0x10 */ u8 unknown_0x10[0x80 - 0x10];
} NPCCollide;
SIZE_ASSERT(NPCCollide, 0x80)

UNKNOWN_FUNCTION(npcSfxOn)
UNKNOWN_FUNCTION(func_801a96a4)
void npcInit();
void npcReset();
UNKNOWN_FUNCTION(npcFlag2On)
void npcMain();
UNKNOWN_FUNCTION(func_801aabe4)
UNKNOWN_FUNCTION(func_801aaca4)
UNKNOWN_FUNCTION(func_801aada8)
UNKNOWN_FUNCTION(npcHitMain)
UNKNOWN_FUNCTION(func_801ad058)
UNKNOWN_FUNCTION(func_801adda4)
void npcDispMain();
UNKNOWN_FUNCTION(func_801aef10)
UNKNOWN_FUNCTION(func_801aefd0)
UNKNOWN_FUNCTION(func_801af4fc)
UNKNOWN_FUNCTION(func_801afefc)
UNKNOWN_FUNCTION(func_801b000c)
UNKNOWN_FUNCTION(func_801b0154)
UNKNOWN_FUNCTION(func_801b0264)
UNKNOWN_FUNCTION(func_801b0480)
UNKNOWN_FUNCTION(func_801b0d10)
UNKNOWN_FUNCTION(func_801b0ed4)
void func_801b19e4();
UNKNOWN_FUNCTION(func_801b20dc)
UNKNOWN_FUNCTION(func_801b2460)
UNKNOWN_FUNCTION(func_801b24c0)
UNKNOWN_FUNCTION(func_801b2574)
UNKNOWN_FUNCTION(func_801b2640)
UNKNOWN_FUNCTION(func_801b285c)
UNKNOWN_FUNCTION(func_801b2928)

// Handles damage & behavior when two enemies collide, e.g. via Thoreau or shells
void npcHandleCollision(NPCCollide * collision);

UNKNOWN_FUNCTION(npcDispSub)
UNKNOWN_FUNCTION(func_801b3860)
UNKNOWN_FUNCTION(mtx_setup)
UNKNOWN_FUNCTION(func_801b4514)
UNKNOWN_FUNCTION(func_801b4fcc)
UNKNOWN_FUNCTION(npcDisp)
UNKNOWN_FUNCTION(npcPartDisp)
UNKNOWN_FUNCTION(npcDisp_xlu)
UNKNOWN_FUNCTION(npcPartDisp_xlu)
UNKNOWN_FUNCTION(func_801b5118)
UNKNOWN_FUNCTION(func_801b5214)
UNKNOWN_FUNCTION(npcDisp_general)
UNKNOWN_FUNCTION(npcPartDisp_general)
UNKNOWN_FUNCTION(npcHitboxDisp)
UNKNOWN_FUNCTION(npcDisp_offscreen)
UNKNOWN_FUNCTION(npcDisp_offscreen_xlu)
NPCEntry * npcEntry(const char * instanceName, const char * animPoseName, s32 instanceId,
                    bool param_4);
NPCEntry * npcIdToPtr(s32 id);
NPCEntry * npcNameToPtr(const char * instanceName);
NPCEntry * npcNameToPtr_NoAssert(const char * instanceName);
void npcDelete(const char *name);
UNKNOWN_FUNCTION(func_801b73cc)
UNKNOWN_FUNCTION(func_801b7a20)
UNKNOWN_FUNCTION(func_801b8064)
UNKNOWN_FUNCTION(npcHandleDmg)
UNKNOWN_FUNCTION(func_801ba124)
UNKNOWN_FUNCTION(func_801bb6fc)
UNKNOWN_FUNCTION(func_801bba3c)
UNKNOWN_FUNCTION(func_801bc5d0)
UNKNOWN_FUNCTION(func_801bc7cc)
UNKNOWN_FUNCTION(npcDropFromCatch)
UNKNOWN_FUNCTION(func_801bcc00)
UNKNOWN_FUNCTION(func_801bcc8c)
UNKNOWN_FUNCTION(func_801bce2c)
UNKNOWN_FUNCTION(func_801bd4ac)
UNKNOWN_FUNCTION(func_801bd584)
UNKNOWN_FUNCTION(func_801bd7dc)
UNKNOWN_FUNCTION(func_801bd86c)
UNKNOWN_FUNCTION(func_801bde20)
UNKNOWN_FUNCTION(func_801be14c)
NPCEntry * npcEntryFromTemplate(NPCEnemyTemplate * enemyTemplate);
bool npcAreOnSpawnEvtsDone();
UNKNOWN_FUNCTION(func_801be6cc)
UNKNOWN_FUNCTION(func_801be7c8)
UNKNOWN_FUNCTION(func_801be9c4)
UNKNOWN_FUNCTION(func_801beed4)
UNKNOWN_FUNCTION(func_801bf554)
UNKNOWN_FUNCTION(func_801bf5cc)
UNKNOWN_FUNCTION(func_801bf60c)
UNKNOWN_FUNCTION(func_801bf6a4)
NPCEntry * npcEntryFromSetupEnemy(s32 setupFileIndex, Vec3 * pos, s32 nTemplateNo,
                                  MiscSetupDataV6 * miscSetupData);
s32 npcGetNextTemplateNpcId();
UNKNOWN_FUNCTION(func_801bfc14)
const char * npcSearchAnimDefs(NPCTribeAnimDef * defs, s32 id);
UNKNOWN_FUNCTION(npcSetStayPose)
UNKNOWN_FUNCTION(npcSetTalkPose)
UNKNOWN_FUNCTION(npcSetAnimSub)
UNKNOWN_FUNCTION(_npcSetAnim)
UNKNOWN_FUNCTION(npcSetAnim)
UNKNOWN_FUNCTION(func_801c018c)
UNKNOWN_FUNCTION(func_801c01a8)
UNKNOWN_FUNCTION(func_801c01c0)
UNKNOWN_FUNCTION(func_801c01dc)
UNKNOWN_FUNCTION(func_801c0284)
UNKNOWN_FUNCTION(npcGetDefense)
UNKNOWN_FUNCTION(npcGetScript)
UNKNOWN_FUNCTION(func_801c0454)
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
UNKNOWN_FUNCTION(func_801c213c)
NPCPart * npcAddPart(NPCEntry * entry, NPCPartDef * partDef);
UNKNOWN_FUNCTION(func_801c23b0)
NPCPart * npcGetPartById(NPCEntry * npc, u32 id);
UNKNOWN_FUNCTION(func_801c2480)
UNKNOWN_FUNCTION(func_801c24cc)
void npcPartUpdatePosition(NPCPart * part);
UNKNOWN_FUNCTION(func_801c25d8)
UNKNOWN_FUNCTION(func_801c3050)
UNKNOWN_FUNCTION(func_801c35fc)
UNKNOWN_FUNCTION(func_801c360c)
void func_801c3694(const char *, const char *);
UNKNOWN_FUNCTION(func_801c3720)
void func_801c37e4(const char *);
void func_801c3848(const char *);
UNKNOWN_FUNCTION(func_801c38a4)
UNKNOWN_FUNCTION(func_801c38b4)
UNKNOWN_FUNCTION(func_801c38c4)
UNKNOWN_FUNCTION(func_801c38d4)
UNKNOWN_FUNCTION(func_801c3930)
UNKNOWN_FUNCTION(func_801c3968)
UNKNOWN_FUNCTION(func_801c3a74)
UNKNOWN_FUNCTION(func_801c3a84)
UNKNOWN_FUNCTION(func_801c3b6c)
UNKNOWN_FUNCTION(func_801c3b80)
UNKNOWN_FUNCTION(func_801c3b94)
UNKNOWN_FUNCTION(func_801c3ba8)
UNKNOWN_FUNCTION(func_801c3bb8)
UNKNOWN_FUNCTION(func_801c3bc4)
UNKNOWN_FUNCTION(func_801c3cb4)
UNKNOWN_FUNCTION(func_801c3d94)
UNKNOWN_FUNCTION(func_801c45a4)
UNKNOWN_FUNCTION(func_801c498c)

void npcGetFrameBoundingBox(NPCEntry * npc, Vec3 * ret1, Vec3 * ret2);

UNKNOWN_FUNCTION(func_801c5140)

// NOTE: npc is the one dealing damage, npcPart->owner is the one taking damage
s32 npcTakeDamage(NPCEntry * npcEntry, NPCPart *npcPart, s32 defenseType, s32 power, u32 flags, s32 param_6);

UNKNOWN_FUNCTION(func_801c7f68)
UNKNOWN_FUNCTION(func_801c7fe0)
UNKNOWN_FUNCTION(func_801c7ff0)
UNKNOWN_FUNCTION(func_801c8000)
UNKNOWN_FUNCTION(npcTimerDisp)
// npcHandleHitXp would go here
UNKNOWN_FUNCTION(npcTimerMain)
UNKNOWN_FUNCTION(func_801c85e4)
UNKNOWN_FUNCTION(func_801c85ec)
UNKNOWN_FUNCTION(func_801c8760)
void npcFleeped(NPCEntry * entry);
UNKNOWN_FUNCTION(func_801c8b7c)
UNKNOWN_FUNCTION(func_801c8b98)
UNKNOWN_FUNCTION(func_801c8d48)
UNKNOWN_FUNCTION(func_801c8d54)
UNKNOWN_FUNCTION(func_801c8d70)
UNKNOWN_FUNCTION(npcCheckFlipFinished)
UNKNOWN_FUNCTION(func_801c9874)
UNKNOWN_FUNCTION(func_801c98f0)
UNKNOWN_FUNCTION(npcFinishFlipInstant)
UNKNOWN_FUNCTION(func_801c99b0)
UNKNOWN_FUNCTION(func_801c9a24)
NPCTribe * npcGetTribe(s32 tribeId);
NPCWork * npcGetWorkPtr();
UNKNOWN_FUNCTION(func_801c9ae4)
UNKNOWN_FUNCTION(func_801ca150)
UNKNOWN_FUNCTION(func_801ca198)
void func_801ca1a4(NPCEntry * npcEntry, NPCAnim * npcAnim);
UNKNOWN_FUNCTION(func_801ca238)
UNKNOWN_FUNCTION(func_801ca2d8)
UNKNOWN_FUNCTION(func_801ca300)
UNKNOWN_FUNCTION(func_801ca310)
UNKNOWN_FUNCTION(func_801ca3b8)
UNKNOWN_FUNCTION(func_801ca3e0)
UNKNOWN_FUNCTION(func_801ca448)
UNKNOWN_FUNCTION(func_801ca748)
UNKNOWN_FUNCTION(func_801ca768)
UNKNOWN_FUNCTION(func_801caab4)
UNKNOWN_FUNCTION(func_801cab08)
UNKNOWN_FUNCTION(func_801cab20)
UNKNOWN_FUNCTION(func_801cab2c)
UNKNOWN_FUNCTION(func_801cb268)
UNKNOWN_FUNCTION(func_801cb274)
UNKNOWN_FUNCTION(func_801cb754)
UNKNOWN_FUNCTION(func_801cbf84)
UNKNOWN_FUNCTION(func_801cbf9c)
UNKNOWN_FUNCTION(func_801cbfe8)
UNKNOWN_FUNCTION(func_801cc0a0)
UNKNOWN_FUNCTION(func_801cc0cc)
UNKNOWN_FUNCTION(npcGetKillXp)
UNKNOWN_FUNCTION(func_801cc134)

s32 npcDamageMario(NPCEntry * npcEntry, NPCPart * part, Vec3 * position, u32 status, s32 damage, u32 flags);

UNKNOWN_FUNCTION(func_801cc644)
UNKNOWN_FUNCTION(func_801cc8d0)
UNKNOWN_FUNCTION(func_801cc9dc)
UNKNOWN_FUNCTION(func_801ccb68)
UNKNOWN_FUNCTION(func_801ccce0)
UNKNOWN_FUNCTION(func_801cce10)
UNKNOWN_FUNCTION(func_801ccf74)
UNKNOWN_FUNCTION(func_801cd124)
UNKNOWN_FUNCTION(func_801cd210)
UNKNOWN_FUNCTION(func_801cd3bc)
UNKNOWN_FUNCTION(func_801cd404)
UNKNOWN_FUNCTION(func_801cd5e8)
UNKNOWN_FUNCTION(func_801cd6d4)
UNKNOWN_FUNCTION(func_801cd82c)
UNKNOWN_FUNCTION(func_801cd834)
UNKNOWN_FUNCTION(func_801cd95c)
UNKNOWN_FUNCTION(func_801cdb84)
UNKNOWN_FUNCTION(func_801ceb08)
UNKNOWN_FUNCTION(func_801ceeac)
UNKNOWN_FUNCTION(func_801cf0a0)
UNKNOWN_FUNCTION(func_801cf16c)
UNKNOWN_FUNCTION(func_801d0278)
UNKNOWN_FUNCTION(func_801d029c)
UNKNOWN_FUNCTION(func_801d02ac)
UNKNOWN_FUNCTION(func_801d0610)
UNKNOWN_FUNCTION(func_801d0624)
UNKNOWN_FUNCTION(func_801d0640)
UNKNOWN_FUNCTION(func_801d1058)
UNKNOWN_FUNCTION(func_801d1064)
UNKNOWN_FUNCTION(func_801d1074)
UNKNOWN_FUNCTION(func_801d1084)
UNKNOWN_FUNCTION(func_801d1094)
UNKNOWN_FUNCTION(func_801d10a4)
UNKNOWN_FUNCTION(func_801d10cc)
UNKNOWN_FUNCTION(func_801d1240)
UNKNOWN_FUNCTION(func_801d1424)
UNKNOWN_FUNCTION(func_801d1580)
UNKNOWN_FUNCTION(func_801d1b9c)
UNKNOWN_FUNCTION(npcTryCatchCard)
UNKNOWN_FUNCTION(func_801d1d7c)
UNKNOWN_FUNCTION(func_801d2028)
UNKNOWN_FUNCTION(func_801d2208)
UNKNOWN_FUNCTION(func_801d2304)
void npcFreezeAll();
UNKNOWN_FUNCTION(func_801d2384)
UNKNOWN_FUNCTION(func_801d23cc)
UNKNOWN_FUNCTION(func_801d23d4)
UNKNOWN_FUNCTION(func_801d2554)
UNKNOWN_FUNCTION(func_801d25b8)
UNKNOWN_FUNCTION(func_801d25f4)

CPP_WRAPPER_END()
