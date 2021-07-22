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



#include "comet.h"
#include "marquee.h"
#include "twinkle.h"
#include "fire.h"
#include "bounce.h"
#include "pacifica.h"
#include "pride2015.h"

void setup() 

{ 
  preferences.begin("Lampe", false);
  effect = preferences.getInt("effect", 0);
  preferences.end();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
 

  Serial.begin(115200);
  while (!Serial) { }
  Serial.println("ESP32 Startup");
  Serial.println("Effekt: Feuer");
  
  preferences.begin("Lampe", false);
  effect = preferences.getInt("effect", 0);
  preferences.end();
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
  
    if (touchRead(TOUCH_PIN)+touchRead(TOUCH_PIN)+touchRead(TOUCH_PIN)+touchRead(TOUCH_PIN)<treshold)
    {delay (200);
      if (touchRead(TOUCH_PIN)+touchRead(TOUCH_PIN)+touchRead(TOUCH_PIN)+touchRead(TOUCH_PIN)<treshold) //safe
      {
    effect ++;
    preferences.begin("Lampe", false);
    preferences.putInt("effect", effect);
    preferences.end();
    Serial.println("Gespeichert. Weiter! ");
    delay(500);
      }
      else { Serial.println("nix"); delay(1000);}
        
    };
     
    
 
  
  

  

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



