#include <common.h>
#include <spm/memory.h>
#include <spm/spmario.h>
#include <spm/wpadmgr.h>
#include <wii/kpad.h>
#include <wii/wpad.h>
#include <msl/math.h>
#include <msl/string.h>

extern "C" {

// .bss
static WpadWork work;

// .sdata
static WpadWork * wp = &work;

extern const volatile f32 wpadScale[2];

WpadWork * wpadGetWork()
{
    return wp;
}

static void * wpad_alloc(u32 size)
{
    return __memAlloc(HEAP_WPAD, size);
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

void wpadAllRumbleOff(s32 mode)
{
    (void) mode;

    for (int i = 0; i < 4; i++)
        WPADControlMotor(i, 0);
}

static inline void wpadFilter(f32 * differences, f32 * averages, f32 * samples, s32 count)
{
    for (s32 i = 0; i < count; i++)
    {
        f32 a = samples[i * 2];
        f32 b = samples[i * 2 + 1];
        averages[i] = 1.4142135f * (a + b) / 2.0f;
        differences[i] = 1.4142135f * a - averages[i];
    }
}

void wpadMain()
{
    s32 controller;

    for (controller = 0; controller < 4; controller++)
    {
        wp->kpadReadRet[controller] = KPADRead(
            controller, wp->statuses[controller], WPAD_STATUS_COUNT);
    }

    for (controller = 0; controller < 4; controller++)
    {
        if ((wp->statuses[controller][0].error == 0)
            && (wp->statuses[controller][0].buttonsHeld != 0))
            break;
    }
    if (controller < 4)
        gp->lastButtonPressTime = gp->time;

    for (controller = 0; controller < 4; controller++)
    {
        KPADStatus * status = &wp->statuses[controller][0];

        if (status->error != 0)
        {
            Vec2 zero = {0.0f, 0.0f};

            status->dpdStatus = 0;
            status->pointingPos = zero;
            wp->pointingPos[controller] = status->pointingPos;
        }
        else
        {
            if (status->dpdStatus == 0)
                status->pointingPos = wp->pointingPos[controller];
            else
                wp->pointingPos[controller] = status->pointingPos;

            if (wp->flags & WPAD_FLAG_ENABLE_CURSOR)
            {
                if (!WPADIsDpdEnabled(controller))
                    KPADEnableDPD(controller);
            }
            else if (WPADIsDpdEnabled(controller))
            {
                KPADDisableDPD(controller);
            }
        }
    }

    for (s32 age = 59; age > 0; age--)
    {
        s32 previous = age - 1;
        for (controller = 0; controller < 4; controller++)
            wp->pastStatuses[controller][age] = wp->pastStatuses[controller][previous];
    }

    for (controller = 0; controller < 4; controller++)
        wp->pastStatuses[controller][0] = wp->statuses[controller][0];

    for (controller = 0; controller < 4; controller++)
    {
        KPADStatus * status = &wp->statuses[controller][0];
        if (status->error == 0)
        {
            if ((status->buttonsHeld & ~WPAD_BTN_REPEAT) != wp->unknown_0x9d3c[controller])
            {
                wp->unknown_0x9d4c[controller] = 0;
                wp->unknown_0x9d60[controller] = gp->time;
            }

            wp->unknown_0x9d4c[controller] =
                (s32) OSTicksToMilliseconds(gp->time - wp->unknown_0x9d60[controller]);
            wp->unknown_0x9d3c[controller] = status->buttonsHeld & ~WPAD_BTN_REPEAT;
        }
    }

    f32 samples[60];
    f32 averages[30];
    f32 differences[30];
    for (s32 i = 0; i < 60; i++)
        samples[i] = wp->pastStatuses[0][i].acceleration.y;

    wpadFilter(differences, averages, samples, 30);

    for (s32 i = 0; i < 30; i++)
        samples[i] = averages[i];

    f32 * filtered = wp->unknown_0x9d80;
    wpadFilter(differences, filtered, samples, 15);

    wp->unknown_0x9dc0 = func_80237750();

    for (controller = 0; controller < 4; controller++)
    {
        u32 buttons = 0;
        KPADStatus * status = &wp->statuses[controller][0];
        if (status->error == 0)
        {
            u8 extensionType = status->extensionType;

            if ((s8) (extensionType == KPAD_EXTENSION_FS
                ? (s32) (status->extension.fs.stickPos.x * 127.0f) : 0) < -48)
                buttons |= WPAD_BTN_DOWN;
            if ((s8) (extensionType == KPAD_EXTENSION_FS
                ? (s32) (status->extension.fs.stickPos.x * 127.0f) : 0) > 48)
                buttons |= WPAD_BTN_UP;
            if ((s8) (extensionType == KPAD_EXTENSION_FS
                ? (s32) (status->extension.fs.stickPos.y * 127.0f) : 0) < -48)
                buttons |= WPAD_BTN_RIGHT;
            if ((s8) (extensionType == KPAD_EXTENSION_FS
                ? (s32) (status->extension.fs.stickPos.y * 127.0f) : 0) > 48)
                buttons |= WPAD_BTN_LEFT;

            wp->unknown_0x14[controller] = buttons & (buttons ^ wp->unknown_0x4[controller]);
            wp->unknown_0x24[controller] = wp->unknown_0x14[controller];

            if (buttons != 0 && buttons == wp->unknown_0x4[controller])
            {
                if (--wp->unknown_0x34[controller] == 0)
                {
                    wp->unknown_0x24[controller] = buttons;
                    wp->unknown_0x34[controller] = (u32) (gp->fps * 6 / 60);
                }
            }
            else
            {
                wp->unknown_0x34[controller] = (u32) (gp->fps * 24 / 60);
            }

            wp->unknown_0x4[controller] = buttons;
        }
    }

    if (WPADIsMotorEnabled())
    {
        for (controller = 0; controller < 4; controller++)
        {
            KPADStatus * status = &wp->statuses[controller][0];
            if (status->error != 0)
                continue;

            if (gp->disableRumble[controller])
            {
                if (wp->rumblePrev[controller] != wp->enableRumble[controller])
                {
                    WPADControlMotor(controller, 0);
                    wp->enableRumble[controller] = false;
                    wp->rumblePrev[controller] = false;
                }
            }
            else if (wp->rumblePrev[controller] != wp->enableRumble[controller])
            {
                switch (wp->enableRumble[controller])
                {
                    case false:
                        WPADControlMotor(controller, 0);
                        break;
                    case true:
                        WPADControlMotor(controller, 1);
                        break;
                }
                wp->rumblePrev[controller] = wp->enableRumble[controller];
            }
        }
    }
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

bool func_80237750()
{
    s32 q;
    s32 i;
    s32 count;
    f32 x;

    if (wp->unknown_0x9dc1 != 0 && wp->unknown_0x9d80[0] <= wp->unknown_0x9d80[1])
        wp->unknown_0x9dc1 = 0;

    if (wp->unkknown_0x9dbc > 0.0f)
    {
        wp->unkknown_0x9dbc -= 0.2f;
        return 1;
    }

    if (wp->unknown_0x9dc1 != 0)
        return 0;

    for (i = 0; i < 14; i++)
    {
        if (wp->unknown_0x9d80[i] < wp->unknown_0x9d80[i+1])
            break;   
    }
    if (i == 0 || i >= 14)
        return 0;
    
    q = i;

    count = 0;
    for (;i < 14; i++)
    {
        if (wp->unknown_0x9d80[i] >= wp->unknown_0x9d80[i+1])
            break;
        
        if (fabsf(wp->unknown_0x9d80[i] - wp->unknown_0x9d80[i+1]) <= 0.01f)
        {
            if (++count > 1)
                break;
        }
        else
        {
            count = 0;
        }
    }
    if (i >= 14)
        return 0;
    
    if (wp->unknown_0x9d80[i] > wp->unknown_0x9d80[0])
        return 0;
    
    if (wp->unknown_0x9d80[i] - wp->unknown_0x9d80[q] < 0.1f)
        return 0;
    
    x = wp->unknown_0x9d80[0] - wp->unknown_0x9d80[q];
    if (x < 1.0f)
        return 0;
    
    wp->unkknown_0x9dbc = x / q;

    if (wp->unkknown_0x9dbc < 0.75f)
    {
        wp->unkknown_0x9dbc = 0.0f;
        return 0;
    }

    if (wp->unkknown_0x9dbc > 3.0f)
        wp->unkknown_0x9dbc *= wpadScale[0];
    
    wp->unknown_0x9dc1 = 1;

    return 1;
}

__declspec(section ".sdata2") const volatile f32 wpadScale[2] = {1.25f, 0.0f};

}
