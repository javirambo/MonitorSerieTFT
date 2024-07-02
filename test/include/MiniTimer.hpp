/**
 * Es una mini clase de timer, para crear varios timers con duty cicle asimétrico.
 * 
 * Ejemplo de uso:
 * 
    void Timer1(bool edge)
    {
        //edge sera true en el flanco positivo
    }
    
    void Timer2(bool edge)
    {        
    }

    MiniTimer mTimer(2);

    void setup()
    {
        mTimer.add(Timer1, 1000, 5000);
        mTimer.add(Timer2, 200, 900);
    }

    void loop()
    {
        mTimer.run();
    }

 *
 * JJTeam - 2021
 */

#include <Arduino.h>

typedef void (*MiniTimerAction)(bool edge);

typedef struct
{
    MiniTimerAction action;
    uint32_t tsUp; // flanco de subida
    uint32_t tsDw; // flanco de bajada
    uint32_t ts;
    bool edge;
    bool enable;
} MiniTimerItem;

/**
 * Tiene una resolucion de Microsegundos (pero no tan pocos....)
 */
class MiniTimer
{
public:
    typedef enum
    {
        TIME_MS,
        TIME_US
    } TIME_UNIT;
    MiniTimerItem *actions;
    uint8_t count;
    uint8_t TotalItems;

    MiniTimer(size_t capacity)
    {
        actions = new MiniTimerItem[capacity];
        TotalItems = capacity;
        count = 0;
    }

    // es una timer para crear en el setup...y no creo que se destruya nunca...
    ~MiniTimer()
    {
        delete[] actions;
    }

    /**
     * Agrega un timer asimétrico (no te excedas en la capacidad inicial!)
     */
    void add(MiniTimerAction action, uint32_t tsUp, uint32_t tsDw, TIME_UNIT unit = TIME_MS)
    {
        if (count < TotalItems)
        {
            actions[count].action = action;
            actions[count].tsUp = (unit == TIME_MS ? tsUp * 1000 : tsUp);
            actions[count].tsDw = (unit == TIME_MS ? tsDw * 1000 : tsDw);
            actions[count].ts = micros();
            actions[count].edge = true;
            actions[count].enable = true;
            ++count;
        }
    }

    /**
     * Agrega un timer simetrico: up+down = period
     */
    void add(MiniTimerAction action, uint32_t period, TIME_UNIT unit = TIME_MS)
    {
        add(action, period, period, unit);
    }

    // solo para debug (necesito apagar algo que me jode...)
    void disable(MiniTimerAction a)
    {
        for (size_t i = 0; i < count; i++)
            if (actions[i].action == a)
                actions[i].enable = false;
    }

    void run()
    {
        for (size_t i = 0; i < count; i++)
        {
            if (!actions[i].enable)
                continue;

            if (actions[i].edge)
            {
                if (micros() - actions[i].ts > actions[i].tsUp)
                {
                    actions[i].ts = micros();
                    actions[i].edge = false;
                    if (!actions[i].tsUp) // si vale 0 no llamo al action
                        actions[i].action(true);
                }
            }
            else
            {
                if (micros() - actions[i].ts > actions[i].tsDw)
                {
                    actions[i].ts = micros();
                    actions[i].edge = true;
                    if (!actions[i].tsDw) // si vale 0 no llamo al action
                        actions[i].action(false);
                }
            }
        }
    }
};
