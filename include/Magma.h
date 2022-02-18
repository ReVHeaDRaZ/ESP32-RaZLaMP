// ----------------- Pattern "Magma"
// Idea (c) Сотнег (SottNick) 2021
// remastered by kostyamat 
// then rewritten for FastLED Array of arrays by Ryan Owens
// https://github.com/DmytroKorniienko/FireLamp_JeeUI/tree/dev

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

#define WIDTH NUM_STRIPS
#define HEIGHT NUM_LEDS
#define N_LEDS NUM_STRIPS*NUM_LEDS
#define enlargedOBJECT_MAX_COUNT WIDTH*3 //width*2
#define SPEED_ADJ (float)N_LEDS/1024 //512

//speed control
byte magmaSpeed = 125; // 1-255 

//scale control
byte magmaScale = 100; //1-255

//control magma bursts
const byte magmaDeltaValue = 3U; // 2-12 Default 6
const byte magmaDeltaHue = 8U; 


static const TProgmemRGBPalette16 MagmaColor_p FL_PROGMEM = {CRGB::Black, 0x240000, 0x480000, 0x660000, 0x9a1100, 0xc32500, 0xd12a00, 0xe12f17, 0xf0350f, 0xff3c00, 0xff6400, 0xff8300, 0xffa000, 0xffba00, 0xffd400, 0xffffff};                      // пытаюсь сделать что-то более приличное

static double fmap(const double x, const double in_min, const double in_max, const double out_min, const double out_max){
    return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}

float randomf(float min, float max)
{
  return fmap(random(1024), 0, 1023, min, max);
}


// ----------------- "Маgма" ---------------------

class EffectMagma {
private:
    //uint16_t ff_x;
    float ff_y, ff_z;
    uint8_t step, ObjectNUM = WIDTH; 
    uint8_t shiftHue[HEIGHT];
    float trackingObjectPosX[enlargedOBJECT_MAX_COUNT];
    float trackingObjectPosY[enlargedOBJECT_MAX_COUNT];
    uint8_t trackingObjectHue[enlargedOBJECT_MAX_COUNT];
    float trackingObjectSpeedX[enlargedOBJECT_MAX_COUNT];
    float trackingObjectShift[enlargedOBJECT_MAX_COUNT];
    float speedfactor;

    void regen();
    void LeapersMove_leaper(uint8_t l);
    void LeapersRestart_leaper(uint8_t l);

public:
    void load();
    bool run();
};


void EffectMagma::load() {
  speedfactor = fmap(magmaSpeed, 1, 255, SPEED_ADJ*0.1, SPEED_ADJ);
  ObjectNUM = map(magmaScale, 1, 255, WIDTH, enlargedOBJECT_MAX_COUNT);
  regen();
}


void EffectMagma::regen() {
  for (uint8_t j = 0; j < HEIGHT; j++) {
    shiftHue[j] = map(j, 0, HEIGHT+HEIGHT/4, 255, 0); // init colorfade table Default HEIGHT+HEIGHT/4
  }


  for (uint8_t i = 0 ; i < enlargedOBJECT_MAX_COUNT ; i++) {
    LeapersRestart_leaper(i);  
    trackingObjectHue[i] = 50U;
  }
}


bool EffectMagma::run() {
  for(int s=0;s<NUM_STRIPS;s++)
    fadeToBlackBy(g_LEDs[s], NUM_LEDS, 50);

  for (uint8_t i = 0; i < ObjectNUM; i++) {
    LeapersMove_leaper(i);
    drawPixelXYF(trackingObjectPosX[i], trackingObjectPosY[i], ColorFromPalette(MagmaColor_p, trackingObjectHue[i], 255));
  }

  for (uint8_t i = 0; i < WIDTH; i++) {
    for (uint8_t j = 0; j < HEIGHT; j++) {
      g_LEDs[i][HEIGHT-1 - j] += ColorFromPalette(MagmaColor_p, qsub8(inoise8(i * magmaDeltaValue, (j + ff_y + random8(2)) * magmaDeltaHue, ff_z), shiftHue[j]), 200U); //255U
    }
  }

  ff_y += speedfactor * 1.25; //2
  ff_z += speedfactor;
  for(byte s=0; s<NUM_STRIPS;s++) blur1d(g_LEDs[s],NUM_LEDS, 32);
  return true;
}

void EffectMagma::LeapersMove_leaper(uint8_t l) {
#define GRAVITY            0.07 //0.1
  trackingObjectShift[l] -= GRAVITY * speedfactor;

  trackingObjectPosX[l] += trackingObjectSpeedX[l] * speedfactor;
  trackingObjectPosY[l] += trackingObjectShift[l] * speedfactor;

  // bounce off the ceiling?
  if (trackingObjectPosY[l] > HEIGHT + HEIGHT/4) {
    trackingObjectShift[l] = -trackingObjectShift[l];
  }
  
  // settled on the floor?
  if (trackingObjectPosY[l] <= (HEIGHT/8-1)) {
    LeapersRestart_leaper(l);
  }

  // bounce off the sides of the screen?
  if (trackingObjectPosX[l] < 0 || trackingObjectPosX[l] > WIDTH-1) {
    LeapersRestart_leaper(l);
  }

}

void EffectMagma::LeapersRestart_leaper(uint8_t l) {
  randomSeed(millis());
  // leap up and to the side with some random component
  trackingObjectSpeedX[l] = randomf(-0.25, 0.25); // 0.75
  trackingObjectShift[l] = randomf(0.70, 1.0); //.5 .85
  trackingObjectPosX[l] = randomf(0, WIDTH);
  trackingObjectPosY[l] = randomf(5, (float)HEIGHT/3-1); //4-1

  // for variety, sometimes go 100% faster
  if (random8() < 24) { //12
    trackingObjectShift[l] += trackingObjectShift[l] * 2;
  }

}

bool load = true;
EffectMagma eff;

void DrawMagma() {
  if (load) {
    eff.load();
    load = false;
  }
  eff.run();
}