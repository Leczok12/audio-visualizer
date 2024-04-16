#include "LedMatrix.hpp"

LedMatrix::LedMatrix(int pin, unsigned long delay, int &brightness, Color &c1, Color &c2, Color &c3)
    : _strip(49, pin, NEO_GRB + NEO_KHZ800),
      _delay(delay),
      _old_time(0),
      _brightness(brightness),
      _c1(c1),
      _c2(c2),
      _c3(c3),
      _fade_index(0),
      _fade({255, 0, 0})
{
}

LedMatrix::~LedMatrix() {}

void LedMatrix::begin()
{
    for (int x = 0; x < 7; x++)
        for (int y = 0; y < 7; y++)
            _matrix[x][y] = 0;

    _strip.begin();
    _strip.clear();
    _strip.show();
}

void LedMatrix::fadeCycyle()
{
    switch (_fade_index)
    {
    case 0:
        _fade.r -= 5;
        _fade.g += 5;
        _fade.b = 0;
        if (_fade.g >= 255)
        {
            _fade.r = 0;
            _fade.g = 255;
            _fade_index = 1;
        }
        break;
    case 1:
        _fade.r = 0;
        _fade.g -= 5;
        _fade.b += 5;
        if (_fade.b >= 255)
        {
            _fade.g = 0;
            _fade.b = 255;
            _fade_index = 2;
        }
        break;
    case 2:
        _fade.r += 5;
        _fade.g = 0;
        _fade.b -= 5;
        if (_fade.r >= 255)
        {
            _fade.b = 0;
            _fade.r = 255;
            _fade_index = 0;
        }
        break;
    default:
        _fade_index = 0;
        break;
    }
}

void LedMatrix::render(int *values, SHAPE_DISPLAY_FLAG s_flag, COLOR_DISPLAY_FLAG c_flag)
{
    draw(values, s_flag);
    paint(c_flag);
}

void LedMatrix::show()
{
    _strip.clear();
    _strip.setBrightness(Adafruit_NeoPixel::gamma8(_brightness));
    for (int x = 0; x < 7; x++)
    {
        for (int y = 0; y < 7; y++)
        {
            int i = (x * 7) + y;
            _strip.setPixelColor(i, Adafruit_NeoPixel::gamma32(_matrix[x][y]));
        }
    }
    _strip.show();
}

void LedMatrix::off()
{
    _strip.clear();
    _strip.show();
}

void LedMatrix::draw(int *values, SHAPE_DISPLAY_FLAG s_flag)
{
    if (!(s_flag & DELAY))
    {
        for (int x = 0; x < 7; x++)
            for (int y = 0; y < 7; y++)
                _matrix[x][y] = 0;
    }

    for (int x = 0; x < 7; x++)
    {
        for (int y = 0; y < values[x] + 1; y++)
        {
            if (y >= 7)
                break;
            _matrix[x][y] = 1;
        }
    }

    if ((s_flag & DELAY) && _old_time + _delay < millis())
    {
        _old_time = millis();
        for (int x = 0; x < 7; x++)
        {
            for (int y = 6; y >= 0; y--)
            {
                if (_matrix[x][y] != 0 && y != 0)
                {
                    _matrix[x][y - 1] = _matrix[x][y];
                    _matrix[x][y] = 0;
                    break;
                }
            }
        }
    }

    if (s_flag & LINE)
    {
        for (int x = 0; x < 7; x++)
        {
            bool clear = false;
            for (int y = 6; y >= 0; y--)
            {
                if (clear)
                    _matrix[x][y] = 0;

                if (_matrix[x][y] != 0)
                    clear = true;
            }
        }
    }
}

void LedMatrix::paint(COLOR_DISPLAY_FLAG c_flag)
{
    uint32_t colors[3]{0, 0, 0};

    if (c_flag & FADE)
    {
        fadeCycyle();
        for (int i = 0; i < 3; i++)
            colors[i] = _fade.toNum();
    }
    else
    {

        if (c_flag & COLOR_1)
            colors[0] = _c1.toNum();
        else if (c_flag & COLOR_2)
            colors[0] = _c2.toNum();
        else if (c_flag & COLOR_3)
            colors[0] = _c3.toNum();

        if (c_flag & COLOR_2)
            colors[1] = _c2.toNum();
        else if (c_flag & COLOR_3)
            colors[1] = _c3.toNum();
        else if (c_flag & COLOR_1)
            colors[1] = _c1.toNum();

        if (c_flag & COLOR_3)
            colors[2] = _c3.toNum();
        else if (c_flag & COLOR_2)
            colors[2] = _c2.toNum();
        else if (c_flag & COLOR_1)
            colors[2] = _c1.toNum();
    }

    if (c_flag & STATIC)
    {
        for (int x = 0; x < 7; x++)
        {
            for (int y = 0; y < 7; y++)
            {
                if (_matrix[x][y] != 0)
                {
                    if (y > 5)
                        _matrix[x][y] = colors[2];
                    else if (y > 3)
                        _matrix[x][y] = colors[1];
                    else
                        _matrix[x][y] = colors[0];
                }
            }
        }
    }
    else
    {
        for (int x = 0; x < 7; x++)
        {
            int i = 0;
            for (int y = 6; y >= 0; y--)
            {
                if (_matrix[x][y] != 0)
                {
                    if (i == 0)
                        _matrix[x][y] = colors[2];
                    else if (i <= 1)
                        _matrix[x][y] = colors[1];
                    else
                        _matrix[x][y] = colors[0];

                    if (colors[1] == colors[2])
                        i++;
                    i++;
                }
            }
        }
    }
}