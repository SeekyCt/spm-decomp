/*
    mario controls the player characters of the game
*/

#pragma once

#include <common.h>
#include <spm/fairy.h>
#include <spm/hitdrv.h>
#include <spm/mobjdrv.h>
#include <spm/npcdrv.h>
#include <wii/mtx.h>

CPP_WRAPPER(spm::mario)

USING(spm::fairy::FairyEntry)
USING(spm::hitdrv::HitObj)
USING(spm::mobjdrv::MobjEntry)
USING(spm::npcdrv::NPCEntry)
USING(wii::mtx::Vec3)
USING(wii::mtx::Vec3i)

// Returns an override for the name
typedef const char * (MarioAnimChangeHandler)(const char * newAnimName);

enum PlayerCharacter
{
/* 0x0 */ PLAYER_MARIO,
/* 0x1 */ PLAYER_PEACH,
/* 0x2 */ PLAYER_BOWSER,
/* 0x3 */ PLAYER_LUIGI
};

typedef struct
{
/* 0x00 */ u8 unknown_0x0[0x34 - 0x0];
} MarioJumpFallPara;
SIZE_ASSERT(MarioJumpFallPara, 0x34)

/*
    Called every frame, fairy is deleted when true is returned
*/
typedef bool (MarioFairyFlagDeleteFunc)(void * data, bool deletionHint);

/*
    Called when deleted, unless deleted from flag delete func
*/
typedef void (MarioFairyDeleteFunc)(void * data, void * param);

/*
    Called every frame, commands unknown
*/
typedef void (MarioFairyMainFunc)(void * data, s32 command);

typedef struct _MarioFairy
{
    /*
        0x8000 deletion hint (for flag delete func)
        0x2 store data in main heap, not map heap
        0x1 active
        Others unknown
    */
/* 0x00 */ u16 flags;
/* 0x02 */ u8 id;
/* 0x03 */ u8 slotId; // index in MarioWork's array this is stored at
/* 0x04 */ void * data;
/* 0x08 */ MarioFairyFlagDeleteFunc * flagDeleteFunc;
/* 0x0C */ MarioFairyDeleteFunc * deleteFunc;
/* 0x10 */ MarioFairyMainFunc * mainFunc;
/* 0x14 */ struct _MarioFairy * next;
/* 0x18 */ struct _MarioFairy * prev;
} MarioFairy;
SIZE_ASSERT(MarioFairy, 0x1c)

struct _MarioStatus;

/*
    Returns true if deleted, false otherwise
    Commands:
        1 start
        2 restart?
*/
typedef bool (MarioStatusMainFunc)(struct _MarioStatus * status, Vec3 * iconPos, s32 command);

/*
    Returns true if it should be deleted, false otherwise
    Return is ignored except in marioStatusDeleteAll
*/ 
typedef bool (MarioStatusDeleteFunc)(struct _MarioStatus * status, s32 level); 

/*
    Called on map change
*/
typedef void (MarioStatusMapChangeFunc)(struct _MarioStatus * status);

typedef struct _MarioStatus
{
    /*
        0x1 active
        Others unknown
    */
/* 0x00 */ u32 flags;
/* 0x04 */ u32 type; // same values as MarioWork.statusFlags
/* 0x08 */ f32 timer; // time remaining, counts down to 0 in increments of marioSpeedScale
/* 0x0C */ void * userWork;
/* 0x10 */ MarioStatusMainFunc * mainFunc;
/* 0x14 */ MarioStatusDeleteFunc * deleteFunc;
/* 0x18 */ MarioStatusMapChangeFunc * mapChangeFunc;
/* 0x1C */ struct _MarioStatus * prev;
/* 0x20 */ struct _MarioStatus * next;
} MarioStatus;
SIZE_ASSERT(MarioStatus, 0x24)

enum MarioGravityType
{
/* 0x0 */ MARIO_GRAVITY_DOWN,
/* 0x1 */ MARIO_GRAVITY_UP,
/* 0x2 */ MARIO_GRAVITY_LEFT,
/* 0x3 */ MARIO_GRAVITY_RIGHT,
/* 0x4 */ MARIO_GRAVITY_MAX
};

/*
    Commands:
        0 start
        1 update
        2 end
*/
typedef void (MarioAcrobatCallback)(s32 command);

typedef struct
{
/* 0x0 */ f32 min;
/* 0x4 */ f32 max;
} MarioPaneBoundary;
SIZE_ASSERT(MarioPaneBoundary, 0x8)

/*
    Returns true if the pane change is allowed
*/
typedef bool (MarioPaneChangeFunc)(s32 newPane);

#define MOT_STAY 0x00 // Standing still
#define MOT_WALK 0x01 // Walking slowly
#define MOT_DASH 0x02 // Walking at full speed
#define MOT_JUMP 0x03 // Jumping normally
#define MOT_CROUCH 0x04 // Crouching
#define MOT_5 0x05
#define MOT_SPRING 0x06 // Jumping off spring object
#define MOT_7 0x07
#define MOT_8 0x08
#define MOT_BOUNCE 0x09 // Jumping off of an NPC
#define MOT_FALL 0x0A // Falling in the air
#define MOT_11 0x0B
#define MOT_12 0x0C
#define MOT_13 0x0D
#define MOT_THUDLEY_AIR 0x0E // In the air while using thudley
#define MOT_THUDLEY_LAND 0x0F // Landing whlie using thudley
#define MOT_HAMMER 0x10 // Using cudge
#define MOT_JABARA 0x11 // Luigi spring jump(?)
#define MOT_SLIT 0x12 // Thin with slim
#define MOT_TALK 0x13 // Talking to an NPC
#define MOT_20 0x14
#define MOT_21 0x15
#define MOT_22 0x16
#define MOT_FORCE_RESET 0x17 // ? TTYD name
#define MOT_24 0x18
#define MOT_BOTTOMLESS 0x19 // Respawning from falling
#define MOT_FLIP_AIR 0x1A // Flip into midair
#define MOT_DAMAGE 0x1B // Taking damage
#define MOT_28 0x1C
#define MOT_29 0x1D
#define MOT_30 0x1E
#define MOT_31 0x1F
#define MOT_32 0x20
#define MOT_33 0x21
#define MOT_34 0x22
#define MOT_35 0x23
#define MOT_GRAB 0x24 // Throwing thoreau
#define MOT_BOMB 0x25 // Placing boomer
#define MOT_FLEEP 0x26 // Using fleep
#define MOT_SHRINK 0x27 // Shrinking with dottie
#define MOT_GROW 0x28 // Growing with dottie
#define MOT_PICCOLO 0x29 // Using piccolo
#define MOT_BARRY 0x2A // Using barry
#define MOT_43 0x2B
#define MOT_CARRIE_MOUNT 0x2C // Mounting carrie
#define MOT_CARRIE_DISMOUNT 0x2D // Dismounting carrie
#define MOT_46 0x2E
#define MOT_47 0x2F
#define MOT_48 0x30
#define MOT_49 0x31
#define MOT_50 0x32
#define MOT_51 0x33
#define MOT_52 0x34
#define MOT_53 0x35
#define MOT_54 0x36
#define MOT_55 0x37
#define MOT_56 0x38
#define MOT_57 0x39
#define MOT_58 0x3A
#define MOT_59 0x3B
#define MOT_60 0x3C
#define MOT_61 0x3D
#define MOT_62 0x3E
#define MOT_63 0x3F
#define MOT_64 0x40
#define MOT_65 0x41
#define MOT_66 0x42
#define MOT_67 0x43
#define MOT_68 0x44
#define MOT_SWIM 0x45 // Off-ground underwater
#define MOT_70 0x46
#define MOT_CHAR_CHANGE 0x47 // Changing character
#define MOT_FAIRY_CHANGE 0x48 // Changing pixl
#define MOT_FLIP 0x49 // Flipping to 2d/3d
#define MOT_GLIDE 0x4A // Peach parasol glide
#define MOT_GUARD 0x4B // Peach parasol guard
#define MOT_FIRE 0x4C // Bowser fire
#define MOT_77 0x4D

// Off ground, in water
#define MARIO_MISC_FLAG_SWIM 0x20000000

// Squirps enabled
#define MARIO_MISC_FLAG_SQUIRPS 0x200000

// Luvbi enabled
#define MARIO_MISC_FLAG_LUVBI 0x80000

// Near top of water?
#define MARIO_MISC_FLAG_0x400 0x400

// Can jump out of water?
#define MARIO_MISC_FLAG_0x200 0x200

// In water
#define MARIO_MISC_FLAG_WATER 0x100

// Jumping out of water
#define MARIO_MISC_FLAG_WATER_JUMP 0x10

// Paper pose on
#define MARIO_DISP_FLAG_PAPER 0x40000000

#define MARIO_DISP_FLAG_0x800000 0x800000

// Lock facing direction towards facingTarget
#define MARIO_DISP_FLAG_LOCK_FACING 0x20


typedef struct
{
    /*
        General flags
        0x80000000 squashed (regrowing with dottie without space)
        0x40000000 mega star damage (double attack & breaking blocks)
        0x10000000 block motion id change
        0x2000000 bg mode
        0x800000 respawn position locked
        0x8 key off
        0x2 ctrl off
        Others unknown
    */
/* 0x0000 */ u32 flags;
    /*
        More general flags, see defines above
    */
/* 0x0004 */ u32 miscFlags;
    /*
        Disp flags, see defines above
    */
/* 0x0008 */ u32 dispFlags;
    /*
        Trig flags
        0x40000000 blocks clear_hitobj_ride
        0x1000 pose changed
        0x1 motion id changed
    */
/* 0x000C */ u32 trigFlags;
    /*
        Status flags
        0x8000 ghost shroom
        0x4000 pal pills
        0x2000 unused?
        0x1000 coin flower
        0x800 slow flower (including triple xp)
        0x400 speed flower (including triple xp)
        0x200 barrier
        0x100 hp regen
        0x80 electrified
        0x40 double attack
        0x20 halved damage
        0x10 flipped controls
        0x8 de-jumped
        0x4 no skills
        0x2 slow
        0x1 poison
        Others unused
    */
/* 0x0010 */ u32 statusFlags;
    /*
        Effect flags
        0x8 slow flower trail
        0x4 speed flower trail
        0x2 stylish trail
        Others unknown
    */
/* 0x0014 */ u8 effectFlags;
/* 0x0015 */ u8 unknown_0x15[0x1c - 0x15];
/* 0x001C */ const char * curPoseName; // current animation name
/* 0x0020 */ const char * paperPoseName;
/* 0x0024 */ s16 poseTime; // time in current animation
/* 0x0026 */ u8 unknown_0x26[0x28 - 0x26];
/* 0x0028 */ MarioAnimChangeHandler * animChangeHandler;
/* 0x002C */ u16 motionId; // see enum above
/* 0x002E */ u16 prevMotionId; // see enum above
/* 0x0030 */ u8 unknown_0x30[0x34 - 0x30];
    // freeze the player when > 0 (for cutscenes, talking, etc)
/* 0x0034 */ u8 ctrl;
/* 0x0035 */ u8 keyOff;
/* 0x0036 */ u8 unknown_0x36[0x38 - 0x36];
/* 0x0038 */ s8 character; // see enum above
/* 0x0039 */ u8 unknown_0x39[0x3b - 0x39];
/* 0x003B */ u8 wallTimer;
/* 0x003C */ s32 subMotionId; // values vary by motion id
/* 0x0040 */ u8 unknown_0x40[0x50 - 0x40];
/* 0x0050 */ f32 airTimer; // time in air
/* 0x0054 */ f32 jumpPeakAirTime; // value of airTimer when reaching top of jump
/* 0x0058 */ u8 unknown_0x58[0x5c - 0x58];
/* 0x005C */ Vec3 position;
/* 0x0068 */ u8 unknown_0x68[0xbc - 0x68];
/* 0x00BC */ Vec3 scale;
/* 0x00C8 */ u8 unknown_0xc8[0x120 - 0xc8];
/* 0x0120 */ s32 camId;
/* 0x0124 */ u8 unknown_0x124[0x128 - 0x124];
/* 0x0128 */ Vec3i framebufferPos;
/* 0x0134 */ u8 unknown_0x134[0x148 - 0x134];
/* 0x0148 */ f32 xzSpeed; // current horizontal speed
/* 0x014C */ f32 walkSpeed; // base walk speed
/* 0x0150 */ f32 dashSpeed; // base dash speed
/* 0x0154 */ u8 unknown_0x154[0x160 - 0x154];
/* 0x0160 */ f32 lastGroundSpeed; // xzSpeed when last on ground
/* 0x0164 */ u8 unknown_0x164[0x174 - 0x164];
/* 0x0174 */ f32 directionWorld; // degrees
/* 0x0178 */ f32 directionView; // degrees
/* 0x017C */ u8 unknown_0x17c[0x180 - 0x17c];
/* 0x0180 */ f32 dispDirectionCurrent; // rotation of the player's model about y axis, degrees
/* 0x0184 */ f32 dispDirectionTarget; // dispDirectionCurrent transitions to this target value
/* 0x0188 */ u8 unknown_0x188[0x194 - 0x188];
/* 0x0194 */ f32 hitboxWidth;
/* 0x0198 */ f32 hitboxHeight;
/* 0x019C */ f32 baseHitboxWidth;
/* 0x01A0 */ f32 baseHitboxHeight;
/* 0x01A4 */ u8 unknown_0x1a4[0x1bc - 0x1a4];
    /*
        1 is interact (in front of player)
        2 is stand on
        3 is jump from
        6 is head
        Others unknown
    */
/* 0x01BC */ HitObj * hitObjs1[10];
/* 0x01E4 */ u8 unknown_0x1e4[0x1fc - 0x1e4];
    /*
        0 is MOBJ interact
        Others unknown
    */
/* 0x01FC */ HitObj * hitObjs2[3];
/* 0x0208 */ u8 unknown_0x208[0x24c - 0x208];
    /*
        0 is main model
        1 is back model (Mario only)
        6 is paper pose
        8 is slim flip model
        Others unknown
    */
/* 0x024C */ s32 animPoseIds[8];
/* 0x026C */ u8 unknown_0x26c[0x30c - 0x26c];
/* 0x030C */ u16 buttonsHeld;
/* 0x030E */ u16 buttonsPressed;
/* 0x0310 */ u16 buttonsHeldRepeat;
/* 0x0312 */ u16 buttonsReleased;
/* 0x0314 */ u8 unknown_0x314[0x316 - 0x314];
/* 0x0316 */ u8 pseudoJoystickX;
/* 0x0317 */ u8 pseudoJoystickY;
/* 0x0318 */ u8 unknown_0x318[0x31c - 0x318];
    /*
        Frames 1/2 has been held
        -1 if not held
        Caps at 100
    */
/* 0x031E */ s16 held1Time;
/* 0x0320 */ s16 held2Time;
/* 0x0322 */ u8 unknown_0x322[0x348 - 0x322];
/* 0x0348 */ s32 sfxIds[4];
/* 0x0358 */ u8 unknown_0x358[0x3a4 - 0x358];
    /*
        Info on entity caught with Thoreau
        catchType indicates the type of caught
        0 nothing
        1 NPC
        2 Fairy
        3 MOBJ
    */
/* 0x03A4 */ union
             {
                 NPCEntry * npc;
                 FairyEntry * fairy;
                 MobjEntry * mobj;
             } caught;
/* 0x03A8 */ s32 catchType;
/* 0x03AC */ u8 unknown_0x3ac[0x3d0 - 0x3ac];
/* 0x03D0 */ f32 xzSpeedFactor;
/* 0x03D4 */ u8 unknown_0x3d4[0x3d8 - 0x3d4];
/* 0x03D8 */ Vec3 respawnPosition;
/* 0x03E4 */ u8 unknown_0x3e4[0x3ec - 0x3e4];
/* 0x03EC */ HitObj * hitObjRideArray[24];
/* 0x044C */ s32 numHitObjRideArray; // number of pointers in the array above
/* 0x0450 */ u8 unknown_0x450[0x694 - 0x450];
/* 0x0694 */ HitObj * hitObjInteractArray[50];
/* 0x075C */ s32 numHitObjInteractArray; // number of pointers in the array above
/* 0x0760 */ u8 unknown_0x760[0x9b8 - 0x760];
/* 0x09B8 */ HitObj * hitObjHeadArray[20];
/* 0x0A08 */ s32 numHitObjHeadArray; // number of pointers in the array above
/* 0x0A0C */ u8 unknown_0xa0c[0xcd8 - 0xa0c];
/* 0x0CD8 */ MarioJumpFallPara jumpFallPara;
/* 0x0D0C */ u8 unknown_0xd0c[0xd58 - 0xd0c];
/* 0x05D8 */ MarioFairy fairy[10];
/* 0x0E70 */ MarioFairy * firstFairy;
/* 0x0E74 */ MarioFairy * lastFairy;
/* 0x0E78 */ u8 unknown_0xe78[0xe80 - 0xe78];
/* 0x0E80 */ MarioStatus statusTbl[32];
/* 0x1300 */ MarioStatus * firstStatus;
/* 0x1304 */ MarioStatus * lastStatus;
/* 0x1308 */ u8 unknown_0x1308[0x1360 - 0x1308];
/* 0x1360 */ s32 gravityType; // see enum above
    // Unit vectors for each direction under current gravity
/* 0x1364 */ Vec3 gravUnitRight; // positive x normally
/* 0x1370 */ Vec3 gravUnitUp; // positive y normally
/* 0x137C */ Vec3 gravUnitForward; // positive z normally
/* 0x1388 */ u8 unknown_0x1388[0x1480 - 0x1388];
/* 0x1480 */ s32 nextCharacter;
/* 0x1484 */ s32 prevPixl;
/* 0x1488 */ s32 curPixl;
/* 0x148C */ u8 unknown_0x148c[0x1498 - 0x148c];
/* 0x1498 */ u16 acrobatFlags;
/* 0x149A */ u8 unknown_0x149a[0x149c - 0x149a];
/* 0x149C */ f32 stylishLevel;
/* 0x14A0 */ u8 unknown_0x14a0[0x14a4 - 0x14a0];
/* 0x14A4 */ s32 lastStylishXp;
/* 0x14A8 */ s32 stylishSfxId;
/* 0x14AC */ s32 stylishCombo;
/* 0x14B0 */ u8 unknown_0x14b0[0x14b4 - 0x14b0];
/* 0x14B4 */ f32 posLockTimer; // freezes the player in place for the stylish spin animation
/* 0x14B8 */ u8 unknown_0x14b8[0x14c0 - 0x14b8];
/* 0x14C0 */ u32 acrobatJoystickFlags;
/* 0x14C4 */ u8 unknown_0x14c4[0x14dc - 0x14c4];
/* 0x14DC */ MarioAcrobatCallback * acrobatCb;
/* 0x14E0 */ s32 acrobatStage;
/* 0x14E4 */ u8 unknown_0x14e4[0x1550 - 0x14e4];
/* 0x1550 */ s32 pane; // row of the map on z-axis the player is in, -1 if no panes
/* 0x1554 */ MarioPaneBoundary * paneBoundaries; // null if no panes
/* 0x1558 */ MarioPaneChangeFunc * paneChangeFunc;
/* 0x155C */ Vec3 facingTarget;
/* 0x1568 */ u8 unknown_0x1568[0x156c - 0x1568];
} MarioWork;
SIZE_ASSERT(MarioWork, 0x156c)

DECOMP_STATIC(MarioWork mario_work)

typedef struct
{
/* 0x00 */ f32 hitboxWidth;
/* 0x04 */ f32 hitboxHeight;
/* 0x08 */ f32 walkSpeed;
/* 0x0C */ f32 dashSpeed;
/* 0x10 */ u8 unknown_0x10[0x14 - 0x10];
} CharacterProperties;
SIZE_ASSERT(CharacterProperties, 0x14)

extern CharacterProperties characterProperties[4]; // index character id

typedef void (MarioMotFunc)(MarioWork * mp);

typedef struct
{
/* 0x0 */ MarioMotFunc * mainFunc;
/* 0x4 */ MarioMotFunc * deleteFunc;
} MarioMotFuncs;
SIZE_ASSERT(MarioMotFuncs, 0x8)

extern MarioMotFuncs marioMotTbl[78]; // index motion id

DECOMP_STATIC(s64 mario_mainLastRunTime) // used to adjust marioGameSpeedScale for lag

/*
    In normal gameplay, this is 1.0 for every frame
    When lagging, this will increase so the game will try catch up
    Fast & slow flowers also double/half this
*/
DECOMP_STATIC(f32 mario_gameSpeedScale)

UNKNOWN_FUNCTION(func_80121e18);

/*
    Returns marioGameSpeedScale
*/
f32 marioGetGameSpeedScale();

UNKNOWN_FUNCTION(func_80121e58);
UNKNOWN_FUNCTION(func_80121f40);

/*
    Returns a pointer to the MarioWork instance
*/
MarioWork * marioGetPtr();

UNKNOWN_FUNCTION(func_80121f54);
UNKNOWN_FUNCTION(func_8012217c);
UNKNOWN_FUNCTION(func_801222a4);

/*
    Changes the player's model
    0 is normal
    1 is mega
*/
void marioSetAnimGroup(s32 group);

UNKNOWN_FUNCTION(func_8012244c);

/*
    Re-reads the characterProperties for the current character into marioWork
*/
void marioUpdateCharProperties();

/*
    Returns the characterProperties hitboxWidth for the current character
*/
f32 marioGetHitboxWidthRaw();

/*
    Returns the characterProperties hitboxHeight for the current character
*/
f32 marioGetHitboxHeightRaw();

/*
    Returns the characterProperties hitboxHeight for the current character, or 63 for Bowser
*/
f32 marioGetHitboxHeight_BowserSpecial();

/*
    Returns the characterProperties dashSpeed for the current character
*/
f32 marioGetDashSpeedRaw();

/*
    Updates the player on entering a new map
    Handles statuses, animations, position and unflipping from Slim
*/
void marioOnMapChange(f32 spawnX, f32 spawnY, f32 spawnZ);

/*
    Returns whether player movement is enabled based on keyOff
*/
bool marioChkKey();

/*
    Returns whether player movement is enabled based on ctrl
*/
bool marioCtrlOnChk();

/*
    Returns whether player movement is disabled based on ctrl
*/
bool marioCtrlOffChk();

/*
    Returns whether player movement is disabled based on keyOff
*/
bool marioKeyOffChk();

/*
    Disables movement, increasing ctrl by one and returning it
*/
s32 marioCtrlOff();

/*
    Reduces ctrl by one and returns it, enabling movement if it's 0
*/
s32 marioCtrlOn();

/*
    Disables movement, increasing keyOff by one and returning it
    XZ speed is also removed
*/
s32 marioKeyOff();

/*
    Reduces keyOff by one and returns it, enabling movement if it's 0
*/
s32 marioKeyOn();

/*
    Sets up the player to walk in the background of the level
*/
void marioBgModeOn();

/*
    Stops the player walking in the background of the level
*/
void marioBgModeOff();

/*
    Checks if any of the specified conditions in the mask are true
    0x1: marioWork.flags & 0x10000 is on
    0x2: marioWork.flags & 0x20000 is on
    0x4: marioWork.flags & 0x100000 is on
    0x8: motion id is Luigi super jump
*/
bool marioChkSts(u32 mask);

/*
    Removes references to a HitObj from hitObjs1 & hitObjs2
*/
void marioResetHitObj(const char * name);  

/*
    Resets marioWork for a new save file
*/
void marioReset();

void marioOfsRotReset();

/*
    Clears sfx ids
*/
void marioSoundInit();

/*
    Asynchronously sets up front & back models for the player
    Returns true if finished
*/
bool marioPoseInit();

/*
    Initialises marioWork
*/
void marioInit();


/*
    Re-initialises marioWork on map unload
*/
void marioReInit();

UNKNOWN_FUNCTION(func_80123f0c);

/*
    Updates respawn position if in a safe location to respawn
*/
void marioUpdateRespawnPos();

UNKNOWN_FUNCTION(func_80124374);

/*
    Updates the player
*/
void marioMain();

UNKNOWN_FUNCTION(func_80125854);
UNKNOWN_FUNCTION(func_80125998);
UNKNOWN_FUNCTION(func_80126034);
UNKNOWN_FUNCTION(func_801265a0);
UNKNOWN_FUNCTION(func_80126618);
UNKNOWN_FUNCTION(func_8012662c);
UNKNOWN_FUNCTION(func_80126688);

/*
    Sets the player's current animation
*/
void marioChgPose(const char * name);
void marioChgPoseTime(const char * name, s16 time);

void marioChgPaper(const char * name);
void marioChgPaper2(const char * name);
void marioSetPaperAnimeLocalTime(s16 time);
void marioPaperOn(const char * name);
void marioPaperOff();
void marioPaperLightOn();
void marioPaperLightOff();

/*
    Returns whether the player's current animation has finished
*/
bool marioIsAnimFinished();

UNKNOWN_FUNCTION(func_80126c98);
UNKNOWN_FUNCTION(func_80126cfc);

/*
    Returns screen coordinates for a point in the 3d world
*/
void marioGetScreenPos(Vec3 * worldPos, f32 * xOut, f32 * yOut, f32 * zOut);

/*
    Returns whether the given screen coordinates are on screen
*/
bool marioChkInScreen(s32 x, s32 y);

/*
    Returns 0.8 for bg mode, 2.0 otherwise
*/
f32 marioGetScale();

UNKNOWN_FUNCTION(func_80126e1c);
UNKNOWN_FUNCTION(func_80126e6c);

/*
    Updates the direction the player is facing
*/
void marioMakeDispDir();

UNKNOWN_FUNCTION(func_801275dc);

/*
    Prepares for rendering the player
*/
void marioPreDisp();

UNKNOWN_FUNCTION(func_80127f5c);
UNKNOWN_FUNCTION(func_801280f8);

/*
    Enables/disables effect flags
*/
void marioEffectFlagOn(u8 mask);
void marioEffectFlagOff(u8 mask);

UNKNOWN_FUNCTION(func_80128378);
UNKNOWN_FUNCTION(func_801289bc);
UNKNOWN_FUNCTION(func_80128d1c);

/*
    Renders the player
*/
void marioDisp();

UNKNOWN_FUNCTION(func_80128f2c);
UNKNOWN_FUNCTION(func_80128fd4);
UNKNOWN_FUNCTION(func_801291f8);

/*
    Converts a front/rear animation name to its rear/front equivalent
*/
DECOMP_STATIC(const char * mario_toRearPose(const char * name))
DECOMP_STATIC(const char * mario_toFrontPose(const char * name))

UNKNOWN_FUNCTION(func_801299f8);
UNKNOWN_FUNCTION(func_80129d8c);
UNKNOWN_FUNCTION(func_80129db8);
UNKNOWN_FUNCTION(func_80129ddc);
UNKNOWN_FUNCTION(func_80129dfc);
UNKNOWN_FUNCTION(func_8012a064);
UNKNOWN_FUNCTION(func_8012a168);
UNKNOWN_FUNCTION(func_8012a354);

/*
    Calculates the damage the player will do to an enemy
*/
s32 marioCalcDamageToEnemy(s32 damageType, s32 tribeId);

/*
    Plays a random damage sound effect at the player's position
*/
void marioPlayDamageSfx();

/*
    Handles an enemy attack
    Flags:
        0x4 don't display damage number
        0x100 inflict No Jump status
        0x200 inflict Slow status
        0x400 inflict No Skills status
        0x800 inflict Flipped Controls status
        0x1000 play Nastasia attack effect
        0x2000 freeze
        0x4000 sleep
        0x10000 inflict Poison status
        0x1, 0x10, 0x8000 unknown
*/
void marioTakeDamage(Vec3 * position, u32 flags, s32 damage);

/*
    Updates the entity being held with Thoreau
*/
void marioUpdateCatch();

/*
    Calculates the position for the entity being held with Thoreau
*/
void marioCalcCatchPos(Vec3 * posOut);

/*
    Drops the entity being held with Thoreau
*/
void marioDropCatch();

UNKNOWN_FUNCTION(func_8012b018);

/*
    Returns whether the player is in 3d
*/
bool marioCheck3d();

UNKNOWN_FUNCTION(func_8012b090);
UNKNOWN_FUNCTION(func_8012b218);
UNKNOWN_FUNCTION(func_8012b2c4);
UNKNOWN_FUNCTION(func_8012b370);
UNKNOWN_FUNCTION(func_8012b39c);
UNKNOWN_FUNCTION(func_8012b498);
UNKNOWN_FUNCTION(func_8012b4ac);
UNKNOWN_FUNCTION(func_8012b4f8);

/*
    Sets the player's gravity direction, see enum above
*/
void marioSetGravity(s32 dir);

/*
    Returns the player's gravity direction, see enum above
*/
s32 marioGetGravity();

/*
    Returns a downward unit vector for the current gravity
*/
void marioGetGravUnitDown(Vec3 * dest);

/*
    Changes the direction of a vector based on current gravity
*/
void marioApplyGravity(Vec3 * in, Vec3 * out);

/*
    Calculates the dot product of a vector with each direction's unit vector
*/
void marioGravityDotProduct(Vec3 * in, Vec3 * out);

UNKNOWN_FUNCTION(func_8012b7f8);
UNKNOWN_FUNCTION(func_8012ba68);
UNKNOWN_FUNCTION(func_8012bb80);

/*
    Changes the active pane boundaries and updates which pane the player is in
*/
void marioSetPaneBoundaries(MarioPaneBoundary * boundaries); 

/*
    Calculates the pane of a vector position
    -1 if no panes are defined or the position doesn't fall within any pane
*/
s32 marioGetPaneForPos(Vec3 * pos);

/*
    Scales the player when they're unable to regrow fully when cancelling Dottie
*/
void marioHandleSquash();

UNKNOWN_FUNCTION(func_8012c0f8);
UNKNOWN_FUNCTION(func_8012c188);
UNKNOWN_FUNCTION(func_8012c218);
UNKNOWN_FUNCTION(func_8012c948);
UNKNOWN_FUNCTION(func_8012ca58);
UNKNOWN_FUNCTION(func_8012caec);
UNKNOWN_FUNCTION(func_8012cb00);
UNKNOWN_FUNCTION(func_8012cb6c);
UNKNOWN_FUNCTION(func_8012cbcc);
UNKNOWN_FUNCTION(func_8012cc38);
UNKNOWN_FUNCTION(func_8012cc98);
UNKNOWN_FUNCTION(func_8012cd9c);
UNKNOWN_FUNCTION(func_8012ce4c);
UNKNOWN_FUNCTION(func_8012cea8);
UNKNOWN_FUNCTION(func_8012cf20);
UNKNOWN_FUNCTION(func_8012cf44);

/*
    Forces respawn position
*/
void marioLockRespawnPos(Vec3 * pos, Unk);

/*
    Re-enables automatic respawn position calculation
*/
void marioUnlockRespawnPos();

/*
    Forces the player model to face towards a point
*/
void marioLockFacingDir(Vec3 * target);

/*
    Re-enables free facing direction for the player model
*/
void marioUnlockFacing();

UNKNOWN_FUNCTION(func_8012d230);
UNKNOWN_FUNCTION(func_8012d494);

/*
    Spawns/removes Squirps following the player
*/
void marioAddTamara();
void marioRemoveTamara();

UNKNOWN_FUNCTION(func_8012d8b4);

/*
    Spawns/removes Luvbi following the player
*/
void marioAddAngeko();
void marioRemoveAngeko();

UNKNOWN_FUNCTION(func_8012d98c);
UNKNOWN_FUNCTION(func_8012d9fc);
bool func_8012dab0();

CPP_WRAPPER_END()

