//  RaZESP32ws2812b Array For Multiple Strips setup for verticle 4 Strip Lamp v2.3 
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

#define NUM_LEDS      72         // FastLED definitions
#define NUM_STRIPS    4
#define LED_PIN       26  //12 Reverse Order
#define LED_PIN2      27  //14
#define LED_PIN3      14  //27
#define LED_PIN4      12  //26
#define BUTTON_PIN    13          // Button Pin for Pattern Change
#define SWITCH_PIN    23          // Switch Pin for WIFI OFF and Sound Reactive mode
#define NUM_PATTERNS  26          // Number of Patterns - 1 for Autochange
#define PATTERN_TIME  30          // Time before Pattern Change

CRGB g_LEDs[NUM_STRIPS][NUM_LEDS] = {0};      // Frame buffer for FastLED

int g_Brightness = 200;           // Brightness Setting
int g_PowerLimit = 40000;         // Power Limit Setting (40000mW at 5v = 8amps)
int Pattern = 9;                  // Pattern Selection- Default 9 AUTO / 50 For Testing
int AutoPatternNumber = 16;       // For Auto Select Pattern Function, Start Pattern
bool ButtonState = 1;             // For Button Active Low
bool SwitchState = 1;             // For Switch Active Low
bool SoundReactive = 0;           // For Selecting Sound Reactive Patterns
bool displayInfo = false;         // Display info on OLED/serial?
bool wifiApMode = false;          // Use Wifi as an AccessPoint = true, as a StationPoint = false
bool useMQTT = false;
bool drawVariant = false;         // To change from to pattern variants every cycle of patterns
bool nightMode = false;
int oldPattern = 9;               // To store last pattern when switching to nightmode
int oldBrightness = 200;          // To store brightness when switching to nightmode

// Clock Variables for Mins and Hrs
byte clockCount0    = 8;
byte clockCount10   = 4;
byte clockCount100  = 6;
byte clockCount1000 = 0;
void ClockCounter();

// Palette Variables
CRGBPalette16 currentPalette(OceanColors_p);
CRGBPalette16 targetPalette(PartyColors_p);
TBlendType    currentBlending;                        // NOBLEND or LINEARBLEND
void CheckPalettes();                                 // For testing palettes
void PowerTest();

// Include Pattern and FX Header Files
#include "Ledgfx.h"               // Utilities
#include "FFT.h"
#include "Patterns.h"             // All Patterns
#include "RazWifi.h"              // RaZ's Wifi Server
#include "RaZMQTT.h"
#include "ntptime.h"

// UnComment out all for OLED display *********************************************************** 
//#include <U8g2lib.h>            // Library for OLED on Heltec
//#define OLED_CLOCK  15          // Pins for the OLED display
//#define OLED_DATA   4
//#define OLED_RESET  16
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);
//int g_lineHeight = 0;
//***************************************************************************************** 


void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  pinMode(LED_PIN4, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
 
  Serial.begin(115200);
  while (!Serial) { }
  Serial.println("ESP32 Startup");
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ)); // For FFT
 
 //OLED SETUP Uncomment for use
 //--------------------------------------------------------------------------------------- 
  //g_OLED.begin();
  //g_OLED.clear();
  //g_OLED.setFont(u8g2_font_profont15_tf);
  //g_lineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent();        // Descent is a negative number so we add it to the total
 //----------------------------------------------------------------------------------------

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs[0], NUM_LEDS);            // Add our LED strip to the FastLED library
  FastLED.addLeds<WS2812B, LED_PIN2, GRB>(g_LEDs[1], NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN3, GRB>(g_LEDs[2], NUM_LEDS);
  FastLED.addLeds<WS2812B, LED_PIN4, GRB>(g_LEDs[3], NUM_LEDS);
  FastLED.setBrightness(g_Brightness);                                    // and set brightness from varible
  FastLED.setMaxPowerInMilliWatts(g_PowerLimit);                          // Set Max Power
  
  SwitchState=digitalRead(SWITCH_PIN);        // Read Switch pin and if LOW
  if(SwitchState==LOW){
        SoundReactive     = 1;
        AutoPatternNumber = 1;
      }else {
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
  // Hue Variables for basic patterns
  uint8_t initialHue = 0;
  uint8_t initialHue2 = 0;
  const uint8_t deltaHue = 16;
  const uint8_t hueDensity = 4;

  // Instantiate Pattern Objects
  BouncingBallEffect balls(NUM_LEDS, NUM_STRIPS, 100, false);               // Setup BouncingBalls Objects
  BouncingBallEffect ballsMirrored(NUM_LEDS, NUM_STRIPS, 100, true);        // Mirrored
  ClassicFireEffect fireOutwards(0, NUM_LEDS, 20, 100, 3, 2, false, true);  // Fire Outwards from Middle
  ClassicFireEffect fireOutwards1(1, NUM_LEDS, 20, 100, 3, 2, false, true); // for strip 2
  ClassicFireEffect fireOutwards2(2, NUM_LEDS, 20, 100, 3, 2, false, true); 
  ClassicFireEffect fireOutwards3(3, NUM_LEDS, 20, 100, 3, 2, false, true);
  ClassicFireEffect fireInwards(0, NUM_LEDS, 20, 100, 3, 2, true, true);    // Fire Inwards toward Middle
  ClassicFireEffect fireInwards1(1,NUM_LEDS, 20, 100, 3, 2, true, true);    // for strip 2
  ClassicFireEffect fireInwards2(2,NUM_LEDS, 20, 100, 3, 2, true, true);
  ClassicFireEffect fireInwards3(3,NUM_LEDS, 20, 100, 3, 2, true, true);
  //ClassicFireEffect fire(NUM_LEDS, 20, 100, 3, 4, false, false);          // Inwards from End
  ClassicFireEffect fire(0, NUM_LEDS, 30, 200, 10, 3, true, false);         // More Intense, Extra Sparking
  ClassicFireEffect fire1(1, NUM_LEDS, 30, 200, 10, 3, true, false);
  ClassicFireEffect fire2(2, NUM_LEDS, 30, 200, 10, 3, true, false);
  ClassicFireEffect fire3(3, NUM_LEDS, 30, 200, 10, 3, true, false);
    
  Comet comet1(1,0,5);              // Create Comets (Direction, Position, Size)
  Comet comet2(-1,50,8);            // Create extra comets for extra strips
  Comet comet3(1,20,10);
  Comet comet4(-1,10,8);

  // Infinite Loop--------------------
  while(true)
  {
    EVERY_N_SECONDS(60) ClockCounter();         // Update Clock every minute

    bLED = !bLED;                               // Blink the Onboard LED off and on every loop
    digitalWrite(LED_BUILTIN, bLED);            // for basic framerate indication
    
    ButtonState=digitalRead(BUTTON_PIN);        // Read Button presses and increment Pattern
    EVERY_N_MILLIS(100) {
      if(ButtonState==LOW){
        Pattern++;
        if(Pattern>NUM_PATTERNS+1) {
          drawVariant = !drawVariant;         // Change to pattern variants every full pattern cycle
          Pattern=0;
        }
      }
    } 
    
    SwitchState=digitalRead(SWITCH_PIN);        // Read Switch state
    if(SwitchState==LOW){                       // to select SoundReactive
        SoundReactive = 1;
      }else {                                   // Else
        SoundReactive = 0;
        WifiCheck();                            // Check Wifi for Clients and Set Patterns
        if((useMQTT) && (wifiApMode==false) )
          MQTTCheck();
      }

    // Handle OLED drawing and Serial Monitor
    //-------------------------------------------------------------------------------
    if(displayInfo==true){
      uint32_t milliwatts = calculate_unscaled_power_mW(g_LEDs[0], NUM_LEDS);  // How much power are we asking for?
      if (NUM_STRIPS>1){
        for(int s = 1; s < NUM_STRIPS; s++) milliwatts += calculate_unscaled_power_mW(g_LEDs[s], NUM_LEDS);
      }

      EVERY_N_MILLISECONDS(250)                   // Only Update stats every 250mSeconds using MacroHelper
      {
        /*
        g_OLED.clearBuffer();
        g_OLED.setCursor(0, g_lineHeight);
        g_OLED.printf("FPS: %u", FastLED.getFPS());
        g_OLED.setCursor(0, g_lineHeight * 2);
        g_OLED.printf("Power: %u mW", milliwatts);
        g_OLED.setCursor(0, g_lineheight * 3);
        g_OLED.printf("Brightness at PowerLimit: %d", calculate_max_brightness_for_power_mW(g_Brightness, g_PowerLimit));
        g_OLED.sendBuffer();
        */
      
        Serial.printf("FPS: %u", FastLED.getFPS());
        Serial.println();
        Serial.printf("Power: %u mW", milliwatts);
        Serial.println();
        Serial.printf("Brightness at PowerLimit: %d", calculate_max_brightness_for_power_mW(g_Brightness, g_PowerLimit));
        Serial.println();
      
      }
    }
    //------------------------------------------------------------------------------

  // Handle Pattern Selection and LEDs
    switch (Pattern)
    {
          case 0:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              DrawBandsToStrip();
            } else{
            if(!drawVariant){
                DrawMarquee();
              }else DrawMarqueeMirrored();
            }
            break;
          case 1:
            if(SoundReactive){
            SampleAudio();
            AnalyseFFT();
            DrawRipplesToBeat();
            } else{
            DrawTwinkle2();
            }
            break;
          case 2:
            if(SoundReactive){
            SampleAudio();
            AnalyseFFT();
            DrawRipplesToBands(2);
            } else{
            comet1.DrawComet(0);    // DrawComet(StripNumber)
            comet2.DrawComet(1);
            comet3.DrawComet(2);
            comet4.DrawComet(3);
            delay(25);
            }
            break;
          case 3:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              DrawRipplesToBands(3);
            } else{
              PulseComet();
            }
            break;
          case 4:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              BeatPulseComet(0);      // Rainbow Beat Comet
            } else{
              balls.Draw(0);
            }
            break;
          case 5:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              BeatPulseComet(1);      // Heat Beat Comet
            } else{
              ballsMirrored.Draw(0);
            }
            break;
          case 6:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              ScaleBands();
              Sndwave();
              ChangePalettePeriodically();
            } else{ EVERY_N_MILLISECONDS(20){
              for (int s=0; s < NUM_STRIPS; s++){
              if (s==0) fill_rainbow(g_LEDs[s], NUM_LEDS, initialHue += hueDensity, deltaHue);
              if (s==1) fill_rainbow(g_LEDs[s], NUM_LEDS, initialHue2 -= hueDensity, deltaHue);
              if (s==2) fill_rainbow(g_LEDs[s], NUM_LEDS, initialHue += hueDensity, deltaHue);
              if (s==3) fill_rainbow(g_LEDs[s], NUM_LEDS, initialHue2 -= hueDensity, deltaHue);
              }
              }
            }
            break;
          case 7:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              ScaleBands();
              BandFillNoise8();
              ChangePalettePeriodically();
            } else {
              FastLED.clear();                      // Clear for Fire Effect
              fire.DrawFire(drawVariant);
              fire1.DrawFire(drawVariant);
              fire2.DrawFire(drawVariant);
              fire3.DrawFire(drawVariant);
            }
            break;
          case 8:
              FastLED.clear();                      // Clear for Fire Effect
              fireInwards.DrawFire(drawVariant);
              fireInwards1.DrawFire(drawVariant);
              fireInwards2.DrawFire(drawVariant);
              fireInwards3.DrawFire(drawVariant);
            break;
          case 9:
            EVERY_N_SECONDS(PATTERN_TIME)           // Auto Pattern Change
            {
              FastLED.clear();
              AutoPatternNumber++;
              if(SoundReactive){
                if (AutoPatternNumber > 7) AutoPatternNumber = 0;
              }
              else if (AutoPatternNumber > NUM_PATTERNS) {
                drawVariant = !drawVariant;
                AutoPatternNumber = 0;
              }
            }
            break;
          case 10:
            FastLED.clear();                      // Clear for Fire Effect
            fireOutwards.DrawFire(drawVariant);
            fireOutwards1.DrawFire(drawVariant);
            fireOutwards2.DrawFire(drawVariant);
            fireOutwards3.DrawFire(drawVariant);
            break;
          case 11:
            FastLED.clear();
            DrawFireworks();
            break;
          case 12:
            FastLED.clear();
            DrawExplosion();
            break;
          case 13:
            EVERY_N_MILLIS(40){   // Change for speed
              DrawRipples();
            }
            break;
          case 14:
            LightningStorm();
            break;
          case 15:
            DrawMatrixEffect();
            break;
          case 16:
            ZigZag(20,10);
            break;
          case 17:
           ChangePalettePeriodically2();
            EVERY_N_MILLISECONDS(10){
              if(!drawVariant){
                DrawRazBounceWithMelt();
              }else DrawRazBounceWithExplodeAll();
            }
            break;
          case 18:
            FastLED.clear();
            if(nightMode)
              currentPalette = NightModeColors_p;
            else
              ChangePalettePeriodically2();
            
            DrawClock();                        
            break;
          case 19:
            ChangePalettePeriodically2();
            EVERY_N_MILLISECONDS(75){
              if(!drawVariant){
                DrawMaxSwirls();
              }else DrawSwirlsTogether();
            }
            break;
          case 20:
            ChangePalettePeriodically2();
            EVERY_N_MILLISECONDS(75){
              if(!drawVariant){
                DrawSwirlsRandom();
              }else DrawAllSwirlPatterns();
            }
            break;
          case 21:
            Snake::draw();
            break;
          case 22:
            LavaLake();
            break;
          case 23:
            DrawMagma();
            break;
          case 24:
            if(!drawVariant){
                Hypnotic::draw();
              }else ColorPlasma();
            break;
          case 25:
            PoolNoise();
            break;
          case 26:
            DrawCrazyBees();
            break;
          case 27:
            Circles::draw();
            break;
          case 50:  // TESTING PATTERN only setable from code
           PowerTest();
            break;
         default:
            comet1.DrawComet(0);    // DrawComet(StripNumber)
            comet2.DrawComet(1);
            comet3.DrawComet(2);
            comet4.DrawComet(3);
    }
    
    // AUTO PATTERNS
    if(Pattern==9){
      switch (AutoPatternNumber)
      {
          case 0:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              DrawBandsToStrip();
            } else{
            DrawMarquee();
            }
            break;
          case 1:
            if(SoundReactive){
            SampleAudio();
            AnalyseFFT();
            DrawRipplesToBeat();
            } else{
            DrawTwinkle2();
            }
            break;
          case 2:
            if(SoundReactive){
            SampleAudio();
            AnalyseFFT();
            DrawRipplesToBands(2);
            } else{
            comet1.DrawComet(0);    // DrawComet(StripNumber)
            comet2.DrawComet(1);
            comet3.DrawComet(2);
            comet4.DrawComet(3);
            delay(25);
            }
            break;
          case 3:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              DrawRipplesToBands(3);
            } else{
              PulseComet();
            }
            break;
          case 4:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              BeatPulseComet(0);      // Rainbow Beat Comet
            } else{
              balls.Draw(0);
            }
            break;
          case 5:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              BeatPulseComet(1);      // Heat Beat Comet
            } else{
              ballsMirrored.Draw(0);
            }
            break;
          case 6:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              ScaleBands();
              Sndwave();
              ChangePalettePeriodically();
            } else{ EVERY_N_MILLISECONDS(20){
              for (int s=0; s < NUM_STRIPS; s++){
              if (s==0) fill_rainbow(g_LEDs[s], NUM_LEDS, initialHue += hueDensity, deltaHue);
              if (s==1) fill_rainbow(g_LEDs[s], NUM_LEDS, initialHue2 -= hueDensity, deltaHue);
              if (s==2) fill_rainbow(g_LEDs[s], NUM_LEDS, initialHue += hueDensity, deltaHue);
              if (s==3) fill_rainbow(g_LEDs[s], NUM_LEDS, initialHue2 -= hueDensity, deltaHue);
              }
              }
            }
            break;
          case 7:
            if(SoundReactive){
              SampleAudio();
              AnalyseFFT();
              ScaleBands();
              BandFillNoise8();
              ChangePalettePeriodically();
            } else {
              FastLED.clear();                      // Clear for Fire Effect
              fire.DrawFire(drawVariant);
              fire1.DrawFire(drawVariant);
              fire2.DrawFire(drawVariant);
              fire3.DrawFire(drawVariant);
            }
            break;
          case 8:
            FastLED.clear();                      // Clear for Fire Effect
            fireInwards.DrawFire(drawVariant);
            fireInwards1.DrawFire(drawVariant);
            fireInwards2.DrawFire(drawVariant);
            fireInwards3.DrawFire(drawVariant);
            break;
          case 9:
            FastLED.clear();                      // Clear for Fire Effect
            fireOutwards.DrawFire(drawVariant);
            fireOutwards1.DrawFire(drawVariant);
            fireOutwards2.DrawFire(drawVariant);
            fireOutwards3.DrawFire(drawVariant);
            break;
          case 10:
            FastLED.clear();
            DrawFireworks();
            break;
          case 11:
            FastLED.clear();
            DrawExplosion();
            break;
          case 12:
            EVERY_N_MILLIS(40){
              DrawRipples();
            }
            break;
          case 13:
            LightningStorm();
            break;
          case 14:
            DrawMatrixEffect();
            break;
          case 15:
            ZigZag(20,10);
            break;
          case 16:
            ChangePalettePeriodically2();
            EVERY_N_MILLISECONDS(10){
              if(!drawVariant){
                DrawRazBounceWithMelt();
              }else DrawRazBounceWithExplodeAll();
            }
            break;
          case 17:
            ChangePalettePeriodically2();
            FastLED.clear();
            if(drawVariant) DrawExplosion(); //Explosions every 2nd cycle
            DrawClock();                        
            break;
          case 18:
            ChangePalettePeriodically2();
            EVERY_N_MILLISECONDS(75){
              DrawMaxSwirls();
            }
            break;
          case 19:
            ChangePalettePeriodically2();
            EVERY_N_MILLISECONDS(75){
              if(!drawVariant){
                DrawSwirlsRandom();
              }else DrawAllSwirlPatterns();
            }
            break;
          case 20:
            Snake::draw();
            break;
          case 21:
            LavaLake();
            break;
          case 22:
            DrawMagma();
            break;
          case 23:
            if(!drawVariant){
                Hypnotic::draw();
              }else ColorPlasma();
            break;
          case 24:
            PoolNoise();
            break;
          case 25:
            DrawCrazyBees();
            break;
          case 26:
            Circles::draw();
            break;
         default:
            comet1.DrawComet(0);    // DrawComet(StripNumber)
            comet2.DrawComet(1);
            comet3.DrawComet(2);
            comet4.DrawComet(3);
      }
    }  
    FastLED.setBrightness(g_Brightness);    // Set Brightness Scale before
    FastLED.delay(1);                       // Delay and Show Leds (part of delay function)
  }
}// End Loop

void PowerTest(){
  int testStrips = 4;
  for(int x=0; x<testStrips; x++){
    for(int y=0; y<NUM_LEDS; y++){
      g_LEDs[x][y] = CRGB(255,255,0);
    }
  }
}

void CheckPalettes(){   // For Testing Palettes
        fill_palette(g_LEDs[0],NUM_LEDS,0,4,BlueFire_p,250,LINEARBLEND);
        fill_palette(g_LEDs[1],NUM_LEDS,0,4,HeatColors_p,250,LINEARBLEND);

}

void ClockCounter(){
    clockCount0++;
    if (clockCount0 > 9) {  // Minutes
        clockCount0 = 0;
        clockCount10++;
    }
    if (clockCount10 == 6) { // Hours
        clockCount10 = 0;
        clockCount100++;
    }
    if (clockCount100 == 3 && clockCount1000 == 1) { // 12 Hours
        clockCount100 = 1;
        clockCount1000 = 0;
    } 
    if (clockCount100 == 10) { // 10 Hours
        clockCount100 = 0;
        clockCount1000++;
    }  
}