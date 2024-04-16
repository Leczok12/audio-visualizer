#include "MSGEQ7.hpp"

MSGEQ7::MSGEQ7(int inputPin, int resetPin, int strobePin, int noise, int maxVal, int minVal)
    : _resetPin(resetPin),
      _strobePin(strobePin),
      _inputPin(inputPin),
      _noise(noise),
      _maxVal(maxVal),
      _minVal(minVal)
{
}

MSGEQ7::~MSGEQ7() {}

void MSGEQ7::begin()
{
    for (auto &v : _data)
        v = 0;

    pinMode(_inputPin, INPUT);
    pinMode(_resetPin, OUTPUT);
    pinMode(_strobePin, OUTPUT);

    digitalWrite(_resetPin, LOW);
    digitalWrite(_strobePin, LOW);
    delay(1);

    digitalWrite(_resetPin, HIGH);
    delay(1);
    digitalWrite(_resetPin, LOW);
    digitalWrite(_strobePin, HIGH);
    delay(1);
    delay(1000);
}

int *MSGEQ7::read(float factor)
{
    int _rawData[5][7];
    for (int j = 0; j < 5; j++)
    {
        for (int i = 0; i < 7; i++)
        {
            digitalWrite(_strobePin, LOW);
            delayMicroseconds(100);
            _rawData[j][i] = analogRead(_inputPin);
            digitalWrite(_strobePin, HIGH);
            delayMicroseconds(100);
        }
    }

    for (int j = 0; j < 7; j++)
    {
        _data[j] = 1023;
        for (int i = 0; i < 5; i++)
            if (_data[j] > _rawData[i][j])
                _data[j] = _rawData[i][j];

        if (_data[j] < _noise)
            _data[j] = 0;
        else
            _data[j] = map((_data[j] - _noise) * factor, 0, 1023 - _noise, _minVal, _maxVal);
    }

    return _data;
}