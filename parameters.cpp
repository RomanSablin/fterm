#include "parameters.h"

#include <QDateTime>

#include "vt100.h"

QColor CParameters::GetColor(const int _color_id)
{
    QColor color;
    color.setRed(0);
    color.setGreen(0);
    color.setBlue(0);

    switch (_color_id) {
    case CParameters::COLOR_RED: // Red
        color.setRgb(0xff0000);
        break;
    case CParameters::COLOR_GREEN: // Green
        color.setRgb(0x158210);
        break;
    case CParameters::COLOR_YELLOW: // Orange
        color.setRgb(0xF0A000);
        break;
    case CParameters::COLOR_BLUE: // Blue
        color.setRgb(0x0000ff);
        break;
    case CParameters::COLOR_MAGENTA: // Magenta
        color.setRgb(0xff00ff);
        break;
    case CParameters::COLOR_CYAN: // Cyan
        color.setRgb(0x00ffff);
        break;
    case CParameters::COLOR_GRAY: // Gray
        color.setRgb(0x808080);
        break;
    case CParameters::COLOR_BROWN: // Brown
        color.setRgb(0xA52A2A);
        break;
    case CParameters::COLOR_WHITE: // White Smoke
        color.setRgb(0xf5f5f5);
        break;
    case CParameters::COLOR_LIGHTBLUE:
        color.setRgb(0xadd8e6);
        break;
    case CParameters::COLOR_LIGHTGREEN:
        color.setRgb(0x90EE90);
        break;
    case CParameters::COLOR_LIGHTRED: // Red orange
        color.setRgb(0xff4500);
        break;
    case CParameters::COLOR_LIGHTCYAN: // DarkCyan
        color.setRgb(0x008B8B);
        break;
    case CParameters::COLOR_LIGHTMAGENTA: // Dark magenta
        color.setRgb(0x8b008b);
        break;
    case CParameters::COLOR_DARKGRAY:
        color.setRgb(0xA9A9A9);
        break;
    default:
        color.setRed(47);
        color.setGreen(79);
        color.setBlue(79);
        break;
    }

    return color;
}

QColor CParameters::GetColor(const QByteArray _array)
{
    QString str1(_array);
    ColorIdT color_id;
    if(str1.contains("[0;30m")) {
        color_id = CParameters::COLOR_BLACK;
    }
    else if(str1.contains("[0;31m")) {
        color_id = CParameters::COLOR_RED;
    }
    else if(str1.contains("[0;32m")) {
        color_id = CParameters::COLOR_GREEN;
    }
    else if(str1.contains("[0;33m")) {
        color_id = CParameters::COLOR_YELLOW;
    }
    else if(str1.contains("[0;34m")) {
        color_id = CParameters::COLOR_BLUE;
    }
    else if(str1.contains("[0;35m")) {
        color_id = CParameters::COLOR_MAGENTA;
    }
    else if(str1.contains("[0;36m")) {
        color_id = CParameters::COLOR_CYAN;
    }
    else if(str1.contains("[0;37m")) {
        color_id = CParameters::COLOR_GRAY;
    }
    else if(str1.contains("[0;37m")) {
        color_id = CParameters::COLOR_DARKGRAY;
    }
    else if(strncmp(str1.toUtf8(), CL_LIGHTRED, 6) == 0) {
        color_id = CParameters::COLOR_LIGHTRED;
    }
    else if(strncmp(str1.toUtf8(), CL_LIGHTGREEN, 6) == 0) {
        color_id = CParameters::COLOR_LIGHTGREEN;
    }
    else if(strncmp(str1.toUtf8(), CL_YELLOW, 6) == 0) {
        color_id = CParameters::COLOR_YELLOW;
    }
    else if(strncmp(str1.toUtf8(), CL_LIGHTBLUE, 6) == 0) {
        color_id = CParameters::COLOR_LIGHTBLUE;
    }
    else if(strncmp(str1.toUtf8(), CL_LIGHTMAGENTA, 6) == 0) {
        color_id = CParameters::COLOR_LIGHTMAGENTA;
    }
    else if(strncmp(str1.toUtf8(), CL_LIGHTCYAN, 6) == 0) {
        color_id = CParameters::COLOR_LIGHTCYAN;
    }
    else if(strncmp(str1.toUtf8(), CL_WHITE, 6) == 0) {
        color_id = CParameters::COLOR_WHITE;
    }
    else
    {
        color_id = CParameters::COLOR_DEFAULT;
    }

    return GetColor (color_id);
}
