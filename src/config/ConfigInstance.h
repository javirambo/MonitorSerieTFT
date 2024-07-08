#pragma once
#include <Arduino.h>
#include "configuraciones.h"
#include "ConfigIni.h"

class ConfigInstance : public ConfigIni
{
   public:
    const String FileName = "/config.ini";
    String WifiSsid, WifiPass;

    void Load()
    {
        WifiSsid = Get(FileName, "WiSsid");
        WifiPass = Get(FileName, "WiPass");
    }

    void SetWifi(const String& ssid, const String& pass)
    {
        Set(FileName, "WiSsid", ssid);
        Set(FileName, "WiPass", pass);
        WifiSsid = ssid;
        WifiPass = pass;
    }
};

extern ConfigInstance ConfigInst;
