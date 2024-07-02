#pragma once
#include <Arduino.h>

//----------------------------------------------------------
// -colores-

#define TERM_NEGRO     0x00  // el background es negro, o gris si tiene brillo
#define TERM_ROJO      0x01
#define TERM_VERDE     0x02
#define TERM_AMARILLO  0x03
#define TERM_AZUL      0x04
#define TERM_MAGENTA   0x05
#define TERM_CIAN      0x06
#define TERM_BLANCO    0x07  // el foreground es gris, o blanco si tiene brillo

#define TERM_RESET     0xFF
#define TERM_BRILLANTE 0x80  // ored con el color BG o FG

//----------------------------------------------------------
// -flags-

#define TERM_DIMER           0x02
#define TERM_NEGRITA         0x04
#define TERM_ITALICA         0x08
#define TERM_SUBRAYADO       0x10
#define TERM_SUBRAYADO_DOBLE 0x20
#define TERM_TACHADO         0x40
#define TERM_INVERTIDO       0x80

#define COLOR_ERROR(x)       SetTerminalColor(TERM_ROJO | TERM_BRILLANTE, x)
#define COLOR_WARN(x)        SetTerminalColor(TERM_AMARILLO, x)
#define COLOR_INFO(x)        SetTerminalColor(TERM_VERDE, x)
#define COLOR_DEBUG(x)       SetTerminalColor(TERM_BLANCO, x)
#define COLOR_LILA(x)        SetTerminalColor(TERM_MAGENTA | TERM_BRILLANTE, x)
#define COLOR_CELESTE(x)     SetTerminalColor(TERM_CIAN | TERM_BRILLANTE, x)
#define NO_COLOR()           SetTerminalColor(TERM_RESET)

void SetTerminalColor(uint8_t colorFG, uint8_t flags = 0, uint8_t colorBG = 0);
void LogTitled(const String& titulo, const String& texto);
void LogTrace(const char* file, int line, const char* format, ...);

//----------------------------------------------------------
// -debug-

#define VOID_LOOP() \
    do              \
    {               \
    } while (0)

#define LogE(format, ...) Log('E', format, ##__VA_ARGS__)
#define LogW(format, ...) Log('W', format, ##__VA_ARGS__)
#define LogI(format, ...) Log('I', format, ##__VA_ARGS__)
#define LogX(format, ...) Log('X', format, ##__VA_ARGS__)

#ifndef MOSTRAR_INFO_DE_DEBUG
    #define MOSTRAR_INFO_DE_DEBUG 0
#endif

#if (MOSTRAR_INFO_DE_DEBUG == 1)
    #define LogD(format, ...)  Log('D', format, ##__VA_ARGS__)
    #define TRACE(format, ...) LogTrace(__FILE__, __LINE__, format, ##__VA_ARGS__)
#else
    #define LogD(format, ...)  VOID_LOOP()
    #define TRACE(format, ...) VOID_LOOP()
#endif
#define NOTRACE(format, ...) VOID_LOOP()

void Log(char level, const char* format, ...);
void LogBuffer(uint8_t* buf, size_t size);

//----------------------------------------------------------

typedef void (*CallbackBotonDebug)();
void VerificarBotonDeDebug(CallbackBotonDebug callback);

extern bool ModoDiagnostico;  // se va a true con el boton de BOOT.