#pragma once
#include <WiFi.h>
#include <PubSubClient.h>
#include "RazWifi.h"

// Add your MQTT Broker IP address, username and password
const char* mqtt_server = "broker";
const char* mqtt_username = "username";
const char* mqtt_password = "password";
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
      PatternName = "Marquee";
      Serial.println(PatternName);
      Pattern = 0;
    }
    else if(messageTemp == "twinkle"){
      PatternName = "Twinkle";
      Serial.println(PatternName);
      Pattern = 1;
    }
    else if(messageTemp == "comet"){
      PatternName = "Comet";
      Serial.println(PatternName);
      Pattern = 2;
    }
    else if(messageTemp == "pulsecomet"){
      PatternName = "PulseComet";
      Serial.println(PatternName);
      Pattern = 3;
    }
    else if(messageTemp == "balls"){
      PatternName = "Balls";
      Serial.println(PatternName);
      Pattern = 4;
    }
    else if(messageTemp == "ballsmirrored"){
      PatternName = "BallsMirrored";
      Serial.println(PatternName);
      Pattern = 5;
    }
    else if(messageTemp == "rainbow"){
      PatternName = "Rainbow";
      Serial.println(PatternName);
      Pattern = 6;
    }
    else if(messageTemp == "fire"){
      PatternName = "Fire";
      Serial.println(PatternName);
      Pattern = 7;
      drawVariant = false;
    }
    else if(messageTemp == "fireinward"){
      PatternName = "FireInward";
      Serial.println(PatternName);
      Pattern = 8;
      drawVariant = false;
    }
    else if(messageTemp == "auto"){
      PatternName = "AUTO";
      Serial.println(PatternName);
      autoPattern = !autoPattern;
    }
    else if(messageTemp == "fireoutwards"){
      PatternName = "FireOutwards";
      Serial.println(PatternName);
      Pattern = 10;
      drawVariant = false;
    }
    else if(messageTemp == "fireworks"){
      PatternName = "Fireworks";
      Serial.println(PatternName);
      Pattern = 11;
    }
    else if(messageTemp == "explosions"){
      PatternName = "Explosions";
      Serial.println(PatternName);
      Pattern = 12;
    }
    else if(messageTemp == "ripples"){
      PatternName = "Ripples";
      Serial.println(PatternName);
      Pattern = 13;
    }
    else if(messageTemp == "lightning"){
      PatternName = "Lightning";
      Serial.println(PatternName);
      Pattern = 14;
    }
    else if(messageTemp == "matrix"){
      PatternName = "Matrix";
      Serial.println(PatternName);
      FastLED.clear();
      Pattern = 15;
    }
    else if(messageTemp == "zigzag"){
      PatternName = "ZigZag";
      Serial.println(PatternName);
      Pattern = 16;
    }
    else if(messageTemp == "night"){
      if(!nightMode){
          oldPattern = Pattern;
          Pattern = 18; // Set to clock pattern
          oldBrightness = g_Brightness;
          g_Brightness = 20;
          nightMode = true;
        }
        else{
          Pattern = oldPattern;
          g_Brightness = oldBrightness;
          nightMode = false;
        }
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
        String tempClientId = mqtt_ClientId + String(random16());
        if (mqttclient.connect(tempClientId.c_str(), mqtt_username, mqtt_password,"willtopic",1,false,"RaZLampDisconnected",true)) {
            Serial.println("connected");
            // Subscribe
            mqttclient.subscribe("razlamp/pattern");
            mqttclient.publish("razlamp/pattern",tempClientId.c_str());
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
    mqttclient.loop();
}