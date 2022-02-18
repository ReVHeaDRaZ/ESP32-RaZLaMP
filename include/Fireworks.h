// FireWorks Effect by Ryan Owens
// Original idea found at Kodekat84/FastLED-Stuff

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

#define NUM_SPARKS 36 // max number of sparks (could be NUM_LEDS / 2);
#define MAX_FIREWORKS 4 // Max number of fireworks at once

CRGBPalette16 fireworksPalette(RainbowColors_p); // Palette for fireworks

struct Firework {

float sparkPos[NUM_SPARKS];
float sparkVel[NUM_SPARKS];
float sparkCol[NUM_SPARKS];
float flarePos;
float flareVel; 
float gravity = -.0035; // m/s/s
float flareBrightness = 1;
float dying_gravity;
int nSparks;
uint8_t colorIndex;
bool fireworksExists = false;
bool exploding = false;
int stripNumber = 0;

// Send up a flare
void flareInit() {
 if(!fireworksExists){

  flarePos = 0;
  flareVel = float(random16(26, (NUM_LEDS)-25)) / (NUM_LEDS);  // trial and error to get reasonable range  70-56  80-80 130-211
  flareBrightness = 1;
  stripNumber = random(0,NUM_STRIPS);

  // initialize launch sparks
    for (int i = 0; i < 5; i++) {
      sparkPos[i] = 0; 
      sparkVel[i] = (float(random8()) / 255) * (flareVel / 3); // random around 33% of flare velocity
      sparkCol[i] = sparkVel[i] * 1000; 
      sparkCol[i] = constrain(sparkCol[i], 0, 255);
    }
  fireworksExists = true;
  }
}

  void FlareDraw() {
     // sparks
    for (int i = 0; i < 20; i++) {
      sparkPos[i] += sparkVel[i];
      sparkPos[i] = constrain(sparkPos[i], 0, (NUM_LEDS-1));
      sparkVel[i] += gravity;
      sparkCol[i] += -.8;
      sparkCol[i] = constrain(sparkCol[i], 0, 255);
      g_LEDs[stripNumber][int(sparkPos[i])] = HeatColor(sparkCol[i]);
      g_LEDs[stripNumber][int(sparkPos[i])] %= 50; // reduce flareBrightness to 50/255
    }
    // flare
    g_LEDs[stripNumber][int(flarePos)] = CHSV(0, 0, int(flareBrightness * 255));
    flarePos += flareVel;
    flareVel += gravity;
    flareBrightness *= .985;
  }
 
/*
 * Explode!
 * Explosion happens where the flare ended.
 * Size is proportional to the height if Fireworks.
 */
void explodeInit(float position, bool endpoint,bool fireworkexplosion) {
  //Serial.println("EXPLODE Loop Start");
  if(endpoint)
    nSparks = 8;
  else if(fireworkexplosion){
    nSparks = position / 4; // works out to look about right
  }else nSparks = 15;

  // initialize sparks
  for (int i = 0; i < nSparks; i++) {
    sparkPos[i] = position; 
    sparkVel[i] = (float(random16(0, 20000)) / 10000.0) - 1.0; // from -1 to 1
     sparkCol[i] = random8(200, 255);
    if(fireworkexplosion)
      sparkVel[i] *= (position / NUM_LEDS-1)*.7; // proportional to height
      else sparkVel[i] *= .5;
  }
  sparkCol[0] = 255;          // this will be our known spark
  dying_gravity = gravity;
  colorIndex = random8();   // Random Color selection
}

  void explodeLoop(bool fireworkexplosion){ // With Firework or Explosion Condition
    const int deltaHue  = 4;
    static byte hue = HUE_RED;
    float c1 = 100; // Color over life thresholds
    float c2 = 50;

  if(sparkCol[0] > c2/128) { // as long as our known spark is lit, work with all the sparks
    if(fireworkexplosion) dying_gravity *= .990; // if fireworks as sparks burn out they fall slower
      else dying_gravity = 0;
    for (int i = 0; i < nSparks; i++) {
      hue += deltaHue;
      sparkPos[i] += sparkVel[i];
      sparkPos[i] = constrain(sparkPos[i], 0, NUM_LEDS-1);
      sparkVel[i] += dying_gravity;
      if(fireworkexplosion)
        sparkCol[i] *= .99;
        else sparkCol[i] *= .98;
      sparkCol[i] = constrain(sparkCol[i], 0, 255); // red cross dissolve
      g_LEDs[stripNumber][int(sparkPos[i])].setHue(hue);

      if(sparkCol[i] > c1) { // fade white to yellow and colorIndex
        g_LEDs[stripNumber][int(sparkPos[i])] = CRGB(255, 255, (255 * (sparkCol[i] - c1)) / (255 - c1));
        if(fireworkexplosion){
          EVERY_N_MILLIS(1){g_LEDs[stripNumber][int(sparkPos[i])] = ColorFromPalette(fireworksPalette, colorIndex, 250, LINEARBLEND);}
        }
      }
      else if (sparkCol[i] < c2) { // fade from red to black and colorIndex
        g_LEDs[stripNumber][int(sparkPos[i])] = CRGB((255 * sparkCol[i]) / c2, 0, 0);
        if(fireworkexplosion){
          g_LEDs[stripNumber][int(sparkPos[i])] += ColorFromPalette(fireworksPalette, colorIndex, (255 * sparkCol[i]) / c2, LINEARBLEND);
          EVERY_N_MILLIS(3){ g_LEDs[stripNumber][int(sparkPos[i])].setHue(hue); }  // Twinkle
        }
      }
      else { // fade from yellow to red and colorIndex
        g_LEDs[stripNumber][int(sparkPos[i])] = CRGB(255, (255 * (sparkCol[i] - c2)) / (c1 - c2), 0);
        if(fireworkexplosion){
          g_LEDs[stripNumber][int(sparkPos[i])] += ColorFromPalette(fireworksPalette, colorIndex, (255 * (sparkCol[i] - c2)) / (c1 - c2), LINEARBLEND);
          EVERY_N_MILLIS(1){g_LEDs[stripNumber][int(sparkPos[i])] = ColorFromPalette(fireworksPalette, colorIndex, 250, LINEARBLEND);}
          EVERY_N_MILLIS(1){ g_LEDs[stripNumber][int(sparkPos[i])].setHue(hue); }  // Twinkle
        }
    }
  } 
  } else {
    fireworksExists = false;
    exploding = false;
  }
}//end Explode loop

}; // End of Struct


Firework fireworks[MAX_FIREWORKS];  //Object for Fireworks
Firework explosion;                 //Object for Single Explosion
Firework explosionAtPoint;          //Object for Single Explosion at a point
Firework explosionAtPoint2;

void DrawFireworks() {

  for (int i = 0; i < MAX_FIREWORKS; i++) {                                 // Check to see if firework has expired, and if so, create a new one.
    if (random8() > 224 && !fireworks[i].fireworksExists) {                 // Randomly create a new firework if this one has expired.
      fireworks[i].flareInit();                                             // Initialize the flare array.
    }
  }
  
  for (int i = 0; i < MAX_FIREWORKS; i++) {                                 // If Firework exists
    if (fireworks[i].fireworksExists) {
      
      if(fireworks[i].flareVel >= -0.2) {                                   // Draw Flare until velocity is -0.2
        fireworks[i].FlareDraw();
      } else if(!fireworks[i].exploding){                                   // Then initialise Explosion
          fireworks[i].explodeInit(fireworks[i].flarePos,false,true);
          fireworks[i].exploding = true;
        }
        else if (fireworks[i].exploding){                                   // Draw explosion if Exploding
          fireworks[i].explodeLoop(true);
        }
    }
      
  }

} // End DrawFireworks

void DrawExplosion(){
  if(!explosion.exploding){                                   // Initialise Single Explosion
          explosion.explodeInit(random(NUM_LEDS/4,(NUM_LEDS)-(NUM_LEDS/4)),false,false);               // With Random position
          explosion.stripNumber = random8(0,NUM_STRIPS);
          explosion.exploding = true;
        }
        else if (explosion.exploding){                        // Draw explosion if Exploding
          explosion.explodeLoop(false);
        }
}

void InitExplosionAtEndPoint(int point, int strip){     // Initialise Single Explosion at an End
     if(!explosionAtPoint.exploding){ 
      explosionAtPoint.explodeInit(point,true,false);                
       for (int i = 0; i < explosionAtPoint.nSparks; i++) {
          explosionAtPoint.sparkVel[i] = (float(random16(0, 1000)) / 1000.0) - 1.0;
          explosionAtPoint.sparkVel[i] *=.2;} // from 0 to -.2 with less range
      explosionAtPoint.stripNumber = strip;
      explosionAtPoint.exploding = true;
     }
}
void InitExplosionAtStartPoint(int point, int strip){     // Initialise Single Explosion at an End
     if(!explosionAtPoint2.exploding){ 
      explosionAtPoint2.explodeInit(point,true,false);
      for (int i = 0; i < explosionAtPoint2.nSparks; i++) {
         explosionAtPoint2.sparkVel[i] = (float(random16(0, 1000)) / 1000.0);
         explosionAtPoint2.sparkVel[i] *=.2;} // from 0 to .2 with less range               
      explosionAtPoint2.stripNumber = strip;
      explosionAtPoint2.exploding = true;
     }
}

void ZigZag(uint8_t speed, uint8_t zigLength){
  static uint8_t ledIndex =   0;
  static int8_t stripIndex = 0;
  static uint8_t time = 0;
  static int8_t direction = 1;
  static int8_t stripDirection = 1;
  static uint8_t colorI = 1;
  currentPalette = RainbowColors_p;
  
  if (explosionAtPoint.exploding){                        // Draw end explosion if Exploding
          for(int j=0; j<NUM_SPARKS; j++)
            g_LEDs[explosionAtPoint.stripNumber][int(explosionAtPoint.sparkPos[j])]= CRGB(0,0,0);
          explosionAtPoint.explodeLoop(false);
        }
  if (explosionAtPoint2.exploding){                        // Draw start explosion if Exploding
          for(int j=0; j<NUM_SPARKS; j++)
            g_LEDs[explosionAtPoint2.stripNumber][int(explosionAtPoint2.sparkPos[j])]= CRGB(0,0,0);
          explosionAtPoint2.explodeLoop(false);
        }
  
  EVERY_N_MILLISECONDS(speed){
    g_LEDs[stripIndex][ledIndex] += ColorFromPalette(currentPalette,colorI += 1,255,currentBlending);
    ledIndex += direction;
    time++;
  
    if(time == zigLength && ledIndex > 0){
      stripIndex+=stripDirection;
      time=0;
    } else if (time > zigLength) time = 0;
  
  if(stripIndex == NUM_STRIPS){
    stripIndex = NUM_STRIPS-1; 
    stripDirection *= -1;
    }
  
  if(stripIndex < 0){
    stripIndex = 0; 
    stripDirection *= -1;
  }
  
    if(ledIndex >= NUM_LEDS-1 || ledIndex < 1) {
      direction *= -1;
      if(ledIndex >= NUM_LEDS-1)
        InitExplosionAtEndPoint(int(ledIndex-1),int(stripIndex));
      if(ledIndex < 2)
        InitExplosionAtStartPoint(int(ledIndex+1),int(stripIndex));
    }
  }
  
  for(int s=0;s<NUM_STRIPS;s++) fadeToBlackBy(g_LEDs[s],NUM_LEDS,10);
}