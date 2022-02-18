#include <Arduino.h>
#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>

#define maxRipples 20             // Min is 2 and value has to be divisible by two because each ripple has a left and right component. This cuts down on bouncing code.


//----------------- Ripple structure definition ----------------------------------------------------------------

struct ripple {

  uint8_t brightness;             // 0 to 255
  int8_t color;                   // 0 to 255
  int16_t pos;                    // -1 to NUM_LEDS  (up to 127 LED's)
  int8_t velocity;                // 1 or -1
  uint8_t life;                   // 0 to 20
  uint8_t maxLife;                // 10. If it's too long, it just goes on and on. . .
  uint8_t fade;                   // 192
  bool exist;                     // 0 to 1
  int stripNumber = 0;
  
  void Move() {
    
    pos += velocity;
    life++;
    
    if (pos > NUM_LEDS - 1) {                           // Bounce back from far end.
      velocity *= -1;
      pos = NUM_LEDS - 1;
    }
    
    if (pos < 0) {                                      // Bounce back from 0.
      velocity *= -1;
      pos = 0;
    }

    brightness = scale8(brightness, fade);              // Adjust brightness accordingly to strip length

    if (life > maxLife) exist = false;                  // Kill it once it's expired.
    
  } // Move()


  
  void Init(uint8_t Fade, uint8_t MaxLife) {            // Typically 216, 20
    
    pos = random8(NUM_LEDS/8, NUM_LEDS-NUM_LEDS/8);     // Avoid spawning too close to edge.
    velocity = 1;                                       // -1 or 1
    life = 0;                                           // 0 to Maxlife
    maxLife = MaxLife;                                  // 10 called
    exist = true;                                       // 0, 1
    brightness = 255;                                   // 0 to 255
    color = millis();                                   // hue;
    fade = Fade;                                        // 192 called
    stripNumber = random(0,NUM_STRIPS);

  } // Init()
  
}; // struct ripple


ripple ripples[maxRipples];

void DrawRipples() {
  currentPalette = OceanColors_p;
  for (int i = 0; i < maxRipples; i += 2) {               // Check to see if ripple has expired, and if so, create a new one.
    if (random8() > 224 && !ripples[i].exist) {           // Randomly create a new ripple if this one has expired.
      ripples[i].Init(210, 40);                           // Initialize the ripple array with Fade and MaxLife values.
      ripples[i + 1] = ripples[i];                        // Everything except velocity is the same for the ripple's other half. Position starts out the same for both halves.
      ripples[i + 1].velocity *= -1;                      // We want the other half of the ripple to go opposite direction.
    }
  }
  
  for (int i = 0; i < maxRipples; i++) {                  // Move the ripple if it exists
    if (ripples[i].exist) {
      g_LEDs[ripples[i].stripNumber][ripples[i].pos] = ColorFromPalette(currentPalette, ripples[i].color, ripples[i].brightness, LINEARBLEND);
      ripples[i].Move();
    }
  }
for (int s=0; s<NUM_STRIPS; s++)
  fadeToBlackBy(g_LEDs[s], NUM_LEDS, 100);
  
} // End DrawRipples


// Beat Detection Functions
void DrawRipplesToBeat() {
  const int threshold = 60;
  currentPalette = PartyColors_p;
  static int rippleNumber = 0;
  
  bandValues[0] = bandValues[0] / AMPLITUDE;
    if (bandValues[0] > TOP) bandValues[0] = TOP;

  // Check to see beat detected and if ripple has expired, and if so, create a new one.
    if (bandValues[0] > threshold && !ripples[rippleNumber].exist) {                                   // Randomly create a new ripple if this one has expired.
      ripples[rippleNumber].Init(210, 40);                              // Initialize the ripple array with Fade and MaxLife values.
      ripples[rippleNumber + 1] = ripples[rippleNumber];                                                // Everything except velocity is the same for the ripple's other half. Position starts out the same for both halves.
      ripples[rippleNumber + 1].velocity *= -1;                         // We want the other half of the ripple to go opposite direction.
      rippleNumber += 2;
      if(rippleNumber == maxRipples) rippleNumber = 0;
    }
  
  for (int i = 0; i < maxRipples; i++) {                                // Move the ripple if it exists
    if (ripples[i].exist) {
      g_LEDs[ripples[i].stripNumber][ripples[i].pos] = ColorFromPalette(currentPalette, ripples[i].color, ripples[i].brightness, LINEARBLEND);
      ripples[i].Move();
    }
  }
for (int s=0; s<NUM_STRIPS; s++)
  fadeToBlackBy(g_LEDs[s], NUM_LEDS, 100);
  
} // End DrawRipplesToSound

void DrawRipplesToBands(uint8_t numberOfBands) {  // Draw Ripples to frequency bands 2-3
  const int threshold = 60;
  currentPalette = PartyColors_p;
  static int rippleNumber = 0;
  
  for (int i=0; i<NUM_BANDS; i++){
    bandValues[i] = bandValues[i] / AMPLITUDE;
    if (bandValues[i] > TOP) bandValues[i] = TOP;
  }
  
  // Check to see beat detected and if ripple has expired, and if so, create a new one.
    if (bandValues[0] > (threshold+5) && !ripples[rippleNumber].exist) {                         // Randomly create a new ripple if this one has expired.
      ripples[rippleNumber].Init(210, 40);                                                   // Initialize the ripple array with Fade and MaxLife values.
      ripples[rippleNumber].pos = random8(NUM_LEDS/8, NUM_LEDS/4);
      ripples[rippleNumber].color = 60;
      ripples[rippleNumber + 1] = ripples[rippleNumber];                                     // Everything except velocity is the same for the ripple's other half. Position starts out the same for both halves.
      ripples[rippleNumber + 1].velocity *= -1;                                              // We want the other half of the ripple to go opposite direction.
      rippleNumber += 2;
      if(rippleNumber == maxRipples) rippleNumber = 0;
    }
    
    if (bandValues[3] > (threshold+50) && !ripples[rippleNumber].exist) {                     // Randomly create a new ripple if this one has expired.
      ripples[rippleNumber].Init(210, 40);                                                    // Initialize the ripple array with Fade and MaxLife values.
      if(numberOfBands<3)                                                                     // If less than 3 bands draw at end of strip
        ripples[rippleNumber].pos = random8((NUM_LEDS/2)+(NUM_LEDS/4), NUM_LEDS-(NUM_LEDS/8));
      else                                                                                    // Else draw in center
        ripples[rippleNumber].pos = random8((NUM_LEDS/2)-10, (NUM_LEDS/2)+10); 
      ripples[rippleNumber].color = 5;
      ripples[rippleNumber + 1] = ripples[rippleNumber];                                      // Everything except velocity is the same for the ripple's other half. Position starts out the same for both halves.
      ripples[rippleNumber + 1].velocity *= -1;                                               // We want the other half of the ripple to go opposite direction.
      rippleNumber += 2;
      if(rippleNumber == maxRipples) rippleNumber = 0;
    }

    if(numberOfBands>2){
      if (bandValues[6] > (threshold) && !ripples[rippleNumber].exist) {                     // Randomly create a new ripple if this one has expired.
        ripples[rippleNumber].Init(210, 40);                                                    // Initialize the ripple array with Fade and MaxLife values.
        ripples[rippleNumber].pos = random8((NUM_LEDS/2)+(NUM_LEDS/4), NUM_LEDS-(NUM_LEDS/8));
        ripples[rippleNumber].color = 90;
        ripples[rippleNumber + 1] = ripples[rippleNumber];                                      // Everything except velocity is the same for the ripple's other half. Position starts out the same for both halves.
        ripples[rippleNumber + 1].velocity *= -1;                                               // We want the other half of the ripple to go opposite direction.
        rippleNumber += 2;
        if(rippleNumber == maxRipples) rippleNumber = 0;
      }
    }
    
  for (int i = 0; i < maxRipples; i++) {                                          // Move the ripple if it exists
    if (ripples[i].exist) {
      g_LEDs[ripples[i].stripNumber][ripples[i].pos] = ColorFromPalette(currentPalette, ripples[i].color, ripples[i].brightness, LINEARBLEND);
      ripples[i].Move();
    }
  }
for (int s=0; s<NUM_STRIPS; s++)
  fadeToBlackBy(g_LEDs[s], NUM_LEDS, 100);
  
} // End DrawRipplesToBands
