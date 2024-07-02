/*
    PigConfig.cpp
    Lectura de configuraciones desde un archivo en el FS.

    JJTeam - 2021
*/

#include "IniFile.h"
#include "PigConfig.h"

//-- unica instancia para todo el proyecto...
PigConfig_t config;

void ReadConfiguration()
{
    IniFile ini("/config.ini");
    if (ini.error)
        log_e("Error al leer config.ini, uso valores por defecto.");

    config.gasAlarmThreshold = ini.getInt("alarm_gas", 200);
    config.temperatureAlarmThreshold = ini.getInt("alarm_temperature", 40);
    config.humidityAlarmThreshold = ini.getInt("alarm_humidity", 88);
    config.maximumNumberOfShots = ini.getInt("valve_max_count", -1);
    config.highShotTime = ini.getInt("valve_high_ms", 60000);
    config.lowShotTime = ini.getInt("valve_low_ms", 1);
    config.pigDetectionUpCm = ini.getInt("pig_up_cm", 80);
    config.pigDetectionDownCm = ini.getInt("pig_down_cm", 110);
}
