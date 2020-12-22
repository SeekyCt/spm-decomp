#include <common.h>
#include <evtmgr.h>
#include <spmario.h>

SpmarioGlobals spmario;
SpmarioGlobals * gp = &spmario;
s32 _spmarioSystemLevel;

// static dvd thread stack 80527370
// static OSThread DVDCheckThread 80527058

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
// 801a9068
// 801a9074
// 801a9080
// 801a908c
