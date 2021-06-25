#include <common.h>
#include <evtmgr.h>
#include <os.h>
#include <spmario.h>

SpmarioGlobals spmario; // 80525550
SpmarioGlobals * gp = &spmario; // 805ae178
// static dvd thread stack 80527370
static OSThread DVDCheckThread; // 80527058

s8 doResetButtonRestart; // 805ae9c0
s8 doShutdown; // 805ae9c1
s8 doHomeButtonRestart; // 805ae9c2
s8 doReturnToWiiMenu; // 805ae9c3
// 805ae9c4
s32 _spmarioSystemLevel; // 805ae9c8
OSThread * pMainThread; // 805ae9cc

// spmarioInit
// spmarioMain
// spmarioDisp

void spmarioSystemLevel(s32 level)
{
    switch (level)
    {
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

s32 spmarioGetSystemLevel()
{
    return _spmarioSystemLevel;
}

// viPostCallback
// wiiDvdCheckThread
// wiiRumbleCheck

void resetButtonRestart()
{
    doResetButtonRestart = 1;
}

void shutdownConsole()
{
    doShutdown = 1;
}

void homeButtonRestart()
{
    doHomeButtonRestart = 1;
}

void returnToWiiMenu()
{
    doReturnToWiiMenu = 1;
}
