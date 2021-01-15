#include <common.h>
#include <kpad.h>
#include <memory.h>
#include <string.h>
#include <wpad.h>
#include <wpadmgr.h>

static WpadWork wpadWork; // 80528f48
static WpadWork * wp = &wpadWork; // 805ae198

WpadWork * wpadGetWork() {
    return wp;
}

static void * wpad_alloc(u32 size) { // 80236984
    return __memAlloc(5, size);
}

static bool wpad_free(void * ptr) { // 80236990
    // Doesn't even try to free and just tells the library it failed (seems the library doesn't use it anyway?)
    (void) ptr;
    return false;
}

void wpadInit() {
    memset(wp, 0, sizeof(*wp));
    WPADRegisterAllocator(&wpad_alloc, &wpad_free);
    KPADInit();
    for (int i = 0; i < 4; i++) {
        KPADCalibrateDPD(i);
        KPADSetPosParam(i, 0.1f, 1.0f);
        KPADSetHoriParam(i, 0.1f, 1.0f);
        KPADSetDistParam(i, 0.1f, 1.0f);
        KPADSetAccParam(i, 0.1f, 1.0f);
        KPADSetBtnRepeat(i, 0.375f, 0.125f);
    }
    KPADSetFSStickClamp(0xf, 0x57); // sets nunchuck clamping despite the game never using it..?
    for (int i = 0; i < 4; i++) {
        wp->rumblePrev[i] = 0;
        wp->enableRumble[i] = 0;
    }
    wp->flags &= ~WPAD_FLAG_ENABLE_CURSOR;
    WPADSetAutoSleepTime(4);
}

void wpadAllRumbleOff() {
    for (int i = 0; i < 4; i++) {
        WPADControlMotor(i, 0);
    }
}

// wpadMain

void wpadCursorOn() {
    wp->flags |= WPAD_FLAG_ENABLE_CURSOR;
}

void wpadCursorOff() {
    wp->flags &= ~WPAD_FLAG_ENABLE_CURSOR;
}

bool wpadGetCursorStatus() {
    return (bool) (wp->flags & WPAD_FLAG_ENABLE_CURSOR);
}

void wpadRumbleOn(s32 controller) {
    wp->enableRumble[controller] = true;
}

void wpadRumbleOff(s32 controller) {
    wp->enableRumble[controller] = false;
}

bool wpadGetRumbleStatus(s32 controller) {
    return wp->enableRumble[controller];
}

u32 wpadGetButtonsHeld(s32 controller) { // 80237688
    return wp->statuses[controller]->buttonsHeld;
}

u32 wpadGetButtonsReleased(s32 controller) { // 8023769c
    return wp->statuses[controller]->buttonsReleased;
}

u32 wpadGetButtonsPressed(s32 controller) { // 802376b0
    return wp->statuses[controller]->buttonsPressed;
}

u32 wpadGetButtonsHeldConditional(s32 controller) { // 802376c4
    u32 held = wp->statuses[controller][0].buttonsHeld;
    return held & 0x80000000 ? held : 0;
}

// 802376e4
// 80237710
// 80237750
