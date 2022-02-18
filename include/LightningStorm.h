#include <Arduino.h>
#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>

const TProgmemRGBPalette16 StormColors_p FL_PROGMEM =
{   
  CRGB::DarkSlateGray, CRGB::DarkSlateGray, CRGB::DarkSlateBlue, CRGB::DarkBlue,
  CRGB::DarkSlateGray, CRGB::Grey, CRGB::DarkSlateGray, CRGB::DarkBlue,
  CRGB::DarkSlateGray, CRGB::DarkBlue, CRGB::DarkBlue, CRGB::DarkSlateGray,
  CRGB::DarkSlateGray, CRGB::Grey, CRGB::DarkGray, CRGB::Grey
};
const TProgmemRGBPalette16 StormColors2_p FL_PROGMEM =
{   
   CRGB::DarkSlateGray, CRGB::DarkBlue, CRGB::DarkBlue, CRGB::DarkSlateGray,
  CRGB::DarkSlateGray, CRGB::Grey, CRGB::DarkGray, CRGB::Grey,
  CRGB::DarkSlateGray, CRGB::DarkSlateGray, CRGB::DarkSlateBlue, CRGB::DarkBlue,
  CRGB::DarkSlateGray, CRGB::Grey, CRGB::DarkSlateGray, CRGB::DarkBlue
};

CRGBPalette16 LightningPalette(StormColors_p);
CRGBPalette16 LightningTargetPalette(StormColors2_p);

void LightningStorm(){// Lightning
    //Lightning Varibles
    uint8_t         lightningfreq = 100;                                 // controls the interval between strikes
    uint8_t         lightningflashes = 6;                                //the upper limit of lightningflashes per strike
    unsigned int    lightningdimmer = 1;
    uint8_t         lightningstart;                                      // Starting location of a flash
    uint8_t         lightninglength;                                     // Length of a flash
    static uint8_t  paletteNumber = 0;
    static int      movepalettespeed = 1;
    static uint8_t  clouddirection = 1;
    //------------------------------------------------

    EVERY_N_MILLIS(random8(lightningfreq)*100){
      lightningstart = random8(NUM_LEDS-1);                                         // Determine starting location of flash
      lightninglength = random8(NUM_LEDS-lightningstart);                         // Determine length of flash (not to go beyond NUM_LEDS-1)
  
      for (int flashCounter = 0; flashCounter < random8(3,lightningflashes); flashCounter++) {
        if(flashCounter == 0) lightningdimmer = 5;                                  // the brightness of the leader is scaled down by a factor of 5
        else lightningdimmer = random8(1,3);                                        // return strokes are brighter than the leader
        for(int s=0; s<NUM_STRIPS; s++){
          fill_palette(g_LEDs[s],NUM_LEDS-1,movepalettespeed,2,LightningPalette,25,LINEARBLEND);
          fill_solid(g_LEDs[s]+lightningstart,lightninglength,CHSV(205, 0, 255/lightningdimmer));
          FastLED.show();                                                             // Show a section of Lightning and Bright Sky LED's
        }
          delay(random8(4,10));                                                       // each flash only lasts 4-10 milliseconds
        for(int s=0; s<NUM_STRIPS; s++){ 
          fill_palette(g_LEDs[s],NUM_LEDS-1,movepalettespeed,2,LightningPalette,10,LINEARBLEND);      // Clear the section of LED's
          FastLED.show();
        }
        if (flashCounter == 0) delay (150);                       // longer delay until next flash after the leader
    
        delay(50+random8(100));                                   // shorter delay between strokes  
      } // end of for
    }// End Lightning Delay
    
    EVERY_N_MILLIS(150){                                          // Slowly move clouds
      movepalettespeed += clouddirection;
      for(int s=0; s<NUM_STRIPS; s++) { 
        fill_palette(g_LEDs[s],NUM_LEDS-1,movepalettespeed,2,LightningPalette,10,LINEARBLEND);
        blur1d(g_LEDs[s],NUM_LEDS,64);
      }
    }
    EVERY_N_SECONDS(random8(25, 40)){                              // Reverse clouds every 25-40sec
    clouddirection *= -1;
    }
    EVERY_N_MILLISECONDS(30) {                                     // Smooth palette transitioning runs continuously.
    uint8_t maxChanges = 5; 
      nblendPaletteTowardPalette(LightningPalette, LightningTargetPalette, maxChanges);  
    }
    EVERY_N_SECONDS(5){                              // Reverse clouds every 25-40sec
    if (paletteNumber == 0){
      LightningTargetPalette = StormColors_p;
      paletteNumber = 1;
    }else if (paletteNumber == 1){
      LightningTargetPalette = StormColors2_p;
      paletteNumber = 0;
    }
    }
  }