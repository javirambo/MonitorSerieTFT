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
#include "display/DisplayBuilder.h"
#include "Reloj/Reloj.h"
#include "utiles/debug.h"
#include "Leds/Leds.h"
#include "display/DisplayTFT.h"
#include "display/GimpImage.h"

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
        DisplayScreenSystemInfo();  // muestra el logo & info, ID, etc...
    }
    LogI("Init ok. FreeHeap=%u. MaxAllocHeap=%u", ESP.getFreeHeap(), ESP.getMaxAllocHeap());
}

extern const GIMP_image_t *wifi1_image;

void loop()
{
    EVERY_N_MILLIS(1000)
    {
        // NextLed(CRGB::Green, false);
        // GIMPImage g{
        //     .width  = wifi1_gimp_width,
        //     .height = wifi1_gimp_width,
        //     .data   = wifi1_gimp_data,
        // };

        display.showGIMPImage(1, 1, wifi1_image);
        
    }
}
