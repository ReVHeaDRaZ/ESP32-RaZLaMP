#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WiFiManager.h>

// Set these to your desired credentials if using AccessPoint Mode.
const char *ssid = "RaZLaMP";
const char *password = "password";  // Add/change in WifiSetup() if you want to use a password. eg. WiFi.softAP(ssid,password)
 
WiFiServer server(80);
String header;

// For timeout
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;          // Define timeout time in milliseconds (example: 2000ms = 2s)

extern int g_Brightness;
extern int Pattern;
extern bool SoundReactive;
extern byte clockCount0;
extern byte clockCount10;
extern byte clockCount100;
extern byte clockCount1000;
extern char textString[];
void SetLetters();

String PatternName = "AUTO";          // To Display on webpage
String Brightness = "75%";

//Initial Wifi Setup Function
void WifiSetup(bool AccessPointMode) {
  // AccessPointMode
  if(AccessPointMode){
    Serial.println();
    Serial.println("Configuring access point...");
 
    // You can remove the password parameter if you want the AP to be open.
    WiFi.softAP(ssid);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  }
  // StationPointMode
  else{
    Serial.println();
    Serial.println("Attempt connecting to network...");
    delay(10);
    WiFiManager wifiManager;
    // If you've previously connected to your WiFi with this ESP32,
    // WiFi manager will more than likely not do anything.
    // Uncomment this if you want to force it to delete your old WiFi details.
    //wifiManager.resetSettings();

    // Static IP Setting
    //wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,116),IPAddress(192,168,0,1),IPAddress(255,255,255,0));

    //Tries to connect to last known WiFi details
    //if it does not connect it starts an access point with the specified name
    //and goes into a blocking loop for 20seconds awaiting configuration
    wifiManager.setConfigPortalTimeout(30);
    if (!wifiManager.autoConnect("RaZLaMPWiFiSetupAP")) {
      Serial.println("failed to connect and hit timeout");
      //can reset and try again, or maybe put it to deep sleep
      //ESP.restart();
      delay(1000);
    }
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  
  server.begin();
  Serial.println("Server started");
  delay(1000);
}
 

void WifiCheck(){
  WiFiClient client = server.available();    // Listen for incoming clients

  if (client) {                                 // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");              // print a message out in the serial port
    String currentLine = "";                    // make a String to hold incoming data from the client
    
    // loop while the client's connected
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;                        // and add to header String
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Set Patterns and Brightness From Selection
            if (header.indexOf("GET /0") >= 0) {
              if(SoundReactive) PatternName = "Spectrum";
              else PatternName = "Marquee";
              Serial.println(PatternName);
              Pattern = 0;
            }
            else if (header.indexOf("GET /1") >= 0) {
              if(SoundReactive) PatternName = "BassRipples";
              else PatternName = "Twinkle";
              Serial.println(PatternName);
              Pattern = 1;
            }
            else if (header.indexOf("GET /2") >= 0) {
              if(SoundReactive) PatternName = "2BandRipples";
              else PatternName = "Comet";
              Serial.println(PatternName);
              Pattern = 2;
            }
            else if (header.indexOf("GET /3") >= 0) {
              if(SoundReactive) PatternName = "3BandRipples";
              else PatternName = "PulseComet";
              Serial.println(PatternName);
              Pattern = 3;
            }
            else if (header.indexOf("GET /4") >= 0) {
              if(SoundReactive) PatternName = "BeatPulseComet";
              else PatternName = "Balls";
              Serial.println(PatternName);
              Pattern = 4;
            }
            else if (header.indexOf("GET /5") >= 0) {
              if(SoundReactive) PatternName = "BeatFireComet";
              else PatternName = "BallsMirrored";
              Serial.println(PatternName);
              Pattern = 5;
            }
            else if (header.indexOf("GET /6") >= 0) {
              if(SoundReactive) PatternName = "SoundWaves";
              else PatternName = "Rainbow";
              Serial.println(PatternName);
              Pattern = 6;
            }
            else if (header.indexOf("GET /7") >= 0) {
              if(SoundReactive) PatternName = "PerlinNoise";
              else PatternName = "Fire";
              Serial.println(PatternName);
              Pattern = 7;
            }
            else if (header.indexOf("GET /8") >= 0) {
              PatternName = "FireInward";
              Serial.println(PatternName);
              Pattern = 8;
            }
            else if (header.indexOf("GET /9") >= 0) {
              PatternName = "AUTO";
              Serial.println(PatternName);
              Pattern = 9;
            }
            else if (header.indexOf("GET /FIREOUT") >= 0) {
              PatternName = "FireOutwards";
              Serial.println(PatternName);
              Pattern = 10;
            }
            else if (header.indexOf("GET /FIREWORKS") >= 0) {
              PatternName = "FireWorks";
              Serial.println(PatternName);
              Pattern = 11;
            }
            else if (header.indexOf("GET /EXPLOSIONS") >= 0) {
              PatternName = "Explosions";
              Serial.println(PatternName);
              Pattern = 12;
            }
            else if (header.indexOf("GET /RIPPLES") >= 0) {
              PatternName = "Ripples";
              Serial.println(PatternName);
              Pattern = 13;
            }
            else if (header.indexOf("GET /LIGHTNING") >= 0) {
              PatternName = "Lightning";
              Serial.println(PatternName);
              Pattern = 14;
            }
            else if (header.indexOf("GET /MATRIX") >= 0) {
              PatternName = "Matrix";
              Serial.println(PatternName);
              Pattern = 15;
            }
            else if (header.indexOf("GET /ZIGZAG") >= 0) {
              PatternName = "ZigZag";
              Serial.println(PatternName);
              Pattern = 16;
            }
            else if (header.indexOf("GET /LETTERS") >= 0) {
              PatternName = "Letters";
              Serial.println(PatternName);
              Pattern = 17;
            }
            else if (header.indexOf("GET /CLOCK") >= 0) {
              PatternName = "Clock";
              Serial.println(PatternName);
              Pattern = 18;
            }
            else if (header.indexOf("GET /SWIRLS") >= 0) {
              PatternName = "Swirls";
              Serial.println(PatternName);
              Pattern = 19;
            }
            else if (header.indexOf("GET /SNAKES") >= 0) {
              PatternName = "Snakes";
              Serial.println(PatternName);
              Pattern = 21;
            }
            else if (header.indexOf("GET /LAVALAKE") >= 0) {
              PatternName = "LavaLake";
              Serial.println(PatternName);
              Pattern = 22;
            }
            else if (header.indexOf("GET /MAGMA") >= 0) {
              PatternName = "Magma";
              Serial.println(PatternName);
              Pattern = 23;
            }
            else if (header.indexOf("GET /CRAZYBEES") >= 0) {
              PatternName = "CrazyBees";
              Serial.println(PatternName);
              Pattern = 26;
            }
            else if (header.indexOf("GET /NIGHT") >= 0) {
              if(!nightMode){
                oldPattern = Pattern;
                Pattern = 18;
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
            else if (header.indexOf("GET /DOWN") >= 0) {
              g_Brightness -= 20;
              if (g_Brightness < 20) g_Brightness = 20;  
              Serial.println(g_Brightness);
            }
            else if (header.indexOf("GET /UP") >= 0) {
              g_Brightness += 20;
              if (g_Brightness > 240) g_Brightness = 240;  
              Serial.println(g_Brightness);
            }
            else if (header.indexOf("GET /SOUNDREACT") >= 0) {
              if(SoundReactive) SoundReactive = 0;
              else if(!SoundReactive) SoundReactive = 1;  
              Serial.println(SoundReactive);
            }
            else if (header.indexOf("GET /TIME_PAGE") >= 0) {
              String timestring = header.substring(19,21);
              String minstimestring = header.substring(27,29);
              clockCount0 = (minstimestring.toInt())%10;
              clockCount10 = (minstimestring.toInt())/10;
              clockCount100 = (timestring.toInt())%10;
              clockCount1000 = (timestring.toInt())/10;
              Serial.println(timestring);
              Serial.println(minstimestring);
            }
            else if (header.indexOf("GET /TEXT_PAGE") >= 0) {
              int j=0;
              int end = header.indexOf("HTTP");
              String temptextstring = header.substring(42,end-1);
              
              Serial.println(header);
              Serial.println(temptextstring.length());
              temptextstring.toUpperCase();
              Serial.println(temptextstring);
              for(int i=0;i<ARRAYSIZE(textString); i++)textString[i]=0; //reset string to null
              for(int i=(temptextstring.length())-1; i>=0; i--){    //set string in reverse
                textString[j]=temptextstring[i];
                j++;  
              }
              textString[j]=0;            //Set last of string to 0
              Serial.println(textString);
              SetLetters();
            }
            // Display the HTML web page-----------------------------------------------------------
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            // CSS to style the on/off buttons 
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; border-radius: 10px; color: white; padding: 15px 35px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".smallbutton { padding: 10px 20px; font-size: 15px;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>RaZ Lamp</h1>");
            
            // Display current pattern and Set brightness  
            client.println("<p>Current Pattern- <b>" + PatternName + "</b></p>");
            if(g_Brightness >= 240) Brightness = "100%";
              else if(g_Brightness > 180) Brightness = "75%";
                else if(g_Brightness > 120) Brightness = "50%";
                  else if(g_Brightness < 60) Brightness = "25%";
                    else if(g_Brightness < 30) Brightness = "10%";
            client.println("<p><a href=\"/DOWN\"><button class=\"button smallbutton\">-</button></a>");
            client.println("Brightness- <b>" + Brightness + "</b>");
            client.println("<a href=\"/UP\"><button class=\"button smallbutton\">+</button></a></p>");
            
            // Display SoundReactive Button
            if (SoundReactive) {
              client.println("<p><a href=\"/SOUNDREACT\"><button class=\"button smallbutton\">SoundReactive</button></a>");
            } else {
              client.println("<p><a href=\"/SOUNDREACT\"><button class=\"button smallbutton button2\">SoundReactive</button></a>");
            }
            // Display NightMode Button
            if(nightMode){
              client.println("<a href=\"/NIGHT\"><button class=\"button smallbutton\">NightMode</button></a></p>");
            } else {
              client.println("<a href=\"/NIGHT\"><button class=\"button smallbutton button2\">NightMode</button></a></p>");
            }
            // Display If the Pattern is OFF or ON button       
            if(!SoundReactive){
            if (PatternName=="Marquee") {
              client.println("<p><a href=\"/0\"><button class=\"button\">Marquee</button></a>");
            } else {
              client.println("<p><a href=\"/0\"><button class=\"button button2\">Marquee</button></a>");
            }          
            if (PatternName=="Twinkle") {
              client.println("<a href=\"/1\"><button class=\"button\">Twinkle</button></a></p>");
            } else {
              client.println("<a href=\"/1\"><button class=\"button button2\">Twinkle</button></a></p>");
            } 
            if (PatternName=="Comet") {
              client.println("<p><a href=\"/2\"><button class=\"button\">Comet</button></a>");
            } else {
              client.println("<p><a href=\"/2\"><button class=\"button button2\">Comet</button></a>");
            } 
            if (PatternName=="PulseComet") {
              client.println("<a href=\"/3\"><button class=\"button\">PulseComet</button></a></p>");
            } else {
              client.println("<a href=\"/3\"><button class=\"button button2\">PulseComet</button></a></p>");
            } 
            if (PatternName=="Balls") {
              client.println("<p><a href=\"/4\"><button class=\"button\">Balls</button></a>");
            } else {
              client.println("<p><a href=\"/4\"><button class=\"button button2\">Balls</button></a>");
            } 
            if (PatternName=="BallsMirrored") {
              client.println("<a href=\"/5\"><button class=\"button\">BallsMirrored</button></a></p>");
            } else {
              client.println("<a href=\"/5\"><button class=\"button button2\">BallsMirrored</button></a></p>");
            }
            if (PatternName=="Rainbow") {
              client.println("<p><a href=\"/6\"><button class=\"button\">Rainbow</button></a>");
            } else {
              client.println("<p><a href=\"/6\"><button class=\"button button2\">Rainbow</button></a>");
            } 
            if (PatternName=="FireOutwards") {
              client.println("<a href=\"/FIREOUT\"><button class=\"button\">FireOutwards</button></a></p>");
            } else {
              client.println("<a href=\"/FIREOUT\"><button class=\"button button2\">FireOutwards</button></a></p>");
            }
            if (PatternName=="Fire") {
              client.println("<p><a href=\"/7\"><button class=\"button\">Fire</button></a>");
            } else {
              client.println("<p><a href=\"/7\"><button class=\"button button2\">Fire</button></a>");
            }
            if (PatternName=="FireInward") {
              client.println("<a href=\"/8\"><button class=\"button\">FireInward</button></a></p>");
            } else {
              client.println("<a href=\"/8\"><button class=\"button button2\">FireInward</button></a></p>");
            }
            if (PatternName=="FireWorks") {
              client.println("<p><a href=\"/FIREWORKS\"><button class=\"button\">FireWorks</button></a>");
            } else {
              client.println("<p><a href=\"/FIREWORKS\"><button class=\"button button2\">FireWorks</button></a>");
            }
            if (PatternName=="Explosions") {
              client.println("<a href=\"/EXPLOSIONS\"><button class=\"button\">Explosions</button></a></p>");
            } else {
              client.println("<a href=\"/EXPLOSIONS\"><button class=\"button button2\">Explosions</button></a></p>");
            }
            if (PatternName=="Ripples") {
              client.println("<p><a href=\"/RIPPLES\"><button class=\"button\">Ripples</button></a>");
            } else {
              client.println("<p><a href=\"/RIPPLES\"><button class=\"button button2\">Ripples</button></a>");
            }
            if (PatternName=="Lightning") {
              client.println("<a href=\"/LIGHTNING\"><button class=\"button\">Lightning</button></a></p>");
            } else {
              client.println("<a href=\"/LIGHTNING\"><button class=\"button button2\">Lightning</button></a></p>");
            }
            if (PatternName=="Matrix") {
              client.println("<p><a href=\"/MATRIX\"><button class=\"button\">Matrix</button></a>");
            } else {
              client.println("<p><a href=\"/MATRIX\"><button class=\"button button2\">Matrix</button></a>");
            }
             if (PatternName=="ZigZag") {
              client.println("<a href=\"/ZIGZAG\"><button class=\"button\">ZigZag</button></a></p>");
            } else {
              client.println("<a href=\"/ZIGZAG\"><button class=\"button button2\">ZigZag</button></a></p>");
            }
             if (PatternName=="Letters") {
              client.println("<p><a href=\"/LETTERS\"><button class=\"button\">Letters</button></a>");
            } else {
              client.println("<p><a href=\"/LETTERS\"><button class=\"button button2\">Letters</button></a>");
            }
            if (PatternName=="Clock") {
              client.println("<a href=\"/CLOCK\"><button class=\"button\">Clock</button></a></p>");
            } else {
              client.println("<a href=\"/CLOCK\"><button class=\"button button2\">Clock</button></a></p>");
            }
            if (PatternName=="Swirls") {
              client.println("<p><a href=\"/SWIRLS\"><button class=\"button\">Swirls</button></a>");
            } else {
              client.println("<p><a href=\"/SWIRLS\"><button class=\"button button2\">Swirls</button></a>");
            }
            if (PatternName=="Snakes") {
              client.println("<a href=\"/SNAKES\"><button class=\"button\">Snakes</button></a></p>");
            } else {
              client.println("<a href=\"/SNAKES\"><button class=\"button button2\">Snakes</button></a></p>");
            }
            if (PatternName=="LavaLake") {
              client.println("<p><a href=\"/LAVALAKE\"><button class=\"button\">LavaLake</button></a>");
            } else {
              client.println("<p><a href=\"/LAVALAKE\"><button class=\"button button2\">LavaLake</button></a>");
            }
            if (PatternName=="Magma") {
              client.println("<a href=\"/MAGMA\"><button class=\"button\">Magma</button></a></p>");
            } else {
              client.println("<a href=\"/MAGMA\"><button class=\"button button2\">Magma</button></a></p>");
            }
            if (PatternName=="CrazyBees") {
              client.println("<p><a href=\"/CRAZYBEES\"><button class=\"button\">CrazyBees</button></a></p>");
            } else {
              client.println("<p><a href=\"/CRAZYBEES\"><button class=\"button button2\">CrazyBees</button></a></p>");
            }
            if (PatternName=="AUTO") {
              client.println("<p><a href=\"/9\"><button class=\"button\">AUTO</button></a></p>");
            } else {
              client.println("<p><a href=\"/9\"><button class=\"button button2\">AUTO</button></a></p>");
            }
            } // End NOT SoundReactive
            
            // SoundReactive Patterns
            if(SoundReactive){
               if (PatternName=="Spectrum") {
              client.println("<p><a href=\"/0\"><button class=\"button\">Spectrum</button></a>");
            } else {
              client.println("<p><a href=\"/0\"><button class=\"button button2\">Spectrum</button></a>");
            }          
            if (PatternName=="BassRipples") {
              client.println("<a href=\"/1\"><button class=\"button\">BassRipples</button></a></p>");
            } else {
              client.println("<a href=\"/1\"><button class=\"button button2\">BassRipples</button></a></p>");
            } 
            if (PatternName=="2BandRipples") {
              client.println("<p><a href=\"/2\"><button class=\"button\">2BandRipples</button></a>");
            } else {
              client.println("<p><a href=\"/2\"><button class=\"button button2\">2BandRipples</button></a>");
            } 
            if (PatternName=="3BandRipples") {
              client.println("<a href=\"/3\"><button class=\"button\">3BandRipples</button></a></p>");
            } else {
              client.println("<a href=\"/3\"><button class=\"button button2\">3BandRipples</button></a></p>");
            } 
            if (PatternName=="BeatPulseComet") {
              client.println("<p><a href=\"/4\"><button class=\"button\">BeatPulseComet</button></a>");
            } else {
              client.println("<p><a href=\"/4\"><button class=\"button button2\">BeatPulseComet</button></a>");
            } 
            if (PatternName=="BeatFireComet") {
              client.println("<a href=\"/5\"><button class=\"button\">BeatFireComet</button></a></p>");
            } else {
              client.println("<a href=\"/5\"><button class=\"button button2\">BeatFireComet</button></a></p>");
            }
            if (PatternName=="SoundWaves") {
              client.println("<p><a href=\"/6\"><button class=\"button\">SoundWaves</button></a>");
            } else {
              client.println("<p><a href=\"/6\"><button class=\"button button2\">SoundWaves</button></a>");
            } 
            if (PatternName=="PerlinNoise") {
              client.println("<p><a href=\"/7\"><button class=\"button\">PerlinNoise</button></a>");
            } else {
              client.println("<p><a href=\"/7\"><button class=\"button button2\">PerlinNoise</button></a>");
            }
            if (PatternName=="FireInward") {
              client.println("<a href=\"/8\"><button class=\"button\">FireInward</button></a></p>");
            } else {
              client.println("<a href=\"/8\"><button class=\"button button2\">FireInward</button></a></p>");
            }
                     
            if (PatternName=="AUTO") {
              client.println("<p><a href=\"/9\"><button class=\"button\">AUTO</button></a></p>");
            } else {
              client.println("<p><a href=\"/9\"><button class=\"button button2\">AUTO</button></a></p>");
            }
            } // End SoundReactive Patterns

            // Time set form
            client.println("<p>SET TIME</p>");
            client.println("<form action=\"/TIME_PAGE\">");
            client.println("<input type=\"text\" name=\"hrs\" value=\"01\" minlength=\"2\" maxlength=\"2\" size=\"3\"> : ");
            client.println("<input type=\"text\" name=\"mins\"value=\"00\" minlength=\"2\" maxlength=\"2\" size=\"3\">");
            client.println("<input type=\"submit\" value=\"Submit\">");

            // Text set form
            client.println("<p>SET LETTERS TEXT</p>");
            client.println("<form action=\"/TEXT_PAGE\">");
            client.println("<input type=\"text\" name=\"letterstext\" value=\"TEXT\" minlength=\"1\" maxlength=\"8\" size=\"9\">");
            client.println("<input type=\"submit\" formaction=\"/TEXT_PAGE\" value=\"Submit\">");
            
            client.println("</body></html>"); // END of HTML
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}