//// ----------------------------- Crazy bees ------------------------------
//Original Idea (c)stepko ReWritten for FastLED Array of arrays by Ryan Owens
//Another version there: https://editor.soulmatelights.com/gallery/655-space-racer
#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

bool beesetup = true; 
#define NUM_BEE 2       // Number of bees
int beePosX[NUM_BEE];
int beePosY[NUM_BEE];
int beeAimX[NUM_BEE];
int beeAimY[NUM_BEE];
const uint8_t beeSpeed = 1;

byte beeHue[NUM_BEE];
void beeAimchange(byte id){
beeAimX[id]=random(0,(NUM_STRIPS-1)*4);
beeAimY[id]=random(0,(NUM_LEDS-1)*4);
beeHue[id] = random8();
}

void BeeSetUp(){
for(byte i = 0; i < NUM_BEE; i++){
beePosX[i]=random(0,(NUM_STRIPS-1)*4);
beePosY[i]=random(0,(NUM_LEDS-1)*4);
beeAimchange(i);
}
}
void DrawCrazyBees() {
  if (beesetup){
    beesetup = false;
    BeeSetUp();
  }
  for(byte s=0;s<NUM_STRIPS;s++)
    fadeToBlackBy(g_LEDs[s], NUM_LEDS, 20);
  
  for(byte i = 0; i < NUM_BEE; i++){
  if(round(beePosX[i]/4)>round(beeAimX[i]/4)){ beePosX[i]-=beeSpeed;}
  if(round(beePosY[i]/4)>round(beeAimY[i]/4)){ beePosY[i]-=beeSpeed;}
  if(round(beePosX[i]/4)<round(beeAimX[i]/4)){ beePosX[i]+=beeSpeed;}
  if(round(beePosY[i]/4)<round(beeAimY[i]/4)){ beePosY[i]+=beeSpeed;}
  if(round(beePosX[i]/4)==round(beeAimX[i]/4) && round(beePosY[i]/4)==round(beeAimY[i]/4)){beeAimchange(i);} 
  
  drawPixelXY(beePosX[i]/4,beePosY[i]/4,CHSV(beeHue[i],60,255));
  drawPixelXY(beeAimX[i]/4+1,beeAimY[i]/4,CHSV(beeHue[i],255,255));
  drawPixelXY(beeAimX[i]/4,beeAimY[i]/4+1,CHSV(beeHue[i],255,255));
  drawPixelXY(beeAimX[i]/4-1,beeAimY[i]/4,CHSV(beeHue[i],255,255));
  drawPixelXY(beeAimX[i]/4,beeAimY[i]/4-1,CHSV(beeHue[i],255,255));
  } 
  for(byte s=0; s<NUM_STRIPS;s++){blur1d(g_LEDs[s], NUM_LEDS, 10);}
}