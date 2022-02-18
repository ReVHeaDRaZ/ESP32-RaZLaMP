//----------------------------------------------------
//Comet Effect - Based on Dave Plummers code written as a class and with Beat Detect
//----------------------------------------------------

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

struct Comet {

const byte fadeAmt = 128;
int cometSize = 8;
const int deltaHue  = 4;
byte hue = HUE_RED;
int iDirection = 1;
int iPos = 0;

Comet(int direction, int position, int size)
{
  iDirection = direction;
  iPos = position;
  cometSize = size;
}

void DrawComet(int s)     // DrawComet(stripnumber)
{
    
    hue += deltaHue;

    iPos += iDirection;
    if (iPos == (NUM_LEDS - cometSize) || iPos == 0)
        iDirection *= -1;     
      for (int i = 0; i < cometSize; i++){
        g_LEDs[s][iPos + i].setHue(hue);
      }
    
      // Randomly fade the LEDs
      for (int j = 0; j < NUM_LEDS; j++)
        if (random(10) > 5){
            g_LEDs[s][j] = g_LEDs[s][j].fadeToBlackBy(fadeAmt);
        }
 }
    
};

void PulseComet()
{
    const byte fadeAmt = 64;
    const int cometSize = 10;
    int iPos = beatsin16(10, 0, NUM_LEDS-cometSize);
    int BPM = 64;               // ColorShift BPM
    byte hue = beatsin8(BPM);
      
    for (int s = 0; s < NUM_STRIPS; s++){
      iPos = beatsin16((s+1)*10, 0, NUM_LEDS-cometSize);
      for (int i = iPos; i < iPos + cometSize; i++)
        g_LEDs[s][i] = CHSV(hue, 255, hue);
      
      for (int j = 0; j < NUM_LEDS; j++)
        if (random(10) > 5)
          g_LEDs[s][j] = g_LEDs[s][j].fadeToBlackBy(fadeAmt);
    }
  }

void BeatPulseComet(uint8_t paletteNumber)
{
    const byte fadeAmt = 128;
    const int cometSize = 25;
    int iPos = beatsin16(10, 0, NUM_LEDS-cometSize);
    int BPM = 15;                     // ColorShift BPM
    byte hue = beatsin8(BPM);
    uint8_t brightness = 10;          // Initial Brightness before beat pulse
    
    for (int i=0; i<NUM_BANDS; i++){
    bandValues[i] = bandValues[i] / AMPLITUDE;
    if (bandValues[i] > TOP) bandValues[i] = TOP;
    }
    if(bandValues[0]>60) brightness = bandValues[0];
    
    switch(paletteNumber)
    {
      case 0:
        currentPalette = RainbowColors_p;
        break;
      case 1:   
        currentPalette = HeatColors_p;
        // Small amount of averaging between frames
        bandValues[0] = ((oldBarHeights[0] * 1) + bandValues[0]) / 2;
        hue = bandValues[0] + 10;
        if (hue>200) hue = 240;
        if (hue<20)  hue = 20;
        oldBarHeights[0] = bandValues[0]; // Save for averaging
        break;
      default:
        currentPalette = RainbowColors_p;
    }
    
    for (int s = 0; s < NUM_STRIPS; s++){
      iPos = beatsin16((s+1)*10, 0, NUM_LEDS-cometSize);
      for (int i = iPos; i < iPos + cometSize; i++)
        g_LEDs[s][i] = ColorFromPalette(currentPalette,hue,brightness,currentBlending);
      
      for (int j = 0; j < NUM_LEDS; j++)
        if (random(10) > 5)
          g_LEDs[s][j] = g_LEDs[s][j].fadeToBlackBy(fadeAmt);
    }
  }
