#pragma once

#ifdef LAMP_VERTICAL_4
    #define USE_OLED      0         // Change to a 1 if you have ESP32 with OLED Screen
    #define NUM_STRIPS    4
    #define NUM_LEDS      72  
    #define LED_PIN1      26        // Define your pins for each strip
    #define LED_PIN2      27
    #define LED_PIN3      14
    #define LED_PIN4      12
    #define BUTTON_PIN    13        // Button Pin for Pattern Change
    #define SWITCH_PIN    23        // Switch Pin for WIFI OFF and Sound Reactive mode
#elif defined(LAMP_SINGLE)
    #define USE_OLED      0         // Change to a 1 if you have ESP32 with OLED Screen
    #define NUM_STRIPS 1
    #define NUM_LEDS_PER_STRIP 240
    #define LED_PIN1 26
    #define BUTTON_PIN    13        // Button Pin for Pattern Change
    #define SWITCH_PIN    23        // Switch Pin for WIFI OFF and Sound Reactive mode
#else
    #error "No lamp layout defined!"
#endif

#define TOTAL_LEDS (NUM_STRIPS * NUM_LEDS)

#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

int g_Brightness = 200;         // Brightness Setting
int g_PowerLimit = 40000;       // Power Limit Setting (40000mW at 5v = 8amps)
bool displayInfo = false;       // Display info on OLED/serial?
bool wifiApMode = false;        // Use Wifi as an AccessPoint = true, as a StationPoint = false
bool useMQTT = true;            // Set to true and edit RaZMQTT.h if you want to use MQTT