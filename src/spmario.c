#include <common.h>
#include <evtmgr.h>
#include <spmario.h>

SpmarioGlobals spmario;
SpmarioGlobals * gp = &spmario;
// static dvd thread stack 80527370
// static OSThread DVDCheckThread 80527058

s8 unknown_805ae9c0;
s8 unknown_805ae9c1;
s8 unknown_805ae9c2;
s8 unknown_805ae9c3;
// 805ae9c4
s32 _spmarioSystemLevel;
// 805ae9cc

// spmarioInit
// spmarioMain
// spmarioDisp

void spmarioSystemLevel(s32 level) {
    switch (level) {
        case 0:
            _spmarioSystemLevel = level;
            gp->systemLevelFlags &= ~3;
            evtStartAll(0xff);
            break;
        case 1:
            _spmarioSystemLevel = level;
            gp->systemLevelFlags |= 1;
            evtStopAll(7);
            break;
        case 2:
            _spmarioSystemLevel = level;
            gp->systemLevelFlags |= 3;
            evtStopAll(0xff);
            break;
    }
}

s32 spmarioGetSystemLevel() {
    return _spmarioSystemLevel;
}

// viPostCallback
// wiiDvdCheckThread
// wiiRumbleCheck

void unknown_801a9068() {
    unknown_805ae9c0 = 1;
}

void unknown_801a9074() {
    unknown_805ae9c1 = 1;
}

void unknown_801a9080() {
    unknown_805ae9c2 = 1;
}

void unknown_801a908c() {
    unknown_805ae9c3 = 1;
}
