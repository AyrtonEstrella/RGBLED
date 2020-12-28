#ifndef LED_ANIMATIONS_H
#define LED_ANIMATIONS_H

/************************* ADDITIONAL LIBRARIES **********************/
#include <Arduino.h>

/************************* LIBRARY OPTIONS **********************/

#define LED_ON_IS_LOW       // Comment this line if LED turns off when control pin is LOW
// #define DEBUG_LED_COLOR    // Uncomment to print the RGB hex codes

/************************* CONSTANTS **********************/
#ifdef LED_ON_IS_LOW
  #define LED_ON  LOW
  #define LED_OFF HIGH
#else 
  #define LED_ON  HIGH
  #define LED_OFF LOW
#endif

#define LED_VERY_FAST_TRANSITION_MS 1
#define LED_FAST_TRANSITION_MS      3
#define LED_SLOW_TRANSITION_MS      10

#define PWM_RANGE_VALUE 255
#define BLACK_COLOR   0x000000
#define RED_COLOR     0xFF0000
#define GREEN_COLOR   0x00FF00
#define BLUE_COLOR    0x0000FF
#define YELLOW_COLOR  0xFFFF00
#define CYAN_COLOR    0x00FFFF
#define MAGENTA_COLOR 0xFF00FF
#define ORANGE_COLOR  0xFFA500
#define WHITE_COLOR   0xFFFFFF
/************************* VARIABLES **********************/

/************************* MACROS **********************/


/************************* CLASS DEFINITION **********************/

class RGBLED {
  private:
    uint8_t _redValue = 0, _toRedValue = 0, _redBrightnessValue = 0;
    uint8_t _greenValue = 0, _toGreenValue = 0, _greenBrightnessValue = 0;
    uint8_t _blueValue = 0, _toBlueValue = 0, _blueBrightnessValue = 0;
    uint32_t _currentColor = 0, _targetColor = 0;
    uint8_t _brightness = 25;

    uint16_t _pwmFrequency = 5000;
    uint8_t _redChannel = 0; _greenChannel = 1; _blueChannel = 2;
    uint8_t _pwmResolution = 8,
    uint16_t _redPin = 0, _greenPin = 0, _bluePin = 0; 

    void UpdateAnimation();
    void UpdateColor();
    void ApplyBrightness();
  public:
    RGBLED(uint16_t redPin, uint16_t greenPin, uint16_t bluePin);

    uint32_t GetCurrentColor();
    void SetBrightness(uint8_t brightness);
    void SetColor(uint32_t color);
    void ColorTransition(uint32_t color, uint8_t transitionDelay = LED_FAST_TRANSITION_MS);
    void Demo();
};

#endif