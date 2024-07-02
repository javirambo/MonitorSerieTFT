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

// GLOBAL y la llama cada vez que se quiera dibujar una imagen con el decoder jpeg
bool CallbackJpegDraw(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *b)
{
    extern Display display;
    display.tft.pushImage(x, y, w, h, b);
    return 1;
}

void Display::eraseText(TextItem it)
{
    switch (it.datum)
    {
        case TC_DATUM:
            tft.fillRect(it.x - it.w / 2, it.y, it.w, it.font->yAdvance, backgroundColor);
            break;

        case TR_DATUM:
            tft.fillRect(it.x - it.w, it.y, it.w, it.font->yAdvance, backgroundColor);
            break;

        case TL_DATUM:
            // TODO: calcular el resto de los datum...por ahora son todos TL
        default:
            tft.fillRect(it.x, it.y, it.w, it.font->yAdvance, backgroundColor);
    }
}

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
    clear(TFT_BLACK);

    // siempre que se use el codec jpeg se usaran estos parametros!
    // TJpgDec.setJpgScale(1);
    // TJpgDec.setCallback(CallbackJpegDraw);

    return isOk;
}

void Display::clear(uint16_t color)
{
    backgroundColor = color;
    tft.fillScreen(backgroundColor);
    tft.setTextDatum(TL_DATUM);  // Set text datum to Top Left
}

// setup text line printing...
void Display::clearLog(const GFXfont *font, uint16_t fgcolor, uint16_t bgcolor)
{
    logLine = 0;
    logFont = (GFXfont *)font;
    tft.fillScreen(bgcolor);
    tft.setTextDatum(TL_DATUM);
    tft.setFreeFont(font);
    tft.setTextColor(fgcolor, bgcolor);
}

// print lines of text, una debajo de otra.
void Display::printLog(String txt, const GFXfont *font)
{
    if (font != NULL)
    {
        logFont = (GFXfont *)font;
        tft.setFreeFont(font);
    }
    tft.drawString(txt, 0, logLine, 1);
    logLine += logFont->yAdvance;

    // si el display no anda (o no existe) logueo todo...
    if (!isOk) log_n("%s", txt.c_str());
}

// add a text, but only a few parameters (the text will set with show)
void Display::addText(uint16_t id, int32_t x, int32_t y, const GFXfont *font, uint8_t datum) { textList.push_back(TextItem(id, x, y, (GFXfont *)font, datum)); }

// **BMP**
// add an image, so we can show with refresh
void Display::addImage(uint16_t id, int32_t x, int32_t y, uint16_t w, uint16_t h, const uint16_t *bmpData) { imageList.push_back(ImageItem(id, x, y, w, h, bmpData)); }

// **JPG**
void Display::addImage(uint16_t id, int32_t x, int32_t y, const uint8_t *jpgData, size_t jpgSize)
{
    uint16_t w, h;
    // TJpgDec.getJpgSize(&w, &h, jpgData, jpgSize);
    imageList.push_back(ImageItem(id, x, y, w, h, jpgData, jpgSize));
}

// just print the text
void Display::print(uint16_t id, uint16_t color, const char *text)
{
    // erase old text (el rect depende del datum)
    eraseText(textList[id]);

    tft.setTextDatum(textList[id].datum);
    tft.setFreeFont(textList[id].font);
    tft.setTextColor(color, color == TFT_TRANSPARENT ? TFT_TRANSPARENT : backgroundColor);
    if (text != NULL) textList[id].w = tft.drawString(text, textList[id].x, textList[id].y, 1);
    // si el display no anda (o no existe) logueo todo...
    if (!isOk) LogW("%s", text);
}

// show text by id and format
void Display::printf(uint16_t id, uint16_t color, const char *format, ...)
{
    char text[50];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(text, format, argptr);
    va_end(argptr);
    print(id, color, text);
}

// show/hide image by id
void Display::showImage(uint16_t index, bool show)
{
    // se borra la imagen con un rect negro...
    if (!show)
    {
        tft.fillRect(imageList[index].x, imageList[index].y, imageList[index].width, imageList[index].height, backgroundColor);
        return;
    }

    // se muestra jpg....
    if (imageList[index].jpgSize > 0)
    {
        DrawJpeg(imageList[index].x, imageList[index].y, (const uint8_t *)imageList[index].data, imageList[index].jpgSize);
        return;
    }

    // ... muestro un bmp...
    tft.pushImage(imageList[index].x, imageList[index].y, imageList[index].width, imageList[index].height, (const uint16_t *)imageList[index].data);
}

/**
 * Si x o y son -1 se centra la imagen.
 */
void Display::DrawJpeg(int32_t x, int32_t y, const uint8_t *jpg, size_t size)
{
    uint16_t w, h;
    // TJpgDec.getJpgSize(&w, &h, jpg, size);

    if (x < 0) x = (tft.width() - w) / 2;

    if (y < 0) y = (tft.height() - h) / 2;

    // TJpgDec.drawJpg(x, y, jpg, size, tft.width(), tft.height());
}

void Display::showGIMPImage(int32_t x, int32_t y, const GIMP_image_t *image)
{
    tft.startWrite();
    const uint16_t *data = (uint16_t *)image->pixel_data;
    int size             = image->width * image->height;

        LogI("%d x %d", image->width, image->height);
    for (unsigned int i = 0; i < size; i++)
    {

        // tft.drawPixel(x + (i % image->width), y + (i / image->width), data[i]);
    }
    tft.endWrite();
}

//-- unica instancia para todo el proyecto...
Display display;
