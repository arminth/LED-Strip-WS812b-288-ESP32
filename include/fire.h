//+--------------------------------------------------------------------------
//
// NightDriver - (c) 2020 Dave Plummer.  All Rights Reserved.
//
// File:        fire.h               
//
// Description:
//
//              LED Flame Effect
//
// History:     Sep-28-2020     davepl      Created
//
//---------------------------------------------------------------------------

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "ledgfx.h"



class ClassicFireEffect
{
protected:
    int     Size;
    int     Cooling;
    int     Sparks;
    int     SparkHeight;
    int     Sparking;
    bool    bReversed;
    bool    bMirrored;

    byte  * heat;

    // When diffusing the fire upwards, these control how much to blend in from the cells below (ie: downward neighbors)
    // You can tune these coefficients to control how quickly and smoothly the fire spreads.  

    static const byte BlendSelf = 2;
    static const byte BlendNeighbor1 = 3;
    static const byte BlendNeighbor2 = 2;
    static const byte BlendNeighbor3 = 1;
    static const byte BlendTotal = (BlendSelf + BlendNeighbor1 + BlendNeighbor2 + BlendNeighbor3);

public:
    
    // Lower sparking -> more flicker.  Higher sparking -> more consistent flame

    ClassicFireEffect(int size, int cooling = 80, int sparking = 50, int sparks = 3, int sparkHeight = 4, bool breversed = true, bool bmirrored = true) 
        : Size(size),
          Cooling(cooling),
          Sparks(sparks),
          SparkHeight(sparkHeight),
          Sparking(sparking),
          bReversed(breversed),
          bMirrored(bmirrored)
    {
        if (bMirrored)
            Size = Size / 2;

        heat = new byte[size] { 0 };
    }

    virtual ~ClassicFireEffect()
    {
        delete [] heat;
    }

    virtual void DrawFire()
    {
        // First cool each cell by a little bit
        for (int i = 0; i < Size; i++)
            heat[i] = max(0L, heat[i] - random(0, ((Cooling * 10) / Size) + 2));

        // Next drift heat up and diffuse it a little but
        for (int i = 0; i < Size; i++)
            heat[i] = (heat[i] * BlendSelf + 
                       heat[(i + 1) % Size] * BlendNeighbor1 + 
                       heat[(i + 2) % Size] * BlendNeighbor2 + 
                       heat[(i + 3) % Size] * BlendNeighbor3) 
                      / BlendTotal;

        // Randomly ignite new sparks down in the flame kernel
        for (int i = 0; i < Sparks; i++)
        {
            if (random(255) < Sparking)
            {
                int y = Size - 1 - random(SparkHeight);
                heat[y] = heat[y] + random(160, 255); // This randomly rolls over sometimes of course, and that's essential to the effect
            }
        }

        // Finally convert heat to a color
        for (int i = 0; i < Size; i++)
        {
            CRGB color = HeatColor(heat[i]);
            int j = bReversed ? (Size - 1 - i) : i;
            DrawPixels(j, 1, color);
            if (bMirrored)
            {
                int j2 = !bReversed ? (2 * Size - 1 - i) : Size + i;
                DrawPixels(j2, 1, color);
            }
        }
    }
};
