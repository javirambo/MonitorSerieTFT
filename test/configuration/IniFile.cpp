/*
    IniFile.cpp
    Lectura de configuraciones desde un archivo en el FS.

    Usar asi:
    IniFile doc("archivo.ini");
    int i = doc.getNumber("clave",300);
    float f = doc.getNumber("clave",300.5);    

    JJTeam - 2021
*/

#include "IniFile.h"
#include <SPI.h>
#include <SPIFFS.h>

IniFile::IniFile(const char *name)
{
    error = !SPIFFS.begin();
    f = SPIFFS.open(name, FILE_READ);
    if (!error)
        error = (!f || f.size() == 0);
}

IniFile::~IniFile()
{
    f.close();
    SPIFFS.end();
}

int IniFile::getInt(const char *key, int defValue)
{
    return (int)getFloat(key, defValue);
}

float IniFile::getFloat(const char *key, float defValue)
{
    if (error)
        return defValue;
    size_t filesize = f.size();
    f.seek(0);
    while (f.position() < filesize)
    {
        linea = f.readStringUntil('\n');

        if (linea.length() < 2)
            continue; // salteo lineas en blanco

        if (linea[0] == ';' || linea[0] == '[')
            continue; // salteo comentarios (;) o nombres [...]

        int i = linea.indexOf('=');

        if (i < 0)
            continue; // linea erronea

        if (!linea[i + 1])
            continue; // sin valor!

        if (linea.substring(0, i).equals(key))
            return linea.substring(i + 1).toFloat(); // encontrado!
    }
    return defValue;
}
