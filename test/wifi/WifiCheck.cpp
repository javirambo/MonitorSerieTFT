/**
 * Configura una serie de paginas web para conectarse a una red WiFi.
 * 
 * Usa el método de "Captive Portal" que muestra una página HTML para configurar la wifi.
 * 
 * Aprovechamos para poner en esa pagina HTML cosas como ver logs, etc
 * 
 * JJTeam - 2021
 */

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>
#include "Tools.h"
#include "PigResources.h"
#include "../logs/FSLog.h"
#include "RTC.h"
#include "WifiCheck.h"

constexpr char HTML_BODY_START[] = "<!DOCTYPE html><html><head>"
                                   "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                                   "<link rel=\"stylesheet\" href=\"style.css\">"
                                   "</head><body>";

constexpr char HTML_BODY_END[] = "</body></html>";

constexpr char HTML_MENU_INDEX[] = "<ul><li><a href='/wifi'>Configurar la conexion de WiFi</a></li>"
                                   "<li><a href='/logs'>Acceso a los logs</a></li></ul>";

/* hostname for mDNS. Should work at least on windows. Try http://myHostname.local */
const String getHostname() { return "pigguard" + getSerialNumber(); }
const String getSoftAP_SSID() { return "Pig Guard " + getSerialNumber(); }
constexpr char TEXT_HTML[] = "text/html";
constexpr char TEXT_PLAIN[] = "text/plain";

/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
char ssid[33] = "";
char password[20] = "";
#define CREDENTIAL_EEPROM_SIZE sizeof(ssid) + sizeof(password) + 3
const byte DNS_PORT = 53;
DNSServer dnsServer;
WebServer server(80);
Rtc rtc;                 // hora en timepo real, tomada de un server de tiempo.
uint16_t web_status = 0; // estado del web server (para mostrar en un icono)

/* Soft AP network parameters */
IPAddress apIP(172, 217, 28, 1);
IPAddress netMsk(255, 255, 255, 0);

/** Should I connect to WLAN asap? */
boolean connect;

// arma un TAG HTML, y puede tener clase (optativa)
String Tag(String tag, String text, String classs = "")
{
    if (classs.isEmpty())
        return "<" + tag + ">" + text + "</" + tag + ">";
    else
        return "<" + tag + " class=\"" + classs + "\">" + text + "</" + tag + ">";
}

// TAG OPTION (value optativo)
String Option(String text, String value = "")
{
    return "<option value=\"" + value + "\">" + text + "</option>";
}

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal()
{
    if (!isIp(server.hostHeader()) && server.hostHeader() != (getHostname() + ".local"))
    {
        LogTrace("Request redirected to captive portal");
        server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
        server.send(302, TEXT_PLAIN, ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
        server.client().stop();           // Stop is needed because we sent no content length
        return true;
    }
    return false;
}

/** Load WLAN credentials from EEPROM */
void loadCredentials()
{
    EEPROM.begin(CREDENTIAL_EEPROM_SIZE);
    EEPROM.get(0, ssid);
    EEPROM.get(0 + sizeof(ssid), password);
    char ok[2 + 1];
    EEPROM.get(0 + sizeof(ssid) + sizeof(password), ok);
    EEPROM.end();
    if (String(ok) != String("OK"))
    {
        ssid[0] = 0;
        password[0] = 0;
    }
    else
        LogDebug("Recovered credentials => [%s/%s]", ssid, password);
}

/** Store WLAN credentials to EEPROM */
void saveCredentials()
{
    EEPROM.begin(CREDENTIAL_EEPROM_SIZE);
    EEPROM.put(0, ssid);
    EEPROM.put(0 + sizeof(ssid), password);
    char ok[2 + 1] = "OK";
    EEPROM.put(0 + sizeof(ssid) + sizeof(password), ok);
    EEPROM.commit();
    EEPROM.end();
}

// para el factory defaults.
void ClearCredentials()
{
    ssid[0] = 0;
    password[0] = 0;
    EEPROM.begin(CREDENTIAL_EEPROM_SIZE);
    EEPROM.put(0, ssid);
    EEPROM.put(0 + sizeof(ssid), password);
    char no_ok[2 + 1] = "**";
    EEPROM.put(0 + sizeof(ssid) + sizeof(password), no_ok);
    EEPROM.commit();
    EEPROM.end();
}

void SendCacheHeader()
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
}

bool isLocalIP()
{
    return server.client().localIP() == apIP;
}

String GetConnectThrough()
{
    return Tag("p", "Est&aacute;s conectado a trav&eacute;s de<br>" +
                        (isLocalIP()
                             ? "soft AP: <b>" + getSoftAP_SSID() + "</b>"
                             : "red wifi: <b>" + String(ssid) + "</b>"));
}

/** Handle root or redirect to captive portal */
void handleRoot()
{
    if (captivePortal())
    { // If caprive portal redirect instead of displaying the page.
        web_status = WEB_SERVER_CONN;
        return;
    }

    SendCacheHeader();

    String Page;
    Page += HTML_BODY_START;
    Page += GetConnectThrough();
    Page += HTML_MENU_INDEX;
    Page += HTML_BODY_END;
    server.send(200, TEXT_HTML, Page);
}

// cada linea de log la decoro y la evío:
void EnrichAndSend(String line)
{
    String color = (line[1] == 'E' ? "r" : (line[1] == 'I' ? "b" : "n"));
    server.sendContent(Tag("p", line, color));
}

void handleLogs()
{
    web_status = LOGS_PAGE_CONN;
    SendCacheHeader();

    // envia con Chunk Mode HTTP 1.1

    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, TEXT_HTML, HTML_BODY_START);
    server.sendContent(Tag("h2", "Logs al iniciar el sistema"));

    server.sendContent("<code>");
    FSLOG.forEachStartup(EnrichAndSend);
    server.sendContent("</code>");

    server.sendContent(Tag("h2", "Logs hist&oacute;ricos"));

    server.sendContent("<code>");
    FSLOG.forEachLine(EnrichAndSend);
    server.sendContent("</code>");

    server.sendContent(HTML_BODY_END);
    server.sendContent(""); // END CHUNK!
}

/** Wifi config page handler */
void handleWifi()
{
    SendCacheHeader();

    String Page;
    Page += HTML_BODY_START;
    Page += Tag("h1", "Pig Guard");
    Page += GetConnectThrough();
    Page += Tag("p", Tag("b", "SoftAP config"));
    Page += Tag("p", "SSID: " + getSoftAP_SSID());
    Page += Tag("p", "IP: " + toStringIp(WiFi.softAPIP()));
    Page += Tag("p", Tag("b", "WLAN config"));
    Page += Tag("p", "SSID: " + String(ssid));
    Page += Tag("p", "IP: " + toStringIp(WiFi.localIP()));

    LogDebug("scanning ssid...");
    int n = WiFi.scanNetworks();

    if (n > 0)
    {
        Page += Tag("h2", "Redes disponibles:");
        Page += "<form method='POST' action='wifisave'>";
        Page += "<select name=\"n\">";
        Page += Option("Seleccione una red");
        for (int i = 0; i < n; i++)
            Page += Option(WiFi.SSID(i) + "(" + WiFi.RSSI(i) + ")", WiFi.SSID(i));
        Page += "</select>"
                "<br><input type='text' placeholder='Ingrese la clave' size=\"15\" name='p'/>"
                "<br><input type='submit' value='Conectar'/>"
                "<br></form>";
        Page += Tag("p", "(Actualiza la p&aacute;gina para refrescar)");
    }
    else
    {
        Page += Tag("p", "No WLAN found", "r");
    }
    server.send(200, TEXT_HTML, Page);
    server.client().stop(); // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave()
{
    LogTrace("wifi save");

    server.arg("n").toCharArray(ssid, sizeof(ssid) - 1);
    server.arg("p").toCharArray(password, sizeof(password) - 1);
    server.sendHeader("Location", "wifi", true);

    SendCacheHeader();

    server.send(302, TEXT_PLAIN, ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop();           // Stop is needed because we sent no content length
    saveCredentials();
    connect = strlen(ssid) > 0; // Request WLAN connect with new credentials if there is a SSID
}

void handleNotFound()
{
    if (captivePortal())
    { // If captive portal redirect instead of displaying the error page.
        return;
    }
    String message = F("File Not Found\n\n");
    message += F("URI: ");
    message += server.uri();
    message += F("\nMethod: ");
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += F("\nArguments: ");
    message += server.args();
    message += F("\n");

    for (uint8_t i = 0; i < server.args(); i++)
        message += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");

    SendCacheHeader();
    server.send(404, TEXT_PLAIN, message);
}

void connectWifi()
{
    LogTrace("Connecting as wifi client...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    WiFi.waitForConnectResult();
}

/**
 * 1- verifica si tiene credenciales guardadas.
 * 2- intenta conectarse a ese SSID/password
 * 2.a- si se conecta, toma la hora y se desconecta.
 * 2.b- si falla, arma un AP para que se pueda configurar.
 */
void WifiSetup()
{
    LogTrace("Configuring access point...");
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(getSoftAP_SSID().c_str(), ""); // sin contraseña (AP OPEN)

    delay(500); // Without delay I've seen the IP address blank
    LogTrace("AP IP address: %s", toStringIp(WiFi.softAPIP()).c_str());

    /* Setup the DNS server redirecting all the domains to the apIP */
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);

    /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
    server.on("/", handleRoot);
    server.on("/wifi", handleWifi);
    server.on("/logs", handleLogs);
    server.on("/wifisave", handleWifiSave);
    server.on("/generate_204", handleRoot); //Android captive portal. Maybe not needed. Might be handled by notFound handler.
    server.on("/fwlink", handleRoot);       //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
    // server.on("/style.css", []()
    //           { server.send(200, "text/css", style_css); });
    // server.on("/logo.jpg", []()
    //           { server.send_P(200, "image/jpeg", (char *)logo_jpg, RES_SIZE(logo_jpg_psize)); });
    server.onNotFound(handleNotFound);
    server.begin(); // Web server start

    LogTrace("HTTP server started");

    loadCredentials();          // Load WLAN credentials from network
    connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID
}

uint32_t WifiLoop()
{
    static uint32_t last_status = WL_IDLE_STATUS;
    static time_t lastConnectTry = 0;
    static time_t lastWebStatusTry = 0;
    static time_t lastPowerTry = 0;

    time_t now = time(0);

    // verifico si se desconecta y lo vuelvo a conectar...
    if (connect)
    {
        LogTrace("Connect requested");
        connect = false;
        connectWifi();
        lastConnectTry = now;
    }

    unsigned int wifi_status = WiFi.status();
    if (wifi_status == 0 && now > lastConnectTry + 60)
    {
        /* If WLAN disconnected and idle try to connect */
        /* Don't set retry time too low as retry interfere the softAP operation */
        connect = true;
    }

    if (last_status != wifi_status)
    { // WLAN status change

        last_status = wifi_status;
        if (wifi_status == WL_CONNECTED)
        {
            /* Just connected to WLAN */
            LogTrace("Connected to %s IP address %s", ssid, toStringIp(WiFi.localIP()).c_str());

            // Setup MDNS responder, & Add service to MDNS-SD
            if (MDNS.begin(getHostname().c_str()))
                MDNS.addService("http", "tcp", 80);

            // system time is not reset at sleep or reset (but power-up reset)
            // tarda 15 seg max o da error....porque reintenta hasta que se pone en hora.
            if (!rtc.isTimeSetted())
            {
                rtc.getTimeFromNtpServer();
                if (rtc.isTimeSetted())
                {
                    LogAtStartUp("Time setted: %s", rtc.getDateTime());
                }
            }
            // una vez conectado el wifi, desconecto el server, para ahorrar energia...
            // server.stop();
        }
        else if (wifi_status == WL_NO_SSID_AVAIL)
        {
            WiFi.disconnect();
        }
    }

    // proceso otros requests...
    dnsServer.processNextRequest();
    server.handleClient();

    ///// preparo el estado de la respuesta:

    // muestro que alguien se conectó al web server solo durante 1 min.
    if (web_status == 0)
        lastWebStatusTry = now;
    if (now > lastWebStatusTry + 60)
        web_status = 0;

    uint32_t RetStatus = web_status;

    // cada N segundos pido la potencia del wifi
    // retorna una medicion de 4 valores: SUPER/OK/POOR/BAD
    if (now > lastPowerTry + 30)
    {
        lastPowerTry = now;

        int8_t pot = WiFi.RSSI();
        if (pot > -70) // potencia SUPER
            RetStatus |= 3;
        if (pot <= -70 && pot > -80) //potencia OK
            RetStatus |= 2;
        if (pot <= -80 && pot > -90) // potencia POOR
            RetStatus |= 1;
        // < -90 BAD !
        RetStatus |= WIFI_POT_SET;
    }

    // puedo saber si esta conectado, y con lo anterior veo la potencia:
    if (wifi_status == WL_CONNECTED)
        RetStatus |= WIFI_OK;

    if (rtc.isTimeSetted())
        RetStatus |= RTC_OK;

    return RetStatus;
}
