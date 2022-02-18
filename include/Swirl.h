/* Swirl Effect by Ryan Owens
*/
#include <Arduino.h>
#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>

extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

#define SWIRL_SIZE 4
const int maxSwirls = 12;

struct Swirl{
    int pos;                // Starting position of Swirl
    int startTime;          // Starting delay
    int lifecount;          // Lifetime count for start delay
    uint8_t count;          // Lifetime count for states of swirl
    uint8_t x;              // X and Y position for pixels of swirl
    uint8_t y;
    CRGB swirlColor;        // Color of Swirl

    Swirl(int _pos, bool randomStartTime){      // Constructor (StartPos and If random startTime)
        pos = _pos;
        startTime = random8(30);
        if(randomStartTime==false) lifecount = 30;  // Set lifecount at maxbounds of randomStart
            else lifecount = 0;
        count = 0;
        swirlColor = ColorFromPalette(currentPalette,pos*1.5,255,LINEARBLEND);
        x = 2;      // Starting pixel of swirl
        y = 1;
    }

    void Reset(){           // Reset Swirl with instant start
        count = 0;
        lifecount = 30;     // For instant start
        x = 2;
        y = 1;
        swirlColor = ColorFromPalette(currentPalette,pos*1.5,255,LINEARBLEND);
        //FastLED.clear();
    }
    void ResetRandom(){     // Reset swirl with random Start time and position
        pos = random8(NUM_LEDS-5);
        startTime = random8(30);
        lifecount = 0;
        count = 0;
        x = 2;
        y = 1;
        swirlColor = ColorFromPalette(currentPalette,pos*1.5,255,LINEARBLEND);
        //FastLED.clear();
    }

    void Draw(bool randomPos){    
      if(lifecount>=startTime){
        switch(count){
            case 0: // First Line
                if(x>1){
                    g_LEDs[x][y+pos] = swirlColor;
                    x--;
                }else {
                    g_LEDs[x][y+pos] = swirlColor;
                    y++;
                    count++;
                }
            break;
            case 1: // First turn
                if(x<SWIRL_SIZE-1){
                    g_LEDs[x][y+pos] = swirlColor;
                    x++;
                }else if(y>1){
                    g_LEDs[x][y+pos] = swirlColor;
                    y--;
                }else{
                    g_LEDs[x][y+pos] = swirlColor;
                    y--;
                    count++;
                }
            break;
            case 2: // Next turn
                if(x>0){
                    g_LEDs[x][y+pos] = swirlColor;
                    x--;
                }else{
                    g_LEDs[x][y+pos] = swirlColor;
                    y++;
                    count++;
                }
            break;
            case 3:
                if(y<SWIRL_SIZE-1){
                    g_LEDs[x][y+pos] = swirlColor;
                    y++;
                }else{
                    g_LEDs[x][y+pos] = swirlColor;
                    x++;
                    count++;
                }
            break;
            case 4: // Last line
                if(x<SWIRL_SIZE-1){
                    g_LEDs[x][y+pos] = swirlColor;
                    x++;
                }else{
                    g_LEDs[x][y+pos] = swirlColor;
                    count++;
                }
            break;
            case 5: // End of Animation
                if(randomPos==false){
                    Reset();
                }else ResetRandom();
            break;
        }
      }
      lifecount++;
    }
}; //End of Struct

// Maximum swirls with random starttime and fixed position
Swirl swirl[maxSwirls] = {Swirl(1,1), Swirl(7,1), Swirl(13,1), Swirl(19,1), Swirl(25,1), 
                            Swirl(31,1), Swirl(37,1), Swirl(43,1), Swirl(49,1), Swirl(55,1), 
                            Swirl(61,1), Swirl(67,1)};

// For Maximum swirls with random starts and positions
Swirl swirlMaxRand[maxSwirls] = {Swirl(1,1), Swirl(7,1), Swirl(13,1), Swirl(19,1), Swirl(25,1), 
                            Swirl(31,1), Swirl(37,1), Swirl(43,1), Swirl(49,1), Swirl(55,1), 
                            Swirl(61,1), Swirl(67,1)};

// For Fixed postions and starts (All Swirl together)
Swirl swirl2[9] = {Swirl(2,0), Swirl(10,0), Swirl(18,0), Swirl(26,0), Swirl(34,0), 
                            Swirl(42,0), Swirl(50,0), Swirl(58,0), Swirl(66,0)};
// Less dense swirls 
//Swirl swirl3[5] = {Swirl(4,0), Swirl(19,0), Swirl(34,0), Swirl(49,0), Swirl(64,0)};

// Less dense and fully random starts and positions
Swirl swirlRandom[4] = {Swirl(random8(NUM_LEDS-5),1), Swirl(random8(NUM_LEDS-5),1), 
                        Swirl(random8(NUM_LEDS-5),1), Swirl(random8(NUM_LEDS-5),1)};


// PATTERN FUNCTIONS
void DrawMaxSwirls(){
    for(int i=0; i<maxSwirls; i++) swirl[i].Draw(0);
    for (int s=0; s<NUM_STRIPS; s++) fadeToBlackBy(g_LEDs[s], NUM_LEDS, 70);
}
void DrawMaxSwirlsRandom(){
    for(int i=0; i<maxSwirls; i++) swirlMaxRand[i].Draw(1);
    for (int s=0; s<NUM_STRIPS; s++) fadeToBlackBy(g_LEDs[s], NUM_LEDS, 120);
}
void DrawSwirlsTogether(){
    for(int i=0; i<9; i++) swirl2[i].Draw(0);
    for (int s=0; s<NUM_STRIPS; s++) fadeToBlackBy(g_LEDs[s], NUM_LEDS, 70);
}
void DrawSwirlsTogetherRainbow(){
    for(int i=0; i<9; i++) {
        swirl2[i].swirlColor = ColorFromPalette(currentPalette,(swirl2[i].y+1)*50,255,LINEARBLEND);
        swirl2[i].Draw(0);
    }
    for (int s=0; s<NUM_STRIPS; s++) fadeToBlackBy(g_LEDs[s], NUM_LEDS, 70);
}
void DrawSwirlsRandom(){
    for(int i=0; i<4; i++) swirlRandom[i].Draw(1);
    for (int s=0; s<NUM_STRIPS; s++) fadeToBlackBy(g_LEDs[s], NUM_LEDS, 100);
}

void DrawAllSwirlPatterns(){
    static int swirlCount=0;
    if(swirlCount<100) DrawSwirlsRandom();
    if(swirlCount>=100 && swirlCount<200) DrawMaxSwirlsRandom();
    if(swirlCount>=200 && swirlCount<300) DrawMaxSwirls();
    if(swirlCount>=300 && swirlCount<400) DrawSwirlsTogether();
    if(swirlCount>=400) DrawSwirlsTogetherRainbow();
    if(swirlCount>500) swirlCount=0;
    swirlCount++;
}