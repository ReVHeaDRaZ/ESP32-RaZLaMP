#pragma once
#include <Arduino.h>
#include "time.h"

#define TZ_Australia_Melbourne PSTR("AEST-10AEDT,M10.1.0,M4.1.0/3")
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 36000;      // Melbourne UTC+10
const int   daylightOffset_sec = 3600;  // 1 hour DST
struct tm timeinfo;

extern byte clockCount0;
extern byte clockCount10;
extern byte clockCount100;
extern byte clockCount1000;

void setRazClockTime(){
    if(timeinfo.tm_hour == 0)
        timeinfo.tm_hour = 12;
    if(timeinfo.tm_hour > 12)
        timeinfo.tm_hour -= 12;
    clockCount0 = (timeinfo.tm_min)%10;
    clockCount10 = (timeinfo.tm_min)/10;
    clockCount100 = (timeinfo.tm_hour)%10;
    clockCount1000 = (timeinfo.tm_hour)/10;
}

void initNtpTime(){
  Serial.println("Setting up time");
  //configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);    // First connect to NTP server
  configTzTime(TZ_Australia_Melbourne, ntpServer);
  if(!getLocalTime(&timeinfo)){
    Serial.println("  Failed to obtain time");
    return;
  }
  Serial.println("  Got the time from NTP");
  setRazClockTime();    // Now we can set the time variables for RaZLamp Clock
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
}
