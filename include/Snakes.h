// Snakes original idea By Elliott Kember on Soulmate LEDs
// adapted to a FastLED array of arrays by Ryan Owens
// + responsive number of snakes
// + response speed of snakes
// + snakes turn white when they eat the fruit
#include <Arduino.h>
#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

class Asnake {
  struct snakeIndicesStruct{uint16_t x,y;}; 
  snakeIndicesStruct snakeIndices[100];
  uint16_t snakeLength;
  int direction;
  int hue;
  int step;
  unsigned long lastStepAt = millis();
  bool dead;
  
  void die() {
    dead = true;
    headX = random(0, NUM_STRIPS);
    
    headY = random(0, NUM_LEDS);
    snakeLength = 3;
    direction = random(0, 3);
    hue = random(0, 255);
    step = 70;
  }
  
  public:
    Asnake() {
      headX = random(0, NUM_STRIPS);
      headY = random(0, NUM_LEDS);
      snakeLength = 3;
      direction = random(0, 3);
      hue = random(0, 255);
      step = 70;
      dead = false;
    }
  
  bool isBlocked(int16_t x, int16_t y) {
    
    if (g_LEDs[x][y] == CRGB(0, 255, 0)) {
      return false;
    } else if (g_LEDs[x][y] == CRGB(0, 0, 0)) {
      return false;
    } else {
      return true;
    }
  }
  
  int16_t headX = 0;
  int16_t headY = 0;
  
  void drawPixels() {
    for (uint16_t i = 0; i < snakeLength; i++) {
      if (celebrates > 0) {
        g_LEDs[snakeIndices[i].x][snakeIndices[i].y] = CRGB::White;
      } else {
        g_LEDs[snakeIndices[i].x][snakeIndices[i].y] = CHSV(hue, 255, 255);
      }
    }
  }
  
  void move(int16_t fruitX, int16_t fruitY) {
    if ((int)(millis() - lastStepAt) < step && !dead) {
      drawPixels();
      return;
    };
    
    dead = false;
    
    lastStepAt = millis();
    
    bool move = false;
    int tries = 0;
    
    bool r = random(0, 100) > 50;
    
    if (r) {
      if (fruitX > headX) {
        direction = 0; // right
      } else if (fruitY > headY) {
        direction = 1; // up
      } else if (fruitX < headX) {
        direction = 2; // left
      } else if (fruitY < headY) {
        direction = 3; // down
      } else {
        direction = random(0, 4);
      }
    } else {
      if (fruitY < headY) {
        direction = 3; // down
      } else if (fruitX < headX) {
        direction = 2; // left
      } else if (fruitY > headY) {
        direction = 1; // up
      } else if (fruitX > headX) {
        direction = 0; // right
      } else {
        direction = random(0, 4);
      }
    }
    
    while (!move && tries <= 10) {
      if (direction == 0 && headX < NUM_STRIPS - 2) {
        if (isBlocked(headX + 1, headY)) {
          move = false;
        } else {
          headX++;
          move = true;
        }
      } else if (direction == 1 && headY < NUM_LEDS - 2) {
        if (isBlocked(headX, headY + 1)) {
          move = false;
        } else {
          headY++;
          move = true;
        }
      } else if (direction == 2 && headX > 0) {
        if (isBlocked(headX - 1, headY)) {
          move = false;
        } else {
          headX--;
          move = true;
        }
      } else if (direction == 3 && headY > 0) {
        if (isBlocked(headX, headY - 1)) {
          move = false;
        } else {
          headY--;
          move = true;
        }
      } else {}
      if (!move) {
        tries++;
        direction++;
        if (direction == 4) {
          direction = 0;
        }
      }
    }
    
    if (!move || tries >= 10) {
      die();
    } else {
      
      snakeIndices[snakeLength].x = headX;
      snakeIndices[snakeLength].y = headY;
      if (fruitX == headX && fruitY == headY) {
        // we got it!
        step = step * 0.9;
        
        celebrate();
        
        if (snakeLength < 100) {
          snakeLength++;
        } else {
          move = false;
          die();
        }
      } else {
        for (int i = 0; i < snakeLength; i++) {
          snakeIndices[i] = snakeIndices[i + 1];
        }
      }
    }
    
    if (move) {
      drawPixels();
    }
    
    if (celebrates > 0) {
      celebrates--;
    }
  }
  
  int celebrates = 0;
  
  void celebrate() {
    celebrates = 3; // How long to celebrate
  }
};

#define SNAKES_COUNT 2

int limit = SNAKES_COUNT;

namespace Snake {
  Asnake snakes[SNAKES_COUNT];
  
  uint16_t fruitX = 3; // Fruit starting point
  uint16_t fruitY = 4;
  
  void resetFruit() {
    fruitX = (int) random(0, NUM_STRIPS - 2);
    fruitY = (int) random(0, NUM_LEDS - 2);
  }
  
  void draw() {
    uint8_t time = 60; // Speed
    
    EVERY_N_MILLISECONDS(time) {
      for(uint8_t s = 0; s<NUM_STRIPS; s++)
        fadeToBlackBy(g_LEDs[s], NUM_LEDS, 80); // Determines extra tail length (can still be hit)
            
      g_LEDs[fruitX][fruitY] = CRGB(0, 255, 0);
      
      for (int i = 0; i < limit; i++) {
        snakes[i].move(fruitX, fruitY);
        if (snakes[i].headX == fruitX && snakes[i].headY == fruitY) {
          resetFruit();
        }
      }
    }
  }
} // namespace Snake
