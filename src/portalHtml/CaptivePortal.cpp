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
constexpr char WIFI_HTML[]  = R"(<!DOCTYPE html>
<html><head><meta name="viewport" content="width=device-width, initial-scale=1.0"></head>
<body style="text-align: center;">
    <h1>Monitor Serie</h1>
    <h2>Wifi Setup</h2>
    <form method='GET' action='ok'>
        <select class="boton" name="ssid" style="height: 40px; width: 200px;">
            {OPTIONS}
        </select>
        <br><br>
        <input class="boton" type='text' placeholder='Ingrese la contrase&ntilde;a' size="20" name='pass' style="height: 40px; width: 194px;"/>
        <br><br>
        <input class="boton bg-ok" type='submit' value='Grabar' style="height: 40px; width: 200px;"/>
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

    String options = "<option>Seleccione una red WiFi de la lista</option>";
    size_t i;
    for (i = 0; i < 20; i++)
    {
        String ssid = WiFi.SSID(i);
        if (ssid.isEmpty()) break;
        options += String("<option value=\"") + ssid + "\">" + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + "dbm)</option>";
    }
    if (i == 0)
    {  // no hay redes. lanzo async para mas tarde ver si tengo algo...
        WiFi.disconnect();
        WiFi.scanNetworks(true);
        return;
    }
    String html = WIFI_HTML;
    html.replace("{OPTIONS}", options);
    server.send(200, TEXT_HTML, html);  // por ultimo envio la pagina

    display.showGIMPImage(300, 0, &celu_img);
}

static void HandleOk()
{
    ConfigInst.SetWifi(server.arg("ssid"), server.arg("pass"));
    server.send(200, TEXT_PLAIN, "OK");
    display.print(TFT_ORANGE, "Grabando SSID:'" + ConfigInst.WifiSsid + "' PASS:'" + ConfigInst.WifiPass + "'");
}

void LoopHtmlServer()
{
    dnsServer.processNextRequest();
    server.handleClient();
}

void CaptivePortalStart(String NombreSsid)
{
    // AP: conexion al access point local para que se conecte un celular:
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(NombreSsid.c_str(), "");  // sin contraseña
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
}
