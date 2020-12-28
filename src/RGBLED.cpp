#include <Arduino.h>
#include "RGBLED.h"

/**
  @brief  Class constructor
  @param redPin Pin attached to red LED
  @param greenPin Pin attached to green LED
  @param bluePin Pin attached to blue LED
  @param polarity Determine if the LED turns on with HIGH or LOW state
*/
RGBLED::RGBLED(uint16_t redPin, uint16_t greenPin, uint16_t bluePin, bool polarity) {
  _redPin = redPin;
  _greenPin = greenPin;
  _bluePin = bluePin;
  _polarity = polarity;

  ledcSetup(_redChannel, _pwmFrequency, _pwmResolution);
  ledcSetup(_greenChannel, _pwmFrequency, _pwmResolution);
  ledcSetup(_blueChannel, _pwmFrequency, _pwmResolution);

  ledcAttachPin(_redPin, _redChannel);
  ledcAttachPin(_greenPin, _greenChannel);
  ledcAttachPin(_bluePin, _blueChannel);
}

/**
  @brief  Update the LED transition animation. This function is responsible for the
          LED smooth transition effect.
*/
void RGBLED::UpdateAnimation() {
  if (_redValue != _toRedValue || _greenValue != _toGreenValue || _blueValue != _toBlueValue) {
    if (_redValue < _toRedValue)
      _redValue += 1;
    else if (_redValue > _toRedValue)
      _redValue -= 1;

    if (_greenValue < _toGreenValue)
      _greenValue += 1;
    else if (_greenValue > _toGreenValue)
      _greenValue -= 1;

    if (_blueValue < _toBlueValue)
      _blueValue += 1;
    else if (_blueValue > _toBlueValue)
      _blueValue -= 1;
  }

  UpdateColor();
}

/**
  @brief  Update the PWM values for the current color values
*/
void RGBLED::UpdateColor() {
  _currentColor = (_redValue << 16) + (_greenValue << 8) + _blueValue;

  ApplyBrightness();  

  if (_polarity == LOW) {
    ledcWrite(_redChannel, PWM_RANGE_VALUE - _redValue);
    ledcWrite(_greenChannel, PWM_RANGE_VALUE - _greenValue);
    ledcWrite(_blueChannel, PWM_RANGE_VALUE - _blueValue);
  }
  else {
    ledcWrite(_redChannel, _redValue);
    ledcWrite(_greenChannel, _greenValue);
    ledcWrite(_blueChannel, _blueValue);
  }
}

/**
  @brief  Get the current color in RGB hex code 
  @return The current LED color
*/
uint32_t RGBLED::GetCurrentColor() {
  return _currentColor;
}

/**
  @brief  Apply the brigthness factor to the PWM value
*/
void RGBLED::ApplyBrightness() {
  _redBrightnessValue = (_redValue * _brightness / 100);
  _greenBrightnessValue = (_greenValue * _brightness / 100);
  _blueBrightnessValue = (_blueValue * _brightness / 100);
}

/**
  @brief  Set the desired _brightness 
  @param _brightness Desired _brightness for all colors
*/
void RGBLED::SetBrightness(uint8_t brightness) {
  _brightness = brightness;
}

/**
  @brief  Receive a 3 byte value containing the RGB combination.
          Examples: RED_COLOR (0xFF0000), GREEN_COLOR (0x00FF00), BLUE_COLOR (0x0000FF)
  @param color The 32-bit color in RGB mode
*/
void RGBLED::SetColor(uint32_t color) {
  _redValue = lowByte(color >> 16);
  _greenValue = lowByte(color >> 8);
  _blueValue = lowByte(color);

  #ifdef DEBUG_LED_COLOR
    Serial.println("Color: " + String(color, HEX));
    Serial.println("red: " + String(_redValue, HEX));
    Serial.println("green: " + String(_greenValue, HEX));
    Serial.println("blue: " + String(_blueValue, HEX));
  #endif

  UpdateColor();
}

/**
  @brief  Set the new color with a transition effect.
  @param color The 32-bit color in RGB mode
  @param transitionDelay Optional parammeter to set the transition delay
*/
void RGBLED::ColorTransition(uint32_t color, uint8_t transitionDelay) {
  _toRedValue = lowByte(color >> 16);
  _toGreenValue = lowByte(color >> 8);
  _toBlueValue = lowByte(color);

  while (_redValue != _toRedValue || _greenValue != _toGreenValue || _blueValue != _toBlueValue) {
    UpdateAnimation();
    delay(transitionDelay);
  }
}

/**
  @brief  LED demo animation
*/
void RGBLED::Demo() {
  ColorTransition(RED_COLOR);
  ColorTransition(GREEN_COLOR);
  ColorTransition(BLUE_COLOR);

  delay(1000);

  ColorTransition(RED_COLOR, LED_SLOW_TRANSITION_MS);
  ColorTransition(GREEN_COLOR, LED_SLOW_TRANSITION_MS);
  ColorTransition(BLUE_COLOR, LED_SLOW_TRANSITION_MS);
}