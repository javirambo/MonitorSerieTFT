#include "DisplayBuilder.h"
#include "fonts/Free_Fonts.h"

/**
 * Muestra informacion inicial.
 */
void DisplayScreenSystemInfo()
{
    if (!display.isOk) return;
    display.clearLog(ROB14, TFT_YELLOW, TFT_BLACK);
    display.printLog("** TERMINAL SERIE ** Build date: " + String(__DATE__), ROB14);
    display.tft.setTextColor(TFT_WHITE);
}

/**
 * Actualiza los iconos de la barra de estado.
 */
void UpdateStatusBar(uint32_t status)
{
    // static uint32_t last_status = 0;
    // if (!display.isOk) return;
    // if (status == last_status) return;

    // last_status = status;

    // display.showImage(ICO_PC, status & LOGS_PAGE_CONN);
    // display.showImage(ICO_WEB, status & WEB_SERVER_CONN);
    // display.showImage(ICO_CLOCK, status & RTC_OK);

    // if (status & WIFI_POT_SET)
    //     display.showImage(ICO_WIFI00 + (status & 3), (status & WIFI_OK));
    // else if (!(status & WIFI_OK))
    //     display.showImage(ICO_WIFI00, false);
}
