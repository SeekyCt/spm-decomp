#include <common.h>
#include <spm/animdrv.h>
#include <spm/mario.h>
#include <spm/mario_hit.h>
#include <spm/mario_motion.h>
#include <spm/system.h>

extern "C" {

extern const char lbl_803345e0[] =
    "MOBJ_ta3_kaze_aka\0"
    "mario_motion.c\0"
    "paramType >= 0 && paramType < JUMPPARA_MAX\0"
    "frm < 5000\0"
    "ジャンプのパラメータおかしくない？\0"
    "落下のパラメータおかしくない？\n(%f %f %f %f)\0"
    "charMode >= MARIO_MODE && charMode < MARIO_MODE_END\0"
    "おかしなキャラになろうとしています\ncharMode = %d\0"
    "p_jyabara\0"
    "S_1";

void marioChangeCharacter(s32 charMode)
{
    MarioWork * mw = marioGetPtr();
    bool valid = charMode >= MARIO_MODE && charMode < MARIO_MODE_END;

    if (valid == false) {
        __assert2(&lbl_803345e0[0x12], 3391, &lbl_803345e0[0xa7],
                  &lbl_803345e0[0xdb], charMode);
    }

    if (mw->character != charMode)
        _marioChangeCharacter(charMode);
}

void _marioChangeCharacter(s32 character)
{
    MarioWork * mw = marioGetPtr();
    u32 poseFlag = 0;
    u32 dispFlag;

    if (mw->character == PLAYER_LUIGI && mw->animPoseIds[6] != -1) {
        animPoseRelease(mw->animPoseIds[6]);
        mw->animPoseIds[6] = -1;
    }

    mw->character = (s8) character;

    if (mw->animPoseIds[func_80126618()] != -1)
        poseFlag = animPoseGetFlagF0(mw->animPoseIds[func_80126618()]);

    marioSetAnimGroup(0);

    for (s32 i = 0; i < 5; i++) {
        if (mw->animPoseIds[i] != -1)
            animPoseSetFlagF0On(mw->animPoseIds[i], poseFlag);
    }

    if (mw->character == PLAYER_LUIGI && mw->animPoseIds[6] <= -1)
        mw->animPoseIds[6] = animPoseEntry(&lbl_803345e0[0x10c], 2);

    dispFlag = mw->dispFlags & 2;
    mw->dispFlags &= ~2;
    mw->curPoseName = NULL;
    marioChgPose(&lbl_803345e0[0x116]);
    mw->dispFlags |= dispFlag;
    marioUpdateCharProperties();
    func_80131ee8();
}

}
