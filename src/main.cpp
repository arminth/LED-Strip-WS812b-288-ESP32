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

#define FASTLED_INTERNAL        // Suppress build banner
#include <FastLED.h>


#define NUM_LEDS    42         // 7 defekt ,FastLED definitions
#define LED_PIN     5

CRGB g_LEDs[NUM_LEDS] = {0};    // Frame buffer for FastLED


int g_lineHeight = 0;
int g_Brightness = 255;         // 0-255 LED brightness scale
int g_PowerLimit = 6000;         // 900mW Power Limit
int effect = 0;
const int effektdauer = 60000; // Dauer je Effekt in Millisekunden


#include "comet.h"
#include "marquee.h"
#include "twinkle.h"
#include "fire.h"
#include "bounce.h"
#include "pacifica.h"
#include "pride2015.h"

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial) { }
  Serial.println("ESP32 Startup");
  Serial.println("Effekt: Feuer");
  

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
  ClassicFireEffect fire(NUM_LEDS, 20, 1000, 10, 4, true, true);    // Inwards toward Middle
  BouncingBallEffect balls(NUM_LEDS, 5, 0, true, 7.0); // bouncing Balls

while (true)
  {  
EVERY_N_MILLISECONDS(effektdauer) // Rotate to next effect and display on Serial
  {
  
  effect ++;
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

    default:
    Serial.println("Effekt: Feuer"); 
    };
  };

switch (effect) {
  case 0:
    FastLED.clear();
    fire.DrawFire();
    FastLED.show(g_Brightness); 
    delay(33);
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
  default:
    effect = 0;

}
  
    
    
  }
}



