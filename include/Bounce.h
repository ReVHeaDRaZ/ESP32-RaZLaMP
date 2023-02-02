//+--------------------------------------------------------------------------
// File:        bounce.h     
//
// Description:
//
//      Bouncing Ball effect by Dave Plummer modified by Ryan Owens 
//      to Draw with floating point on multiple LED strips
//
//      Jun-15-2021     Ryan Owens   
//
//---------------------------------------------------------------------------

#include <sys/time.h>                   // For time-of-day

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

using namespace std;
#include <vector>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))   // Count elements in a static array

static const CRGB ballColors [] =
{
    CRGB::Green,
    CRGB::Red,
    CRGB::Blue,
    CRGB::Orange,
    CRGB::Indigo
};

class BouncingBallEffect
{
  private:

    double InitialBallSpeed(double height) const
    {
        return sqrt(-2 * Gravity * height);         // Because MATH!
    }

    size_t  _cLength;           
    size_t  _cBalls;
    byte    _fadeRate;
    bool    _bMirrored;
    
    const double Gravity = -9.81;                   // Because PHYSICS!
    const double StartHeight = 1;                   // Drop balls from max height initially
    const double ImpactVelocity = InitialBallSpeed(StartHeight);
    const double SpeedKnob = 3.0;                   // Higher values will slow the effect

    vector<double> ClockTimeAtLastBounce, Height, BallSpeed, Dampening;
    vector<CRGB>   Colors;

    static double Time()
    {
        timeval tv = { 0 };
        gettimeofday(&tv, nullptr);
        return (double)(tv.tv_usec / 1000000.0 + (double) tv.tv_sec);
    }
    
  public:

    // BouncingBallEffect
    //
    // Caller specs strip length, number of balls, persistence level (255 is least), and whether the
    // balls should be drawn mirrored from each side.

    BouncingBallEffect(size_t cLength, size_t ballCount = 3, byte fade = 0, bool bMirrored = false)
        : _cLength(cLength - 1),
          _cBalls(ballCount),
          _fadeRate(fade),
          _bMirrored(bMirrored),
          ClockTimeAtLastBounce(ballCount),
          Height(ballCount),
          BallSpeed(ballCount),
          Dampening(ballCount),
          Colors(ballCount)
    {
        for (size_t i = 0; i < ballCount; i++)
        {
            Height[i]                = StartHeight;         // Current Ball Height
            ClockTimeAtLastBounce[i] = Time();              // When ball last hit ground state
            Dampening[i]             = 0.90 - i / pow(_cBalls, 2);  // Bounciness of this ball
            BallSpeed[i]             = InitialBallSpeed(Height[i]); // Don't dampen initial launch
            Colors[i]                = ballColors[i % ARRAYSIZE(ballColors) ];
        }
    }

    // Draw
    //
    // Draw each of the balls.  When any ball settles with too little energy, it is "kicked" to restart it

    virtual void Draw(int stripNumber)
    {
        if (_fadeRate != 0)
        {
            for (int s = 0; s < NUM_STRIPS; s++){
                for (size_t i = 0; i < _cLength; i++)
                    g_LEDs[s][i].fadeToBlackBy(_fadeRate);
            }
        }
        else
            FastLED.clear();
        
        // Draw each of the balls

        for (size_t i = 0; i < _cBalls; i++)
        {
            double TimeSinceLastBounce = (Time() - ClockTimeAtLastBounce[i]) / SpeedKnob;

            // Use standard constant acceleration function - https://en.wikipedia.org/wiki/Acceleration
            Height[i] = 0.5 * Gravity * pow(TimeSinceLastBounce, 2.0) + BallSpeed[i] * TimeSinceLastBounce;

            // Ball hits ground - bounce!
            if (Height[i] < 0)
            {
                Height[i] = 0;
                BallSpeed[i] = Dampening[i] * BallSpeed[i];
                ClockTimeAtLastBounce[i] = Time();

                if (BallSpeed[i] < 0.02)
                    BallSpeed[i] = InitialBallSpeed(StartHeight);
            }

            //size_t position = (size_t)(Height[i] * (_cLength - 1) / StartHeight);
            float position = Height[i] * (_cLength - 1) / StartHeight;

            //g_LEDs[i][position]   += Colors[i];
            //g_LEDs[i][position+1] += Colors[i];
            DrawPixels(i, position,1,Colors[i]);
            
            if (_bMirrored)
            {
                //g_LEDs[i][_cLength - 1 - position] += Colors[i];
                //g_LEDs[i][_cLength - position]     += Colors[i];
                DrawPixels(i,_cLength - 1 - position,1,Colors[i]);
            }
        }
    }
};

BouncingBallEffect balls(NUM_LEDS, NUM_STRIPS, 100, false);               // Setup BouncingBalls Objects
BouncingBallEffect ballsMirrored(NUM_LEDS, NUM_STRIPS, 100, true);        // Mirrored