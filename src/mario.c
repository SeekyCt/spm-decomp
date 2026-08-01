#include <common.h>
#include <msl/string.h>
#include <spm/animdrv.h>
#include <spm/camdrv.h>
#include <spm/fairy.h>
#include <spm/guide.h>
#include <spm/mario.h>
#include <spm/mario_hit.h>
#include <spm/mario_motion.h>
#include <spm/mot_walk.h>
#include <spm/seqdrv.h>
#include <spm/spmario.h>
#include <spm/system.h>
#include <wii/mtx.h>

extern "C" {

extern const char lbl_80333648[] = "p_wii_mario";
extern const char lbl_80333654[] = "p_big_mario";
extern const char lbl_80333660[] = "p_wii_peach";
extern const char lbl_8033366c[] = "p_big_peach";
extern const char lbl_80333678[] = "p_wii_kuppa";
extern const char lbl_80333684[] = "p_big_kuppa";
extern const char lbl_80333690[] = "p_wii_luigi";
extern const char lbl_8033369c[] = "p_big_luigi";
extern const char lbl_803336a8[] = "p_wii_mario_r";
extern const char lbl_803336b8[] = "sp_waku";
extern const char lbl_803336c0[] = "sp_naka";
extern const char lbl_803336c8[] = "sp_bou";
extern const char lbl_803336d0[] = "sp_ball";
extern const char lbl_803336d8[] = "gomu";
extern const char lbl_803336e0[] = "osage";
extern const char lbl_803336e8[] = "R_momi";
extern const char lbl_803336f0[] = "R_earring";
extern const char lbl_803336fc[] = "u_poni";
extern const char lbl_80333704[] = "R_u_kami";
extern const char lbl_80333710[] = "L_u_kami_1";
extern const char lbl_8033371c[] = "L_u_kami_2";
extern const char lbl_80333728[] = "u_kami_down";
extern const char lbl_80333734[] = "l_ha";
extern const char lbl_8033373c[] = "r_ha";
extern const char lbl_803339a4[];
extern const char * lbl_804265e8[4][2];
extern const char * lbl_80426608[4][2];
extern void func_80130a50();
extern void func_80130c80(s32 param);
extern void func_80154c80();
extern void func_80154c88();
extern void func_801560f0(s32 param);
extern void func_801521ec();
extern void func_80176918();
extern void func_8017719c();
extern s32 func_80176688();

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} MarioPoseQuaternion;

typedef struct
{
    s32 flags;
    Vec3 position;
    union
    {
        f32 direction;
        MarioPoseQuaternion rotation;
    };
} MarioPoseTransform;
SIZE_ASSERT(MarioPoseTransform, 0x20)

// .bss
u64 lbl_80510450;
static MarioWork mario_work;
MarioPoseTransform lbl_805119c8[3];

// .data
CharacterProperties characterProperties[4] = {
    {20.0f, 39.0f, 1.5f, 3.0f, 1.4f},
    {24.0f, 48.0f, 1.3f, 2.6f, 1.0f},
    {47.0f, 54.0f, 1.1f, 2.2f, 1.4f},
    {20.0f, 45.0f, 1.5f, 3.0f, 1.4f},
};

// .sbss
static f32 mario_gameSpeedScale;
f32 lbl_805ae8ec;
const char * lbl_805ae8f0;
s32 lbl_805ae8f4;
static s64 mario_mainLastRunTime;

s32 func_80121e18(const void * param)
{
    u32 flags = ((const u32 *) param)[1];

    if (flags & 0x80000000)
        return 0;
    if (flags & 0x2000)
        return 2;
    return !(flags & (0x04000000 | 0x6));
}

f32 marioGetGameSpeedScale()
{
    return mario_gameSpeedScale;
}

void func_80121e58()
{
    MarioWork * mp = &mario_work;
    f32 target;

    if (4.0f * mp->unknown_0x11c > 12.0f)
        target = 12.0f;
    else if (mp->unknown_0x11c > func_80118f40() + 0.5f)
        target = 4.0f * marioGetDashSpd();
    else if (mp->unknown_0x11c > 1.0f)
        target = 4.0f * func_80118f40();
    else if (mp->unknown_0x11c > 0.1f)
        target = mp->unknown_0x11c;
    else
        target = 0.0f;

    if (target > lbl_805ae8ec) {
        lbl_805ae8ec = 0.3f * mario_gameSpeedScale + lbl_805ae8ec;
        if (lbl_805ae8ec > target)
            lbl_805ae8ec = target;
    } else {
        lbl_805ae8ec = lbl_805ae8ec - 0.3f * mario_gameSpeedScale;
        if (lbl_805ae8ec < target)
            lbl_805ae8ec = target;
    }
}

f32 func_80121f40()
{
    return lbl_805ae8ec;
}

MarioWork * marioGetPtr()
{
    return &mario_work;
}

void func_80121f54()
{
    const char ** table = &lbl_804265e8[0][0];
    MarioWork * mp = marioGetPtr();
    const char ** names;
    s32 i;

    switch (mp->character) {
        case PLAYER_MARIO:
        case PLAYER_LUIGI:
            names = &table[16];
            break;
        case PLAYER_PEACH:
            names = &table[22];
            break;
        case PLAYER_BOWSER:
            names = &table[32];
            break;
    }

    for (i = 0; *names != NULL; names++, i++)
        mp->unknown_0x26c[i] = animPoseGetGroupIdx(mp->animPoseIds[0], *names);
    mp->unknown_0x26c[i] = -1;

    switch (mp->character) {
        case PLAYER_MARIO:
        case PLAYER_BOWSER:
        case PLAYER_LUIGI:
            names = &lbl_805ae8f0;
            break;
        case PLAYER_PEACH:
            names = &table[37];
            break;
    }

    for (i = 0; *names != NULL; names++, i++)
        mp->unknown_0x294[i] = animPoseGetGroupIdx(mp->animPoseIds[0], *names);
    mp->unknown_0x294[i] = -1;

    switch (mp->character) {
        case PLAYER_MARIO:
        case PLAYER_PEACH:
        case PLAYER_BOWSER:
        case PLAYER_LUIGI:
            names = &table[42];
            break;
    }

    for (i = 0; *names != NULL; names++, i++)
        mp->unknown_0x2bc[i] = animPoseGetGroupIdx(mp->animPoseIds[0], *names);
    mp->unknown_0x2bc[i] = -1;

    if (mp->character != PLAYER_MARIO) {
        mp->unknown_0x2e4[0] = -1;
        mp->unknown_0x2f8[0] = -1;
    } else {
        names = &table[46];
        for (i = 0; *names != NULL; names++, i++)
            mp->unknown_0x2e4[i] = animPoseGetGroupIdx(mp->animPoseIds[1], *names);
        mp->unknown_0x2e4[i] = -1;

        names = &table[52];
        for (i = 0; *names != NULL; names++, i++)
            mp->unknown_0x2f8[i] = animPoseGetGroupIdx(mp->animPoseIds[1], *names);
        mp->unknown_0x2f8[i] = -1;
    }
}

bool marioMainPoseDispCb(void * param, s32 animGroupIdx, s32)
{
    MarioWork * mp = (MarioWork *) param;
    u32 dispFlags;
    s32 i;

    if (mp->character == PLAYER_PEACH &&
        ((dispFlags = mp->dispFlags) & 0x08000000)) {
        if (!(dispFlags & 0x00400000)) {
            for (i = 0; mp->unknown_0x26c[i] != -1; i++) {
                if (i > 3 && i < 8 && animGroupIdx == mp->unknown_0x26c[i])
                    return true;
            }
        }

        for (i = 0; mp->unknown_0x294[i] != -1; i++) {
            if (animGroupIdx == mp->unknown_0x294[i])
                return true;
        }
    } else {
        dispFlags = mp->dispFlags;
        if (!(dispFlags & 0x00400000)) {
            for (i = 0; mp->unknown_0x26c[i] != -1; i++) {
                if (animGroupIdx == mp->unknown_0x26c[i])
                    return true;
            }
        } else {
            for (i = 0; mp->unknown_0x294[i] != -1; i++) {
                if (animGroupIdx == mp->unknown_0x294[i])
                    return true;
            }
        }
    }

    if (!(dispFlags & 0x00800000) || (dispFlags & 0x01000000)) {
        for (i = 0; mp->unknown_0x2bc[i] != -1; i++) {
            if (animGroupIdx == mp->unknown_0x2bc[i])
                return true;
        }
    }

    return false;
}

bool marioSubPoseDispCb(void * param, s32 animGroupIdx, s32)
{
    MarioWork * mp = (MarioWork *) param;
    u32 dispFlags = mp->dispFlags;
    s32 i;

    if (!(dispFlags & 0x00400000)) {
        for (i = 0; mp->unknown_0x2e4[i] != -1; i++) {
            if (animGroupIdx == mp->unknown_0x2e4[i])
                return true;
        }
    }

    if (!(dispFlags & 0x00800000)) {
        for (i = 0; mp->unknown_0x2f8[i] != -1; i++) {
            if (animGroupIdx == mp->unknown_0x2f8[i])
                return true;
        }
    }

    return false;
}

void marioSetAnimGroup(s32 group)
{
    MarioWork * mp = marioGetPtr();
    s32 character = mp->character;
    bool valid = group >= 0 && group < 2;

    if (valid == false)
        __assert(&lbl_803339a4[0], 521, &lbl_803339a4[8]);

    if (mp->animPoseIds[0] >= 0)
        animPoseRelease(mp->animPoseIds[0]);

    mp->animPoseIds[0] = animPoseEntry(lbl_804265e8[character][group], 2);
    if (group == 0)
        animPoseSetDispCallback(mp->animPoseIds[0], marioMainPoseDispCb, mp);

    if (mp->animPoseIds[1] >= 0)
        animPoseRelease(mp->animPoseIds[1]);

    const char * poseName = lbl_80426608[character][group];
    if (poseName != NULL) {
        mp->animPoseIds[1] = animPoseEntry(poseName, 2);
        if (group == 0)
            animPoseSetDispCallback(mp->animPoseIds[1], marioSubPoseDispCb, mp);
    } else {
        mp->animPoseIds[1] = -1;
        mp->dispFlags &= ~0x80000000;
    }

    if (group == 0)
        func_80121f54();
}

void marioUpdateCharProperties()
{
    MarioWork * mp = marioGetPtr();

    if (mp->flags & 0x40000000) {
        CharacterProperties * properties = &characterProperties[mp->character];

        switch (mp->character) {
            case PLAYER_MARIO:
            case PLAYER_LUIGI:
                mp->baseHitboxWidth = 150.0f;
                mp->hitboxWidth = 150.0f;
                mp->baseHitboxHeight = 200.0f;
                mp->hitboxHeight = 200.0f;
                break;
            case PLAYER_PEACH:
                mp->baseHitboxWidth = 150.0f;
                mp->hitboxWidth = 150.0f;
                mp->baseHitboxHeight = 275.0f;
                mp->hitboxHeight = 275.0f;
                break;
            case PLAYER_BOWSER:
                mp->baseHitboxWidth = 200.0f;
                mp->hitboxWidth = 200.0f;
                mp->baseHitboxHeight = 290.0f;
                mp->hitboxHeight = 290.0f;
                break;
        }

        mp->walkSpeed = properties->walkSpeed * 1.5f;
        mp->dashSpeed = properties->dashSpeed * 1.5f;
        mp->unknown_0x158 = properties->unknown_0x10 * 1.5f;
        func_80131ee8();
    } else if (mp->miscFlags & 0x4000) {
        CharacterProperties * properties = &characterProperties[mp->character];

        if (mp->character == PLAYER_BOWSER) {
            mp->baseHitboxHeight = 7.5f;
            mp->hitboxHeight = 7.5f;
        } else {
            mp->baseHitboxHeight = 5.0f;
            mp->hitboxHeight = 5.0f;
        }

        mp->baseHitboxWidth = 7.5f;
        mp->hitboxWidth = 7.5f;
        mp->walkSpeed = properties->walkSpeed;
        mp->dashSpeed = properties->dashSpeed;
        mp->unknown_0x158 = properties->unknown_0x10;
        func_80131ee8();
    } else {
        CharacterProperties * properties = &characterProperties[mp->character];

        mp->baseHitboxWidth = properties->hitboxWidth;
        mp->hitboxWidth = properties->hitboxWidth;
        mp->baseHitboxHeight = properties->hitboxHeight;
        mp->hitboxHeight = properties->hitboxHeight;
        mp->walkSpeed = properties->walkSpeed;
        mp->dashSpeed = properties->dashSpeed;
        mp->unknown_0x158 = properties->unknown_0x10;

        if (mp->flags & 0x02000000) {
            mp->hitboxWidth = properties->hitboxWidth * 0.4f;
            mp->hitboxHeight = properties->hitboxHeight * 0.4f;
        }

        Vec3 scale = {1.0f, 1.0f, 1.0f};
        Vec3 unknown = {1.0f, 1.0f, 1.0f};
        mp->scale = scale;
        mp->unknown_0xc8 = unknown;
        mp->flags &= ~0x80000000;
        func_80131ee8();
    }
}

f32 marioGetHitboxWidthRaw()
{
    return characterProperties[mario_work.character].hitboxWidth;
}

f32 marioGetHitboxHeightRaw()
{
    return characterProperties[mario_work.character].hitboxHeight;
}

f32 marioGetHitboxHeight_BowserSpecial()
{
    if (mario_work.character == PLAYER_BOWSER)
        return 63.0f;
    return characterProperties[mario_work.character].hitboxHeight;
}

f32 marioGetDashSpeedRaw(s32 character)
{
    return characterProperties[character].dashSpeed;
}

void marioOnMapChange(f32 spawnX, f32 spawnY, f32 spawnZ)
{
    MarioWork * mp = &mario_work;

    if (strncmp(gp->mapName, gp->prevMapName, 3) == 0) {
        func_80154c88();
    } else {
        func_801560f0(7);
        func_80154c88();
    }

    mp->flags |= 1;
    Vec3 position = {spawnX, spawnY, spawnZ};
    mp->position = position;
    marioChgPose(&lbl_803339a4[51]);

    if (mp->flags & 0x02000000) {
        if (mp->motionId == MOT_SLIT) {
            mp->flags |= 0x00100000;
            func_80176918();
        }
        marioChgMot(MOT_STAY);
    } else {
        switch ((s32) mp->motionId) {
            case MOT_SLIT:
                func_8017719c();
                break;
            default:
                marioChgMot(MOT_STAY);
                break;
        }
    }
}

bool marioChkKey()
{
    if (seqGetSeq() != SEQ_GAME)
        return false;
    if (mario_work.keyOff != 0)
        return false;
    return func_80176688() != 0;
}

bool marioCtrlOnChk()
{
    return mario_work.ctrl == 0;
}

s32 marioCtrlOffChk()
{
    return mario_work.ctrl;
}

s32 marioKeyOffChk()
{
    return mario_work.keyOff;
}

s32 marioCtrlOff()
{
    mario_work.flags |= 2;
    mario_work.ctrl++;
    return mario_work.ctrl;
}

s32 marioCtrlOn()
{
    MarioWork * mp = marioGetPtr();
    mp->ctrl--;
    if (mp->ctrl == 0)
        mp->flags &= ~2;
    return mp->ctrl;
}

s32 marioKeyOff()
{
    mario_work.flags |= 8;
    mario_work.xzSpeedFactor = 0.0f;
    mario_work.keyOff++;
    mario_work.xzSpeed = 0.0f;
    return mario_work.keyOff;
}

s32 marioKeyOn()
{
    MarioWork * mp = marioGetPtr();
    mp->keyOff--;
    if (mp->keyOff == 0)
        mp->flags &= ~8;
    return mp->keyOff;
}

void marioReset()
{
    MarioWork * mp = &mario_work;
    func_80130c80(9);
    func_801560f0(9);
    marioRemoveTamara();
    marioRemoveAngeko();

    mp->dispFlags = 0;
    mp->miscFlags = 0;
    mp->trigFlags = 0;
    marioUpdateCharProperties();
    mp->flags &= ~0x100000;

    {
        MarioWork * paperWork = &mario_work;
        if ((paperWork->dispFlags & MARIO_DISP_FLAG_PAPER) &&
            paperWork->paperPoseName != NULL) {
            animPoseSetPaperAnimGroup(paperWork->animPoseIds[0], 0, 0);
            paperWork->dispFlags &= ~MARIO_DISP_FLAG_PAPER;
            paperWork->paperPoseName = NULL;

            if (paperWork->animPoseIds[5] >= 0) {
                animPaperPoseRelease(paperWork->animPoseIds[5]);
                paperWork->animPoseIds[5] = -1;
            }
        }
    }

    {
        MarioWork * paperWork = &mario_work;
        if ((paperWork->dispFlags & MARIO_DISP_FLAG_PAPER) &&
            !(paperWork->dispFlags & 2) && !(paperWork->flags & 0x40000000)) {
            if (paperWork->paperPoseName == NULL ||
                strcmp(paperWork->paperPoseName, NULL) != 0) {
                paperWork->paperPoseName = NULL;
                paperWork->poseTime = 0;
                paperWork->trigFlags |= 0x2000;
            }
        }
    }

    Vec3 unknown98 = {0.0f, 0.0f, 0.0f};
    Vec3 unknown8C = {0.0f, 0.0f, 0.0f};
    Vec3 unknownA4 = {0.0f, 0.0f, 0.0f};
    Vec3 rotation = {0.0f, 0.0f, 0.0f};
    MarioWork * finalWork = &mario_work;
    mp->dispFlags &= ~8;
    mp->dispFlags &= ~0x100;
    finalWork->ttydRotation = rotation;
    finalWork->unknown_0xa4 = unknownA4;
    finalWork->unknown_0x8c = unknown8C;
    finalWork->unknown_0x98 = unknown98;
    mp->xzSpeed = 0.0f;
    marioChgMot(0);
    fairyOn();
}

void marioOfsRotReset()
{
    MarioWork * mp = &mario_work;
    Vec3 rotation = {0.0f, 0.0f, 0.0f};
    Vec3 unknownA4 = {0.0f, 0.0f, 0.0f};
    Vec3 unknown8C = {0.0f, 0.0f, 0.0f};
    Vec3 unknown98 = {0.0f, 0.0f, 0.0f};

    mp->ttydRotation = rotation;
    mp->unknown_0xa4 = unknownA4;
    mp->unknown_0x8c = unknown8C;
    mp->unknown_0x98 = unknown98;
}

void marioSoundInit()
{
    mario_work.sfxIds[0] = -1;
    mario_work.sfxIds[1] = -1;
    mario_work.sfxIds[2] = -1;
    mario_work.sfxIds[3] = -1;
}

bool marioPoseInit()
{
    const char * poseName;
    MarioWork * mp = &mario_work;
    s32 i;

    for (i = 0; i < 2; i++) {
        if (mp->animPoseIds[i] == -1) {
            poseName = NULL;

            if (i == 0)
                poseName = lbl_804265e8[mp->character][0];
            else if (i == 1)
                poseName = lbl_80426608[mp->character][0];

            if (poseName != NULL) {
                if (animGroupBaseAsync(poseName, 2, NULL) != 0)
                    mp->animPoseIds[i] = animPoseEntry(poseName, 2);
                else
                    return false;
            }
        }
    }

    if (mp->animPoseIds[0] != -1)
        animPoseSetDispCallback(mp->animPoseIds[0], marioMainPoseDispCb, mp);
    if (mp->animPoseIds[1] != -1)
        animPoseSetDispCallback(mp->animPoseIds[1], marioSubPoseDispCb, mp);

    func_80121f54();
    return true;
}

void marioInit()
{
    MarioWork * mp = &mario_work;

    memset(mp, 0, sizeof(*mp));
    mp->unknown_0x18 = 0;
    mp->character = PLAYER_MARIO;
    mp->sfxIds[0] = -1;
    mp->sfxIds[1] = -1;
    mp->sfxIds[2] = -1;
    mp->sfxIds[3] = -1;
    mp->stylishSfxId = -1;
    mp->unknown_0x248 = 1.0f;
    mp->camId = 5;
    mp->unknown_0x39 = 0;
    mp->animPoseIds[2] = -1;
    mp->animPoseIds[3] = -1;
    mp->animPoseIds[4] = -1;
    mp->animPoseIds[0] = -1;
    mp->animPoseIds[1] = -1;
    mp->animPoseIds[5] = -1;
    mp->animPoseIds[6] = -1;
    mp->animPoseIds[7] = -1;
    marioUpdateCharProperties();

    Vec3 zero = {0.0f, 0.0f, 0.0f};
    Vec3 one = {1.0f, 1.0f, 1.0f};
    mp->position = zero;
    mp->scale = one;
    mp->unknown_0xc8 = one;
    mp->unknown_0xec = mp->unknown_0xe0;
    mp->unknown_0xe0 = zero;
    mp->unknown_0x1a4 = 1.0f;
    marioChgPose(&lbl_803339a4[51]);
    marioChgMot(0);

    Vec3 respawnPosition = {0.0f, 0.0f, 0.0f};
    mp->respawnPosition = respawnPosition;
    mp->unknown_0x1308 = -1;
    lbl_805ae8f4 = 0;

    MarioPoseTransform * transform = lbl_805119c8;
    for (s32 i = 0; i < 3; i++, transform++) {
        transform->position = mp->position;

        if (mp->dispFlags & 0x40) {
            f32 scale;
            Mtx34 concatMtx;
            Mtx34 scaleMtx;
            transform->flags = 1;

            if (mario_work.flags & 0x02000000)
                scale = 0.8f;
            else
                scale = 2.0f;

            PSMTXScale(scaleMtx, 1.0f / (scale * mp->scale.x),
                       1.0f / (scale * mp->scale.y),
                       1.0f / (scale * mp->scale.z));
            ((void (*)(const Mtx34, const Mtx34, Mtx34)) PSMTXConcat)(
                scaleMtx, mp->unknown_0x130c, concatMtx);
            ((void (*)(MarioPoseQuaternion *, const Mtx34)) QUATMtx)(
                &transform->rotation, concatMtx);
        } else {
            transform->flags = 0;
            transform->direction = mp->dispDirectionCurrent;
        }
    }

    func_80130a50();
    func_80154c80();
    guideInit();
    fairyInit();

    Vec3 gravRight = {1.0f, 0.0f, 0.0f};
    Vec3 gravUp = {0.0f, 1.0f, 0.0f};
    Vec3 gravForward = {0.0f, 0.0f, 1.0f};
    mp->gravityType = 0;
    mp->gravUnitRight = gravRight;
    mp->gravUnitUp = gravUp;
    mp->gravUnitForward = gravForward;
    mp->unknown_0x154c = -1;
    mp->pane = -1;
    mp->unknown_0x164 = 0.0f;
    func_801521ec();
}

void func_80124374()
{
    if (mario_work.respawnPositionLockMode == 1)
        mario_work.miscFlags |= 0x100;
    else
        mario_work.miscFlags &= ~0x300;
}

bool func_80126618()
{
    return (mario_work.dispFlags & 0x80000000) != 0;
}

void func_8012662c()
{
    MarioWork * mp = marioGetPtr();
    if (mp->animPoseIds[0] >= 0)
        ((void (*)(s32)) func_80043b90)(mp->animPoseIds[0]);
    if (mp->animPoseIds[1] >= 0)
        ((void (*)(s32)) func_80043b90)(mp->animPoseIds[1]);
    if (mp->animPoseIds[5] >= 0)
        ((void (*)(s32)) func_80043b90)(mp->animPoseIds[5]);
}

void func_80126688()
{
    MarioWork * mp = marioGetPtr();
    if (mp->animPoseIds[0] >= 0)
        ((void (*)(s32)) func_80043ca4)(mp->animPoseIds[0]);
    if (mp->animPoseIds[1] >= 0)
        ((void (*)(s32)) func_80043ca4)(mp->animPoseIds[1]);
    if (mp->animPoseIds[5] >= 0)
        ((void (*)(s32)) func_80043ca4)(mp->animPoseIds[5]);
}

void marioPaperLightOn()
{
    ((void (*)(s32, s32)) animPoseSetMaterialLightFlagOn)(mario_work.animPoseIds[0], 1);
}

void marioPaperLightOff()
{
    ((void (*)(s32, s32)) animPoseSetMaterialLightFlagOff)(mario_work.animPoseIds[0], 1);
}

MarioAnimFinishedResult marioIsAnimFinished()
{
    if (mario_work.trigFlags & 0x1000)
        return MARIO_ANIM_NOT_FINISHED;

    s32 poseId = mario_work.animPoseIds[(mario_work.dispFlags & 0x80000000) != 0];
    if (animPoseGetLoopTimes(poseId) >= 1.0f)
        return MARIO_ANIM_FINISHED;
    return MARIO_ANIM_NOT_FINISHED;
}

bool marioChkInScreen(s32 x, s32 y)
{
    return x >= 0 && x <= 600 && y >= 0 && y <= 480;
}

f32 marioGetScale()
{
    if (mario_work.flags & 0x02000000)
        return 2.0f;
    return 0.8f;
}

void marioEffectFlagOff(u8 mask)
{
    mario_work.effectFlags &= ~mask;
}

void func_80129d8c()
{
    if (mario_work.unknown_0x3d4 == 0)
        mario_work.unknown_0x3d4 = 2;
    else
        mario_work.unknown_0x3d4 = 1;
}

s32 func_80129db8()
{
    MarioWork * mp = marioGetPtr();
    if (mp->flags & 1)
        return mp->unknown_0x3d4;
    return 0;
}

void func_80129ddc()
{
    MarioWork * mp = marioGetPtr();
    if (mp->flags & 1)
        mp->unknown_0x3d4 = 0;
}

bool marioCheck3d()
{
    return camCheck3d(mario_work.camId);
}

bool func_8012b498()
{
    return (mario_work.miscFlags & 0x4000) != 0;
}

u32 func_8012b4f8()
{
    return mario_work.miscFlags & 0x80000000;
}

s32 marioGetGravity()
{
    return mario_work.gravityType;
}

void marioGetGravUnitDown(Vec3 * dest)
{
    PSVECScale(&mario_work.gravUnitUp, dest, -1.0f);
}

void marioApplyGravity(Vec3 * in, Vec3 * out)
{
    Vec3 temp;

    PSVECScale(&mario_work.gravUnitRight, out, in->x);
    PSVECScale(&mario_work.gravUnitUp, &temp, in->y);
    PSVECAdd(out, &temp, out);
    PSVECScale(&mario_work.gravUnitForward, &temp, in->z);
    PSVECAdd(out, &temp, out);
}

void marioGravityDotProduct(Vec3 * in, Vec3 * out)
{
    out->x = PSVECDotProduct(&mario_work.gravUnitRight, in);
    out->y = PSVECDotProduct(&mario_work.gravUnitUp, in);
    out->z = PSVECDotProduct(&mario_work.gravUnitForward, in);
}

void marioSetPaneBoundaries(MarioPaneBoundary * boundaries)
{
    Vec3 position;
    s32 pane;
    MarioWork * mp = marioGetPtr();

    mp->paneBoundaries = boundaries;
    if (mp->motionId == MOT_BOTTOMLESS)
        position = mp->respawnPosition;
    else
        position = mp->position;

    pane = marioGetPaneForPos(&position);
    if (pane != mp->pane)
    {
        if (mp->paneChangeFunc != NULL)
        {
            if (mp->paneChangeFunc(pane))
                mp->pane = pane;
        }
        else
        {
            mp->pane = pane;
        }
    }
}

s32 marioGetPaneForPos(Vec3 * pos)
{
    MarioPaneBoundary * boundary = mario_work.paneBoundaries;
    if (boundary == NULL)
        return -1;

    for (s32 pane = 0; boundary->min != 2147483648.0f;)
    {
        f32 min = boundary->min;
        if (min <= pos->z && pos->z <= boundary->max)
            return pane;
        boundary++;
        pane++;
    }
    return -1;
}

#pragma opt_propagation off
void marioLockRespawnPos(Vec3 * pos, Unk mode)
{
    MarioWork * mp = marioGetPtr();
    mp->respawnPosition = *pos;
    mp->respawnPositionLockMode = mode != 0;
    mp->flags |= 0x800000;
}
#pragma opt_propagation reset

void marioUnlockRespawnPos()
{
    MarioWork * mp = marioGetPtr();
    if (!(mp->flags & 0x800000))
        return;

    mp->flags &= ~0x800000;
    marioUpdateRespawnPos();
}

void marioLockFacingDir(Vec3 * target)
{
    mario_work.facingTarget = *target;
    mario_work.dispFlags |= MARIO_DISP_FLAG_LOCK_FACING;
}

void marioUnlockFacing()
{
    mario_work.dispFlags &= ~MARIO_DISP_FLAG_LOCK_FACING;
}

void marioRemoveTamara()
{
    if (mario_work.miscFlags & MARIO_MISC_FLAG_SQUIRPS)
    {
        mario_work.miscFlags &= ~MARIO_MISC_FLAG_SQUIRPS;
        fairyFreeExtra();
    }
}

void marioRemoveAngeko()
{
    if (mario_work.miscFlags & MARIO_MISC_FLAG_LUVBI)
    {
        mario_work.miscFlags &= ~MARIO_MISC_FLAG_LUVBI;
        fairyFreeExtra();
    }
}

bool func_8012dab0()
{
    return (mario_work.miscFlags & 8) != 0;
}

}
