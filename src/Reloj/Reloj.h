#pragma once
#include <Arduino.h>
#include "ezTime.h"
#include "utiles/debug.h"
#include "configuraciones.h"

// uso el ezTime: https://github.com/ropg/ezTime

class RelojTR
{
   public:
    Timezone LTZ;

    void Init() { setInterval(3600 * 4); }

    bool Synced() { return timeStatus() == timeSet; }

    void Sync()
    {
        waitForSync();                    // espera actualizar la hora del NTP (en milisegundos)
        auto s = LTZ.now() - (3600 * 3);  // -3 hora Argentina
        LTZ.setTime(s);
        Print();
    }

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
        static char buf[55];
        sprintf(buf, "Hora Argentina: %02d/%02d/%04d %02d:%02d:%02d.%03d", LTZ.day(), LTZ.month(), LTZ.year(), LTZ.hour(), LTZ.minute(), LTZ.second(), LTZ.ms());
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