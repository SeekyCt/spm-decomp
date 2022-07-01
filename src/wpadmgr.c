#include <common.h>
#include <spm/memory.h>
#include <spm/spmario.h>
#include <spm/wpadmgr.h>
#include <wii/kpad.h>
#include <wii/string.h>
#include <wii/wpad.h>

// .rodata
#include "orderdoubles/80343e98_80343ea0.inc"

// .bss
static WpadWork wpadWork;

// .sdata
static WpadWork * wp = &wpadWork;

// .sdata2
#include "orderfloats/805b61f8_805b622c.inc"

WpadWork * wpadGetWork()
{
    return wp;
}

static void * wpad_alloc(u32 size)
{
    return __memAlloc(5, size);
}

static bool wpad_free(void * ptr)
{
    // Doesn't even try to free and just tells the library it failed (seems the library doesn't use it anyway?)
    (void) ptr;

    return false;
}

void wpadInit()
{
    memset(wp, 0, sizeof(*wp));

    WPADRegisterAllocator(&wpad_alloc, &wpad_free);

    KPADInit();

    for (int i = 0; i < 4; i++)
    {
        KPADCalibrateDPD(i);
        KPADSetPosParam(i, 0.1f, 1.0f);
        KPADSetHoriParam(i, 0.1f, 1.0f);
        KPADSetDistParam(i, 0.1f, 1.0f);
        KPADSetAccParam(i, 0.1f, 1.0f);
        KPADSetBtnRepeat(i, 0.375f, 0.125f);
    }

    KPADSetFSStickClamp(0xf, 0x57); // sets nunchuck clamping despite the game never using it..?

    for (int i = 0; i < 4; i++)
    {
        wp->rumblePrev[i] = 0;
        wp->enableRumble[i] = 0;
    }

    wp->flags &= ~WPAD_FLAG_ENABLE_CURSOR;

    WPADSetAutoSleepTime(4);
}

void wpadAllRumbleOff()
{
    for (int i = 0; i < 4; i++)
        WPADControlMotor(i, 0);
}

asm void wpadMain() 
{
    #include "asm/80236af8.s"
}

void wpadCursorOn()
{
    wp->flags |= WPAD_FLAG_ENABLE_CURSOR;
}

void wpadCursorOff()
{
    wp->flags &= ~WPAD_FLAG_ENABLE_CURSOR;
}

bool wpadGetCursorStatus()
{
    return (wp->flags & WPAD_FLAG_ENABLE_CURSOR) != 0;
}

void wpadRumbleOn(s32 controller)
{
    wp->enableRumble[controller] = true;
}

void wpadRumbleOff(s32 controller)
{
    wp->enableRumble[controller] = false;
}

bool wpadGetRumbleStatus(s32 controller)
{
    return wp->enableRumble[controller];
}

u32 wpadGetButtonsHeld(s32 controller)
{
    return wp->statuses[controller]->buttonsHeld;
}

u32 wpadGetButtonsReleased(s32 controller)
{
    return wp->statuses[controller]->buttonsReleased;
}

u32 wpadGetButtonsPressed(s32 controller)
{
    return wp->statuses[controller]->buttonsPressed;
}

u32 wpadGetButtonsHeldRepeat(s32 controller)
{
    u32 held = wp->statuses[controller][0].buttonsHeld;
    return held & WPAD_BTN_REPEAT ? held : 0;
}

u32 func_802376e4(s32 controller)
{
    if (wp->unknown_0x9d4c[controller] > 50)
        return wp->unknown_0x9d3c[controller];
    else
        return 0;
}

void func_80237710(s32 controller)
{
    wp->unknown_0x9d4c[controller] = 0;
    wp->unknown_0x9d60[controller] = gp->time;
}

asm bool func_80237750()
{
    #include "asm/80237750.s"
}
