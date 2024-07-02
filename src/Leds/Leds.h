#pragma once
#include "configuraciones.h"
#include "FastLed/FastLED.h"

void LedsInit();
void NextLed(CRGB col, bool dejarCola = true);
void AllLeds(CRGB col, uint8_t brillo = 255);
void LedsInFire();
void LedsOcean();
void LedBlink(int ledNum, CRGB col);
void LedON(int ledNum, CRGB colorON, CRGB colorOFF);  // apaga el resto
void LedON(int ledNum, CRGB colorON);                 // no toca el resto
bool VerificaBotonDeFabrica();
void LedsSemaforo(uint8_t *data, int size);
void AllLedsTemporizados(CRGB col, int segundos);
// void LedsLogoTemporizados(CRGB col, int segundos);
void LoopLeds();
void FlashGordon(CRGB col, bool alverre);
[[noreturn]] void Navidad();
void LedsKitt();
void LedsTemporizadosOFF();
// void LedsLogo(CRGB col);
