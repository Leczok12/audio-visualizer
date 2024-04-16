#include <Arduino.h>
#include <EEPROM.h>
#include "../types.hpp"

class DataManager
{
public:
    DataManager(bool &power,
                int &shape_mode,
                int &color_mode,
                int &audio_factor,
                int &brightness,
                Color &c1,
                Color &c2,
                Color &c3);

    ~DataManager();

    void read();
    void update();

private:
    bool &_power;
    int &_shape_mode;
    int &_color_mode;
    int &_audio_factor;
    int &_brightness;
    Color &_c1;
    Color &_c2;
    Color &_c3;
};
