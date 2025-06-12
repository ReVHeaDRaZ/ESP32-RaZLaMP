#pragma once

#ifdef LAMP_VERTICAL_4
    #define USE_OLED      0                                 // Change to a 1 if you have ESP32 with OLED Screen
    #define NUM_STRIPS    4
    #define NUM_LEDS      72  
    const uint8_t LED_PINS[NUM_STRIPS] = {26, 27, 14, 12};  // Define your pins for each strip
    #define BUTTON_PIN    13                                // Button Pin for Pattern Change
    #define SWITCH_PIN    23                                // Switch Pin for WIFI OFF and Sound Reactive mode
#else
    #error "No lamp layout defined!"
#endif

#define TOTAL_LEDS (NUM_STRIPS * NUM_LEDS)

#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];