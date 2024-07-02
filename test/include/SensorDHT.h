/*
The MIT License (MIT)

Copyright (c) 2016-2017 winlin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


      MODIFICADO POR JJTeam - 2021
*/

#pragma once
#include <Arduino.h>

// High 8bits are time duration.
// Low 8bits are error code.
// For example, 0x0310 means t=0x03 and code=0x10,
// which is start low signal(0x10) error.
// @see https://github.com/winlinvip/SimpleDHT/issues/25
#define SensorDHTError(t, err) ((t << 8) & 0xff00) | (err & 0x00ff)

// Get the time duration from error.
#define SensorDHTErrorDuracion(err) ((err & 0xff00) >> 8)
// Get the error code defined bellow.
#define SensorDHTCodigoError(err) (err & 0x00ff)

// Success.
#define SensorDHT_TodoOK 0            // Error to wait for start low signal.
#define SensorDHTErrorACKLow 16       // Error to wait for start high signal.
#define SensorDHTErrorACKHigh 17      // Error to wait for data start low signal.
#define SensorDHTErrorDataLow 18      // Error to wait for data read signal.
#define SensorDHTErrorDataRead 19     // Error to wait for data EOF signal.
#define SensorDHTErrorDataEOF 20      // Error to validate the checksum.
#define SensorDHTErrorDataChecksum 21 // Error when temperature and humidity are zero, it shouldn't happen.
#define SensorDHTErrorZeroSamples 22  //

#define T_Request_LOW 1000
#define T_Request_HIGH 30
#define T_MinACK_LOW 30
#define T_MinACK_HIGH 50
#define LevelTimeout 500000 // 500ms

class SensorDHT
{
public:
  SensorDHT();
  ~SensorDHT();

  void Init(uint8_t Pin);
  int Test();
  void SetModoDiagnostico(bool ActivarModo);

  int GetData(void); // actualiza los valores de HR y T pero no devuelve nada
  float GetTemperatura(void);
  float GetHumedadRelativa(void);

private:
  long LevelTime(uint8_t Level);
  long LevelTime(uint8_t Level, int FirstWait, int Interval);
  uint8_t InvertirBits(uint8_t &b);

public:
  float HumedadRelativa;
  float Temperatura;

private:
  bool ModoDiagnostico;
  uint8_t Pin;
  uint8_t RawData[5];
};
