#include <Arduino.h>
#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>


void Lightning(){// Lightning
    //Lightning Varibles
    uint8_t       lightningfreq = 100;                                       // controls the interval between strikes
    uint8_t       lightningflashes = 8;                                          //the upper limit of lightningflashes per strike
    unsigned int  lightningdimmer = 1;
    uint8_t       lightningstart;                                             // Starting location of a flash
    uint8_t       lightninglength;                                               // Length of a flash
    //------------------------------------------------

    EVERY_N_MILLIS(random8(lightningfreq)*100){
      lightningstart = random8(NUM_LEDS);                               // Determine starting location of flash
      lightninglength = random8(NUM_LEDS-lightningstart);                        // Determine length of flash (not to go beyond NUM_LEDS-1)
  
      for (int flashCounter = 0; flashCounter < random8(3,lightningflashes); flashCounter++) {
        if(flashCounter == 0) lightningdimmer = 5;                         // the brightness of the leader is scaled down by a factor of 5
        else lightningdimmer = random8(1,3);                               // return strokes are brighter than the leader
    
        fill_solid(g_LEDs+lightningstart,lightninglength,CHSV(255, 0, 255/lightningdimmer));
        FastLED.show();                       // Show a section of LED's
        delay(random8(4,10));                                     // each flash only lasts 4-10 milliseconds
        fill_solid(g_LEDs+lightningstart,lightninglength,CHSV(255,0,0));           // Clear the section of LED's
        FastLED.show();
    
        if (flashCounter == 0) delay (150);                       // longer delay until next flash after the leader
    
        delay(50+random8(100));                                   // shorter delay between strokes  
      } // end of for
    }// End Lightning Delay
  }