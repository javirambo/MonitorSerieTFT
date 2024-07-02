
#ifndef _Fs_Buffer_h
#define _Fs_Buffer_h

#include <Print.h>
#include "FS.h"

//----------------------------------------------------------------------------

typedef void (*ForEachLineCallback)(String line);

class FsBuffer : public Print
{
private:
    uint8_t pinCSmicroSD;    // pin de CS de la micro-SD
    uint32_t maxFileSize;    // tamaño maximo de cada archivo
    uint8_t filesCount;      // cantidad de archivos que usaremos
    uint8_t fileIndexActual; // archivo que se esta escribiendo
    String bufFileName;      // filename
    String cfgFileName;      // configuraciones
    String basePath;         // permite que co-existan varios FsBuffers
    void initFile();         // crea el archivo a usar
    void storeConfig();      // guarda cosas en flash (por si un reset)
    void nextFile();         // Cambio de archivo. Voy al siguiente circularmente.
    String getFileName(int index);

protected:
    bool initialized = false;
    bool microSDExists = false;  // se grabará en SD si está disponible, sino usa la flash solo para ERROR.
    bool fileSystemError = true; // true si no puedo grabar en SD ni flash!
    File open(const String &path, const char *mode);
    void mkdir(const String &folder);
    void remove(const String &path);
    void printFromFile(String filename, Print &printer);
    void forEachLineFromFile(String filename, ForEachLineCallback callback);

public:
    void begin(int pin_CS_microSD, uint32_t bytesPerFile, uint8_t filesQuantity, String &basePath);
    size_t write(uint8_t c);
    size_t write(const uint8_t *txt);
    size_t write(const uint8_t *txt, size_t len);
    void printTo(Print &printer);
    void forEachLine(ForEachLineCallback callback);
    void clear();
};

//----------------------------------------------------------------------------
#endif // Fs_Buffer_h