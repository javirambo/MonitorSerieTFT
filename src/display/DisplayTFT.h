/*
  DisplayTFT.h
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

#ifndef _display_tft_h
#define _display_tft_h

#include <TFT_eSPI.h>
// #include "fonts/Free_Fonts.h"
// #include "fonts/DSEG7_Classic_Regular_32.h"
// #include "fonts/DSEG7_Classic_Regular_50.h"
// #include "fonts/Roboto_Thin_14.h"
// #include "fonts/Free_Fonts.h"
#include "GimpImage.h"

// #define ROB14  &Roboto_Thin_14
// #define DSEG32 &DSEG7_Classic_Regular_32
// #define DSEG50 &DSEG7_Classic_Regular_50

class Display
{
   private:
    bool tftInitialized = false;  // por si no se conecta el display
    uint16_t bgColor    = TFT_BLACK;
    uint16_t textColor  = TFT_WHITE;
    int32_t yTextPos    = 0;  // ultima linea de log impresa.

    //  TFT_WIDTH  320 x  TFT_HEIGHT 480

   public:
    const uint8_t altoStatusBar       = 16;
    const uint8_t altoFont            = 8;
    const uint8_t cantidadLineasDeLog = 27;  //(TFT_HEIGHT - altoStatusBar) / (altoFont + 2) - 1;  // 29;  // logs que entran en 1 pantalla

    // Use hardware SPI
    TFT_eSPI tft = TFT_eSPI();
    bool isOk    = false;

    Display() {}

    // de alguna forma tengo que determinar si existe el display...
    bool isWorking();

    // construct only with the background color
    bool begin();

    void clearScreen(uint16_t color);

    void clearText(uint16_t fgcolor, uint16_t bgcolor);
    void clearText();

    void print(String txt);

    void print(uint8_t x, uint8_t y, String txt);

    void showGIMPImage(uint8_t x, uint8_t y, const GimpImage_t *image);
};

//-- unica instancia para todo el proyecto...
extern Display display;

#endif  // _display_tft_h