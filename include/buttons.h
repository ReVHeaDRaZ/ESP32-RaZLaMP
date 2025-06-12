#pragma once
#include "config.h"
#include "Patterns.h"

int buttonTime = 0;               // To measure how long button is pressed for
const int buttonLongPressTime = 50; // How long to register a long button press
bool buttonState = HIGH;             // For Button Active Low
bool switchState = HIGH;             // For Switch Active Low

void handleButtons();

void setupButtons(){
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    handleButtons();
}

void handleButtons(){
    buttonState=digitalRead(BUTTON_PIN);        // Read Button state
    switchState=digitalRead(SWITCH_PIN);        // Read Switch state
    
    if(switchState==LOW)                        // to select SoundReactive
        SoundReactive = 1;
    else
        SoundReactive = 0;

    EVERY_N_MILLIS(10) {
      if(buttonState==LOW)
        buttonTime++;

      if(buttonState==HIGH && buttonTime>buttonLongPressTime){ // Change AutoPattern state
        buttonTime=0;
        autoPattern=!autoPattern;
        if(autoPattern)
          DrawAutoOnText();
        else
          DrawAutoOffText();
      } else if(buttonState==HIGH && buttonTime>1){ // Increment Pattern number
        buttonTime=0;
        FastLED.clear();
        Pattern++;
        if(Pattern>NUM_PATTERNS){
          Pattern = 0;
          drawVariant = !drawVariant;
        }
      }

    } 
}