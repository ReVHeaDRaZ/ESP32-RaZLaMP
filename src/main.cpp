//  RaZESP32ws2812b Array For Multiple Strips setup for verticle 4 Strip Lamp v2.4 
//  ------------------------------------------------------------------------------------
//  Added LetterBounce and Clock, tweaked fireworks velocity, Added switch to turn
//  OFF WIFI and SoundReactive ON due to WIFI Power spikes, fixed Lightning Flickers
//  Changed LetterBounce.h to implement changing text and 8 Characters
//  Added Change text option to RazWifi.h :)
//  Added Swirl Effect, Snakes, MAGMA, Circles and NoisePatterns
//  Upped Max Power to 40000mW - 8amps at 5v
//  Added MQTT support and automatically set Time from NTP if connected to Internet

#include <Arduino.h>
#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>

#include "config.h"

CRGB g_LEDs[NUM_STRIPS][NUM_LEDS] = {0};      // Frame buffer for FastLED

// Include Pattern and FX Header Files
#include "clock.h"
#include "Ledgfx.h"               // Utilities
#include "FFT.h"
#include "Patterns.h"             // All Patterns
#include "RazWifi.h"              // RaZ's Wifi Server
#include "RaZMQTT.h"
#include "ntptime.h"
#include "buttons.h"

void setup() 
{
  setupLeds();
  setupButtons();
 
  Serial.begin(115200);
  while (!Serial) { }
  Serial.println("ESP32 Startup");
  
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ)); // For FFT
  
  if(!SoundReactive){
    // Setup Wifi Server (true-AccessPointMode, false-StationPointMode)
    WifiSetup(wifiApMode);
    // Setup MQTT and NTP if connected to Internet
    if(wifiApMode==false){
      mqttclient.setServer(mqtt_server, 1883);
      mqttclient.setCallback(callback);
      initNtpTime();
    }                          
  }
  
}

 
void loop() 
{
  bool bLED = 0;
    
  // Infinite Loop--------------------
  while(true)
  {
    bLED = !bLED;                               // Blink the Onboard LED off and on every loop
    digitalWrite(LED_BUILTIN, bLED);            // for basic framerate indication

    EVERY_N_SECONDS(60){    // Update Clock every minute
      if(getLocalTime()){   // Try update time from NTP
        setRazClockTime();
      }else
        ClockCounter();     // Otherwise increment time manually
    }
 
    handleButtons();
        
    if(!SoundReactive){
        WifiCheck();                            // Check Wifi for Clients and MQTT
        if((useMQTT) && (wifiApMode==false) )
          MQTTCheck();
    }

    // Handle OLED drawing and Serial Monitor
    displayInfomation();

    // Handle Pattern Selection and LEDs
    handlePatterns();
    
    FastLED.setBrightness(g_Brightness);    // Set Brightness Scale before
    FastLED.delay(1);                       // Delay and Show Leds (part of delay function)
  }
}// End Loop