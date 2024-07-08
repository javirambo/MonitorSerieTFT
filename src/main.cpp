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
#include <WiFi.h>
#include <SPIFFS.h>

extern void CaptivePortalStart();

#define BUF_LEN 1024
int idx = 0;
char buf[BUF_LEN];

void ShowWifiIcon()
{
    int pot = WiFi.RSSI();
    // LogW("pot wifi: %d", pot);
    if (WiFiClass::status() != WL_CONNECTED)
        display.showGIMPImage(300, 0, &nowifi_img);
    else if (pot > -50)
        display.showGIMPImage(300, 0, &wifi4_img);
    else if (pot > -65)
        display.showGIMPImage(300, 0, &wifi3_img);
    else if (pot > -75)
        display.showGIMPImage(300, 0, &wifi2_img);
    else
        display.showGIMPImage(300, 0, &wifi1_img);
}

void setup()
{
    Serial.begin(115200);
    SPIFFS.begin(true);
    pinMode(PIN_LED_ALIVE, OUTPUT);
    pinMode(PIN_DIAG_MODE_JUMPER, INPUT_PULLUP);
    ConfigInst.Load();
    LedsInit();
    display.init();
    if (!display.isOk)
    {
        LogE("El display parece que no funciona!");
    }

    LogI("Init ok. FreeHeap=%u. MaxAllocHeap=%u", ESP.getFreeHeap(), ESP.getMaxAllocHeap());

    // muestro el logo inicial:
    display.showGIMPImage(0, 0, &logo_img);

    // el boton de DIAG permite setear el WIFI:
    if (digitalRead(PIN_DIAG_MODE_JUMPER) == 0)
    {
        CaptivePortalStart();
        esp_restart();
    }

    delay(4000);
    // muestra info, ID, etc...
    display.clearScreen(TFT_BLACK);
    display.tft.setTextColor(TFT_BLUE);
    display.tft.drawString("* TERMINAL SERIE * Build date: " + String(__DATE__), 8, 3, 1);
    display.print(TFT_SKYBLUE, "==> Conectando al wifi " + ConfigInst.WifiSsid + "...");

    // inicio wifi:
    WiFi.begin(ConfigInst.WifiSsid, ConfigInst.WifiPass);
    WiFi.onEvent([](WiFiEvent_t event) { display.print(TFT_RED, "==> Wifi desconectado."); }, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent([](WiFiEvent_t event) { display.print(TFT_SKYBLUE, "==> Wifi conectado."); }, ARDUINO_EVENT_WIFI_STA_CONNECTED);
}

void SendToDatabase(const char* buf)
{
    // display.showGIMPImage(260, 0, &base_img);
    FastLED.setBrightness(50);
    NextLed(TFT_YELLOW, false);
}

uint16_t AnsiToTftColor(int colorAnsi)
{
    if (colorAnsi == -1) return TFT_WHITE;
    colorAnsi = colorAnsi % 10;
    if (colorAnsi == TERM_AZUL) return TFT_BLUE;
    if (colorAnsi == TERM_ROJO) return TFT_RED;
    if (colorAnsi == TERM_VERDE) return TFT_GREEN;
    if (colorAnsi == TERM_AMARILLO) return TFT_YELLOW;
    if (colorAnsi == TERM_CIAN) return TFT_CYAN;
    if (colorAnsi == TERM_MAGENTA) return TFT_MAGENTA;
    if (colorAnsi == TERM_BLANCO) return TFT_WHITE;
    return TFT_WHITE;
}

void loop()
{
    // leo el puerto serie, en cada linea muestro y grabo en la base.
    static int estado = 0;
    static String x{};
    static int colorAnsi = -1;
    int c                = Serial.read();
    if (c != -1)
    {
        // \033[0m blanco
        // \033[0;30m rojo
        // \033[1;30m rojo brillante
        if (estado == 1 && c == '[')
        {
            estado = 2;  // esperar la 'm'
            x      = "";
        }
        else if (estado == 2)
        {
            if (c == 'm')
            {
                estado    = 0;
                colorAnsi = x.toInt();
                LogI("llego color %u", colorAnsi);
            }
            else if (c == ';')
            {
                x = "";  // descarto el background color
            }
            else
            {
                x += String(c);
            }
        }
        else
        {
            if (c == '\033')
            {
                estado = 1;
            }
            else if (c == '\n' || c == '\r')
            {
                buf[idx] = 0;
                if (idx > 1)
                {
                    display.print(AnsiToTftColor(colorAnsi), buf);
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
                    display.print(AnsiToTftColor(colorAnsi), buf);
                    SendToDatabase(buf);
                }
            }
        }
    }

    // estoy vivo:
    EVERY_N_MILLIS(1950)
    {
        digitalWrite(PIN_LED_ALIVE, 1);
        delay(50);
        digitalWrite(PIN_LED_ALIVE, 0);
        ShowWifiIcon();  //(muestro la potencia del wifi)
    }

    // AVISO PARA QUE SEPAN COMO CAMBIAR LA PASSWORD DEL WIFI:
    EVERY_N_MINUTES(5)
    {
        if (WiFiClass::status() != WL_CONNECTED)
        {
            LedsKitt();
            display.print(TFT_RED, "==> No se pudo conectar al wifi " + ConfigInst.WifiSsid);
            static bool once = false;
            if (!once)
            {
                once = true;
                display.print(TFT_GREENYELLOW, "============================");
                display.print(TFT_GREENYELLOW, "Solo veras logs en pantalla,");
                display.print(TFT_GREENYELLOW, "pero no se grabarán en la base.");
                display.print(TFT_GREENYELLOW, "CONFIGURACION DEL WIFI:");
                display.print(TFT_GREENYELLOW, "- Apretá fuerte el botón de DIAG al encenderlo.");
                display.print(TFT_GREENYELLOW, "- Conectate con un celu al AP.");
                display.print(TFT_GREENYELLOW, "- El resto ya lo sabes...");
                display.print(TFT_GREENYELLOW, "============================");
            }
        }
    }
}
