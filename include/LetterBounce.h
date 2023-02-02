/* LetterBounce Effects by Ryan Owens
    Letters can be set from textString
*/
#include <Arduino.h>
#define FASTLED_INTERNAL          // Supress Build Banner
#include <FastLED.h>
#include <RazFont.h>
extern CRGB g_LEDs[NUM_STRIPS][NUM_LEDS];

const byte fontWidth  = 4;      // Font data sizes
const byte fontHeight = 5;
const byte fontDataLength = fontWidth*fontHeight;

const int maxLetters = 8;                           // maxLetters able to fit on display
char textString[9] = {'P','M','A','L','Z','A','R'}; // Starting letters, setable from WIFI Server

struct LetterBounce {
    int dataX = 0;              // For reading font array data
    int dataY = 0;
    
    int   yPos = NUM_LEDS-fontHeight;
    int   endPos;               // For letter stop position on strip
    byte  letter;               // To choose letter to be drawn
    
    float vel       = -1;
    float gravity   = -0.1;
    float dampen    = -0.95;
    bool  done      = 0;        // If the letter has finished bouncing
    int   particleRowTime = 0;  // For timing when rows of particles errode

    int particleX[20];          // Particle X,Y,Velocity and color
    float particleY[20];
    float particleV[20];
    CRGB particleC[20];

    
    // Constructor to set letter type 0-R, 1-A, 2-Z
    LetterBounce(byte _letter, int _endPos){
        endPos = _endPos;
        letter = _letter;
    }
    
    // Draw letters and set particle positions
    void DrawFromFont(int ypos, CRGB color, const byte array[]){
        for(int i=0; i<fontDataLength; i++){
            particleC[i] = CRGB::Black;
            if(array[i]==1){
                dataY = i / fontWidth;
                dataX = i % fontWidth;
                g_LEDs[dataX][dataY+ypos] = color;
                particleX[i] = dataX;
                particleY[i] = dataY+ypos;
                particleC[i] = color;
            }
        }
    }
    
    // Single object RAZ animations
    void DrawRazUpDown(){ // Draws RAZ Scrolling UP/DOWN change color on direction
        static int ypos = 47;
        static int direction = -1;
        static byte paletteColor;
        currentPalette = RainbowColors_p;

           FastLED.clear();
            ypos += direction;
            if(ypos<=0 || ypos>=47) {direction*=-1; paletteColor=random8();}
            DrawFromFont(ypos+20, ColorFromPalette(currentPalette,paletteColor,250,currentBlending),razR);
            DrawFromFont(ypos+10, ColorFromPalette(currentPalette,paletteColor,250,currentBlending),razA);
            DrawFromFont(ypos, ColorFromPalette(currentPalette,paletteColor,250,currentBlending),razZ);
        }

    void DrawRazDown(){ // Scroll RAZ Palette colored to Bottom and fade out
        static int ypos = 47;
        static int count = 150;
        currentPalette = PartyColors_p;
        EVERY_N_MILLISECONDS(15){
            if(ypos>0){
                ypos--;
                FastLED.clear();
            }
            if(count>50){
                DrawFromFont(ypos+20, ColorFromPalette(currentPalette,100,250,currentBlending),razR);
                DrawFromFont(ypos+10, ColorFromPalette(currentPalette,150,250,currentBlending),razA);
                DrawFromFont(ypos, ColorFromPalette(currentPalette,250,250,currentBlending),razZ);
            }else {
                for(int s=0;s<NUM_STRIPS;s++){
                    for(int i=0; i<NUM_LEDS;i++) g_LEDs[s][i].fadeToBlackBy(40);
                }
            }
            count--;

            if(count<0) {
                count=150;
                ypos=47;
            }
        }
    }
    void DrawNumber(byte number, int _pos, CRGB _color){
        switch(number){
            case 0:
                DrawFromFont(_pos,_color,raz0);
                break;
            case 1:
                DrawFromFont(_pos,_color,raz1);
                break;
            case 2:
                DrawFromFont(_pos,_color,razZ);
                break;
            case 3:
                DrawFromFont(_pos,_color,raz3);
                break;
            case 4:
                DrawFromFont(_pos,_color,raz4);
                break;
            case 5:
                DrawFromFont(_pos,_color,raz5);
                break;
            case 6:
                DrawFromFont(_pos,_color,raz6);
                break;
            case 7:
                DrawFromFont(_pos,_color,raz7);
                break;
            case 8:
                DrawFromFont(_pos,_color,raz8);
                break;
            case 9:
                DrawFromFont(_pos,_color,raz9);
                break;
            case 10:
                DrawFromFont(_pos,_color,razColon);
                break;
            default:
                DrawFromFont(_pos,_color,razSpace);
        } 
    }
    
    void DrawLetter(byte letter, int _pos, CRGB _color){
        switch(letter){
            case 0:
                DrawFromFont(_pos,_color,razSpace);
                break;
            case 32:
                DrawFromFont(_pos,_color,razSpace);
                break;
            case 65:
                DrawFromFont(_pos,_color,razA);
                break;
            case 66:
                DrawFromFont(_pos,_color,razB);
                break;
            case 67:
                DrawFromFont(_pos,_color,razC);
                break;
            case 68:
                DrawFromFont(_pos,_color,razD);
                break;
            case 69:
                DrawFromFont(_pos,_color,razE);
                break;
            case 70:
                DrawFromFont(_pos,_color,razF);
                break;
            case 71:
                DrawFromFont(_pos,_color,razG);
                break;
            case 72:
                DrawFromFont(_pos,_color,razH);
                break;
            case 73:
                DrawFromFont(_pos,_color,raz1);
                break;
            case 74:
                DrawFromFont(_pos,_color,razJ);
                break;
            case 75:
                DrawFromFont(_pos,_color,razK);
                break;
            case 76:
                DrawFromFont(_pos,_color,razL);
                break;
            case 77:
                DrawFromFont(_pos,_color,razM);
                break;
            case 78:
                DrawFromFont(_pos,_color,razN);
                break;
            case 79:
                DrawFromFont(_pos,_color,raz0);
                break;
            case 80:
                DrawFromFont(_pos,_color,razP);
                break;
            case 81:
                DrawFromFont(_pos,_color,razQ);
                break;
            case 82:
                DrawFromFont(_pos,_color,razR);
                break;
            case 83:
                DrawFromFont(_pos,_color,raz5);
                break;
            case 84:
                DrawFromFont(_pos,_color,razT);
                break;
            case 85:
                DrawFromFont(_pos,_color,razU);
                break;
            case 86:
                DrawFromFont(_pos,_color,razV);
                break;
            case 87:
                DrawFromFont(_pos,_color,razW);
                break;
            case 88:
                DrawFromFont(_pos,_color,razX);
                break;
            case 89:
                DrawFromFont(_pos,_color,razY);
                break;
            case 90:
                DrawFromFont(_pos,_color,razZ);
                break;
            default:
                DrawFromFont(_pos,_color,razSpace);
        } 
    }

    // Seperate object functions
    void DrawLetterDown(CRGB _color){  // Draws single letters down strip
        if(yPos>0){                    // and sets done at bottom
            yPos--;
            DrawLetter(letter, yPos, _color);
            if(yPos<=endPos) yPos=0;
        }else {
            done = 1;
            yPos = NUM_LEDS-fontHeight;
        }     
    }

    void DrawLetterBounce(CRGB _color){     // Drops single letters from top
        yPos += vel;                        // with gravity and bounce
        vel += gravity;
        
        // Bounce
        if(yPos<=endPos){
            vel *= dampen;
            yPos = endPos;
        }
            
        DrawLetter(letter, yPos, _color);
        
        // Set done when finished bouncing
        if(vel<0.1 && vel>-0.1){
            if(yPos<=endPos+1){
                done = 1;
                yPos = NUM_LEDS-fontHeight;
            }
        }
    }

    void InitParticles(){
        for(int i=0;i<20;i++){
            particleV[i] = (float(random16(50, 1000)) / 1000.0)*.5;
        }
        particleRowTime=0;
    }
    void DrawParticles(){           // Add Velocity and draw particles
        for(int i=0;i<20;i++){
            particleY[i]+=particleV[i];
            if(particleY[i]<NUM_LEDS-1)
                DrawPixels(particleX[i],particleY[i],1,particleC[i]);
        }
        particleRowTime++;
    }
    void DrawParticlesGradually(){  // Add velocity row by row and draw
        if(particleRowTime>1){
            for(byte v=16;v<20;v++)particleY[v]+=particleV[v];
        }
        if(particleRowTime>30){
            for(byte v=12;v<16;v++)particleY[v]+=particleV[v];
        }
        if(particleRowTime>60){
            for(byte v=8;v<12;v++)particleY[v]+=particleV[v];
        }
        if(particleRowTime>90){
            for(byte v=4;v<8;v++)particleY[v]+=particleV[v];
        }
        if(particleRowTime>120){
            for(byte v=0;v<4;v++)particleY[v]+=particleV[v];
        }
        for(int i=0;i<20;i++){
            if(particleY[i]<NUM_LEDS-1)
                DrawPixels(particleX[i],particleY[i],1,particleC[i]);
        }
    particleRowTime++;
    }
};

// Instansiate Objects for seperate letters (ASCII,endPos)
LetterBounce letters[maxLetters] = {LetterBounce(textString[0],0),
                                    LetterBounce(textString[1],9),
                                    LetterBounce(textString[2],18),
                                    LetterBounce(textString[3],27),
                                    LetterBounce(textString[4],36),
                                    LetterBounce(textString[5],45),
                                    LetterBounce(textString[6],54),
                                    LetterBounce(textString[7],63)};
LetterBounce razNumbers(0,0);

// Pattern Functions----------------------------------------------------------------------------
void CountTo100(){                      // Draws numbers counting to 100
    static byte numbercount   = 0;
    static byte numbercount10 = 0;

    int randpos = random8(0+fontHeight*3,NUM_LEDS-(fontHeight*3));
    FastLED.clear();
    razNumbers.DrawNumber(numbercount10,randpos+fontHeight+2,ColorFromPalette(currentPalette,0,255));
    razNumbers.DrawNumber(numbercount,randpos,ColorFromPalette(currentPalette,0,255));
    numbercount++;
    if(numbercount>9){
        numbercount=0;
        numbercount10++;
        if(numbercount10>9)numbercount10=0;
    }
}
void DrawClock(){       // Draw Clock using main.cpp global variables
    int clockpos=15;
    //for(int s=0;s<NUM_STRIPS;s++){    // Draw Background
     //   fill_solid(g_LEDs[s],NUM_LEDS,ColorFromPalette(currentPalette,150,5));
    //}
    if(!clockCount1000==0)
        razNumbers.DrawNumber(clockCount1000,clockpos+(fontHeight+2)*5,ColorFromPalette(currentPalette,0,255));
    razNumbers.DrawNumber(clockCount100,clockpos+(fontHeight+2)*4,ColorFromPalette(currentPalette,0,255));
    razNumbers.DrawNumber(10,clockpos+(fontHeight+3)*2,ColorFromPalette(currentPalette,0,255)); //Colon
    razNumbers.DrawNumber(clockCount10,clockpos+fontHeight+2,ColorFromPalette(currentPalette,0,255));
    razNumbers.DrawNumber(clockCount0,clockpos,ColorFromPalette(currentPalette,0,255));
}
void DrawAutoOnText(){       // Draw "AUTO ON"
    int textPos=0;
    const char tempText[8] = "NO OTUA";
    for(int i=0; i<8; i++)
        razNumbers.DrawLetter(tempText[i],textPos+((fontHeight+2)*i),CRGB::Red);
}
void DrawAutoOffText(){       // Draw "AUTO OFF"
    int textPos=0;
    const char tempText[9] = "FFO OTUA";
    for(int i=0; i<9; i++)
        razNumbers.DrawLetter(tempText[i],textPos+((fontHeight+2)*i),CRGB::Red);
}

void DrawRazEffect(){           // Scrolls each letter to bottom after each other
    static int count = 100;     // and fades out when done
    static int currentLetter = 0;
    
    if(!letters[currentLetter].done){
        FastLED.clear();
        for(int i=0; i<=currentLetter-1;i++) letters[i].DrawLetter(letters[i].letter, letters[i].endPos, CRGB::Green);
        letters[currentLetter].DrawLetterDown(CRGB::Red);
        if(letters[currentLetter].letter==0) currentLetter = maxLetters-1; // If end of string
    }else if(currentLetter<maxLetters-1) currentLetter++;
        
     else{
        if(count==100) letters[currentLetter].DrawLetter(letters[currentLetter].letter, letters[currentLetter].endPos, CRGB::Green);
        for(int s=0;s<NUM_STRIPS;s++){
            for(int i=0;i<NUM_LEDS;i++){
                g_LEDs[s][i].fadeToBlackBy(5);
            }
        }
        count--;
     }
    if(count<=0){
        count = 100;
        for(int i=0; i<maxLetters; i++) letters[i].done=0;
        currentLetter=0;
    }        
}

void DrawRazBounce(){               // Same as above with bounce
    static int count = 150;
    static int currentLetter = 0;
    
    if(!letters[currentLetter].done){
        FastLED.clear();
        for(int i=0; i<=currentLetter-1;i++) letters[i].DrawLetter(letters[i].letter, letters[i].endPos, CRGB::Green);
        letters[currentLetter].DrawLetterBounce(CRGB::Red);
        if(letters[currentLetter].letter==0) currentLetter = maxLetters-1; // If end of string
    }else if(currentLetter<maxLetters-1) currentLetter++;
      else{
        if(count==150) letters[currentLetter].DrawLetter(letters[currentLetter].letter, letters[currentLetter].endPos, CRGB::Green);
        for(int s=0;s<NUM_STRIPS;s++){
            for(int i=0;i<NUM_LEDS;i++){
                g_LEDs[s][i].fadeToBlackBy(5);
            }
        }
        count--;
      }
    if(count<=0){
        count = 150;
        for(int i=0; i<maxLetters; i++) letters[i].done=0;
        currentLetter=0;
    }          
}

void DrawRazBounceWithPalette(){               // Same as above with bounce and palette
    static int count = 150;
    static int currentLetter = 0;
    
    if(!letters[currentLetter].done){
        FastLED.clear();
        for(int i=0; i<=currentLetter-1;i++) letters[i].DrawLetter(letters[i].letter, letters[i].endPos, ColorFromPalette(currentPalette,letters[i].endPos*3,255,currentBlending));
        letters[currentLetter].DrawLetterBounce(ColorFromPalette(currentPalette,letters[currentLetter].yPos*3,255,currentBlending));
        if(letters[currentLetter].letter==0) currentLetter = maxLetters-1; // If end of string
    }else if(currentLetter<maxLetters-1) currentLetter++;
      else{
        if(count==150) letters[currentLetter].DrawLetter(letters[currentLetter].letter, letters[currentLetter].endPos, ColorFromPalette(currentPalette,letters[currentLetter].endPos*3,255,currentBlending));
        for(int s=0;s<NUM_STRIPS;s++){
            for(int i=0;i<NUM_LEDS;i++){
                g_LEDs[s][i].fadeToBlackBy(5);
            }
        }
        count--;
      }
    if(count<=0){
        count = 150;
        for(int i=0; i<maxLetters; i++) letters[i].done=0;
        currentLetter=0;
    }          
}

void DrawRazBounceWithExplodeAll(){               // Same as above with bounce, palette
    static int count = 300;                     // and explode particles
    static int currentLetter = 0;
    
    if(!letters[currentLetter].done){
        FastLED.clear();
        for(int i=0; i<=currentLetter-1;i++) letters[i].DrawLetter(letters[i].letter, letters[i].endPos, ColorFromPalette(currentPalette,letters[i].endPos*4,255,currentBlending));
        letters[currentLetter].DrawLetterBounce(ColorFromPalette(currentPalette,letters[currentLetter].yPos*4,255,currentBlending));
        if(letters[currentLetter].letter==0) currentLetter = maxLetters-1; // If end of string
    }else if(currentLetter<maxLetters-1) currentLetter++;

      else{ if(count==300){ 
            letters[currentLetter].DrawLetter(letters[currentLetter].letter, letters[currentLetter].endPos, ColorFromPalette(currentPalette,letters[currentLetter].endPos*4,255,currentBlending));
            for(int i=0; i<maxLetters; i++) letters[i].InitParticles();
          }
        FastLED.clear();
        
        for(int i=currentLetter; i>0; i--){
            letters[i].DrawParticles();
        }    
        count--;
      }
    
    if(count<=0){
        count = 300;
        for(int i=0; i<maxLetters; i++) letters[i].done=0;
        currentLetter=0;
    }          
}
void DrawRazBounceWithExplode(){               // Same as above with bounce, palette
    static int count = 350;                     // and explode particles
    static int currentLetter = 0;
    
    if(!letters[currentLetter].done){
        FastLED.clear();
        for(int i=0; i<=currentLetter-1;i++) letters[i].DrawLetter(letters[i].letter, letters[i].endPos, ColorFromPalette(currentPalette,letters[i].endPos*4,255,currentBlending));
        letters[currentLetter].DrawLetterBounce(ColorFromPalette(currentPalette,letters[currentLetter].yPos*4,255,currentBlending));
        // If end of string
        if(letters[currentLetter].letter==0){ 
            for(int temp=0; temp<maxLetters; temp++){
                letters[temp].done=1;
                letters[temp].particleRowTime=51;
            }
            currentLetter = maxLetters-1; 
        }
    }else if(currentLetter<maxLetters-1 && count==350) currentLetter++;
      
      else{ 
        if(count==350){ 
            letters[currentLetter].DrawLetter(letters[currentLetter].letter, letters[currentLetter].endPos, ColorFromPalette(currentPalette,letters[currentLetter].endPos*4,255,currentBlending));
            for(int i=0; i<maxLetters; i++) letters[i].InitParticles();
        }
        FastLED.clear();
        letters[currentLetter].DrawParticles();
        
        for(int i=currentLetter-1; i>=0; i--){
            letters[i].DrawLetter(letters[i].letter, letters[i].endPos, ColorFromPalette(currentPalette,letters[i].endPos*4,255,currentBlending));
        }
        
        if(letters[currentLetter].particleRowTime>50) currentLetter--;
        for(int i=currentLetter+1; i<maxLetters-1; i++) letters[i].DrawParticles();

        count--;
      }
    if(count<=0){
        count = 350;
        for(int i=0; i<maxLetters; i++) letters[i].done=0;
        currentLetter=0;
    }          
}

void DrawRazBounceWithMelt(){               // Same as above with bounce, palette
    static int count = 850;                     // and explode particles
    static int currentLetter = 0;
    
    if(!letters[currentLetter].done){
        FastLED.clear();
        for(int i=0; i<=currentLetter-1;i++) letters[i].DrawLetter(letters[i].letter, letters[i].endPos, ColorFromPalette(currentPalette,letters[i].endPos*4,255,currentBlending));
        letters[currentLetter].DrawLetterBounce(ColorFromPalette(currentPalette,letters[currentLetter].yPos*4,255,currentBlending));
        // IF end of string, set remaining letters done and finished time
        if(letters[currentLetter].letter==0){ 
            for(int temp=0; temp<maxLetters; temp++){
                letters[temp].done=1;
                letters[temp].particleRowTime=121;
            }
            currentLetter = maxLetters-1; 
        }
    }else if(currentLetter<maxLetters-1 && count==850) currentLetter++;
      else{ 
        if(count==850){ 
            letters[currentLetter].DrawLetter(letters[currentLetter].letter, letters[currentLetter].endPos, ColorFromPalette(currentPalette,letters[currentLetter].endPos*4,255,currentBlending));
            for(int i=0; i<maxLetters; i++) letters[i].InitParticles();
        }
        FastLED.clear();
        letters[currentLetter].DrawParticlesGradually();
        
        for(int i=currentLetter-1; i>=0; i--){
            letters[i].DrawLetter(letters[i].letter, letters[i].endPos, ColorFromPalette(currentPalette,letters[i].endPos*4,255,currentBlending));
        }
        
        if(letters[currentLetter].particleRowTime>120) currentLetter--;
        for(int i=currentLetter+1; i<maxLetters-1; i++) letters[i].DrawParticlesGradually();
    
        count--;
    }
    if(count<=0){
        count = 850;
        for(int i=0; i<maxLetters; i++) letters[i].done=0;
        currentLetter=0;
    }          
}


void SetLetters(){
    //Reset all letters
    for(int i=0; i<maxLetters; i++) letters[i].letter=0;
    //Then set from textString (Set on WIFI Server)
    for(int i=0; i<=strlen(textString); i++) letters[i].letter = textString[i];
    for(int i=0; i<8; i++) Serial.print(textString[i]);
}