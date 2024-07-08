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
#include <vector>

// #define ROB14  &Roboto_Thin_14
// #define DSEG32 &DSEG7_Classic_Regular_32
// #define DSEG50 &DSEG7_Classic_Regular_50

class Display
{
   private:
    bool tftInitialized = false;  // por si no se conecta el display
    uint16_t bgColor    = TFT_BLACK;
    uint16_t textColor  = TFT_WHITE;
    std::vector<String> logs;
    std::vector<uint16_t> colores;

    //  TFT_WIDTH  320 x  TFT_HEIGHT 240

   public:
    const int altoStatusBar       = 18;
    const int altoFont            = 8;
    const int cantidadLineasDeLog = 27;

    TFT_eSPI tft = TFT_eSPI();  // Use hardware SPI
    bool isOk    = false;

    void init();
    bool isWorking();
    void clearScreen(uint16_t color);
    void print(uint16_t color, String txt);
    void showGIMPImage(int x, int y, const GimpImage_t *image);
};

//-- unica instancia para todo el proyecto...
extern Display display;

#endif  // _display_tft_h