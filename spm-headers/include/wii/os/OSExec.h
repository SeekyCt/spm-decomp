#pragma once

#include <common.h>

CPP_WRAPPER(wii::os)

UNKNOWN_FUNCTION(PackArgs);
void Run(void * code);
UNKNOWN_FUNCTION(__OSGetExecParams);
UNKNOWN_FUNCTION(__OSLaunchNextFirmware);
UNKNOWN_FUNCTION(__OSLaunchMenu);
UNKNOWN_FUNCTION(__OSRelaunchTitle);
UNKNOWN_FUNCTION(__OSBootDolSimple);
UNKNOWN_FUNCTION(__OSBootDol);
UNKNOWN_FUNCTION(_ES_GetTicketViews);
UNKNOWN_FUNCTION(_ES_LaunchTitle);

CPP_WRAPPER_END()
