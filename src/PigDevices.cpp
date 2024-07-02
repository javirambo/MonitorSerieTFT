// /**
//  * Dispositivos usados en el PIG GUARD
//  * 
//  * JJTeam 2021
//  */

// // #include "PigDevices.h"
// // #include "logs/FSLog.h"

// void PigDevices::init()
// {
//     pinMode(PIN_DIAG_MODE_JUMPER, INPUT_PULLUP); // Modo Diagnostico es con jumper a masa.
//     modoDiagnostico = !digitalRead(PIN_DIAG_MODE_JUMPER);

//     if (modoDiagnostico)
//         LogDebug("MODO DIAGNOSTICO ACTIVO");

//     SensorDistancia.SetModoDiagnostico(modoDiagnostico);
//     SensorDistancia.Init(PIN_SENSOR_DISTANCIA);

//     DHT22.SetModoDiagnostico(modoDiagnostico);
//     DHT22.Init(PIN_SENSOR_DHT);

//     SensorGas.SetModoDiagnostico(modoDiagnostico);
//     SensorGas.Init(PIN_SENSOR_GAS, 1000); //aparentemente RL en el modulo sensor es de 1K
//     SensorGas.SetTipoGas(CH4);

//     LogDebug("Valvula en Pin %2u", PIN_VALVULA);
//     pinMode(PIN_VALVULA, OUTPUT);
//     digitalWrite(PIN_VALVULA, LOW);

//     LogDebug("Leds WS2812 en Pin %2u", PIN_LEDS_RGB);
//     FastLED.addLeds<WS2812, PIN_LEDS_RGB, GRB>(leds, RGB_LEDS_QUANTITY);
//     SetLedColor(CRGB::Black);

//     // el led interno lo uso para indicar que está vivo.
//     pinMode(PIN_LED_ALIVE, OUTPUT);

//     // Para llevar a configuracion de fabrica, Debe RESETEARSE estando BOOT presionado.
//     pinMode(PIN_BOOT_BUTTON, INPUT_PULLUP);
// }

// void PigDevices::SetLedColor(CRGB color)
// {
//     FastLED.setBrightness(255);
//     FastLED.showColor(color);
// }

// void PigDevices::SetLedAlive(int on)
// {
//     digitalWrite(PIN_LED_ALIVE, on);
// }

// void PigDevices::SetValve(bool on)
// {
//     digitalWrite(PIN_VALVULA, on);
// }

// void PigDevices::ReadDHT22()
// {
//     static bool lastError = false;
//     int err = DHT22.GetData();
//     if (err != SensorDHT_TodoOK && !lastError)
//     {
//         lastError = true; // solo grabo una vez el error
//         LogError("Fallo en DHT22: Error=%d Duracion=%d",
//                  SensorDHTCodigoError(err), SensorDHTErrorDuracion(err));
//     }
//     else
//     {
//         lastError = false;
//     }
// }

// /**
//  * Se encienden AMARILLOS los leds rgb y parpadea el led de OK.
//  * Si se pulsa el BOOT durante este momento, se entra en modo FACTORY DEFAULTS.
//  * Hay que esperar unos segundos (a modo de confirmacion) pero si se suelta antes se aborta.
//  */
// bool PigDevices::isFactoryDefaultPressed()
// {
//     bool pressed = false;
//     Serial.println();
//     for (size_t i = 0; i < 32; i++)
//     {
//         if (pressed)
//         {
//             if (digitalRead(PIN_BOOT_BUTTON) != 0)
//             {
//                 Serial.println(" Abort!"); // no se presiono lo suficiente.
//                 digitalWrite(PIN_LED_ALIVE, 0);
//                 SetLedColor(CRGB::Black);
//                 return false;
//             }
//             Serial.print("* ");
//             SetLedColor(CRGB::Yellow);
//             digitalWrite(PIN_LED_ALIVE, 1);
//             delay(333);
//             SetLedColor(CRGB::Black);
//             digitalWrite(PIN_LED_ALIVE, 0);
//             delay(666);
//         }
//         else if (digitalRead(PIN_BOOT_BUTTON) == 0)
//         {
//             pressed = true;
//             i = 28; // dejo solo 4 pulsos finales
//         }
//         else
//         {
//             Serial.print("*");
//             SetLedColor(CRGB::Yellow);
//             digitalWrite(PIN_LED_ALIVE, 1);
//             delay(44);
//             SetLedColor(CRGB::Black);
//             digitalWrite(PIN_LED_ALIVE, 0);
//             delay(88);
//         }
//     }
//     if (pressed)
//     {
//         Serial.println("!Boot pressed");
//         digitalWrite(PIN_LED_ALIVE, 1);
//         SetLedColor(CRGB::Magenta);
//         return true;
//     }
//     else
//         Serial.println("!"); // Abort! no se presiono nunca.
//     SetLedColor(CRGB::Black);
//     return false;
// }

// //-- unica instancia para todo el proyecto...
// PigDevices io;
