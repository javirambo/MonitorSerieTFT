/**
 * Configura una serie de paginas web para conectarse a una red WiFi.
 *
 * Usa el método de "Captive Portal" que muestra una página HTML para configurar la wifi.
 *
 * Aprovechamos para poner en esa pagina HTML cosas como ver logs, etc
 *
 * JJTeam - 2021
 */

#include <Arduino.h>
#include "utiles/debug.h"
#include "Leds/Leds.h"
#include <WebServer.h>
#include <WiFi.h>
#include "DNSServer.h"
#include "config/ConfigInstance.h"
#include "display/DisplayTFT.h"

constexpr char TEXT_HTML[]  = "text/html";
constexpr char TEXT_PLAIN[] = "text/plain";
const String SELECCIONE_UNA_RED("Seleccione una red WiFi de la lista");
int wifiCount  = 0;
bool Finishela = false;

constexpr char WIFI_HTML[] = R"(<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
    <h1>Monitor Serie Wifi Setup</h1>
    <form method='GET' action='ok'>
        <select class="boton" name="ssid" style="height: 40px;">
            {OPTIONS}
        </select>
        <br><br>
        <input class="boton" type='text' placeholder='Ingrese la contrase&ntilde;a' size="20" name='pass' style="height: 40px;"/>
        <br><br>
        <input class="boton bg-ok" type='submit' value='Grabar' style="height: 40px;"/>
    </form>
</body>
</html>)";

static DNSServer dnsServer;
static IPAddress apIP(192, 168, 9, 66);
static IPAddress netMsk(255, 255, 255, 0);
WebServer server(80);

String GetHostname() { return String("javiercito"); }

static void SendCacheHeader()
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
}

static void HandleRoot()
{
    SendCacheHeader();  // los portales cautivos no deben cachear!

    String options = String("<option>") + SELECCIONE_UNA_RED + "</option>";
    for (size_t i = 0; i < wifiCount; i++)
    {
        options += String("<option value=\"") + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + "dbm)</option>";
    }
    String html = WIFI_HTML;
    html.replace("{OPTIONS}", options);
    server.send(200, TEXT_HTML, html);  // por ultimo envio la pagina

    display.showGIMPImage(300, 0, &celu_img);
}

static void HandleOk()
{
    ConfigInst.SetWifi(server.arg("ssid"), server.arg("pass"));
    LogE("Me apretaron OK en la pagina web. La pass wifi es: %s  %s", ConfigInst.WifiSsid.c_str(), ConfigInst.WifiPass.c_str());
    server.send(200, TEXT_PLAIN, "OK");
    Finishela = true;
}

void CaptivePortalStart()
{
    LogE("SE ABRE EL PORTAL DE CONFIGURACION");

    WiFi.begin("javiercito", "");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    display.showGIMPImage(300, 0, &nowifi_img);
    AllLeds(CRGB::Black);

    int reintentos = 0;
    do
    {
        LogI("Buscando redes wifi...");
        WiFi.scanNetworks(true, true, false, 777);  // ojo que menos de 600 da error porque no alcanza a revisar las redes...
        do
        {
            wifiCount = WiFi.scanComplete();
        } while (wifiCount == WIFI_SCAN_RUNNING);
        LogI("Encontradas %d redes wifi", wifiCount < 0 ? 0 : wifiCount);

        if (wifiCount < 0)
        {
            AllLeds(CRGB::Red);
            LogE("FALLO AL BUSCAR REDES WIFI. REINTENTO...");
            delay(2000);
            AllLeds(CRGB::Black);
            reintentos++;
        }
    } while (reintentos < 5 && wifiCount <= 0);

    // AP: conexion al access point local para que se conecte un celular:
    String NombreSsid = String("Monitor Serie Setup");
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(NombreSsid.c_str(), "");  // sin contraseña

    display.showGIMPImage(300, 0, &wifi4_img);

    delay(500);  // Without delay I've seen the IP address blank
    LogI("Access Point started: %s - IP: %s", NombreSsid.c_str(), WiFi.softAPIP().toString().c_str());

    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(53, "*", apIP);

    server.on("/hotspot-detect.html", HandleRoot);  //  se redirigen al captive portal
    server.on("/success.html", HandleRoot);         //
    server.on("/generate_204", HandleRoot);         //
    server.on("/ncsi.txt", HandleRoot);             //
    server.on("/fwlink", HandleRoot);               // (Microsoft captive portal)
    server.on("/", HandleRoot);                     // index o root
    server.on("/ok", HandleOk);                     // fin
    server.begin();                                 // Web server start
    LogI("HTTP server started at IP: %s", server.client().localIP().toString().c_str());

    while (!Finishela)
    {
        dnsServer.processNextRequest();
        server.handleClient();
    }
    delay(1000);
    Finishela = false;
}
