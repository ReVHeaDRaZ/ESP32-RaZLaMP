#pragma once
#include <Arduino.h>

// Clock Variables for Mins and Hrs
byte clockCount0    = 8;
byte clockCount10   = 4;
byte clockCount100  = 6;
byte clockCount1000 = 0;

void ClockCounter(){
    clockCount0++;
    if (clockCount0 > 9) {  // Minutes
        clockCount0 = 0;
        clockCount10++;
    }
    if (clockCount10 == 6) { // Hours
        clockCount10 = 0;
        clockCount100++;
    }
    if (clockCount100 == 3 && clockCount1000 == 1) { // 12 Hours
        clockCount100 = 1;
        clockCount1000 = 0;
    } 
    if (clockCount100 == 10) { // 10 Hours
        clockCount100 = 0;
        clockCount1000++;
    }  
}