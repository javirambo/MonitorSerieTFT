/**
 * Configura una serie de paginas web para conectarse a una red WiFi.
 * 
 * Usa el método de "Captive Portal" que muestra una página HTML para configurar la wifi.
 * 
 * Aprovechamos para poner en esa pagina HTML cosas como ver logs, etc
 * 
 * JJTeam - 2021
 */

#include "RTC.h"

extern uint32_t WifiLoop();
extern void WifiSetup();
extern void ClearCredentials();
extern Rtc rtc;

enum
{
    WIFI_POT0 = 1,    // 2 bits para la potencia
    WIFI_POT1 = 2,    // ""
    WIFI_POT_SET = 4, // se es 1 es porque viene la potencia en los 2 bits anteriores
    WIFI_OK = 8,
    RTC_OK = 16,
    WEB_SERVER_CONN = 32,
    LOGS_PAGE_CONN = 64,
};