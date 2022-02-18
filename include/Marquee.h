//-------------------------------------------------
//              Marquee Effect
//-------------------------------------------------
#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>


void DrawMarquee()
{
    static byte j = 0;
    j+=4;
    byte k = j;

    // Roughly equivalent to fill_rainbow(g_LEDs, NUM_LEDS, j, 8);

    CRGB c;
    for (int s = 0; s < NUM_STRIPS; s++){
        for (int i = 0; i < NUM_LEDS; i ++)
            g_LEDs[s][i] = c.setHue(k+=8);
        
        static int scroll = 0;
        scroll++;

        for (int i = scroll % 5; i < NUM_LEDS - 1; i += 5)
        {
            g_LEDs[s][i] = CRGB::Black;
            g_LEDs[s][i+1] = CRGB::Black;

        }
    }
    delay(60);
}

void DrawMarqueeMirrored()
{
    static byte j = 0;
    j+=4;
    byte k = j;

    // Roughly equivalent to fill_rainbow(g_LEDs, NUM_LEDS, j, 8);

    CRGB c;
    for (int s = 0; s < NUM_STRIPS; s++){
        for (int i = 0; i < (NUM_LEDS + 1) / 2; i ++)
        {
            g_LEDs[s][i] = c.setHue(k);
            g_LEDs[s][NUM_LEDS - 1 - i] = c.setHue(k);
            k+= 8;
        }


        static int scroll = 0;
        scroll++;

        for (int i = scroll % 5; i < NUM_LEDS / 2; i += 5)
    {
            g_LEDs[s][i] = CRGB::Black;
            g_LEDs[s][NUM_LEDS - 1 - i] = CRGB::Black;
        }   
    }
    delay(60);
}

void DrawMarqueeFloat()
{
    FastLED.clear();
    static byte j = 0;
    j+=4;
    byte k = j;

    // Roughly equivalent to fill_rainbow(g_LEDs, NUM_LEDS, j, 8);

    CRGB c;
    for (int s = 0; s < NUM_STRIPS; s++){
        static float scroll = 0;
        scroll+=0.1;

        for (float i = scroll; i < NUM_LEDS - 1; i += 5)
        {
            c.setHue(k+=8);
           DrawPixels(s,i,2,c);
        }
    }
    delay(60);
}