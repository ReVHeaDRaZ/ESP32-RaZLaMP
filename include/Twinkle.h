//+--------------------------------------------------------------------------
//
// NightDriver - (c) 2020 Dave Plummer.  All Rights Reserved.
//
// File:        
//
// Description:
//
//   
//
// History:     Sep-15-2020     davepl      Created
//
//---------------------------------------------------------------------------

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

#define NUM_COLORS 5
static const CRGB TwinkleColors [NUM_COLORS] = 
{
    CRGB::Red,
    CRGB::Blue,
    CRGB::Purple,
    CRGB::Green,
    CRGB::Yellow
};

void DrawTwinkle()
{
    FastLED.clear(false);
    for (int s=0; s<NUM_STRIPS; s++){
        for (int i=0; i<NUM_LEDS/4; i++) 
        {
            g_LEDs[s][random(NUM_LEDS)] = TwinkleColors[random(0, NUM_COLORS)];
            FastLED.show(g_Brightness);
        }
    }
}

void DrawTwinkle2()
{
    static int passCount = 0;
    if (passCount++ == NUM_LEDS)
    {
        passCount = 0;
        FastLED.clear(false);
    }
    for(int s=0; s<NUM_STRIPS; s++){
    g_LEDs[s][random(NUM_LEDS)] = TwinkleColors[random(0, NUM_COLORS)];
    }
    delay(20);       
}