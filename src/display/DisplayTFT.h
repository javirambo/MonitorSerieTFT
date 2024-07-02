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
#include <vector>
#include "GimpImage.h"
// #include "Tjpg/TJpg_Decoder.h"

// struct GIMPImage
// {
//     unsigned int width;
//     unsigned int height;
//     const char *data;
// };

// #define HEADER_PIXEL(data, pixel)                                           \
//     {                                                                       \
//         pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4));         \
//         pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
//         pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33)));      \
//         data += 4;                                                          \
//     }

class ImageItem
{
   public:
    uint16_t id;
    int32_t x;
    int32_t y;
    uint16_t width;
    uint16_t height;
    void *data;
    size_t jpgSize;  // para bmps = -1
    ImageItem() {}
    ImageItem(uint16_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *bmp)  // BMPs
        : id(id), x(x), y(y), width(w), height(h), data((void *)bmp), jpgSize(-1)
    {
    }
    ImageItem(uint16_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *jpgData, size_t jpgSize)  // JPGs
        : id(id), x(x), y(y), width(w), height(h), data((void *)jpgData), jpgSize(jpgSize)
    {
    }
};

class TextItem
{
   public:
    uint16_t id;
    int32_t x;
    int32_t y;
    GFXfont *font;
    uint8_t datum;
    uint16_t h;  // tamaño del rect para borrarlo
    uint16_t w;
    TextItem() {}
    TextItem(uint16_t id, uint16_t x, uint16_t y, GFXfont *f, uint8_t datum) : id(id), x(x), y(y), font(f), datum(datum), h(0), w(0) {}
};

class Display
{
   private:
    bool tftInitialized = false;  // por si no se conecta el display
    uint16_t backgroundColor;
    std::vector<ImageItem> imageList;
    std::vector<TextItem> textList;
    // solo para modo log en pantalla completa
    int32_t logLine;
    GFXfont *logFont;

    void eraseText(TextItem it);

   public:
    // Use hardware SPI
    TFT_eSPI tft = TFT_eSPI();
    bool isOk;

    // de alguna forma tengo que determinar si existe el display...
    bool isWorking();

    // construct only with the background color
    bool begin();

    void clear(uint16_t color);

    // setup text line printing...
    void clearLog(const GFXfont *font, uint16_t fgcolor, uint16_t bgcolor);

    // print lines of text, una debajo de otra.
    void printLog(String txt, const GFXfont *font = NULL);

    // add a text, but only a few parameters (the text will set with show)
    void addText(uint16_t id, int32_t x, int32_t y, const GFXfont *font, uint8_t datum);

    // add  **BMP** image
    void addImage(uint16_t id, int32_t x, int32_t y, uint16_t w, uint16_t h, const uint16_t *bmpData);

    // add **JPG** image
    void addImage(uint16_t id, int32_t x, int32_t y, const uint8_t *jpgData, size_t jpgSize);

    // just print the text
    void print(uint16_t id, uint16_t color, const char *text);

    // show text by id and format
    void printf(uint16_t id, uint16_t color, const char *format, ...);

    // show/hide image by id
    void showImage(uint16_t id, bool show);

    // Si x o y son -1 se centra la imagen.
    void DrawJpeg(int32_t x, int32_t y, const uint8_t *jpg, size_t size);

    void showGIMPImage(int32_t x, int32_t y, const GIMP_image_t *image);
};

//-- unica instancia para todo el proyecto...
extern Display display;

#endif  // _display_tft_h