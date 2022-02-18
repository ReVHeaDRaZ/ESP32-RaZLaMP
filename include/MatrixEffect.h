// Multi line matrix effect
// By Ryan Owens

#include <Arduino.h>
#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>

#define maxMatrix 4

struct MatrixEffect {
  float  speed = float(random16(30, (NUM_LEDS)-10)) / (NUM_LEDS);  // Random speed
  float  ipos = NUM_LEDS-1;                                        // Position of Head Led
  uint8_t  endpos = random16(1, NUM_LEDS-(NUM_LEDS/4));            // Random end position
  uint8_t  lastpos;
  int stripNumber = 0;

  void InitMatrix(){
    speed = float(random16(30, (NUM_LEDS)-10)) / (NUM_LEDS);        // Random speed
    ipos = NUM_LEDS-1;                                              // Position of Head Led
    endpos = random16(1, NUM_LEDS-(NUM_LEDS/4));                    // Random end position
    stripNumber = random(0,NUM_STRIPS);
  }
  void MoveMatrix(){
    for(int i=NUM_LEDS-1; i > endpos; i--){
      //g_LEDs[int(ipos)] += ColorFromPalette(currentPalette, 1, 255, currentBlending);               // Draw Head
      g_LEDs[stripNumber][int(ipos)] += CHSV(122,100,12);
      if(ipos<NUM_LEDS-1) {
        g_LEDs[stripNumber][int(ipos+1)] = ColorFromPalette(currentPalette, random8(0,16), random8(5,50), currentBlending);   // Draw Trail
        if(random8(255)>244){ g_LEDs[stripNumber][int(ipos)+1] = CRGB(0,0,0);     // Make random blanks on tail
        }
      }
    }
    ipos-=speed;
  }
}; // end of matrixeffect struct

MatrixEffect matrix[maxMatrix];

void DrawMatrixEffect(){
  currentPalette = ForestColors_p;
  for (int i = 0; i < maxMatrix; i++) {                     // Reset Position and random speed at end position
    if (matrix[i].ipos <= matrix[i].endpos) {               // and make head same as tail for fade to work
      g_LEDs[matrix[i].stripNumber][int(matrix[i].ipos+1)] = ColorFromPalette(currentPalette, random8(0,16), random8(5,50), currentBlending);
      matrix[i].lastpos = matrix[i].endpos;
      matrix[i].InitMatrix();
    }
  }
  for (int i = 0; i < maxMatrix; i++) {
    matrix[i].MoveMatrix();
  }
  EVERY_N_MILLISECONDS(150){
  for(int s=0; s<NUM_STRIPS;s++)
    fadeToBlackBy(g_LEDs[s],NUM_LEDS,1);
  }
  delay(10);
}