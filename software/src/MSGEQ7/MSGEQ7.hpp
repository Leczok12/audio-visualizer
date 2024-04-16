#include <Arduino.h>

class MSGEQ7
{
public:
    MSGEQ7(int inputPin, int resetPin, int strobePin, int noise = 100, int maxVal = 1023, int minVal = 0);
    ~MSGEQ7();

    void begin();
    int *read(float factor = 1.0);

private:
    int _resetPin;
    int _strobePin;
    int _inputPin;
    int _noise;
    int _maxVal;
    int _minVal;

    int _data[7];
};