#pragma once

#include "DisplayTFT.h"
#include "fonts/DSEG7_Classic_Regular_32.h"
#include "fonts/DSEG7_Classic_Regular_50.h"
#include "fonts/Roboto_Thin_14.h"
#include "fonts/Free_Fonts.h"

#define ROB14  &Roboto_Thin_14
#define DSEG32 &DSEG7_Classic_Regular_32
#define DSEG50 &DSEG7_Classic_Regular_50

typedef enum
{
    ICON_WIFI1,
    ICON_WIFI2,
    ICON_WIFI3,
    ICON_WIFI4,
    ICON_CLOCK,
    ICON_WEB,
    ICON_PC,
} STATUS_ICONS_ID;

void DisplayScreenSystemInfo();
void UpdateStatusBar(uint32_t status);
