#include <common.h>
#include <msl/string.h>
#include <spm/animdrv.h>
#include <spm/mario.h>
#include <spm/mario_motion.h>
#include <spm/mot_walk.h>

extern "C" {

extern const char lbl_80333268[];
extern u32 lbl_80426508[8];

extern void func_80118bbc(HitObj * hitObj, u32 interval);
extern bool func_80146570();
extern bool func_801513a4();
extern bool func_8016c694();
extern void func_801765a4();

void func_80119040(MarioWork * mp)
{
    bool initialized = false;

    if (mp->trigFlags & 1) {
        u32 flags = mp->flags;

        mp->trigFlags &= ~1;
        mp->flags &= ~0x000f0000;
        mp->miscFlags &= ~1;

        if (flags & 0x00100000)
            func_801765a4();

        f32 motionTimer = mp->unknown_0x4c;
        if (!motionTimer) {
            MarioWork * speedWork = marioGetPtr();
            f32 speed;

            if (speedWork->miscFlags & 0x00800000)
                speed = 2.0f;
            else
                speed = speedWork->walkSpeed;

            mp->xzSpeed = func_80118e88(speed, true);
        }

        if (mp->unknown_0x3ac != 1 && mp->unknown_0x3ac != 3) {
            if (mp->caught.npc != NULL) {
                marioChgPose(&lbl_80333268[0x27]);
            } else if (mp->flags & 0x8000) {
                if (!func_801265a0())
                    marioChgPose(&lbl_80333268[0x2b]);
            } else if (mp->miscFlags & 0x00800000) {
                marioChgPose(&lbl_80333268[0x2f]);
            } else {
                marioChgPose(&lbl_80333268[0x33]);
            }
        }

        if (!(mp->dispFlags & 0x01000000))
            mp->subMotionId = 0;

        mp->unknown_0x44 = 0.0f;
        mp->miscFlags &= ~0x30000000;
        mp->unknown_0x1348 = mp->dispDirectionTarget;
        mp->airTimer = 0.0f;
        initialized = true;
    }

    if (mp->unknown_0x1348 != mp->dispDirectionTarget) {
        f32 loop = animPoseGetLoopTimes(mp->animPoseIds[func_80126618()]);
        f32 frame = 0.25f + loop;
        s32 whole = (s32) frame;
        f32 fraction = frame - (f32) whole;

        if (fraction < 0.5f)
            mp->unknown_0x134c = 0.5f + (f32) whole;
        else
            mp->unknown_0x134c = 1.0f + (f32) whole;
    }

    mp->unknown_0x1348 = mp->dispDirectionTarget;

    if (mp->unknown_0x3ac == 3 && strcmp(mp->curPoseName, &lbl_80333268[0x33]) == 0)
        mp->unknown_0x3ac = 0;

    if (mp->unknown_0x3ac == 1 && marioIsAnimFinished()) {
        mp->unknown_0x3ac = 2;
        marioChgPose(&lbl_80333268[0x27]);
    }

    if (mp->unknown_0x3ac == 3 && marioIsAnimFinished()) {
        mp->unknown_0x3ac = 0;
        marioChgPose(&lbl_80333268[0x33]);
    }

    if (mp->unknown_0x248 == 1.0f) {
        if (func_8016c694()) {
            if (strcmp(mp->curPoseName, &lbl_80333268[0x33]) == 0)
                marioChgPose(&lbl_80333268[0x37]);
        } else if (strcmp(mp->curPoseName, &lbl_80333268[0x37]) == 0) {
            marioChgPose(&lbl_80333268[0x33]);
        }
    }

    if (func_80146ae8())
        return;

    if (!(mp->flags & 0x01000000) || !initialized) {
        if (func_801466a8())
            return;
    }

    if (func_80148870())
        return;
    if (func_80146570())
        return;
    if (func_801468c8())
        return;

    if (func_801513a4() && !(mp->statusFlags & 2)) {
        marioChgMot(MOT_DASH);
        return;
    }

    f32 motionTimer = mp->unknown_0x4c;
    f32 direction = mp->unknown_0x16c;
    mp->directionView = direction;

    if (!motionTimer)
        func_80148c28(mp, 1, 0);

    HitObj * hitObj = mp->hitObjs1[2];
    if ((hitObj != NULL && !(hitObj->flags & 2)) ||
        (mp->miscFlags & 0x8000) || mp->unknown_0x3b0 != 0) {
        u32 interval;

        if (mp->miscFlags & 0x00800000) {
            interval = 20;
        } else if (mp->miscFlags & 0x4000) {
            interval = lbl_80426508[mp->character * 2] / 2;
        } else {
            interval = lbl_80426508[mp->character * 2];
            if (mp->unknown_0x248 == 1.0f && func_8016c694())
                interval = lbl_80426508[mp->character * 2 + 1];
        }

        if (mp->statusFlags & 0x400)
            interval /= 2;
        if ((mp->statusFlags & 0x800) || (mp->effectFlags & 8))
            interval *= 2;

        func_80118bbc(mp->hitObjs1[2], interval + (interval & 1));
    }

    mp->unknown_0x44 += 1.0f;

    if ((mp->flags & 0x8000) && !func_801265a0())
        marioChgPose(&lbl_80333268[0x2b]);

    bool shouldStop;
    f32 stopSpeed = mp->unknown_0x168;
    if (!stopSpeed && !mp->xzSpeed &&
        (mp->dispDirectionCurrent == mp->dispDirectionTarget ||
         animPoseGetLoopTimes(mp->animPoseIds[func_80126618()]) >= mp->unknown_0x134c))
        shouldStop = true;
    else
        shouldStop = false;

    if (shouldStop)
        marioChgMot(MOT_STAY);
}

}
