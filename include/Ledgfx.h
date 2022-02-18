// File:        ledgfx.h
//
// Description:
//
//   LED Drawing Routines from Dave's Garage Tutorial series
//    Modified for multiple strips and Pattern Functions added
//---------------------------------------------------------------------------

#pragma once

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#include <sys/time.h>                   // For time-of-day

// Utility Macros

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))
#define TIMES_PER_SECOND(x) EVERY_N_MILLISECONDS(1000/x)
// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                              // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

//inline double RandomDouble()
//{
//    return random(UINT32_MAX) / (double) UINT32_MAX;
//}

// FramesPerSecond
//
// Tracks a weighted average to smooth out the values that it calcs as the simple reciprocal
// of the amount of time taken specified by the caller.  So 1/3 of a second is 3 fps, and it
// will take up to 10 frames or so to stabilize on that value.
 
double FramesPerSecond(double seconds)
{
  static double framesPerSecond; 
  framesPerSecond = (framesPerSecond * .9) + (1.0 / seconds * .1);
  return framesPerSecond;
}

inline float RandomFloat()
{
    float r = random(1000000L) / 1000000.0f;
    return r;
}

inline double UnixTime()
{
    timeval tv = { 0 };
    gettimeofday(&tv, nullptr);
    return (double)(tv.tv_usec / 1000000.0 + (double) tv.tv_sec);
}

// FractionalColor
//
// Returns a fraction of a color; abstracts the fadeToBlack out to this function in case we
// want to improve the color math or do color correction all in one location at a later date.
CRGB ColorFraction(CRGB colorIn, float fraction)
{
  fraction = min(1.0f, fraction);
  return CRGB(colorIn).fadeToBlackBy(255 * (1.0f - fraction));
}

// Dave Plummers Draw Floating Point Y coordinate with Strip(X) select
void DrawPixels(int strip, float fPos, float count, CRGB color)
{
  // Calculate how much the first pixel will hold
  float availFirstPixel = 1.0f - (fPos - (long)(fPos));
  float amtFirstPixel = min(availFirstPixel, count);
  float remaining = min(count, FastLED.size()-fPos);
  int iPos = fPos;

  // Blend (add) in the color of the first partial pixel
  if (remaining > 0.0f)
  {
    g_LEDs[strip][iPos++] += ColorFraction(color, amtFirstPixel);
    remaining -= amtFirstPixel;
  }

  // Now draw any full pixels in the middle
  while (remaining > 1.0f)
  {
    g_LEDs[strip][iPos++] += color;
    remaining--;
  }

  // Draw tail pixel, up to a single full pixel
  if (remaining > 0.0f)
  {
    g_LEDs[strip][iPos] += ColorFraction(color, remaining);
  }
}

// PALETTE CHANGE FUNCTIONS
void ChangePalettePeriodically() {
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if(lastSecond != secondHand) {
    lastSecond = secondHand;
    if(secondHand ==  0)  { targetPalette = RainbowColors_p; }
    if(secondHand == 10)  { targetPalette = ForestColors_p; }
    if(secondHand == 20)  { targetPalette = OceanColors_p; }
    if(secondHand == 30)  { targetPalette = LavaColors_p; }
    if(secondHand == 40)  { targetPalette = CloudColors_p; }
    if(secondHand == 50)  { targetPalette = PartyColors_p; }
  }
}

void ChangePalettePeriodically2() {
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if(lastSecond != secondHand) {
    lastSecond = secondHand;
    if(secondHand ==  0)  { targetPalette = RainbowColors_p; }
    if(secondHand == 10)  { targetPalette = PartyColors_p; }
    if(secondHand == 20)  { targetPalette = ForestColors_p; }
    if(secondHand == 30)  { targetPalette = RainbowColors_p; }
    if(secondHand == 40)  { targetPalette = PartyColors_p; }
    if(secondHand == 50)  { targetPalette = ForestColors_p; }
  }
}

// Draw Floating Point XY coordinate
void drawPixelXYF(float x, float y, CRGB color)
{
  if (x < 0 || y < 0 || x > ((float)NUM_STRIPS - 1) || y > ((float)NUM_LEDS - 1)) return;
  uint8_t xx = (x - (int)x) * 255, yy = (y - (int)y) * 255, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  #define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) { 
    int16_t xn = x + (i & 1), yn = y + ((i >> 1) & 1);
    CRGB clr = g_LEDs[xn][yn];
    clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
    clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
    clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
    g_LEDs[(int)round(x)][yn] = clr; //Added round x for small width matrix
  }
}