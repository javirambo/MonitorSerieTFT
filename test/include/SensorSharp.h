/**
 * JJTeam 2021
 */

#pragma once
#include <Arduino.h>

/**
 * Codigo ajustado para sensor Sharp GP2y0a02yk
 * basado en lo sufrido simplifico la clase con las siguientes caracteristicas:
 *  > 12 bits de resolucion en el ADC, necesito resolucion porque voy a trabajar en la parte mas horizontal de la respuesta de la curva Vo vs D
 *  > hago  un gran numero de muestras para que por teorema de limite central el ruido se acerque a su media y ganar precision en 
 * la medicion para tener mejor discriminacion de la presencia del animal
 *  > para usar el rango completo de la salida del sensor uso el atenuador ADC_6dB que no es muy alineal pero lo corrijo con polinomio de tercer grado ad-hoc
 *  > el polinomio linealiza la funcion de D(V) tomando en cuenta la parte de la curva hiperbolica del sesnsor (entre 20 y 150 cm) y la alinealidad del ADC en 6 dB
 *  > la mejor deteccion se dio con un tiempo de 20 ms entre muestras
*/

class SensorSharp
{
public:
  SensorSharp(bool ModoDiag = false);
  ~SensorSharp();

  void Init(uint8_t Pin);
  int Test();
  void SetModoDiagnostico(bool ActivarModo);

  float GetDistancia(void);

  void SetUmbralActivacion(float UmbralSuperior);
  void SetUmbralReset(float UmbralInferior);

public:
  float Distancia; // ultimo valor de salida que es la concentracion de gas en cm

private:
  uint8_t PinEntrada;
  float UltimoValorRaw;        // aca guardo el ultimo valor leido pero en float, asi uso el filtrado
  float UltimoValorEnVolt;     // aca guardo el ultimo valor leido ya convertido a
  uint16_t DelayEntreMuestras; //
  uint8_t TotalMuestras;       //
  float UmbralActivacion;      // umbral de activacion , es cuando de infinito (!) paso a una distancia de deteccion mas cercana
  float UmbralReset;           // Umbral de desactivado, cuando de deteccion paso a infinito.. reset del sistema de deteccion

  bool ModoDiagnostico;
};
