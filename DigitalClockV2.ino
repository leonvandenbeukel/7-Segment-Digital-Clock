/*
 * MIT License
 * 
 * Copyright (c) 2018 Leon van den Beukel
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Source: 
 * https://github.com/leonvandenbeukel/7-Segment-Digital-Clock
 * 
 * External libraries you need:
 * Adafruit RTCLib:   https://github.com/adafruit/RTClib
 * FastLED:           https://github.com/FastLED/FastLED
 */

#include <FastLED.h>
#include <Wire.h>
#include "RTClib.h"

#define NUM_LEDS 30     
#define DATA_PIN 6
CRGB LEDs[NUM_LEDS];

// Change this color if you want another color, for example CRGB::Red
CRGB color = CRGB::White;   

RTC_DS3231 rtc;

void setup () {

  // Initialize LED strip
  FastLED.delay(3000);

  // Check if you're LED strip is a RGB or GRB version (third parameter)
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(LEDs, NUM_LEDS);

  Serial.begin(9600);
  while (!Serial) { /* Wait until serial is ready */ }

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop () {
  
  DateTime now = rtc.now();

  int hl = now.hour() / 10;
  int hr = now.hour() % 10;
  int ml = now.minute() / 10;
  int mr = now.minute() % 10;

  displayTime(0, mr);    
  displayTime(7, ml);    

  displayTime(16, hr);    
  displayTime(23, hl);    

  FastLED.show();

  delay(400);
}

void displayTime(int startindex, int number) {

  byte numbers[] = {
    0b00111111, // 0    
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9   
  };

  LEDs[0 + startindex] = ((numbers[number] & 1) == 1) ? color : CRGB::Black;
  LEDs[1 + startindex] = ((numbers[number] & 2) == 2) ? color : CRGB::Black;
  LEDs[2 + startindex] = ((numbers[number] & 4) == 4) ? color : CRGB::Black;
  LEDs[3 + startindex] = ((numbers[number] & 8) == 8) ? color : CRGB::Black;
  LEDs[4 + startindex] = ((numbers[number] & 16) == 16) ? color : CRGB::Black;
  LEDs[5 + startindex] = ((numbers[number] & 32) == 32) ? color : CRGB::Black;
  LEDs[6 + startindex] = ((numbers[number] & 64) == 64) ? color : CRGB::Black;  

  // Dots
  LEDs[14] = color;
  LEDs[15] = color;
}
