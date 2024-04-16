#ifndef _TYPES_
#define _TYPES_
#include <Arduino.h>
enum SHAPE_DISPLAY_FLAG
{
    NORMAL = 0,
    DELAY = (1 << 0),
    LINE = (1 << 1),
};

enum COLOR_DISPLAY_FLAG
{
    COLOR_1 = (1 << 0),
    COLOR_2 = (1 << 1),
    COLOR_3 = (1 << 2),
    STATIC = (1 << 3),
    FADE = (1 << 4),
};

enum BUTTON_MODE
{
    DISPLAY_ADJUSTMENT,
    AUDIO_FACTOR_ADJUSTMENT,
    BRIGHTNESS_ADJUSTMENT,
    COLOR_ADJUSTMENT,
};

template <class T>
struct LimitedValue
{
    T value;
    T min;
    T max;

    void decrease(T v)
    {
        value -= v;
        if (value < min)
            value = min;
    }
    void increase(T v)
    {
        value += v;
        if (value > max)
            value = max;
    }
};

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    uint32_t toNum()
    {
        uint32_t res;
        res += r;
        res = res << 8;
        res += g;
        res = res << 8;
        res += b;
        return res;
    }

    void fromNum(uint32_t num)
    {
        r = (num >> 16) & 0xFF;
        g = (num >> 8) & 0xFF;
        b = (num >> 0) & 0xFF;
    }
};

#endif