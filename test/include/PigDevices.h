/**
 * Dispositivos usados en el PIG GUARD
 * 
 * JJTeam 2021
 */

#ifndef _pig_devices_h
#define _pig_devices_h

#include "SensorDHT.h"
#include "SensorSharp.h"
#include "SensorMQ2.h"
#define FASTLED_INTERNAL
#include "../src/FastLed/FastLED.h"

#define RGB_LEDS_QUANTITY 5     // cantidad de leds rgb que vamos a manejar
#define PIN_SENSOR_GAS 35       // sensores
#define PIN_SENSOR_DISTANCIA 34 //
#define PIN_SENSOR_DHT 27       //
#define PIN_LED_ALIVE 2         // led interno en la placa
#define PIN_VALVULA 13          //
#define PIN_DIAG_MODE_JUMPER 32 // (para el circuito PIG GUARD-CORE, REV 4.0, Jul 2021)
#define PIN_LEDS_RGB 25         // leds tipo WS2812B
#define PIN_CS_MICROSD 5        // para grabar logs y estadisticas (eventualmente)
#define PIN_BOOT_BUTTON 0       // pin de grabacion (BOOT) que se usa para resetear a configuracion de fabrica. (Debe RESETEARSE estando BOOT presionado)

class PigDevices
{
private:
    CRGB leds[RGB_LEDS_QUANTITY]; // leds rgb tipo WS2812B

public:
    bool modoDiagnostico = false;
    SensorDHT DHT22;
    SensorSharp SensorDistancia;
    SensorMQ2 SensorGas;

    void init();
    void SetLedColor(CRGB color);
    void SetLedAlive(int on);
    void SetValve(bool on);
    void ReadDHT22();
    bool isFactoryDefaultPressed();
};

//-- unica instancia para todo el proyecto...
extern PigDevices io;

#endif // _pig_devices_h