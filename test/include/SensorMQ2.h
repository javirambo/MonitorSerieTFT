/**
 * JJTeam 2021
 */

#pragma once
#include <Arduino.h>

// valores calculados por regresion lineal de tabla de datasheet para la ecuacion Y = A * (RS/R0)^B con ajuste >0.99
constexpr float A[] = {1.0f, 26.267f, 18.9538f, 21.607f, 30.536f, 20.3706f, 19.7613f};
constexpr float B[] = {1.0f, -0.476337f, -0.464894f, -0.377437f, -0.332671f, -0.373217f, -0.463681f};

// valores segun tabla de internet
constexpr float M[] = {1.0f, -2.2459f, -2.2879f, -2.6208f, -3.1157f, -2.7028f, -2.2879f};
constexpr float Y0[] = {0.0f, 2.9845f, 2.7901f, 3.6075f, 4.5134f, 3.5595f, 2.7901f};

typedef enum
{
  AIRE = 0,
  H2,
  LPG,
  CH4,
  CO,
  ALCOHOL,
  PROPANO
} Gases_t;

class SensorMQ2
{
public:
  SensorMQ2(bool ModoDiag = false);
  ~SensorMQ2();

  void Init(uint8_t Pin, float RL);
  int Test();
  void SetModoDiagnostico(bool ActivarModo);

  void SetModoLog(bool Modo); // true elijo el modelo logaritmico, false si uso el de regresion para el modelo potencial
  void SetHumedadRelativa(float HumedadR);
  void SetTemperatura(float Temp);
  void SetTipoGas(Gases_t Gas);

  float CalibraR0(void); // Realiza la calibracion del sensor para hallar la R0 y RS0

  Gases_t GetTipoGas();
  const char *GetNombreGas(Gases_t);
  float GetR0(void);
  bool GetModoLOG(void);
  float GetPPM(void);

private:
  float GetValorR(float ValorV);
  float LeeSensor(void);

public:
  float PPM; // ultimo valor de salida que es la concentracion de gas en ppm

private:
  bool ModoDiagnostico;

  uint8_t PinEntrada;
  float UltimoValorRaw;        // aca guardo el ultimo valor leido pero en float, asi uso el filtrado
  float UltimoValorEnVolt;     // aca guardo el ultimo valor leido ya convertido a
  uint16_t DelayEntreMuestras; //
  uint8_t TotalMuestras;       //
  float UmbralActivacion;      // umbral de activacion
  float UmbralReset;           // Umbral de desactivado

  float VRef;
  float R0;
  float RS0;
  float RL;
  float HR;
  float T;

  Gases_t TipoGas;

  bool ModoLOG;
  bool CompensarPorHR;
  bool CompensarPorT;
};
