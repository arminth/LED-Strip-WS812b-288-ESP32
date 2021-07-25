//+--------------------------------------------------------------------------
//
// NightDriver - (c) 2018 Dave Plummer.  All Rights Reserved.
//
// Original File:        LED Episode 08
// File:                 Atomic WiFi Firelamp
// Description:
//
//   Draws  effects on addressable strips using FastLED
//   Provides WiFi access to controll the effects both in AP Mode and connected WiFi mode
//   for the Atomic Fire Lamp
//   https://www.youtube.com/watch?v=_wCOCI18nAk
// History:     Sep-15-2020     davepl      Created
//              Oct-05-2020     davepl      Revised for Episode 07
//              Oct-11-2020     davepl      Revised for Episode 08
//              Oct-16-2020     davepl      Revised for Episode 09
//              06-06-2021      arminth     used and changed for own LED-Strip creation
//                                          added effects pacifica and pride2015 from Mark Kriegsman                
//              06-22-2021      arminth     added WiFi
//              06-24-2021      arminth     included WiFiManager
//---------------------------------------------------------------------------

#include <Arduino.h>            // Arduino Framework
#include <Preferences.h>
Preferences preferences;
#define FASTLED_INTERNAL        // Suppress build banner
#include <FastLED.h>
#include <WiFi.h>
#include <WiFiManager.h> 
WiFiManager wm;
#define NUM_LEDS    52      // FastLED definitions, 52 LED's per arm
#define LED_PIN     5

CRGB g_LEDs[NUM_LEDS] = {0};    // Frame buffer for FastLED


int g_lineHeight = 0;
int g_Brightness = 200 ;        // 0-255 LED brightness scale
int g_PowerLimit = 4500;         // 4500mW Power Limit
int effect = 0;   // effect number
int versuche = 0; // number of trys to connenct to WiFi


const char* softap_ssid     = "Firelamp";
const char* softap_password = "12345678";
String modes[13] = {"Fire","Balls","Comet","Twinkle","Marquee","Marquee Mirrored","Pacifica","Pride 2015", "Lamp full","Lamp less bright","Lamp medium","Lamp dimm","Lamp OFF"};
String ssid_connected = softap_ssid; //which SSID are we connected



String Effekt = "Last Poweroff"; //Name of actual effect
WiFiServer server(80);// Set web server port number to 80

String header; // Variable to store the HTTP request
boolean modi = false; //configuration modified?


//included effects
#include "comet.h"
#include "marquee.h"
#include "twinkle.h"
#include "fire.h"
#include "bounce.h"
#include "pacifica.h"
#include "pride2015.h"
//start fire and bouncingballs
ClassicFireEffect fire(NUM_LEDS, 40, 80, 50, 4, true, false); 
BouncingBallEffect balls(NUM_LEDS, 5, 0, false, 4.0); // bouncing Balls

void effect_on()  // update enabled selected effect
 {
   switch (effect) { 
 
  case 0:
    FastLED.clear();
    fire.DrawFire();
    FastLED.show(g_Brightness); 
    delay(30);
    break;
  case 1:
   balls.Draw();
   FastLED.show(g_Brightness);
    break;
  case 2:
    DrawComet();
    FastLED.show(g_Brightness);
    break;  
  case 3:
    DrawTwinkle();
    FastLED.show(g_Brightness);
    break;  
  case 4:
    DrawMarquee();
    FastLED.show(g_Brightness);
    break;  
  case 5:  
    DrawMarqueeMirrored();
    FastLED.show(g_Brightness);
    break;  
  case 6:  
    EVERY_N_MILLISECONDS( 20) {
    pacifica_loop();
    FastLED.show(g_Brightness);}
    break;  
  case 7:
    pride();
    FastLED.show(g_Brightness);
    break;  
    case 8:
    fill_solid(g_LEDs,NUM_LEDS, CRGB::White);
    FastLED.show(g_Brightness);
    break;
    
    case 9:
    fill_solid(g_LEDs,NUM_LEDS, CRGB::White);
    FastLED.show(int(g_Brightness*0.4));
    break;
    case 10:
    fill_solid(g_LEDs,NUM_LEDS, CRGB::White);
    FastLED.show(int(g_Brightness*0.25));
    break;
    case 11:
    fill_solid(g_LEDs,NUM_LEDS, CRGB::White);
    FastLED.show(int(g_Brightness*0.1));
    break;
    case 12:
    fill_solid(g_LEDs,NUM_LEDS, CRGB::Black);
    FastLED.show();
    break;    
  default:
    effect = 0;

  } // of switch
 };

void setup() 

{ pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
 

 
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);               // Add our LED strip to the FastLED library
  
  FastLED.setBrightness(g_Brightness);
  set_max_power_indicator_LED(LED_BUILTIN);                               // Light the builtin LED if we power throttle
  FastLED.setMaxPowerInMilliWatts(g_PowerLimit);                          // Set the power limit, above which brightness will be throttled
  

  Serial.begin(115200);
  while (!Serial) { }
  Serial.println("ESP32 Startup");
 
fill_solid(g_LEDs,NUM_LEDS, CRGB::Blue); // indicate lamp start in green
    FastLED.show(int(g_Brightness*0.75));


wm.setConnectTimeout(20); //First try to connect to known networks for 20 seconds, then switch to AP portal
wm.setConfigPortalTimeout(60); //After 60 seconds of AP portal open and no activity, exit portal to enable local AP operation

if (!(wm.autoConnect("Firelamp_AP_config", "12345678"))) { //no known network found, no input from  or captive AP-Portal was exited without selection of an SSID to connect to
  WiFi.softAP(softap_ssid, softap_password); //we need to open our own AP for the lamp
    Serial.println("");
    ssid_connected = softap_ssid;
Serial.println("AP WiFi provided.");
Serial.println("IP address: ");
Serial.println(WiFi.softAPIP());// this will display the Ip address of the ESP in SoftAP-Mode (default: 192.168.4.1)

for (int i=0; i<5; i++){ // Lamp is started in AP-Mode - 5 blue blinks
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Blue);
    FastLED.show(int(g_Brightness*0.75));
  delay(250);
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Black);
    FastLED.show(int(g_Brightness*0.75));
    delay(250);}

 }
 else {ssid_connected = WiFi.SSID(); //we are connected to WiFi! 5 green blinks!
 for (int i=0; i<5; i++){
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Green);
    FastLED.show(int(g_Brightness*0.75));
  delay(250);
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Black);
    FastLED.show(int(g_Brightness*0.75));
    delay(250);}
    }  


 server.begin(); // start webserver vor lamp
    
  

  // read last Effect from the config-flash
  preferences.begin("Lampe", false);
  effect = preferences.getInt("effect", 0);
  preferences.end();
  Effekt = modes[effect];
}




void loop() 
{


while (true)
  
  { 
  effect_on(); 
    

  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) { 
    
    
    
                                // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {  
               // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
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
            
            // Switches the effects
            if (header.indexOf("GET /fire/on") >= 0) {
              effect=0;
              modi = true;
            } else if (header.indexOf("GET /balls/on") >= 0) {
              effect = 1;
              modi = true;
            } else if (header.indexOf("GET /comet/on") >= 0) {
              effect =2;
              modi = true;
            } else if (header.indexOf("GET /twinkle/on") >= 0) {
              effect =3;
              modi = true;
            } else if (header.indexOf("GET /marquee/on") >= 0) {
              effect = 4;
              modi = true;
            }else if (header.indexOf("GET /marquee_m/on") >= 0) {
              effect = 5;
              modi = true;
            }else if (header.indexOf("GET /pacifica/on") >= 0) {
              effect = 6;
              modi = true;
            }else if (header.indexOf("GET /pride2015/on") >= 0) {
              effect = 7;
              modi = true;
            }else if (header.indexOf("GET /lamp100/on") >= 0) {
              effect = 8;
              modi = true;
            }else if (header.indexOf("GET /lamp75/on") >= 0) {
              effect = 9;
              modi = true;
            }else if (header.indexOf("GET /lamp50/on") >= 0) {
              effect = 10;
              modi = true;
            }else if (header.indexOf("GET /lamp25/on") >= 0) {
              effect = 11;
              modi = true;
            }else if (header.indexOf("GET /off/on") >= 0) {
              effect = 12;
              modi = true;
            }else if (header.indexOf("GET /resetwifi/on") >= 0) {
              wm.resetSettings();  
              ESP.restart();  
              
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style  on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #770000; border: none; color: black; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Atomic Firelamp Web Server</h1><br>");
            client.println("<h2>Connected to: <br>");
            client.print(ssid_connected);
            client.println("</h2>");
            client.print("<h2>Lampeffect: ");
            client.print(Effekt);
            client.println("</h2>");
            client.println("<p><a href=\"/fire/on\"><button class=\"button\">Fire</button></a>");
            client.println("<a href=\"/balls/on\"><button class=\"button\">Balls</button></a></p>");
            client.println("<p><a href=\"/comet/on\"><button class=\"button\">Comet</button></a>");
            client.println("<a href=\"/twinkle/on\"><button class=\"button\">Twinkle</button></a></p>");
            client.println("<p><a href=\"/marquee/on\"><button class=\"button\">Marquee</button></a>");
            client.println("<a href=\"/marquee_m/on\"><button class=\"button\">Marquee mirrored</button></a></p>");
            client.println("<p><a href=\"/pacifica/on\"><button class=\"button\">Pacifica</button></a>");
            client.println("<a href=\"/pride2015/on\"><button class=\"button\">Pride</button></a></p>");
            client.println("<p><a href=\"/lamp100/on\"><button class=\"button\">Lamp Full</button></a>");
            client.println("<a href=\"/lamp75/on\"><button class=\"button\">Lamp less bright</button></a></p>");
            client.println("<p><a href=\"/lamp50/on\"><button class=\"button\">Lamp  medium</button></a>");
            client.println("<a href=\"/lamp25/on\"><button class=\"button\">Lamp  dimm</button></a></p><br>");
            client.println("<p><a href=\"/off/on\"><button class=\"button\">Lamp OFF</button></a><br><br><br><br>");
            client.println("<a href=\"/resetwifi/on\"><button class=\"button2\">Reset Wifi and restart</button></a></p>");
            client.println("</body></html>");
            
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
      if (!client.available()) // if the client keeps the connection open as Chrome does, we need to update the effects within the while loop.
      { effect_on();
      } //of if
    } //of while connected

    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  
 
 if (modi) { //if mode has changed, write to flash and display new effect on serial
    preferences.begin("Lampe", false);
    preferences.putInt("effect", effect);
    preferences.end();
    Serial.println("Gespeichert. Weiter! ");
    modi=false;

    Serial.print("Effect: ");
    Serial.println(modes[effect]);
    
 };
 };




 };

 

 
    
     
    
 
  
  

  

  
  
    
    
  




