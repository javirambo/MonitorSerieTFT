# Monitor Serie con Display TFT y ESP32

Circuito Usado

![Circuito Usado](docs/Schematic_Monitor-Serie-Tft-Esp32_2024-07-02%20(1).png)


### Display

Debe usarse Platformio para compilar el proyecto.

El archivo *platformio.ini*, sección *lib_deps* hay una librería (*TFT_eSPI*) para manejar el display, que puede traer problemas.

Como es automática en Platformio, y se baja de internet, hay que editar el archivo "User_Setup.h" y comentar las líneas que dan duplicación de define

Las definiciones de pines y modelo de display están en el mismo *platformio.ini*
