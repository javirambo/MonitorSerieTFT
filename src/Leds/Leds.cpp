#include "Leds/Leds.h"
#include "utiles/debug.h"

CRGB *leds             = nullptr;
bool EsperaDeEncendido = false;  // hay un efecto que bloquea los efectos y espera un tiempo para encenderlos
uint32_t TimeoutEspera = 0;      // "

void LedsInit()
{
    static bool inicializado = false;
    if (!inicializado)
    {
        leds = (CRGB *)malloc(sizeof(CRGB) * RGB_LEDS_QUANTITY);
        CFastLED::addLeds<WS2812B, PIN_LEDS_RGB, GRB>(leds, RGB_LEDS_QUANTITY);
        AllLeds(CRGB::Black);
        inicializado = true;
    }
    EsperaDeEncendido = false;
    TimeoutEspera     = 0;
}

// es el loop del Arduino que controla el tiempo fuera para el 'EsperaDeEncendido'
void LoopLeds()
{
    static uint32_t T = 0;
    if (millis() < T) return;
    T = millis() + 1000;
    if (TimeoutEspera == 0)
        EsperaDeEncendido = false;
    else
        TimeoutEspera--;
}

void LedsTemporizadosOFF()
{
    TimeoutEspera     = 0;
    EsperaDeEncendido = false;
}

void NextLed(CRGB col, bool dejarCola /*= true*/)
{
    if (EsperaDeEncendido) return;

    static int led      = 0;
    static bool sentido = true;
    if (!dejarCola) leds[led] = CRGB::Black;

    if (sentido)
    {
        if (++led == RGB_LEDS_QUANTITY - 1) sentido = false;
    }
    else
    {
        if (--led == 0) sentido = true;
    }

    if (dejarCola) fadeToBlackBy(leds, RGB_LEDS_QUANTITY, 140);
    leds[led] = col;
    FastLED.show();
}

void AllLeds(CRGB col, uint8_t brillo)
{
    if (EsperaDeEncendido) return;
    fill_solid(leds, RGB_LEDS_QUANTITY, col);
    if (brillo < 255) fadeLightBy(leds, RGB_LEDS_QUANTITY, brillo);
    FastLED.show();
}

void AllLedsTemporizados(CRGB col, int segundos)
{
    EsperaDeEncendido = true;
    TimeoutEspera     = (segundos == 0) ? 1 : segundos;
    fill_solid(leds, RGB_LEDS_QUANTITY, col);
    FastLED.show();
}

void LedBlink(int ledNum, CRGB col)
{
    leds[ledNum] = col;
    FastLED.setBrightness(255);
    FastLED.show();
    delay(50);
}

// enciende un LED y apaga el resto
void LedON(int ledNum, CRGB colorON, CRGB colorOFF)
{
    fill_solid(leds, RGB_LEDS_QUANTITY, colorOFF);
    leds[ledNum] = colorON;
    FastLED.setBrightness(255);
    FastLED.show();
}

// enciende un led y no toca el resto.
void LedON(int ledNum, CRGB colorON)
{
    leds[ledNum] = colorON;
    FastLED.setBrightness(255);
    FastLED.show();
}

void FlashGordon(CRGB col, bool alverre)
{
    FastLED.setBrightness(255);

    for (int i = 0; i < RGB_LEDS_QUANTITY; ++i)
    {
        memset(leds, 0, RGB_LEDS_QUANTITY);

        if (alverre)
            leds[RGB_LEDS_QUANTITY - i - 1] = col;
        else
            leds[i] = col;

        FastLED.show();
        delay(4);
    }
}

// Espera que se pulse el botón, y espera que esté pulsado durante 5 segundos, sino abort!
bool VerificaBotonDeFabrica()
{
    bool pulsado;
    bool toggle        = false;
    const char *frase  = "\rSI DEJA PULSADO MAS DE 5 SEGUNDOS ESTE BOTON INGRESARA AL PORTAL DE CONFIGURACION....";
    const int lenFrase = strlen(frase);
    int indexFrase     = 0;
    int tiempo         = 0;
    bool ok            = false;

    pinMode(GPIO_NUM_0, INPUT_PULLUP);

    SetTerminalColor(TERM_MAGENTA, TERM_ITALICA);
    printf("Pulse BOOT durante 4 segundos para volver a valores de fabrica:\n");
    fflush(stdout);
    while (tiempo < 250)
    {
        pulsado = !digitalRead(GPIO_NUM_0);

        if (pulsado)
        {
            NextLed(blend(CRGB::Red, CRGB::Magenta, indexFrase), false);
            delay(lenFrase - indexFrase);
        }
        else
            NextLed(CRGB::Blue, false);

        EVERY_N_MILLISECONDS(50)
        {
            if (pulsado)
            {
                toggle = true;
                printf("%c", frase[indexFrase++]);
                fflush(stdout);
            }
            else
            {
                if (toggle)
                    printf("\n");
                else
                    printf(".");
                fflush(stdout);
                indexFrase = 0;  // si no se pulsa se resetea el indexFrase
                toggle     = false;
            }

            // si se pulsa durante 4 segundos chau.
            if (indexFrase > lenFrase)
            {
                SetTerminalColor(TERM_AMARILLO);
                printf("!!!");
                ok = true;  // PULSADO!
                goto finLeerBoton;
            }
            tiempo++;
        }
    }
finLeerBoton:
    SetTerminalColor(TERM_RESET);
    printf("\n");  // NO-PULSADO!
    fflush(stdout);
    AllLeds(CRGB::Black);  // apago el 🌈
    return ok;
}

// Knight Industries Two Thousand
void LedsKitt()
{
    for (int i = 0; i < 12; ++i)
    {
        if (i < RGB_LEDS_QUANTITY) leds[i] = CRGB::Red;
        fadeToBlackBy(leds, RGB_LEDS_QUANTITY, 90);
        FastLED.show();
        FastLED.delay(15);
    }
    for (int i = 0; i < 16; ++i)
    {
        if (i < RGB_LEDS_QUANTITY) leds[RGB_LEDS_QUANTITY - i - 1] = CRGB::Red;
        fadeToBlackBy(leds, RGB_LEDS_QUANTITY, 90);
        FastLED.show();
        FastLED.delay(15);
    }
    AllLeds(CRGB::Black);
}
