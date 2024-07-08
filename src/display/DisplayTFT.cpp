/*
  DisplayTFT.cpp
  Permite utilizar un display TFT ILI9341
  y acomodar los objetos en pantalla.
  Específicamente para el proyecto PIG GUARD de Inventu.

  ----------------------------------------------------
  IMPORTANTE:
  Quitar de User_Setup.h todos los define y colocarlos en playformio.ini

  Estos son los pines a usar:
    -DTFT_MOSI=23
    -DTFT_MISO=19
    -DTFT_CS=26
    -DTFT_SCLK=18
    -DTFT_DC=4
    -DTFT_RST=-1
    -DSPI_FREQUENCY=40000000
    -DSPI_READ_FREQUENCY=20000000
  ----------------------------------------------------

  JJTeam 2021
*/

#include "DisplayTFT.h"
#include "utiles/debug.h"
#include "utiles/SplitString.h"
#include <vector>

// construct only with the background color
void Display::init()
{
    tftInitialized = true;
    tft.begin();
    tft.setRotation(1);
    tft.setSwapBytes(true);
    isOk = isWorking();
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(TL_DATUM);  // Set text datum to Top Left
    clearScreen(TFT_BLACK);
}

// de alguna forma tengo que determinar si existe el display...
bool Display::isWorking()
{
    for (int z = 0; z < 5; z++)
    {
        for (int x = 0; x < 20; x++)
            for (int y = 0; y < 15; y++) tft.fillRect(x * 16, y * 16, 16, 16, tft.color565(random(0xff), random(0xff), random(0xff)));
        delay(100);
    }
    tft.drawPixel(1, 1, TFT_RED);
    uint16_t pix = tft.readPixel(1, 1);
    return tftInitialized && pix == TFT_RED;
}

// borra toda la pantalla (incluida la status bar).
void Display::clearScreen(uint16_t color)
{
    if (!tftInitialized) return;

    // status bar:
    tft.fillRect(0, 0, tft.width(), altoStatusBar, TFT_BLACK);
    // texto:
    bgColor = color;
    tft.fillRect(0, altoStatusBar, tft.width(), tft.height() - altoStatusBar, bgColor);

    logs.clear();
    colores.clear();
}

void Display::addLinesToLog(uint16_t color, String &txt)
{
    // si la linea tiene mas de 52 caracteres la divido en varias lineas.
    // (porque no entran mas de 52 en el display)
    char buf[55];
    char *s = buf;
    char *p = (char *)txt.c_str();
    int l   = 0;
    while ((*s = *p))
    {
        if (*s == '\t') *s = ' ';
        s++;
        p++;
        l++;
        if (l >= 52)  // corto la linea y la agrego al log.
        {
            *s = 0;
            logs.push_back(buf);
            colores.push_back(color);
            l = 0;
            s = buf;
        }
    }
    logs.push_back(buf);  // agrego la ultima (o unica) linea de log.
    colores.push_back(color);
}

void Display::print(uint16_t color, String txt)
{
    if (!tftInitialized) return;

    addLinesToLog(color, txt);

    // y dejo solo las ultimas lineas que puedo mostrar.
    if (logs.size() > cantidadLineasDeLog)
    {
        logs.erase(logs.begin(), logs.begin() + logs.size() - cantidadLineasDeLog);
        colores.erase(colores.begin(), colores.begin() + colores.size() - cantidadLineasDeLog);
    }

    // borro toda el area a escribir.
    tft.fillRect(0, altoStatusBar, tft.width(), tft.height() - altoStatusBar, bgColor);

    // muestro todas las lineas posibles:
    int y = altoStatusBar + 1;
    for (int i = 0; i < logs.size(); i++, y += altoFont)
    {
        tft.setTextColor(colores[i]);
        tft.drawString(logs[i], 0, y, 1);
        // otra forma
        // tft.setCursor(0, y);
        // tft.print(logs[i]);
    }
}

void Display::showGIMPImage(int x1, int y1, const GimpImage_t *image)
{
    if (!tftInitialized) return;

    // si la imagen es < a 1/3 de la pantalla, la muestro asi nomas:
    // if (image->width * image->height * 2 < sizeof(bufferImg))
    {
        // int offset;
        // for (int y = 0; y < image->height; y++)
        // {
        //     for (int x = 0; x < image->width; x++)
        //     {
        //         offset            = y * image->width + x;
        //         bufferImg[offset] = image->pixel_data[offset];
        //     }
        // }
        tft.setAddrWindow(x1, y1, image->width, image->height);
        // tft.pushColors(bufferImg, image->size, true);
        tft.pushColors(image->pixel_data, image->size, true);
    }

    // sino la tengo que paginar:
    // else
    // {
    //     int alto = image->height / 3;
    //     for (int top = 0; top < image->height; top += alto)
    //     {
    //         // for (int y = 0; y < alto; y++)
    //         // {
    //         //     for (int x = 0; x < image->width; x++)
    //         //     {
    //         //         bufferImg[y * image->width + x] = image->pixel_data[(y + top) * image->width + x];
    //         //     }
    //         // }
    //         tft.setAddrWindow(x1, y1 + top, image->width, alto);
    //         // tft.pushColors(bufferImg, image->width * alto, true);
    //         tft.pushColors(image->pixel_data + (top * image->width), image->width * alto, true);
    //     }
    // }
}

//-- unica instancia para todo el proyecto...
Display display;
