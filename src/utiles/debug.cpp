#include <Arduino.h>
#include <stdarg.h>
#include "Leds/Leds.h"
#include "debug.h"
#include "Reloj/Reloj.h"

// imprime en stdout los codigos del color (ejemplo: "\033[1;m")
void SetTerminalColor(uint8_t colorFG, uint8_t flags, uint8_t colorBG)
{
    String str = "\033["; 
    if (colorFG != 0xFF)
    {
        if (flags == 0) str += '0';
        if (flags & TERM_NEGRITA) str += '1';
        if (flags & TERM_DIMER) str += '2';
        if (flags & TERM_ITALICA) str += '3';
        if (flags & TERM_SUBRAYADO) str += '4';
        if (flags & TERM_INVERTIDO) str += '7';
        if (flags & TERM_TACHADO) str += '9';
        if (flags & TERM_SUBRAYADO_DOBLE) str += "21";
        str += ';';

        if (colorFG & TERM_BRILLANTE)
            colorFG = (colorFG - 128) + 90;  // tiene brillo
        else
            colorFG += 30;  // color normal
        str += String(colorFG);

        if (colorBG)
        {
            if (colorBG & TERM_BRILLANTE)
                colorBG = (colorBG - 128) + 100;  // tiene brillo
            else
                colorBG += 40;  // fondo normal
            str += ';';
            str += String(colorBG);
        }
        str += 'm';
    }
    else
        str += "0m";
    printf("%s", str.c_str());
}

void LogTitled(const String& titulo, const String& texto)
{
    COLOR_CELESTE(0);
    printf("%s", titulo.c_str());
    COLOR_WARN(TERM_ITALICA);
    printf("%s", texto.c_str());
    NO_COLOR();
    printf("\n");
}

void LogTrace(const char* file, int line, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    static uint32_t TraceID = 0;
    COLOR_CELESTE(TERM_ITALICA);
    printf("~%d~ %s:%d ", ++TraceID, file, line);
    COLOR_ERROR(TERM_DIMER);

    char buf[1000];
    vsprintf(buf, format, args);
    printf("%s\n", buf);
    NO_COLOR();

    va_end(args);
}

void LogBuffer(uint8_t* buf, size_t size)
{
    COLOR_CELESTE(0);
    printf("[ ");
    for (size_t i = 0; i < size; i++) printf("%02X ", buf[i]);
    printf("]\n");
    NO_COLOR();
}

void Log(char level, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    bool mostrar         = false;
    uint8_t addTimestamp = ModoDiagnostico ? TERM_DIMER | TERM_ITALICA : 0;

    if (ModoDiagnostico && level == 'D')
    {
        mostrar = true;
        COLOR_DEBUG(addTimestamp);
    }
    else if (level == 'I')
    {
        mostrar = true;
        COLOR_INFO(addTimestamp);
    }
    else if (level == 'W')
    {
        mostrar = true;
        COLOR_WARN(addTimestamp);
    }
    else if (level == 'E')
    {
        mostrar = true;
        COLOR_ERROR(addTimestamp);
    }
    else if (level == 'X')
    {
        mostrar = true;
        COLOR_LILA(addTimestamp);
    }
    else if (ModoDiagnostico)  // cualquier otra letra, pero solo en MD
    {
        mostrar = true;
        COLOR_CELESTE(addTimestamp);
    }

    if (mostrar)
    {
        char buf[1000];
        int len = vsprintf(buf, format, args);
        if (addTimestamp) printf("%s", Reloj.GetFecha4Log());
        printf("%s", buf);
        NO_COLOR();
        printf("\n");
        if (len >= 1000) printf("\n\n**** Longitud de texto demasiado grande ****\n\n");
    }

    va_end(args);
}

bool ModoDiagnostico = false;  // true => con el boton de BOOT

// MODO DIAGNOSTICO: Si aprieto el BOOT en cualquier momento, cambia el Modo.
void VerificarBotonDeDebug(CallbackBotonDebug callback)
{
    static bool toggle = false;

    // no necesito que sea tan rapido el polling
    EVERY_N_MILLISECONDS(25)
    {
        if (!gpio_get_level(GPIO_NUM_0))
        {
            if (!toggle)
            {
                ModoDiagnostico = !ModoDiagnostico;

                if (ModoDiagnostico)
                    LogD("Modo Diagnostico ON");
                else
                    LogW("Modo Diagnostico OFF");

                FlashGordon(CRGB::White, false);  // Test de Leds con Semaforo

                toggle = true;
                FastLED.setBrightness(ModoDiagnostico ? 20 : 100);  // el brillo indica si está o no en MD.
                FastLED.show();
                callback();
            }
        }
        else
            toggle = false;
    }
}
