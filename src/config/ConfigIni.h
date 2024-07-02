#pragma once
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <map>
#include "utiles/SplitString.h"
#include "utiles/debug.h"

class ConfigIni
{
   public:
    // ojo que si no existe retornara 0.
    int GetInt(String FileName, String name) { return atoi(Get(FileName, name).c_str()); }

    String Get(String FileName, String name)
    {
        String temp;

        File fp = SPIFFS.open(String(FileName.c_str()), "r");
        if (fp)
        {
            while (fp.available())
            {
                String linea            = fp.readStringUntil('\n');
                std::vector<String> ini = SplitString(linea.c_str(), '=');
                if (ini.size() > 1 && ini[0] == name)
                {
                    temp = ini[1];
                    break;
                }
            }
            fp.close();
        }
        else
        {
            LogE("Existe el archivo %s?", FileName.c_str());
        }
        return temp;
    }

    void Set(String FileName, String name, String value)
    {
        // -1- meto todo en un mapa:
        std::map<String, String> map;

        File fp = SPIFFS.open(String(FileName.c_str()), "r");
        if (fp)
        {
            while (fp.available())
            {
                String linea            = fp.readStringUntil('\n');
                std::vector<String> ini = SplitString(linea.c_str(), '=');
                if (ini.size() > 1) map[ini[0]] = ini[1];
            }
            fp.close();
        }

        // -2- agrego o reemplazo el string: (solo si es distinto)
        if (map[name] != value)
        {
            map[name] = value;

            // -3- grabo todo :
            fp = SPIFFS.open(String(FileName.c_str()), "w");
            if (fp)
            {
                for (auto &m : map)
                {
                    String entry = m.first + "=" + m.second;
                    fp.print(entry);
                    fp.print('\n');
                    // TRACE("save: %s", entry.c_str());
                }
                fp.close();
            }
            else
            {
                LogE("No se pudo escribir en el archivo %s", FileName.c_str());
            }
        }
    }
};
