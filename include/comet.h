//+--------------------------------------------------------------------------
//
// NightDriver - (c) 2020 Dave Plummer.  All Rights Reserved.
//
// File:                  
//
// Description:
//
//   
//
// History:     Sep-28-2020     davepl      Created
//
//---------------------------------------------------------------------------

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDs[];

void DrawComet()
{
    const byte fadeAmt = 128;
    const int cometSize = 2;
    const int deltaHue  = 2;

    static byte hue = HUE_RED;
    static int iDirection = 1;
    static int iPos = 0;

    hue += deltaHue;

    iPos += iDirection;
    if (iPos == NUM_LEDS) 
        iPos = 0;

    
    for (int i = 0; i < cometSize; i++)
        g_LEDs[iPos + i].setHue(hue);
    
    // Randomly fade the LEDs
    for (int j = 0; j < NUM_LEDS; j++)
        if (random(8) > 4)
            g_LEDs[j] = g_LEDs[j].fadeToBlackBy(fadeAmt);  

    delay(33);
}