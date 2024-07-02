/*
    PigConfig.h
    Lectura de configuraciones desde un archivo en el FS.

    JJTeam - 2021
*/

#ifndef _pig_config_h
#define _pig_config_h

typedef struct
{
    int gasAlarmThreshold;         // umbrales de alarma
    int temperatureAlarmThreshold; //   "
    int humidityAlarmThreshold;    //   "
    int maximumNumberOfShots;      // luego de disparar esta cantidad pasa a chancha acostada
    int lowShotTime;               // tiempo en alto/bajo del disparo de la valvula
    int highShotTime;              // (en milisegundos, pero multiplos de 100)
    int pigDetectionUpCm;          // distancia de deteccion de la chancha en cm
    int pigDetectionDownCm;        // distancia de chancha sentada
} PigConfig_t;

void ReadConfiguration();

//-- unica instancia para todo el proyecto...
extern PigConfig_t config;

#endif // _pig_config_h