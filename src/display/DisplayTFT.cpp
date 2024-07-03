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

// de alguna forma tengo que determinar si existe el display...
bool Display::isWorking()
{
    for (int32_t x = 0; x < 20; x++)
        for (int32_t y = 0; y < 15; y++) tft.fillRect(x * 16, y * 16, 16, 16, tft.color565(random(0xff), random(0xff), random(0xff)));
    delay(250);

    tft.drawPixel(1, 1, TFT_RED);
    uint16_t pix = tft.readPixel(1, 1);
    return tftInitialized && pix == TFT_RED;
}

// construct only with the background color
bool Display::begin()
{
    if (tftInitialized) return isOk;
    tftInitialized = true;
    tft.begin();
    tft.setRotation(1);
    tft.setSwapBytes(true);
    isOk = isWorking();
    clearScreen(TFT_BLACK);
    return isOk;
}

// borra toda la pantalla (incluida la status bar).
void Display::clearScreen(uint16_t color)
{
    bgColor = color;
    tft.fillScreen(bgColor);
    tft.setTextDatum(TL_DATUM);  // Set text datum to Top Left
}

// borra el area de texto (no el status bar) y setea el color y el font.
void Display::clearText(uint16_t textcolor, uint16_t bgcolor)
{
    yTextPos = 0;
    // logFont     = (GFXfont *)font;
    // bgColor   = bgcolor;
    textColor = textcolor;
    // tft.fillRect(0, altoStatusBar, tft.width(), tft.height(), bgColor);
    // tft.setTextDatum(TL_DATUM);
    // tft.setFreeFont(logFont);

    tft.setTextColor(textColor, bgColor, true);
    // tft.setTextColor(textColor, bgColor); // ver ......

    // tft.setViewport(0, altoStatusBar, tft.width(), tft.height() - altoStatusBar, TL_DATUM);
    clearScreen(bgcolor);
}

void Display::clearText() { clearText(textColor, bgColor); }

void Display::print(uint8_t x, uint8_t y, String txt)
{
    auto lineas = SplitString(txt, '\n');
    if (lineas.size() == 1)
    {
        tft.drawString(txt, x, y, 1);  // no modifico la linea. a no ser que tenga ENTERS.
    }
    else if (lineas.size() > 1)
    {
        // tiene ENTERS (varias lineas)
        yTextPos = y;
        for (int i = 0; i < lineas.size(); i++)
        {
            tft.drawString(lineas[i], x, yTextPos, 1);
            // x = 0;  // todas las siguientes lineas comienzan contra el margen 0.
            yTextPos += altoFont;
        }
    }
}

void Display::print(String txt)
{
    print(0, yTextPos, txt);
    yTextPos += altoFont;  // queda apuntando a la sig linea.
}

void Display::showGIMPImage(uint8_t x1, uint8_t y1, const GimpImage_t *image)
{
    tft.startWrite();
    for (int y = 0; y < image->height; y++)     //
        for (int x = 0; x < image->width; x++)  //
            tft.drawPixel(x + x1, y + y1, image->pixel_data[y * image->width + x]);
    tft.endWrite();
}

//-- unica instancia para todo el proyecto...
Display display;
