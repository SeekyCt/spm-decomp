#include <common.h>
#include <msl/math.h>
#include <spm/mario.h>
#include <spm/mario_motion.h>
#include <spm/mot_walk.h>
#include <spm/system.h>

extern "C" {

typedef struct
{
    f32 acceleration;
    f32 braking;
} MarioMoveParams;

MarioMoveParams lbl_804275e8[4] = {
    {1.0f, 1.2f},
    {1.0f, 1.5f},
    {1.0f, 1.2f},
    {1.0f, 0.55f},
};

extern const f32 lbl_805b279c;
extern const f32 lbl_805b27b0;
extern const f32 lbl_805b27b4;
extern const f32 lbl_805b27b8;
extern const f32 lbl_805b27bc;
extern const f32 lbl_805b27c0;
extern const f32 lbl_805b2800;
extern const f32 lbl_805b280c;
extern const f32 lbl_805b2834;
extern const f32 lbl_805b2844;
extern const f32 lbl_805b284c;
extern const f32 lbl_805b2858;
extern const f32 lbl_805b285c;
extern const f32 lbl_805b2860;
extern const f32 lbl_805b2864;
extern const f32 lbl_805b2868;
extern const f32 lbl_805b286c;
extern const f32 lbl_805b2870;

extern f32 func_80113cfc();

#pragma dont_inline on

void func_80148c28(MarioWork * mp, s32 mode, s32 preserveMomentum)
{
    s32 turnAmount = 32;
    s32 turnMaximum = 480;
    bool accelerating = false;
    MarioMoveParams * params = &lbl_804275e8[mp->character];
    bool smoothDirection = false;
    bool restoreDirection = false;
    f32 angleThreshold = lbl_805b2834;

    if ((mp->miscFlags & 0x10000000) ||
        (preserveMomentum && mp->unknown_0x168 == lbl_805b279c)) {
        if (mp->hitObjs1[1] != NULL && mp->airTimer != lbl_805b279c) {
            mp->xzSpeedFactor = lbl_805b279c;
            mp->xzSpeed = lbl_805b279c;
        }
        return;
    }

    if (mp->statusFlags & 2)
        mode = 1;

    if (mode == 2) {
        turnMaximum = 1440;
        turnAmount = 64;
    } else if (mp->unknown_0x168 == lbl_805b279c && mode > 0 &&
               mp->xzSpeedFactor > lbl_805b27b0) {
        turnAmount = 64;
    }

    if (mode == 1 && mp->xzSpeedFactor > (f32) turnMaximum) {
        turnMaximum = 1440;
        turnAmount *= 2;
    }

    if (preserveMomentum) {
        if (mp->motionId == MOT_BOUNCE ||
            (mp->prevMotionId == MOT_BOUNCE && mp->motionId == MOT_FALL)) {
            turnAmount /= 2;
        } else {
            turnAmount -= turnAmount / 4;
        }
        angleThreshold = lbl_805b27bc;
    } else if (mp->motionId == MOT_CROUCH) {
        smoothDirection = true;
    }

    mp->unknown_0x3c8 = mp->unknown_0x3c4;
    mp->unknown_0x3c4 = mp->unknown_0x16c;

    if (mp->miscFlags & 0x20) {
        turnAmount = (s32) ((f32) turnAmount * params->braking);
        goto apply_turn;
    }

    if (!smoothDirection) {
        if (mode == 1 && mp->xzSpeedFactor > lbl_805b27b0) {
            if (mp->unknown_0x168 != lbl_805b279c &&
                fabsf(compAngle(mp->unknown_0x16c, mp->unknown_0x3c8)) >= angleThreshold) {
                turnAmount = (s32) ((f32) turnAmount *
                    (lbl_805b2800 * params->braking));
            } else {
                turnAmount = (s32) ((f32) turnAmount * params->braking);
            }
            goto apply_turn;
        }

        if (mp->unknown_0x168 != lbl_805b279c &&
            fabsf(compAngle(mp->unknown_0x3c4, mp->unknown_0x3c8)) < angleThreshold) {
            turnAmount = (s32) ((f32) turnAmount * params->acceleration);
            accelerating = true;
            goto apply_turn;
        }

        if (preserveMomentum && mp->unknown_0x168 != lbl_805b279c &&
            fabsf(compAngle(mp->unknown_0x3c4, mp->unknown_0x3c8)) < lbl_805b2858) {
            if (mp->xzSpeed != lbl_805b279c) {
                if (mp->xzSpeedFactor < lbl_805b27b0 &&
                    fabsf(compAngle(mp->unknown_0x3c4, mp->unknown_0x3c8)) < lbl_805b284c) {
                    turnAmount = (s32) ((f32) turnAmount *
                        (lbl_805b27c0 * params->acceleration));
                } else {
                    turnAmount = (s32) ((f32) turnAmount *
                        (lbl_805b27c0 * params->acceleration));
                    restoreDirection = true;
                }
            }
            accelerating = true;
        } else if (mp->unknown_0x168 != lbl_805b279c) {
            turnAmount = (s32) ((f32) turnAmount *
                (lbl_805b2800 * params->braking));
            if (preserveMomentum)
                turnAmount = (s32) ((f32) turnAmount * lbl_805b27b8);
        } else {
            turnAmount = (s32) ((f32) turnAmount * params->braking);
            if (preserveMomentum)
                turnAmount = (s32) ((f32) turnAmount * lbl_805b27b8);
        }
    } else {
        if (mp->unknown_0x168 != lbl_805b279c &&
            fabsf(compAngle(mp->unknown_0x16c, mp->unknown_0x3c8)) < lbl_805b2834)
            turnAmount = (s32) ((f32) turnAmount *
                (lbl_805b27c0 * params->braking));
        else
            turnAmount = (s32) ((f32) turnAmount * params->braking);
    }

apply_turn:
    turnAmount *= marioGetGameSpeedScale();

    if (accelerating) {
        mp->xzSpeedFactor += (f32) turnAmount;
        if (mp->xzSpeedFactor > (f32) turnMaximum)
            mp->xzSpeedFactor = (f32) turnMaximum;

        if (restoreDirection)
            mp->unknown_0x3c4 = mp->unknown_0x3c8;
    } else {
        if (mp->unknown_0x168 == lbl_805b279c &&
            (mp->motionId == MOT_BOUNCE || mp->prevMotionId == MOT_BOUNCE))
            turnAmount = (s32) ((f32) turnAmount * lbl_805b285c);

        if (mp->motionId == MOT_SWIM) {
            if (mp->unknown_0x168 == lbl_805b279c && mp->motionId == MOT_SWIM)
                turnAmount = (s32) ((f32) turnAmount * lbl_805b285c);
        } else {
            MarioWork * checkWork = marioGetPtr();
            bool specialSpeed;

            if (checkWork->motionId == MOT_SWIM)
                specialSpeed = true;
            else if ((checkWork->miscFlags & 0x100) &&
                     (checkWork->miscFlags & 0x20))
                specialSpeed = true;
            else
                specialSpeed = false;

            if (specialSpeed)
                turnAmount = (s32) ((f32) turnAmount * lbl_805b285c);
        }

        if (preserveMomentum && mp->character == PLAYER_BOWSER)
            turnAmount = (s32) ((f32) turnAmount * lbl_805b2860);

        mp->xzSpeedFactor -= (f32) turnAmount;
        if (mp->xzSpeedFactor < lbl_805b279c)
            mp->xzSpeedFactor = lbl_805b279c;
        else
            mp->unknown_0x3c4 = mp->unknown_0x3c8;
    }

    if (mp->hitObjs1[1] != NULL && mp->airTimer != lbl_805b279c && accelerating &&
        fabsf(compAngle(mp->unknown_0x218, mp->directionView)) >= lbl_805b2844)
        mp->xzSpeedFactor = lbl_805b279c;

    if (restoreDirection) {
        f32 difference;
        f32 previous;
        f32 target;

        previous = mp->unknown_0x3c8;
        if (previous < lbl_805b2864 &&
            mp->unknown_0x16c > lbl_805b2868) {
            target = mp->unknown_0x16c - lbl_805b286c;
        } else {
            previous = mp->unknown_0x3c8;
            if (previous > lbl_805b2868 &&
                mp->unknown_0x16c < lbl_805b2864) {
                target = lbl_805b286c + mp->unknown_0x16c;
            } else {
                previous = mp->unknown_0x3c8;
                target = mp->unknown_0x16c;
            }
        }

        difference = target - previous;
        mp->unknown_0x3c4 =
            lbl_805b2870 * difference + mp->unknown_0x3c4;
        if (mp->unknown_0x3c4 < lbl_805b279c)
            mp->unknown_0x3c4 += lbl_805b286c;
        if (mp->unknown_0x3c4 >= lbl_805b286c)
            mp->unknown_0x3c4 -= lbl_805b286c;
    }

    if (mp->xzSpeedFactor > lbl_805b279c)
        mp->directionView = mp->unknown_0x3c4;

    MarioWork * checkWork = marioGetPtr();
    bool specialSpeed;

    if (checkWork->motionId == MOT_SWIM)
        specialSpeed = true;
    else if ((checkWork->miscFlags & 0x100) && (checkWork->miscFlags & 0x20))
        specialSpeed = true;
    else
        specialSpeed = false;

    if (specialSpeed) {
        mp->xzSpeed = mp->xzSpeedFactor * func_80113cfc() / lbl_805b27b4;
        return;
    }

    if ((mp->motionId == MOT_BOUNCE ||
         (mp->prevMotionId == MOT_BOUNCE && mp->motionId == MOT_FALL)) &&
        mp->unknown_0x15c > func_80119000()) {
        mp->xzSpeed = mp->unknown_0x15c * mp->xzSpeedFactor / lbl_805b27b4;
        if (mp->xzSpeed < func_80119000()) {
            mp->unknown_0x15c = func_80119000();
            if (mp->xzSpeed > func_80118fc0()) {
                f32 range = func_80119000() - func_80118fc0();
                mp->xzSpeedFactor = lbl_805b27b0 +
                    lbl_805b27b4 * ((mp->xzSpeed - func_80118fc0()) / range);
                if (mp->xzSpeedFactor > lbl_805b27b4)
                    mp->xzSpeedFactor = lbl_805b27b4;
            } else {
                mp->xzSpeedFactor = lbl_805b27b0 *
                    (mp->xzSpeed / func_80118fc0());
                if (mp->xzSpeedFactor > lbl_805b27b0)
                    mp->xzSpeedFactor = lbl_805b27b0;
            }
        }
        return;
    }

    if ((mp->flags & 0x00030000) && mp->lastGroundSpeed > func_80119000()) {
        mp->xzSpeed = mp->lastGroundSpeed * mp->xzSpeedFactor / lbl_805b27b4;
        if (mp->xzSpeed < func_80119000()) {
            mp->lastGroundSpeed = func_80119000();
            mp->xzSpeedFactor = lbl_805b27b4;
        }
        return;
    }

    if (mp->xzSpeedFactor <= lbl_805b27b0) {
        mp->xzSpeed = func_80118f40() * mp->xzSpeedFactor / lbl_805b27b0;
    } else {
        f32 walkSpeed = func_80118f40();
        f32 range = marioGetDashSpd() - walkSpeed;
        f32 scaledFactor = mp->xzSpeedFactor - lbl_805b27b0;
        scaledFactor *= range;
        f32 interpolation = scaledFactor / lbl_805b280c;
        f32 finalWalkSpeed = func_80118f40();
        mp->xzSpeed = finalWalkSpeed + interpolation;
    }
}

#pragma dont_inline off

}
