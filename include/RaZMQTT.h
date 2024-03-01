#pragma once
#include <WiFi.h>
#include <PubSubClient.h>
#include "RazWifi.h"

// Add your MQTT Broker IP address, username and password
const char* mqtt_server = "razstuff.mywire.org";
const char* mqtt_username = "raz";
const char* mqtt_password = "Pantera1981";
const char* mqtt_ClientId = "RazLamp";

WiFiClient RaZLampMQTTClient;
PubSubClient mqttclient(RaZLampMQTTClient);

extern int Pattern;

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more with MQTT

  // Changes the pattern according to the message
  if (String(topic) == "razlamp/pattern") {
    Serial.print("Changing pattern to ");
    
    if(messageTemp == "marquee"){
      autoPattern = false;
      PatternName = "Marquee";
      Serial.println(PatternName);
      Pattern = 0;
    }
    else if(messageTemp == "twinkle"){
      autoPattern = false;
      PatternName = "Twinkle";
      Serial.println(PatternName);
      Pattern = 1;
    }
    else if(messageTemp == "comet"){
      autoPattern = false;
      PatternName = "Comet";
      Serial.println(PatternName);
      Pattern = 2;
    }
    else if(messageTemp == "pulsecomet"){
      autoPattern = false;
      PatternName = "PulseComet";
      Serial.println(PatternName);
      Pattern = 3;
    }
    else if(messageTemp == "balls"){
      autoPattern = false;
      PatternName = "Balls";
      Serial.println(PatternName);
      Pattern = 4;
    }
    else if(messageTemp == "ballsmirrored"){
      autoPattern = false;
      PatternName = "BallsMirrored";
      Serial.println(PatternName);
      Pattern = 5;
    }
    else if(messageTemp == "rainbow"){
      autoPattern = false;
      PatternName = "Rainbow";
      Serial.println(PatternName);
      Pattern = 6;
    }
    else if(messageTemp == "fire"){
      autoPattern = false;
      PatternName = "Fire";
      Serial.println(PatternName);
      Pattern = 7;
      drawVariant = false;
    }
    else if(messageTemp == "fireinward"){
      autoPattern = false;
      PatternName = "FireInward";
      Serial.println(PatternName);
      Pattern = 8;
      drawVariant = false;
    }
    else if(messageTemp == "auto"){
      if(nightMode==true){
        Pattern = oldPattern;
        g_Brightness = oldBrightness;
        nightMode = false;
      }
      PatternName = "AUTO";
      Serial.println(PatternName);
      autoPattern = !autoPattern;
      FastLED.clear();
      if(autoPattern)
        DrawAutoOnText();
      else
        DrawAutoOffText();
    }
    else if(messageTemp == "fireoutwards"){
      autoPattern = false;
      PatternName = "FireOutwards";
      Serial.println(PatternName);
      Pattern = 10;
      drawVariant = false;
    }
    else if(messageTemp == "fireworks"){
      autoPattern = false;
      PatternName = "Fireworks";
      Serial.println(PatternName);
      Pattern = 11;
    }
    else if(messageTemp == "explosions"){
      autoPattern = false;
      PatternName = "Explosions";
      Serial.println(PatternName);
      Pattern = 12;
    }
    else if(messageTemp == "ripples"){
      autoPattern = false;
      PatternName = "Ripples";
      Serial.println(PatternName);
      Pattern = 13;
    }
    else if(messageTemp == "lightning"){
      autoPattern = false;
      PatternName = "Lightning";
      Serial.println(PatternName);
      Pattern = 14;
    }
    else if(messageTemp == "matrix"){
      autoPattern = false;
      PatternName = "Matrix";
      Serial.println(PatternName);
      FastLED.clear();
      Pattern = 15;
    }
    else if(messageTemp == "zigzag"){
      autoPattern = false;
      PatternName = "ZigZag";
      Serial.println(PatternName);
      Pattern = 16;
    }
    else if(messageTemp == "lavalake"){
      autoPattern = false;
      PatternName = "LavaLake";
      Serial.println(PatternName);
      Pattern = 22;
    }
    else if(messageTemp == "clock"){
      autoPattern = false;
      PatternName = "Clock";
      Serial.println(PatternName);
      Pattern = 18;
    }
    else if(messageTemp == "night"){
      if(!nightMode){
          autoPattern = false;
          oldPattern = Pattern;
          Pattern = 18; // Set to clock pattern
          oldBrightness = g_Brightness;
          g_Brightness = 20;
          nightMode = true;
        }
        // else{
        //   Pattern = oldPattern;
        //   g_Brightness = oldBrightness;
        //   nightMode = false;
        // }
    }
    
  }
}

void reconnect() {
  static long lastMQTTReconnectAttempt = 0;
  if(!mqttclient.connected()) {
    long now = millis();
    if (now - lastMQTTReconnectAttempt > 5000) {
        lastMQTTReconnectAttempt = now;
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        randomSeed(millis());
        String tempClientId = mqtt_ClientId + String(esp_random());
        if (mqttclient.connect(tempClientId.c_str(), mqtt_username, mqtt_password,"razlamp/status",1,false,"RaZLampDisconnected",true)) {
            Serial.println("connected");
            // Subscribe
            mqttclient.subscribe("razlamp/pattern");
            mqttclient.publish("razlamp/status",tempClientId.c_str());
        } else {
        Serial.print("failed, rc=");
        Serial.print(mqttclient.state());
        Serial.println(" try again in 5 seconds");
        }
    }
  }
}

void MQTTCheck(){
    if (!mqttclient.connected())
        reconnect();
    if(WiFi.isConnected())
      mqttclient.loop();
}