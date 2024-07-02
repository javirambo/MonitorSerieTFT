#pragma once
#include <Arduino.h>
#include "configuraciones.h"
#include "ConfigIni.h"

class ConfigInstance : public ConfigIni
{
   public:
    const String FileName = "/config.ini";

    void Load()
    {
        // WifiSsid           = Get(FileName, "WiSsid");
    }

    void SetWifi(const String& ssid, const String& pass)
    {
        // Set(FileName, "WiPass", pass);
    }

    // void Print() const { printf("%s\n", ToString().c_str()); }
};

extern ConfigInstance ConfigInst;
