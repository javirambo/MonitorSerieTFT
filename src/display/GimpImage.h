#pragma once
#include <stdint.h>
#include "iconos/wifi1.h"
#include "iconos/wifi2.h"
#include "iconos/wifi3.h"
#include "iconos/wifi4.h"
#include "iconos/celu.h"
#include "iconos/ddbb1.h"
#include "iconos/nowifi.h"
#include "iconos/minion.h"
#include "iconos/clock.h"

typedef struct
{
    uint16_t width;
    uint16_t height;
    uint32_t size;         // ya esta calculado (cantidad de pixels)
    uint16_t* pixel_data;  // ojo!! cada pixel es de 16 bits!!
} GimpImage_t;

/*
    Estas imagenes fueron tomadas con GIMP con la opcion:
    - Exportar imagen -> Codigo fuente en C -> y marcar:
        - guardar como RGB565,
        - usar macros en vez de struct.
    - Solo uso el array de datos.
    - (sacar el static)
*/

extern GimpImage_t wifi1_img;
extern GimpImage_t wifi2_img;
extern GimpImage_t wifi3_img;
extern GimpImage_t wifi4_img;
extern GimpImage_t celu_img;
extern GimpImage_t base_img;
extern GimpImage_t nowifi_img;
extern GimpImage_t logo_img;
extern GimpImage_t clock_img;
