#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];


class Circle {
public:
  float thickness = 1.0;
  long startTime;
  uint16_t offset;
  int16_t centerX;
  int16_t centerY;
  int hue;
  int bpm = random(10,30);

  void move() {
    centerX = random(0, NUM_STRIPS);
    centerY = random(0, NUM_LEDS);
  }

  void reset() {
    startTime = millis();
    centerX = random(0, NUM_STRIPS);
    centerY = random(0, NUM_LEDS);
    hue = random(0, 255);
    offset = random(0, 60000 / bpm);
  }

  float radius() {
    float radius = beatsin16(10, 0, 400, offset) / 100.0;
    return radius;
  }
};

#define NUMBER_OF_CIRCLES 10

namespace Circles {
  Circle circles[NUMBER_OF_CIRCLES] = {};

  void drawCircle(Circle circle) {
    int16_t centerX = circle.centerX;
    int16_t centerY = circle.centerY;
    int hue = circle.hue;
    float radius = circle.radius();

    int16_t startX = centerX - ceil(radius);
    int16_t endX = centerX + ceil(radius);
    int16_t startY = centerY - ceil(radius);
    int16_t endY = centerY + ceil(radius);

    for (int16_t x = startX; x < endX; x++) {
      for (int16_t y = startY; y < endY; y++) {
        if (x < 0 || x > NUM_STRIPS-1 || y < 0 || y > NUM_LEDS-1)
          continue;
        double distance = sqrt(sq(x - centerX) + sq(y - centerY));
        if (distance > radius)
          continue;

        uint16_t brightness;
        if (radius < 1) { // last pixel
          brightness = 255.0 * radius;
        } else {
          double percentage = distance / radius;
          double fraction = 1.0 - percentage;
          brightness = 255.0 * fraction;
        }
        g_LEDs[x][y] += CHSV(hue, 255, brightness);
      }
    }
  }

  bool setup = false;

  void draw() {
    if (!setup) {
      for (int i = 0; i < NUMBER_OF_CIRCLES; i++) {
        circles[i].reset();
      }
      setup = true;
    }
    FastLED.clear();

    for (int i = 0; i < NUMBER_OF_CIRCLES; i++) {
      if (circles[i].radius() < 0.001) {
        circles[i].move();
      }
      drawCircle(circles[i]);
    }
    //for(byte s=0; s<NUM_STRIPS;s++){blur1d(g_LEDs[s], NUM_LEDS, 10);}
  }
}; // namespace Circles
