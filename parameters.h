#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>
#include <QString>
#include <QColor>

class CParameters
{
public:
    enum ColorIdT
    {
        COLOR_DEFAULT,
        COLOR_BLACK,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_YELLOW,
        COLOR_BLUE,
        COLOR_MAGENTA,
        COLOR_CYAN,
        COLOR_GRAY,
        COLOR_BROWN,
        COLOR_DARKGRAY,
        COLOR_LIGHTRED,
        COLOR_LIGHTGREEN,
        COLOR_LIGHTBLUE,
        COLOR_LIGHTMAGENTA,
        COLOR_LIGHTCYAN,
        COLOR_WHITE
    };

    static const bool IsUSBFilterEnabled = true;
    static const uint32_t FTDI_VENDOR_ID = 0x0403;

    static QColor GetColor(const QByteArray _array);
    static QColor GetColor(const int _color_id);
};

#endif // PARAMETERS_H
