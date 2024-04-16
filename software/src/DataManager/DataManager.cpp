#include "DataManager.hpp"

DataManager::DataManager(bool &power,
                         int &display_mode,
                         int &color_mode,
                         int &audio_factor,
                         int &brightness,
                         Color &c1,
                         Color &c2,
                         Color &c3)
    : _power(power),
      _shape_mode(display_mode),
      _color_mode(color_mode),
      _audio_factor(audio_factor),
      _brightness(brightness),
      _c1(c1),
      _c2(c2),
      _c3(c3)
{
    EEPROM.begin();
}

DataManager::~DataManager()
{
}

void DataManager::read()
{
    int values[14];

    for (int i = 0; i < 14; i++)
        values[i] = EEPROM.read(i);

    if (values[4] == 0) // EEPROM was never used
        return;

    _power = values[0];
    _shape_mode = values[1];
    _color_mode = values[2];
    _audio_factor = values[3];
    _brightness = values[4];
    _c1.r = values[5];
    _c1.g = values[6];
    _c1.b = values[7];
    _c2.r = values[8];
    _c2.g = values[9];
    _c2.b = values[10];
    _c3.r = values[11];
    _c3.g = values[12];
    _c3.b = values[13];
}
void DataManager::update()
{
    EEPROM.update(0, _power);
    EEPROM.update(1, _shape_mode);
    EEPROM.update(2, _color_mode);
    EEPROM.update(3, _audio_factor);
    EEPROM.update(4, _brightness);
    EEPROM.update(5, _c1.r);
    EEPROM.update(6, _c1.g);
    EEPROM.update(7, _c1.b);
    EEPROM.update(8, _c2.r);
    EEPROM.update(9, _c2.g);
    EEPROM.update(10, _c2.b);
    EEPROM.update(11, _c3.r);
    EEPROM.update(12, _c3.g);
    EEPROM.update(13, _c3.b);
}