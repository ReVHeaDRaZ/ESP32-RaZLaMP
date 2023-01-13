#pragma once
#include <WiFi.h>
#include <PubSubClient.h>
#include "RazWifi.h"

// Add your MQTT Broker IP address, username and password
const char* mqtt_server = "yourbroker.com";
const char* mqtt_username = "username";
const char* mqtt_password = "password";

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
    }
    else if(messageTemp == "fireinward"){
      PatternName = "FireInward";
      Serial.println(PatternName);
      Pattern = 8;
    }
    else if(messageTemp == "auto"){
      PatternName = "AUTO";
      Serial.println(PatternName);
      Pattern = 9;
    }
    else if(messageTemp == "fireoutwards"){
      PatternName = "FireOutwards";
      Serial.println(PatternName);
      Pattern = 10;
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
      Pattern = 15;
    }
    else if(messageTemp == "zigzag"){
      PatternName = "ZigZag";
      Serial.println(PatternName);
      Pattern = 16;
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
        if (mqttclient.connect("RaZLampClient", mqtt_username, mqtt_password)) {
            Serial.println("connected");
            // Subscribe
            mqttclient.subscribe("razlamp/pattern");
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