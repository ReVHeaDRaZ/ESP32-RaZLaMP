#pragma once

#include "Marquee.h"
#include "Twinkle.h"
#include "Comet.h"
#include "Bounce.h"
#include "Fire.h"
#include "Fireworks.h"
#include "Ripples.h"
//#include "Lightning.h"
#include "LightningStorm.h"
#include "MatrixEffect.h"
#include "LetterBounce.h"
#include "Swirl.h"
#include "Snakes.h"
#include "RadialFire.h"
#include "NoisePatterns.h"
#include "Magma.h"
#include "CrazyBees.h"
#include "Circles.h"

#define NUM_PATTERNS  27          // Number of Patterns
#define PATTERN_TIME  30          // Time before Pattern Change

int Pattern = 17;                 // For Pattern Selection-Starts on 17 to Draw "RaZLamp"/ 50 For Testing
bool autoPattern = true;

bool SoundReactive = 0;           // For Selecting Sound Reactive Patterns

bool drawVariant = false;         // To change from to pattern variants every cycle of patterns
bool nightMode = false;
int oldPattern = 9;               // To store last pattern when switching to nightmode
int oldBrightness = 200;          // To store brightness when switching to nightmode


// Hue Variables for basic patterns
  uint8_t initialHue = 0;
  uint8_t initialHue2 = 0;
  const uint8_t deltaHue = 16;
  const uint8_t hueDensity = 4;

void handlePatterns(){
        if(autoPattern){
      EVERY_N_SECONDS(PATTERN_TIME)           // Auto Pattern Change
      {
        FastLED.clear();
        Pattern++;
        if(SoundReactive){
          if (Pattern > 7) Pattern = 0;
        }
        else if (Pattern > NUM_PATTERNS) {
          drawVariant = !drawVariant;
          Pattern = 0;
        }
      }
    }
    
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
            DrawTwinkleFade();
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
            // Where auto pattern function was
            // Now empty ready for another pattern
            Pattern++;
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
                DrawRazBounceWithMeltAll();
              }else 
              DrawRazBounceWithExplodeAll();
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
}