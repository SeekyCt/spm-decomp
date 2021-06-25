#include <common.h>
#include <kpad.h>
#include <memory.h>
#include <string.h>
#include <wpad.h>
#include <wpadmgr.h>
#include <spmario.h>

static WpadWork wpadWork; // 80528f48
static WpadWork * wp = &wpadWork; // 805ae198

WpadWork * wpadGetWork()
{
    return wp;
}

static void * wpad_alloc(u32 size) // 80236984
{
    return __memAlloc(5, size);
}

static bool wpad_free(void * ptr) // 80236990
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

/* Unfinished
void wpadMain()
{
    // Try read all controllers
    for (s32 i = 0; i < 4; i++)
        wp->kpadReadRet[i] = KPADRead(i, wp->statuses[i], 0x10);

    // Check if any controllers were valid and had buttons held, log time if so
    s32 unpressedNum = 0;
    if ((wp->statuses[0][0].error != 0) || (wp->statuses[0][0].buttonsHeld == 0))
    {
        unpressedNum = 1;
        if ((wp->statuses[1][0].error != 0) || (wp->statuses[1][0].buttonsHeld == 0))
        {
            unpressedNum = 2;
            if ((wp->statuses[2][0].error != 0) || (wp->statuses[2][0].buttonsHeld == 0))
            {
                unpressedNum = 3;
                if ((wp->statuses[3][0].error != 0) || (wp->statuses[3][0].buttonsHeld == 0))
                    unpressedNum = 4;
            }
        }
    }
    if (unpressedNum < 4)
        gp->lastButtonPressTime = gp->time;

    // Handle cursor positions
    for (s32 i = 0; i < 4; i++)
    {
        if (wp->statuses[i][0].error != 0)
        {
            wp->statuses[i][0].dpdStatus = 0;
            wp->statuses[i][0].pointingPos.x = 0.0f;
            wp->statuses[i][0].pointingPos.y = 0.0f;
            wp->pointingPos[i].x = 0.0f;
            wp->pointingPos[i].y = 0.0f;
        }
        else
        {
            // If errored, use last valid values, otherwise read values
            if (wp->statuses[i][0].dpdStatus == 0)
            {
                wp->statuses[i][0].pointingPos.x = wp->pointingPos[i].x;
                wp->statuses[i][0].pointingPos.y = wp->pointingPos[i].y;
            }
            else
            {
                wp->pointingPos[i].x = wp->statuses[i][0].pointingPos.x;
                wp->pointingPos[i].y = wp->statuses[i][0].pointingPos.y;
            }

            // Handle enabling / disabling cursor
            if (wp->flags & WPAD_FLAG_ENABLE_CURSOR)
            {
                if (!WPADIsDpdEnabled(i))
                    KPADEnableDpd(i);
            }
            else {
                if (WPADIsDpdEnabled(i))
                    KPADDisableDpd(i);
            }
        }

        // Backup statuses
        for (int i = 59; i > 0; i--)
        {
            for (int j = 0; j < 4; j++)
                wp->pastStatuses[j][i] = wp->pastStatuses[j][i - 1];
        }

        //
    }
}
*/

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
    return (bool) (wp->flags & WPAD_FLAG_ENABLE_CURSOR);
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

// func_80237750
