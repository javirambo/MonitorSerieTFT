/*
    IniFile.h
    Lectura de configuraciones desde un archivo en el FS.

    Usar asi:
    IniFile doc("archivo.ini");
    int i = doc.getNumber("clave",300);
    float f = doc.getNumber("clave",300.5);    

    JJTeam - 2021
*/

#ifndef _ini_file_h
#define _ini_file_h

#include <FS.h>

class IniFile
{
private:
    File f;
    String linea;

public:
    bool error = false;
    IniFile(const char *name);
    ~IniFile();
    float getFloat(const char *key, float defValue);
    int getInt(const char *key, int defValue);
};

#endif // _ini_file_h