/**
 * Proyecto PIG GUARD
 * FASE 1 - Deteccion de valores con los sensores y muestra en display TFT.
 * JJTeam
 * Junio 2021, 12/oct/2021
 *
 * 13/10/2021:
 * - Esta version es la definitiva que se estaria probando en campo solo para FASE 1.
 * - Se quita todo lo que es FASE 2.
 */

#include <Arduino.h>
#include "Leds/Leds.h"
#include "config/ConfigInstance.h"
#include "Reloj/Reloj.h"
#include "utiles/debug.h"
#include "Leds/Leds.h"
#include "display/DisplayTFT.h"

void MostrarInfoInicial()
{
    display.clearScreen(TFT_BLACK);
    display.clearText(TFT_YELLOW, TFT_BLACK);
    display.print("** TERMINAL SERIE ** Build date: " + String(__DATE__));
    // display.tft.setTextColor(TFT_WHITE);
}

void UpdateStatusBar(uint32_t status)
{
    if (!display.isOk) return;

    if (status % 4 == 0) display.showGIMPImage(100, 0, &wifi1);
    if (status % 4 == 1) display.showGIMPImage(100, 0, &wifi2);
    if (status % 4 == 2) display.showGIMPImage(100, 0, &wifi3);
    if (status % 4 == 3) display.showGIMPImage(100, 0, &wifi4);

    display.showGIMPImage(180, 0, &celu);
    display.showGIMPImage(200, 0, &ddbb2);
    display.showGIMPImage(220, 0, &nowifi);
}

void setup()
{
    Serial.begin(115200);
    delay(100);
    LedsInit();
    display.begin();
    if (!display.isOk)
    {
        LogE("El display parece que no funciona!");
    }
    else
    {
        MostrarInfoInicial();  // muestra el logo & info, ID, etc...
    }
    LogI("Init ok. FreeHeap=%u. MaxAllocHeap=%u", ESP.getFreeHeap(), ESP.getMaxAllocHeap());
}

void SendToDatabase(const char* buf) {}

#define BUF_LEN 1024
int idx = 0;
char buf[BUF_LEN];

void loop()
{
    if (Serial.available())
    {
        int c = Serial.read();
        if (c != -1)
        {
            if (c == '\n' || c == '\r')
            {
                buf[idx] = 0;
                if (idx > 3)
                {
                    display.print(buf);
                    SendToDatabase(buf);
                }
                idx = 0;
            }
            else
            {
                buf[idx++] = c;
                if (idx >= (BUF_LEN - 1))
                {
                    buf[idx] = 0;
                    idx      = 0;
                    display.print(buf);
                    SendToDatabase(buf);
                }
            }
        }
    }
    EVERY_N_MILLIS(1000)
    {
        // NextLed(CRGB::Green, false);
        static int s = 0;
        UpdateStatusBar(s++);
        display.print("linea: g | <>$#....-_____ " + String(s) + " -- " + String(display.cantidadLineasDeLog));
        if (s >= display.cantidadLineasDeLog) display.clearScreen(TFT_BLACK);
    }
}
