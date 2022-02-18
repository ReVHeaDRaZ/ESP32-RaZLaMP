// NOISE PATTERNS
#include <Arduino.h>
#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

//// ----------------------------- PoolNoise ------------------------------
//(c)stepko
//16.10.20
bool loadingFlag = true;
#define POOLSAT 255
#define POOLHUE 155
#define POOLSCALE 40 
#define POOLBRI (0)

uint8_t poolhue;

void N(){
   for (byte y = 0; y < NUM_LEDS; y++) {
        for (byte x = 0; x < NUM_STRIPS; x++) {
          uint8_t pixelHue8 = inoise8 (x*POOLSCALE,y*POOLSCALE,millis()/16);
 g_LEDs[x][y] = ColorFromPalette(currentPalette, pixelHue8);}
   }
   for(int s = 0; s<NUM_STRIPS;s++)
    blur1d(g_LEDs[s], NUM_LEDS, 32 );
   }


void PoolNoise() {
   if (loadingFlag)
  {loadingFlag = false;
  poolhue = POOLHUE;
  }fill_solid( currentPalette, 16, CHSV(poolhue,POOLSAT,230));
    currentPalette[9] = CHSV(poolhue,POOLSAT-60,150);
    currentPalette[8] = CHSV(poolhue,255-POOLSAT,100);
    currentPalette[7] = CHSV(poolhue,255-POOLSAT,100);
    currentPalette[6] = CHSV(poolhue,POOLSAT-60,150);
  N();
  delay(16);
  }
//------------------------------------------------------------------------

// LAVA LAKE NOISE
  void LavaLake(){
for (byte x = 0; x < NUM_STRIPS; x++) {
    for (byte y = 0; y < NUM_LEDS; y++) {
      g_LEDs[x][y] = ColorFromPalette(LavaColors_p, inoise8((x * y) - (y*NUM_STRIPS), (y * 50) + millis() / 100, millis() / 10), 255 - y * (255 / NUM_LEDS));
      
    }
  }
}

// STEPKO COLOR PLASMA
  
 uint8_t CodeR(byte x, byte y, double t) {
  
       return sin8((x - 8) * cos8((y + 20) * 4) / 4 + t);

 }
 uint8_t CodeG(byte x, byte y, double t) {
      return (sin8(x * 16 + t / 3) + cos8(y * 8 + t / 2)) / 2;

 }
 uint8_t CodeB(byte x, byte y, double t) {
       return sin8(cos8(x * 8 + t / 3) + sin8(y * 8 + t / 4) + t);
 }

 void ColorPlasma() {
   double t = millis() / 10;
   for (byte x = 0; x < NUM_STRIPS; x++) {
     for (byte y = 0; y < NUM_LEDS; y++) {
       g_LEDs[x][y].r = CodeR(x, y, t);
       g_LEDs[x][y].g = CodeG(x, y, t);
       g_LEDs[x][y].b = CodeB(x, y, t);
     }
   }
 }

 //rgb-hiphotic  
// rgb led matrix demo
//Yaroslaw Turbin 
//https://twitter.com/ldir_ko
//https://vk.com/ldirko
//https://www.reddit.com/user/ldirko/

static const uint8_t Hypnotic_exp_gamma[256] = {
0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,
1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,
4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,   6,   6,   7,   7,
7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,  11,  11,  12,  12,
12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,
19,  20,  20,  21,  21,  22,  23,  23,  24,  24,  25,  26,  26,  27,  28,
28,  29,  30,  30,  31,  32,  32,  33,  34,  35,  35,  36,  37,  38,  39,
39,  40,  41,  42,  43,  44,  44,  45,  46,  47,  48,  49,  50,  51,  52,
53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,
68,  70,  71,  72,  73,  74,  75,  77,  78,  79,  80,  82,  83,  84,  85,
87,  89,  91,  92,  93,  95,  96,  98,  99,  100, 101, 102, 105, 106, 108,
109, 111, 112, 114, 115, 117, 118, 120, 121, 123, 125, 126, 128, 130, 131,
133, 135, 136, 138, 140, 142, 143, 145, 147, 149, 151, 152, 154, 156, 158,
160, 162, 164, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187,
190, 192, 194, 196, 198, 200, 202, 204, 207, 209, 211, 213, 216, 218, 220,
222, 225, 227, 229, 232, 234, 236, 239, 241, 244, 246, 249, 251, 253, 254,
255};
namespace Hypnotic{
void draw() {

 int a = millis()/8;
 for (int x = 0; x < NUM_STRIPS; x++) {
   for (int y = 0; y < NUM_LEDS; y++) {
     g_LEDs[x][y].b=Hypnotic_exp_gamma [sin8((x-8)*cos8((y+20)*4)/4+a)];
     g_LEDs[x][y].g=Hypnotic_exp_gamma [(sin8(x*16+a/3)+cos8(y*8+a/2))/2];
     g_LEDs[x][y].r=Hypnotic_exp_gamma [sin8(cos8(x*8+a/3)+sin8(y*8+a/4)+a)];
   }
}
}
} // end of namespace