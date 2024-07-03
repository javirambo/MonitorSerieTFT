#pragma once
#include <stdint.h>
#include "iconos/wifi1.h"
#include "iconos/wifi2.h"
#include "iconos/wifi3.h"
#include "iconos/wifi4.h"
#include "iconos/celu.h"
#include "iconos/ddbb1.h"
#include "iconos/ddbb2.h"
#include "iconos/nowifi.h"

typedef struct
{
    uint8_t width;
    uint8_t height;
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

//------------------------------------------------------------
// todos los iconos deben tener estas dimensiones.
#define GIMP_IMAGE_WIDTH  (18)
#define GIMP_IMAGE_HEIGHT (16)
// esto define la variable a usar:
#define GIMP_IMAGE(nombre)        GimpImage_t nombre = {.width = GIMP_IMAGE_WIDTH, .height = GIMP_IMAGE_HEIGHT, .pixel_data = (uint16_t*)nombre##_gimp_image_data};
#define GIMP_IMAGE_EXTERN(nombre) extern GimpImage_t nombre;
//------------------------------------------------------------

GIMP_IMAGE_EXTERN(wifi1);  // esto define y usa una variable "GimpImage_t wifi1;"
GIMP_IMAGE_EXTERN(wifi2);
GIMP_IMAGE_EXTERN(wifi3);
GIMP_IMAGE_EXTERN(wifi4);
GIMP_IMAGE_EXTERN(celu);
GIMP_IMAGE_EXTERN(ddbb2);
GIMP_IMAGE_EXTERN(nowifi);
