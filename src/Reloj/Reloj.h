#pragma once
#include <Arduino.h>
#include "ezTime.h"
#include "utiles/debug.h"
#include "configuraciones.h"

// uso el ezTime: https://github.com/ropg/ezTime

//-------------------------------------------------------------------
// Las fecha/hora que viajan por la trama LORA serán en milisegundos
// desde el 1 de NOVIEMBRE de 2023. [1698807600000] (hora Argentina)
// (asi las podemos usar con 5 bytes)
#define HORA_BASE_EFFICAST_MS 1698807600000
//-------------------------------------------------------------------

class RelojTR
{
   public:
    int64_t MillisIniciales = 0;  // MS reales (que vino del nihao) en el momento inicial, o sea, cuando se puso en hora.
    int64_t MillisSucios    = 0;  // MS del micro  en el momento de ponerse en hora (con el nihao)
    Timezone LTZ;                 // TODO: ver como vamos a hacer en otros paises

    // La hora se actualiza automaticamente cada N segundos.
    void Init() { setInterval(3600 * 4); }

    // (solo lo usa el Master porque lo pone en hora con el NTP)
    bool Synced() { return timeStatus() == timeSet; }

    // este verifica si está en hora para Master o Slave.
    bool EstaEnHora() { return LTZ.now() * 1000LL > HORA_BASE_EFFICAST_MS; }

    void Sync()
    {
        waitForSync();  // espera actualizar la hora del NTP (en milisegundos)
        // LTZ.setLocation(F("America/Argentina/Buenos_Aires"));
        Print();  // muestra la hora
    }

    // esto es para poner en hora el minion (no usa NTP),
    // lo hace el minion en el Nihao.
    void SetTime(int64_t TimeMillis)
    {
        // antes de poner en hora, guardo los valores iniciales para ajustar los eventos viejos de la cache de telemetrias.
        if (MillisIniciales == 0)
        {
            MillisIniciales = TimeMillis;
            MillisSucios    = GetTimeMillis();
            LogI(" MillisIniciales=%lld      MillisSucios=%lld", MillisIniciales, MillisSucios);
        }

        auto segs = TimeMillis / 1000;
        auto ms   = TimeMillis % 1000;
        LTZ.setTime((time_t)segs, ms);
    }

    // si el TS es viejo, estará fuera de hora, lo ajusto con los datos guardados en el momento de poner en hora.
    int64_t AdjustTime(int64_t TS)
    {
        if (TS < MillisIniciales)
            return TS + MillisIniciales - MillisSucios;
        else
            return TS;
    }

    // retorna fecha/hora en milisegundos
    int64_t GetTimeMillis()
    {
        int64_t result = 0;
        result += static_cast<int64_t>(LTZ.now()) * 1000LL;
        result += static_cast<int64_t>(LTZ.ms());
        return result;
    }

    time_t GetTimeSegs() { return LTZ.now(); }

    // "15/12/2023 11:31:02.169"
    char* ToString()
    {
        static char buf[25];
        sprintf(buf, "%02d/%02d/%04d %02d:%02d:%02d.%03d", LTZ.day(), LTZ.month(), LTZ.year(), LTZ.hour(), LTZ.minute(), LTZ.second(), LTZ.ms());
        return buf;
    }

    //"151223-113102.796"
    char* GetFecha4Log()
    {
        static char buf[20];
        sprintf(buf, "%02d%02d%02d-%02d%02d%02d.%03d ", LTZ.day(), LTZ.month(), LTZ.year() - 2000, LTZ.hour(), LTZ.minute(), LTZ.second(), LTZ.ms());
        return buf;
    }

    void Print() { LogI("Reloj %s", ToString()); }
};

extern RelojTR Reloj;