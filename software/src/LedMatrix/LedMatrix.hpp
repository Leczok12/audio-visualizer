#include "../config.hpp"
#include "../types.hpp"
#include "Adafruit_NeoPixel.h"

class LedMatrix
{
public:
    LedMatrix(int pin, unsigned long delay, int &brightness, Color &c1, Color &c2, Color &c3);
    ~LedMatrix();

    void begin();
    void render(int *values, SHAPE_DISPLAY_FLAG s_flag, COLOR_DISPLAY_FLAG c_flag);
    void show();
    void off();

private:
    Adafruit_NeoPixel _strip;
    const unsigned long _delay;
    unsigned long _old_time;

    uint32_t _matrix[7][7];

    int &_brightness;
    Color &_c1;
    Color &_c2;
    Color &_c3;

    void draw(int *values, SHAPE_DISPLAY_FLAG s_flag);
    void paint(COLOR_DISPLAY_FLAG c_flag);

    void fadeCycyle();
    int _fade_index;
    Color _fade;
};
