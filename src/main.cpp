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
//---------------------------------------------------------------------------

#include <Arduino.h>            // Arduino Framework
#include <Preferences.h>
Preferences preferences;
#define FASTLED_INTERNAL        // Suppress build banner
#include <FastLED.h>
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define NUM_LEDS    52      // FastLED definitions
#define LED_PIN     5
#define TOUCH_PIN    27 // Pin to attach the Touchsensor
CRGB g_LEDs[NUM_LEDS] = {0};    // Frame buffer for FastLED


int g_lineHeight = 0;
int g_Brightness = 200 ;        // 0-255 LED brightness scale
int g_PowerLimit = 4500;         // 900mW Power Limit
int effect = 0;
int touch = 0;
const int treshold = 150; // If summ of 4 touchRead() in a row is below treshold, we have a touch! 
const char* ssid     = "Nase2";
const char* password = "Andrea_1234567";
String greenstate = "off";// state of green LED
String redstate = "off";// state of red LED
String offstate = "off";// state of red LED
WiFiServer server(80);// Set web server port number to 80

String header; // Variable to store the HTTP request
boolean modi = false;
#include "comet.h"
#include "marquee.h"
#include "twinkle.h"
#include "fire.h"
#include "bounce.h"
#include "pacifica.h"
#include "pride2015.h"

void setup() 

{ WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  while (!Serial) { }
  Serial.println("ESP32 Startup");
 WiFi.begin(ssid, password);
Serial.print("Connecting to ");
Serial.println(ssid);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
} 
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());// this will display the Ip address of the Pi which should be entered into your browser
 server.begin(); 
  
  
  preferences.begin("Lampe", false);
  effect = preferences.getInt("effect", 0);
  preferences.end();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
 

 
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);               // Add our LED strip to the FastLED library
  
  FastLED.setBrightness(g_Brightness);
  set_max_power_indicator_LED(LED_BUILTIN);                               // Light the builtin LED if we power throttle
  FastLED.setMaxPowerInMilliWatts(g_PowerLimit);                          // Set the power limit, above which brightness will be throttled
 
}



void loop() 
{
  
//ClassicFireEffect(int size, int cooling = 80, int sparking = 50, int sparks = 3, int sparkHeight = 4, bool breversed = true, bool bmirrored = true) 
  //ClassicFireEffect fire(NUM_LEDS, 30, 100, 3, 2, false, true);   // Outwards from Middle
  //ClassicFireEffect fire(NUM_LEDS, 20, 50, 70, 4, true, true);    // Inwards toward Middle cool sparks
  //ClassicFireEffect fire(NUM_LEDS, 20, 1000, 10, 4, true, true);    // Inwards toward Middle
  //ClassicFireEffect fire(NUM_LEDS, 20, 100, 3, 4, true, false);     // Outwards from Zero
  //ClassicFireEffect fire(NUM_LEDS, 20, 100, 3, 4, false, false);     // Inwards from End
  //ClassicFireEffect fire(NUM_LEDS, 50, 300, 30, 12, true, false);     // More Intense, Extra Sparking

  //ClassicFireEffect fire(NUM_LEDS, 20, 100, 3, NUM_LEDS, true, false);     // Fan with correct rotation

  // instantiate effect classses
  //FireEffectSmooth fire(NUM_LEDS, false, false,3, 0.5,0.5,0.5);    // Inwards toward Middle
  ClassicFireEffect fire(NUM_LEDS, 40, 100, 50, 10                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  , true, false);    // Inwards toward Middle
  BouncingBallEffect balls(NUM_LEDS, 5, 0, false, 4.0); // bouncing Balls

while (true)
  
  {
      WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
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
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /green/on") >= 0) {
              Serial.println("green on");
              greenstate = "on";
              effect ++;
              modi = true;
            } else if (header.indexOf("GET /green/off") >= 0) {
              Serial.println("green off");
              greenstate = "off";
              effect ++;
              modi = true;
            } else if (header.indexOf("GET /red/on") >= 0) {
              Serial.println("red on");
              redstate = "on";
              effect --;
              modi = true;
            } else if (header.indexOf("GET /red/off") >= 0) {
              Serial.println("red off");
              redstate = "off";
              effect --;
              modi = true;
            } else if (header.indexOf("GET /off/on") >= 0) {
              Serial.println("lamp off");
              offstate = "on";
              effect = 12;
              modi = true;
            }
            else if (header.indexOf("GET /off/off") >= 0) {
              Serial.println("lamp off");
              offstate = "off";
              effect = 12;
              modi = true;
            }
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Atomic Firelamp Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for effect ++  
            //client.println("<p>green - State " + greenstate + "</p>");
            // If the green LED is off, it displays the ON button       
            if (greenstate == "off") {
              client.println("<p><a href=\"/green/on\"><button class=\"button\">Effect ++</button></a></p>");
            } else {
              client.println("<p><a href=\"/green/off\"><button class=\"button button2\">Effect ++</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for effect -- 
            //client.println("<p>red - State " + redstate + "</p>");
            // If the red LED is off, it displays the ON button       
            if (redstate == "off") {
              client.println("<p><a href=\"/red/on\"><button class=\"button\"> Effect - - </button></a></p>");
            } else {
              client.println("<p><a href=\"/red/off\"><button class=\"button button2\"> Effect - - </button></a></p>");
            }
            client.println("</body></html>");
            // Display current state, and ON/OFF buttons for Lamp Off 
            //client.println("<p>red - State " + redstate + "</p>");
            // If the red LED is off, it displays the ON button       
            if (offstate == "off") {
              client.println("<p><a href=\"/off/on\"><button class=\"button\">Lamp off</button></a></p>");
            } else {
              client.println("<p><a href=\"/off/off\"><button class=\"button button2\">Lamp off</button></a></p>");
            }
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
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }


 if (modi) {
    preferences.begin("Lampe", false);
    preferences.putInt("effect", effect);
    preferences.end();
    Serial.println("Gespeichert. Weiter! ");
    delay(500);
    modi=false;
    }
    
        
    
     
    
 
  
  

  

  switch (effect) {
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
    FastLED.show(196);
    break;
    case 10:
    fill_solid(g_LEDs,NUM_LEDS, CRGB::White);
    FastLED.show(128);
    break;
    case 11:
    fill_solid(g_LEDs,NUM_LEDS, CRGB::White);
    FastLED.show(64);
    break;
    case 12:
    fill_solid(g_LEDs,NUM_LEDS, CRGB::Black);
    FastLED.show();
    break;    
  default:
    effect = 0;

}
  
    
    
  
}
}


