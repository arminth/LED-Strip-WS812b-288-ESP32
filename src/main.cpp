//+--------------------------------------------------------------------------
//
// NightDriver - (c) 2018 Dave Plummer.  All Rights Reserved.
//
// File:        LED Episode 08
//
// Description:
//
//   Draws sample effects on a an addressable strip using FastLED
//
// History:     Sep-15-2020     davepl      Created
//              Oct-05-2020     davepl      Revised for Episode 07
//              Oct-11-2020     davepl      Revised for Episode 08
//              Oct-16-2020     davepl      Revised for Episode 09
//              06-06-2021      arminth     used and changed for own LED-Strip creation
//                                          added effects pacifica and pride2015 from Mark Kriegsman                
//              06-22-2021      arminth     added WiFi
//---------------------------------------------------------------------------

#include <Arduino.h>            // Arduino Framework
#include <Preferences.h>
Preferences preferences;
#define FASTLED_INTERNAL        // Suppress build banner
#include <FastLED.h>
#include <WiFi.h>


#define NUM_LEDS    52      // FastLED definitions, 52 LED's per arm
#define LED_PIN     5

CRGB g_LEDs[NUM_LEDS] = {0};    // Frame buffer for FastLED


int g_lineHeight = 0;
int g_Brightness = 200 ;        // 0-255 LED brightness scale
int g_PowerLimit = 4500;         // 4500mW Power Limit
int effect = 0;   // effect number
int versuche = 0; // number of trys to connenct to WiFi

const char* ssid     = "Nase2";
const char* ssid2     = "Nase";
const char* ssid3     = "Nase3";
const char* ssid4     = "Nase4";
const char* password = "Andrea_1234567";
const char* softap_ssid     = "Firelamp";
const char* softap_password = "12345678";

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
ClassicFireEffect fire(NUM_LEDS, 40, 80, 50, 4, true, false); 
BouncingBallEffect balls(NUM_LEDS, 5, 0, false, 4.0); // bouncing Balls

void effect_on()
 {
   switch (effect) { //enable actual effect
 
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
    FastLED.show(int(g_Brightness*0.75));
    break;
    case 10:
    fill_solid(g_LEDs,NUM_LEDS, CRGB::White);
    FastLED.show(int(g_Brightness*0.5));
    break;
    case 11:
    fill_solid(g_LEDs,NUM_LEDS, CRGB::White);
    FastLED.show(int(g_Brightness*0.25));
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
 WiFi.begin(ssid, password);
 ssid_connected = ssid;
Serial.print("Connecting to ");
Serial.println(ssid);
while ((WiFi.status() != WL_CONNECTED) and (versuche<10)) {
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Green);
    FastLED.show(int(g_Brightness*0.75));
  delay(250);
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Black);
    FastLED.show(int(g_Brightness*0.75));
    delay(250);
  Serial.print(".");
  versuche ++;
 
} 
if (versuche>=10)
  { WiFi.disconnect(); 
   versuche = 0;
   WiFi.begin(ssid2, password);
   ssid_connected = ssid2;
   Serial.print("Connecting to ");
   Serial.println(ssid2);
   
   while ((WiFi.status() != WL_CONNECTED) and (versuche<10)) {
   fill_solid(g_LEDs,NUM_LEDS, CRGB::Yellow);
    FastLED.show(int(g_Brightness*0.75));
  delay(250);
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Black);
    FastLED.show(int(g_Brightness*0.75));
    delay(250);
    Serial.print(".");
    versuche ++;
     } 
    if (versuche>=10)
  { WiFi.disconnect(); 
   versuche = 0;
   WiFi.begin(ssid3, password);
   ssid_connected = ssid3;
   Serial.print("Connecting to ");
   Serial.println(ssid3);
   while ((WiFi.status() != WL_CONNECTED) and (versuche<10)) {
    fill_solid(g_LEDs,NUM_LEDS, CRGB::Orange);
    FastLED.show(int(g_Brightness*0.75));
  delay(250);
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Black);
    FastLED.show(int(g_Brightness*0.75));
    delay(250);
    Serial.print(".");
    versuche ++;
   } 
   if (versuche>=10)
  { WiFi.disconnect(); 
   versuche = 0;
   WiFi.begin(ssid4, password);
   ssid_connected = ssid4;
   Serial.print("Connecting to ");
   Serial.println(ssid4);
   while ((WiFi.status() != WL_CONNECTED) and (versuche<10)) {
    fill_solid(g_LEDs,NUM_LEDS, CRGB::Red);
    FastLED.show(int(g_Brightness*0.75));
  delay(250);
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Black);
    FastLED.show(int(g_Brightness*0.75));
    delay(250);
    Serial.print(".");
    versuche ++;
   }
  }
  } 
  }

if (versuche <10) {Serial.println(""); // we have WiFi Access
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());// this will display the Ip address of the ESP which should be entered into your browser


 server.begin(); }
  else { // We need to open AP
    
    WiFi.softAP(softap_ssid, softap_password);
    Serial.println("");
    ssid_connected = softap_ssid;
Serial.println("AP WiFi provided.");
Serial.println("IP address: ");
Serial.println(WiFi.softAPIP());// this will display the Ip address of the ESP in SoftAP-Mode

for (int i=0; i<5; i++){
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Blue);
    FastLED.show(int(g_Brightness*0.75));
  delay(250);
  fill_solid(g_LEDs,NUM_LEDS, CRGB::Black);
    FastLED.show(int(g_Brightness*0.75));
    delay(250);}


    
 server.begin();
    };
  

  // read last Effect from the config-flash
  preferences.begin("Lampe", false);
  effect = preferences.getInt("effect", 0);
  preferences.end();
  
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
              Effekt = "Fire";
              modi = true;
            } else if (header.indexOf("GET /balls/on") >= 0) {
              effect = 1;
              Effekt = "Balls";
              modi = true;
            } else if (header.indexOf("GET /comet/on") >= 0) {
              effect =2;
              Effekt = "Comet"; 
              modi = true;
            } else if (header.indexOf("GET /twinkle/on") >= 0) {
              effect =3;
              Effekt = "Twinkle"; 
              modi = true;
            } else if (header.indexOf("GET /marquee/on") >= 0) {
              effect = 4;
              Effekt = "Marquee"; 
              modi = true;
            }else if (header.indexOf("GET /marquee_m/on") >= 0) {
              effect = 5;
              Effekt = "Marquee mirrored"; 
              modi = true;
            }else if (header.indexOf("GET /pacifica/on") >= 0) {
              effect = 6;
              Effekt = "Pacifica"; 
              modi = true;
            }else if (header.indexOf("GET /pride2015/on") >= 0) {
              effect = 7;
              Effekt = "Pride2015"; 
              modi = true;
            }else if (header.indexOf("GET /lamp100/on") >= 0) {
              effect = 8;
              Effekt = "Lamp 100%"; 
              modi = true;
            }else if (header.indexOf("GET /lamp75/on") >= 0) {
              effect = 9;
              Effekt = "Lamp 75%";
              modi = true;
            }else if (header.indexOf("GET /lamp50/on") >= 0) {
              effect = 10;
              Effekt = "Lamp 50%";
              modi = true;
            }else if (header.indexOf("GET /lamp25/on") >= 0) {
              effect = 11;
              Effekt = "Lamp 25%";
              modi = true;
            }else if (header.indexOf("GET /off/on") >= 0) {
              effect = 12;
              Effekt = "Lamp OFF";
              modi = true;
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
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Atomic Firelamp Web Server</h1><br>");
            client.println("<h2>Connected to: <br>");
            client.print(ssid_connected);
            client.println("</h2>");
            client.print("<h2>Lampeffect: ");
            client.print(Effekt);
            client.println("</h2>");
            
                
            
              client.println("<p><a href=\"/fire/on\"><button class=\"button\">Fire</button></a></p>");
              client.println("<p><a href=\"/balls/on\"><button class=\"button\">Balls</button></a></p>");
              client.println("<p><a href=\"/comet/on\"><button class=\"button\">Comet</button></a></p>");
              client.println("<p><a href=\"/twinkle/on\"><button class=\"button\">Twinkle</button></a></p>");
              client.println("<p><a href=\"/marquee/on\"><button class=\"button\">Marquee</button></a></p>");
              client.println("<p><a href=\"/marquee_m/on\"><button class=\"button\">Marquee mirrored</button></a></p>");
              client.println("<p><a href=\"/pacifica/on\"><button class=\"button\">Pacifica</button></a></p>");
              client.println("<p><a href=\"/pride2015/on\"><button class=\"button\">Pride</button></a></p>");
              client.println("<p><a href=\"/lamp100/on\"><button class=\"button\">Lamp 100%</button></a></p>");
              client.println("<p><a href=\"/lamp75/on\"><button class=\"button\">Lamp  75%</button></a></p>");
              client.println("<p><a href=\"/lamp50/on\"><button class=\"button\">Lamp  50%</button></a></p>");
              client.println("<p><a href=\"/lamp25/on\"><button class=\"button\">Lamp  25%</button></a></p><br>");
              client.println("<p><a href=\"/off/on\"><button class=\"button\">Lamp OFF</button></a></p>");
            
               
           
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
  
 
 if (modi) { //configuration changed, write to flash and display new effect on serial
    preferences.begin("Lampe", false);
    preferences.putInt("effect", effect);
    preferences.end();
    Serial.println("Gespeichert. Weiter! ");
    delay(500);
    modi=false;
    switch (effect) { //display effect to serial
   case 1:
     Serial.println("Effekt: Bälle"); 
     

      break;
  
    case 2:
    Serial.println("Effekt: Komet");
    
      break;  
    case 3:
    Serial.println("Effekt: Twinkle");
   
      break;
    
    case 4:
    Serial.println("Effekt: Marquee");
   
      break;
    case 5:
    Serial.println("Effekt: Marquee mirrored");
   
      break;
    case 6:
    Serial.println("Effekt: Pacifica");
   
      break;
    case 7:
    Serial.println("Effekt: Pride2015");
   
      break;
    case 11:
    Serial.println("Effekt: Leselampe 25%");
   
      break;  
    case 10:
    Serial.println("Effekt: Leselampe 50%");
   
      break;  
    case 9:
    Serial.println("Effekt: Leselampe 75%");
   
      break;  
     case 8:
    Serial.println("Effekt: Leselampe 100%");
   
      break;  
    case 12:
    Serial.println("Effekt: Lampe aus");
   
      break;  


    default:
    Serial.println("Effekt: Feuer"); 
    }; 
 };
 };




 };

 

 
    
     
    
 
  
  

  

  
  
    
    
  




