#pragma once

#include "Marquee.h"
#include "Twinkle.h"
#include "Comet.h"
#include "Bounce.h"
#include "Fire.h"
#include "Fireworks.h"
#include "Ripples.h"
//#include "Lightning.h"
#include "LightningStorm.h"
#include "MatrixEffect.h"
#include "LetterBounce.h"
#include "Swirl.h"
#include "Snakes.h"
#include "RadialFire.h"
#include "NoisePatterns.h"
#include "Magma.h"
#include "CrazyBees.h"
#include "Circles.h"

#define NUM_PATTERNS  27          // Number of Patterns
#define PATTERN_TIME  30          // Time before Pattern Change

int Pattern = 17;                 // For Pattern Selection-Starts on 17 to Draw "RaZLamp"/ 50 For Testing
bool autoPattern = true;

bool SoundReactive = 0;           // For Selecting Sound Reactive Patterns

bool drawVariant = false;         // To change from to pattern variants every cycle of patterns
bool nightMode = false;
int oldPattern = 9;               // To store last pattern when switching to nightmode
int oldBrightness = 200;          // To store brightness when switching to nightmode


// Hue Variables for basic patterns
  uint8_t initialHue = 0;
  uint8_t initialHue2 = 0;
  const uint8_t deltaHue = 16;
  const uint8_t hueDensity = 4;