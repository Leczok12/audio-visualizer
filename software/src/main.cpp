#include <Arduino.h>
#include <QuickButton.h>
#include "types.hpp"
#include "config.hpp"
#include "LedMatrix/LedMatrix.hpp"
#include "DataManager/DataManager.hpp"
#include "MSGEQ7/MSGEQ7.hpp"

bool power = true;
int shapeMode = 0;
int colorMode = 0;
LimitedValue<int> audioFactor = {10, 1, 30};
LimitedValue<int> brightness = {255, 30, 255};
Color color1 = {255, 255, 255};
Color color2 = {255, 255, 255};
Color color3 = {255, 255, 255};

MSGEQ7 msgeq7(PIN_INPUT, PIN_RESET, PIN_STROBE, 70, 7, 0);
LedMatrix ledMatrix(PIN_LED, DELAY_TIME, brightness.value, color1, color2, color3);
unsigned long matrixLastRender = 0;

DataManager dataManager(power,
                        shapeMode,
                        colorMode,
                        audioFactor.value,
                        brightness.value,
                        color1,
                        color2,
                        color3);

QuickButton button1(PIN_BUTTON_1);
QuickButton button2(PIN_BUTTON_2);
QuickButton button3(PIN_BUTTON_3);

unsigned long buttonLastPress = 0;
int configIndex = 0; // max 5
BUTTON_MODE buttonMode = DISPLAY_ADJUSTMENT;

COLOR_DISPLAY_FLAG c_flag = COLOR_1;
SHAPE_DISPLAY_FLAG s_flag = NORMAL;

void updateFlags()
{
  if (shapeMode == 0)
    s_flag = NORMAL;
  else if (shapeMode == 1)
    s_flag = DELAY;
  else if (shapeMode == 2)
    s_flag = LINE;
  else if (shapeMode == 3)
    s_flag = DELAY | LINE;

  if (colorMode == 0)
    c_flag = COLOR_1;
  else if (colorMode == 1)
    c_flag = COLOR_2;
  else if (colorMode == 2)
    c_flag = COLOR_3;
  else if (colorMode == 3)
    c_flag = COLOR_1 | COLOR_2;
  else if (colorMode == 4)
    c_flag = COLOR_1 | COLOR_3;
  else if (colorMode == 5)
    c_flag = COLOR_1 | COLOR_2 | COLOR_3;
  else if (colorMode == 6)
    c_flag = COLOR_1 | COLOR_2 | STATIC;
  else if (colorMode == 7)
    c_flag = COLOR_1 | COLOR_3 | STATIC;
  else if (colorMode == 8)
    c_flag = COLOR_1 | COLOR_2 | COLOR_3 | STATIC;
  else if (colorMode == 9)
    c_flag = FADE;
}

void reset()
{
  power = true;
  shapeMode = 0;
  colorMode = 0;
  audioFactor.value = 10;
  brightness.value = 255;
  color1 = {255, 255, 255};
  color2 = {255, 255, 255};
  color3 = {255, 255, 255};

  updateFlags();
  dataManager.update();
}

void onClick(int id, unsigned long d, int c)
{
  if (power == false)
  {
    if (id == 1)
    {
      power = true;
      dataManager.update();
    }

    return;
  }

  if (id == 1)
  {
    if (d > 5000)
    {
      buttonMode = COLOR_ADJUSTMENT;
    }
    else if (d > 1000)
    {
      buttonMode = DISPLAY_ADJUSTMENT;
    }
    else if (c >= 3)
    {
      reset();
    }
    else
    {
      if (buttonMode != COLOR_ADJUSTMENT)
      {
        power = !power;
        buttonMode = DISPLAY_ADJUSTMENT;
      }
      else
      {
        configIndex++;
        if (configIndex > 5)
          configIndex = 0;
      }
    }
  }
  else if (id == 2)
  {
    if (d > 1000)
    {
      buttonMode = AUDIO_FACTOR_ADJUSTMENT;
    }
    else if (buttonMode == DISPLAY_ADJUSTMENT)
    {
      shapeMode++;
      if (shapeMode > SHAPES_CONUT)
        shapeMode = 0;

      updateFlags();
    }
  }
  else if (id == 3)
  {
    if (d > 1000)
    {
      buttonMode = BRIGHTNESS_ADJUSTMENT;
    }
    else if (buttonMode == DISPLAY_ADJUSTMENT)
    {
      colorMode++;
      if (colorMode > COLOR_CONUT)
        colorMode = 0;

      updateFlags();
    }
  }

  buttonLastPress = millis();
  dataManager.update();
}

void setup()
{
  Serial.begin(9600);

  dataManager.read();
  updateFlags();
  msgeq7.begin();
  ledMatrix.begin();

  button1.onClick([](unsigned long d, int c)
                  { onClick(1, d, c); });
  button2.onClick([](unsigned long d, int c)
                  { onClick(2, d, c); });
  button3.onClick([](unsigned long d, int c)
                  { onClick(3, d, c); });
}

void loop()
{
  button1.loop();
  button2.loop();
  button3.loop();

  if (buttonLastPress + BUTTON_TIME < millis() && buttonMode != DISPLAY_ADJUSTMENT)
  {
    if (button2.isPressed())
    {
      button2.reset();
      switch (buttonMode)
      {
      case BRIGHTNESS_ADJUSTMENT:
        brightness.increase(5);
        break;
      case AUDIO_FACTOR_ADJUSTMENT:
        audioFactor.increase(1);
        break;
      default:
        break;
      }
      buttonLastPress = millis();
    }
    if (button3.isPressed())
    {
      button3.reset();
      switch (buttonMode)
      {
      case BRIGHTNESS_ADJUSTMENT:
        brightness.decrease(5);
        break;
      case AUDIO_FACTOR_ADJUSTMENT:
        audioFactor.decrease(1);
        break;
      default:
        break;
      }
      buttonLastPress = millis();
    }
  }
  if (buttonLastPress + BUTTON_RESET_TIME < millis())
  {
    buttonMode = DISPLAY_ADJUSTMENT;
  }

  if (!power)
  {
    ledMatrix.off();
    msgeq7.read();
  }
  else if (buttonMode == COLOR_ADJUSTMENT)
  {
  }
  else if (matrixLastRender + 25 < millis())
  {
    matrixLastRender = millis();
    ledMatrix.render(msgeq7.read(audioFactor.value / 10.0), s_flag, c_flag);
    // ledMatrix.render(msgeq7.read(audioFactor.value / 10.0), NORMAL, COLOR_1);
    ledMatrix.show();
  }
}