#ifndef _CONFIG_
#define _CONFIG_

#include <Arduino.h>

constexpr int PIN_BUTTON_1 = 6;
constexpr int PIN_BUTTON_2 = 7;
constexpr int PIN_BUTTON_3 = 8;

constexpr int PIN_LED = 13;

// MSGEQ7
constexpr int PIN_INPUT = A0;
constexpr int PIN_STROBE = 10;
constexpr int PIN_RESET = 9;

// VARIABLES
constexpr unsigned long BUTTON_RESET_TIME = 6000;
constexpr unsigned long BUTTON_TIME = 200;
constexpr unsigned long DELAY_TIME = 75;

constexpr int SHAPES_CONUT = 3;
constexpr int COLOR_CONUT = 9;

#endif