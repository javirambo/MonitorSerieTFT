/**
 * Acceso a los recursos de la carpeta resources, que se linkean y terminan en la flash del ESP32.
 * 
 * JJTeam - 2021
 */

#ifndef _pig_resources_h
#define _pig_resources_h

#include <Arduino.h>

//-- archivos linkeados (se suben a la flash en el linker, y queda esta referencia para usarlos)
//-- agregar los archivos en el platformio.ini
// @see: https://docs.platformio.org/en/latest/platforms/espressif32.html#embedding-binary-data

#define RES_SIZE(a) (ptrdiff_t) & a

#define EMBED_IMAGE(_resource)                                                          \
    extern const uint8_t _resource[] asm("_binary_resources_" #_resource "_start");     \
    extern const uint8_t _resource##_end[] asm("_binary_resources_" #_resource "_end"); \
    extern const size_t _resource##_psize asm("_binary_resources_" #_resource "_size");

#define EMBED_TEXT(_resource)                                                        \
    extern const char _resource[] asm("_binary_resources_" #_resource "_start");     \
    extern const char _resource##_end[] asm("_binary_resources_" #_resource "_end"); \
    extern const size_t _resource##_psize asm("_binary_resources_" #_resource "_size");

// EMBED_IMAGE(chancha_parada_jpg)
// EMBED_IMAGE(chancha_sentada_jpg)
// EMBED_IMAGE(gas_metano_jpg)
// EMBED_IMAGE(humedad_blue_jpg)
// EMBED_IMAGE(humedad_green_jpg)
// EMBED_IMAGE(humedad_red_jpg)
// EMBED_IMAGE(temperatura_blue_jpg)
// EMBED_IMAGE(temperatura_green_jpg)
// EMBED_IMAGE(temperatura_red_jpg)
// EMBED_IMAGE(logo_jpg)
EMBED_IMAGE(web_jpg)
EMBED_IMAGE(clock_jpg)
EMBED_IMAGE(pc_jpg)
EMBED_IMAGE(wifi00_jpg)
EMBED_IMAGE(wifi01_jpg)
EMBED_IMAGE(wifi10_jpg)
EMBED_IMAGE(wifi11_jpg)
// EMBED_TEXT(style_css)

#endif // _pig_resources_h