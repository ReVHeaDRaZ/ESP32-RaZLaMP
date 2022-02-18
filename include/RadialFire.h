//RadialFire by Ryan Owens
//Original Idea from Stepko and Sutaburosu at Soulmate LEDs
//23/12/21

#include <Arduino.h>
#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];


bool setupm = 1;
#define C_X (NUM_STRIPS / 2)
#define C_Y (NUM_LEDS / 2)
byte XY_angle[NUM_STRIPS][NUM_LEDS];
byte XY_radius[NUM_STRIPS][NUM_LEDS];

void RadialFire() {

  if (setupm) {
    setupm = 0;
    for (int8_t x = -C_X; x < C_X+(NUM_STRIPS%2); x++) {
      for (int8_t y = -C_Y; y < C_Y+(NUM_LEDS%2); y++) {
        // XY_angle[x + C_X][y + C_Y] = atan2(y, x) * (180. / 2. / PI) * LED_COLS;

        XY_radius[x + C_X][y + C_Y] = hypot(x, y); //thanks Sutaburosu
      }
    }

     for (int x = 0; x < NUM_STRIPS; x++) {
       for (int y = 0; y < NUM_LEDS; y++) {
        byte angle = (atan2((x-C_X), (y-C_Y)) / PI)*128+127;  //thanks ldirko 
        XY_angle [x][y] = angle;
       }
     }

  }

  static byte scaleX = 8; //16
  static byte scaleY = 1; //1

  static byte speed = 24;
  static uint32_t t;
  t += speed;
  for (uint8_t x = 0; x < NUM_STRIPS; x++) {
    for (uint8_t y = 0; y < NUM_LEDS; y++) {
      byte angle = XY_angle[x][y];
      byte radius = XY_radius[x][y];
      int16_t Bri = inoise8(angle*scaleX, (radius * scaleY)-t ) - radius * (255 / NUM_LEDS);
      byte Col = Bri;
      if (Bri < 0) Bri = 0;
      if (Bri != 0) Bri = 256 - (Bri * 0.2);
      nblend(g_LEDs[x][y], ColorFromPalette(HeatColors_p, Col, Bri), speed);
    }
  }

  delay(20);
}