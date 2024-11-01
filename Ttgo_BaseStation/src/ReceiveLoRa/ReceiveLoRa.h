#ifndef RECEIVELORA_H
#define RECEIVELORA_H

#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>

typedef struct Point {
    byte id;
    double lat, lng;
    byte day, month, year;
    byte hour, minutes, seconds;
} Point;

class ReceiveLoRa {
public:
    ReceiveLoRa(int sckPin, int misoPin, int mosiPin, int ssPin, int rstPin, int dio0Pin, long frequency);
    void begin();
    bool receiveData(Point &position);

private:
    bool parseString(const char *str, Point &position);

    int _sckPin;
    int _misoPin;
    int _mosiPin;
    int _ssPin;
    int _rstPin;
    int _dio0Pin;
    long _frequency;
};

#endif
