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
#include "display/DisplayTFT.h"
#include <WiFi.h>
#include <SPIFFS.h>
#include "internet/WiFiMulti.h"
#include "portalHtml/CaptivePortal.h"

WiFiMulti wifiMulti;

#define BUF_LEN 1024
int idx = 0;
char buf[BUF_LEN];

const char* host = "iot.efficast.ai";
const int port   = 80;

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

void PonerEnHora()
{
    if (WiFi.status() == WL_CONNECTED && !Reloj.Synced())
    {
        LogI("Me pongo en hora...");
        display.print(TFT_SKYBLUE, "Me pongo en hora...");
        display.showGIMPImage(280, 0, &clock_img);
        Reloj.Init();
        Reloj.Sync();  // aca se prenden los leds.
        display.print(TFT_SKYBLUE, Reloj.ToString());
        AllLeds(CRGB::Black);
    }
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
    if (!display.isOk) LogE("El display parece que no funciona!");

    LogI("Init ok. FreeHeap=%u. MaxAllocHeap=%u", ESP.getFreeHeap(), ESP.getMaxAllocHeap());

    // muestro el logo inicial:
    display.showGIMPImage(0, 0, &logo_img);

    delay(4000);
    // muestra info, ID, etc...
    display.clearScreen(TFT_BLACK);
    display.tft.setTextColor(TFT_BLUE);
    display.tft.drawString("* TERMINAL SERIE * Build date: " + String(__DATE__), 8, 3, 1);
    display.print(TFT_SKYBLUE, "Conectando al wifi ...");

    // inicio wifi:
    WiFi.mode(WIFI_AP_STA);
    // Add list of wifi networks
    wifiMulti.addAP("example", "example");
    wifiMulti.addAP(ConfigInst.WifiSsid.c_str(), ConfigInst.WifiPass.c_str());
}

void SendToDatabase(const char* buf)
{
    FastLED.setBrightness(50);

    WiFiClient client;
    if (!client.connect(host, port))
    {
        LogE("Error al conectar al servidor php");
        display.print(TFT_RED, "No se pudo conectar a la base!");
        NextLed(TFT_RED, false);
        return;
    }

    display.showGIMPImage(280, 0, &base_img);
    NextLed(TFT_YELLOW, false);

    // ejemplo: "https://iot.efficast.ai/api/logapp/insert.php?ts=2&log=prueba"

    String requestBody = "ts=" + String(Reloj.GetTimeMillis()) + "&log=" + String(buf);
    client.print(String("GET /api/logapp/insert.php?") + requestBody + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

#ifdef ESPERAR_RESPUESTA
    Serial.println("esperando respuesta...");
    uint32_t T = millis() + 5000;
    while (client.connected() && T > millis())
    {
        String line = client.readStringUntil('\n');
        if (line == "\r") break;
    }
    String response = "";
    while (client.available()) response += client.readStringUntil('\n');
    Serial.println(response);
#endif

    client.stop();
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
        // quito el icono cada 2 segs (el de la BBDD)
        display.tft.fillRect(280, 0, base_img.width, base_img.height, TFT_BLACK);
    }

    //--------------------------------------------
    // reintento con otro wifi:
    uint16_t RetValue = wifiMulti.run(10000);
    int8_t scanResult = (RetValue >> 8);
    uint8_t status    = (RetValue & 0xFF);

    static uint8_t statusAnt = -1;
    if (statusAnt != status)
    {
        statusAnt = status;
        if (status != WL_CONNECTED)
        {
            display.print(TFT_ORANGE, "No pude conectar al wifi, reintento en segundos.");
        }
        else
        {
            static char mensaje[150];
            sprintf(mensaje, "Wifi:%s, RSSI:%d, Ch:%d", WiFi.SSID().c_str(), WiFi.RSSI(), WiFi.channel());
            display.print(TFT_SKYBLUE, String(mensaje));

            static bool once = false;
            if (!once)
            {
                once = true;
                PonerEnHora();
                String NombreSsid = String("Monitor Serie Setup");
                CaptivePortalStart(NombreSsid);
                display.print(TFT_ORANGE, "Podes configurar desde:");
                display.print(TFT_ORANGE, " > Access Point: " + NombreSsid);
                display.print(TFT_ORANGE, " > HTTP Server : " + WiFi.localIP().toString());
                display.print(TFT_ORANGE, "Podes ver los LOGS en:");
                display.print(TFT_ORANGE, " > https://iot.efficast.ai/api/logapp/");
            }
        }
    }

    LoopHtmlServer();
}
