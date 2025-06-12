#pragma once
#define USE_OLED      0         // Change to a 1 if you have ESP32 with OLED Screen

#ifdef LAMP_VERTICAL_4
    #define NUM_STRIPS    4
    #define NUM_LEDS      72  
    #define LED_PIN1      26        // Define your pins for each strip
    #define LED_PIN2      27
    #define LED_PIN3      14
    #define LED_PIN4      12
    #define BUTTON_PIN    13        // Button Pin for Pattern Change
    #define SWITCH_PIN    23        // Switch Pin for WIFI OFF and Sound Reactive mode

#elif defined(LAMP_SINGLE)
    #define NUM_STRIPS  1
    #define NUM_LEDS    72
    #define LED_PIN1    26
    #define BUTTON_PIN  13
    #define SWITCH_PIN  23

#elif defined(BAR_LIGHTS)
    #define NUM_STRIPS  2
    #define NUM_LEDS    144
    #define LED_PIN1    26
    #define LED_PIN2    27
    #define BUTTON_PIN  13
    #define SWITCH_PIN  23
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

#if USE_OLED 
    #include <U8g2lib.h>            // Library for OLED on Heltec
    #define OLED_CLOCK  15          // Pins for the OLED display
    #define OLED_DATA   4
    #define OLED_RESET  16
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);
    int g_lineHeight = 0;
#endif

void setupLeds(){
    pinMode(LED_BUILTIN, OUTPUT);
    // Add our LED strips to the FastLED library
    pinMode(LED_PIN1, OUTPUT);
    FastLED.addLeds<WS2812B, LED_PIN1, GRB>(g_LEDs[0], NUM_LEDS);

    #ifdef LED_PIN2
        pinMode(LED_PIN2, OUTPUT);
        FastLED.addLeds<WS2812B, LED_PIN2, GRB>(g_LEDs[1], NUM_LEDS);
    #endif
    
    #ifdef LED_PIN3
        pinMode(LED_PIN3, OUTPUT);
        FastLED.addLeds<WS2812B, LED_PIN3, GRB>(g_LEDs[2], NUM_LEDS);
    #endif

    #ifdef LED_PIN4
        pinMode(LED_PIN4, OUTPUT);
        FastLED.addLeds<WS2812B, LED_PIN4, GRB>(g_LEDs[3], NUM_LEDS);
    #endif 
  
    FastLED.setBrightness(g_Brightness);                                    // and set brightness from varible
    FastLED.setMaxPowerInMilliWatts(g_PowerLimit);                          // Set Max Power
    
    #if USE_OLED
        g_OLED.begin();
        g_OLED.clear();
        g_OLED.setFont(u8g2_font_profont15_tf);
        g_lineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent();        // Descent is a negative number so we add it to the total
    #endif
}

void displayInfomation(){
    if(displayInfo==true){
      uint32_t milliwatts = calculate_unscaled_power_mW(g_LEDs[0], NUM_LEDS);  // How much power are we asking for?
      if (NUM_STRIPS>1){
        for(int s = 1; s < NUM_STRIPS; s++) milliwatts += calculate_unscaled_power_mW(g_LEDs[s], NUM_LEDS);
      }

      EVERY_N_MILLISECONDS(250)                   // Only Update stats every 250mSeconds using MacroHelper
      {
        #if USE_OLED
        g_OLED.clearBuffer();
        g_OLED.setCursor(0, g_lineHeight);
        g_OLED.printf("FPS: %u", FastLED.getFPS());
        g_OLED.setCursor(0, g_lineHeight * 2);
        g_OLED.printf("Power: %u mW", milliwatts);
        g_OLED.setCursor(0, g_lineheight * 3);
        g_OLED.printf("Brightness at PowerLimit: %d", calculate_max_brightness_for_power_mW(g_Brightness, g_PowerLimit));
        g_OLED.sendBuffer();
        #endif
      
        Serial.printf("FPS: %u", FastLED.getFPS());
        Serial.println();
        Serial.printf("Power: %u mW", milliwatts);
        Serial.println();
        Serial.printf("Brightness at PowerLimit: %d", calculate_max_brightness_for_power_mW(g_Brightness, g_PowerLimit));
        Serial.println();
      
      }
    }
}