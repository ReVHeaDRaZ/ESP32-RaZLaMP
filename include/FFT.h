// RaZ's FFT Header
// (Heavily) adapted from https://github.com/G6EJD/ESP32-8266-Audio-Spectrum-Display/blob/master/ESP32_Spectrum_Display_02.ino
// Edited for single strip support for compatibility with different layouts.

#include <arduinoFFT.h>

#define SAMPLES         512         // Must be a power of 2
#define SAMPLING_FREQ   40000       // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE       1000         // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control. 200 for KY-38
#define AUDIO_IN_PIN    35          // Signal in on this pin
#define NUM_BANDS       8           // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
#define NOISE           600         // Used as a crude noise filter, values below this are ignored
#define TOP             254         // Don't allow the band values to go over this amount

// Sampling and FFT varibles
unsigned int sampling_period_us;
byte peak[] =           {0,0,0,0,0,0,0,0};          // The length of these arrays must be >= NUM_BANDS
int oldBarHeights[] =   {0,0,0,0,0,0,0,0};
int bandValues[] =      {0,0,0,0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime;
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

void SampleAudio(){
  // Reset bandValues[]
  for (int i = 0; i<NUM_BANDS; i++){
    bandValues[i] = 0;
  }

  // Sample the audio pin
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(AUDIO_IN_PIN); // A conversion takes about 9.7uS on an ESP32
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* chill */ }
  }

  // Compute FFT
  FFT.DCRemoval();
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();
}

void AnalyseFFT(){
  // Analyse FFT results
  for (int i = 2; i < (SAMPLES/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    if (vReal[i] > NOISE) {                    // Add a crude noise filter
    
    //8 bands, for 512 Samples 16kHz top band
      if (i<=2 )           bandValues[0]  += (int)vReal[i]; //125hz
      if (i>3   && i<=5  ) bandValues[1]  += (int)vReal[i]; //250hz
      if (i>5   && i<=7 ) bandValues[2]  += (int)vReal[i];  //500hz
      if (i>7  && i<=15 ) bandValues[3]  += (int)vReal[i];  //1000hz
      if (i>15  && i<=30 ) bandValues[4]  += (int)vReal[i]; //2000hz
      if (i>30  && i<=53) bandValues[5]  += (int)vReal[i];  //4000hz
      if (i>53 && i<=200) bandValues[6]  += (int)vReal[i];  //8000hz
      if (i>200          ) bandValues[7]  += (int)vReal[i]; //16000hz

    /*8 bands, 12kHz top band
      if (i<=3 )           bandValues[0]  += (int)vReal[i];
      if (i>3   && i<=6  ) bandValues[1]  += (int)vReal[i];
      if (i>6   && i<=13 ) bandValues[2]  += (int)vReal[i];
      if (i>13  && i<=27 ) bandValues[3]  += (int)vReal[i];
      if (i>27  && i<=55 ) bandValues[4]  += (int)vReal[i];
      if (i>55  && i<=112) bandValues[5]  += (int)vReal[i];
      if (i>112 && i<=229) bandValues[6]  += (int)vReal[i];
      if (i>229          ) bandValues[7]  += (int)vReal[i];*/

    /*16 bands, 12kHz top band
      if (i<=2 )           bandValues[0]  += (int)vReal[i];
      if (i>2   && i<=3  ) bandValues[1]  += (int)vReal[i];
      if (i>3   && i<=5  ) bandValues[2]  += (int)vReal[i];
      if (i>5   && i<=7  ) bandValues[3]  += (int)vReal[i];
      if (i>7   && i<=9  ) bandValues[4]  += (int)vReal[i];
      if (i>9   && i<=13 ) bandValues[5]  += (int)vReal[i];
      if (i>13  && i<=18 ) bandValues[6]  += (int)vReal[i];
      if (i>18  && i<=25 ) bandValues[7]  += (int)vReal[i];
      if (i>25  && i<=36 ) bandValues[8]  += (int)vReal[i];
      if (i>36  && i<=50 ) bandValues[9]  += (int)vReal[i];
      if (i>50  && i<=69 ) bandValues[10] += (int)vReal[i];
      if (i>69  && i<=97 ) bandValues[11] += (int)vReal[i];
      if (i>97  && i<=135) bandValues[12] += (int)vReal[i];
      if (i>135 && i<=189) bandValues[13] += (int)vReal[i];
      if (i>189 && i<=264) bandValues[14] += (int)vReal[i];
      if (i>264          ) bandValues[15] += (int)vReal[i];*/
    }
  }
}

void DrawBandsToStrip(){
  // Process the FFT data into bar heights
  for (byte band = 0; band < NUM_BANDS; band++) {

    // Scale the bars for the display
    int barHeight = bandValues[band] / AMPLITUDE;
    if (barHeight > TOP) barHeight = TOP;

    // Small amount of averaging between frames
    barHeight = ((oldBarHeights[band] * 1) + barHeight) / 2;

    // Move peak up
    if (barHeight > peak[band]) {
      peak[band] = min(TOP, barHeight);
    }

   // Draw bands to Strips with barHeight used for intensity   
  for(int s=0; s<NUM_STRIPS; s++){  
    for(int j=0; j<(NUM_LEDS/20); j++){
      g_LEDs[s][(band+1)*(NUM_LEDS/10)+j]=CHSV(((band+1)*20),255,barHeight);
    }
  }

   // Save oldBarHeights for averaging later
    oldBarHeights[band] = barHeight;
  }// End Band For Loop
}

void ScaleBands(){
for (int i=0; i<NUM_BANDS; i++){
    bandValues[i] = bandValues[i] / AMPLITUDE;
    if (bandValues[i] > TOP) bandValues[i] = TOP;
  }
}

void Sndwave() {
  for(int s = 0; s < NUM_STRIPS; s++){
    g_LEDs[s][NUM_LEDS/2] = ColorFromPalette(currentPalette, bandValues[0], bandValues[0], currentBlending); // Put the sample into the center
  
    for (int i = NUM_LEDS - 1; i > NUM_LEDS/2; i--) {       //move to the left      // Copy to the left, and let the fade do the rest.
      g_LEDs[s][i] = g_LEDs[s][i - 1];
    }

    for (int i = 0; i < NUM_LEDS/2; i++) {                  // move to the right    // Copy to the right, and let the fade to the rest.
      g_LEDs[s][i] = g_LEDs[s][i + 1];
    }
  }
} // sndwave()

void BandFillNoise8() {                                                       // Add Perlin noise with modifiers from the soundmems routine.
  static int16_t xdist;
  static int16_t ydist;
  int maxLen = bandValues[0];
  if (bandValues[0] >NUM_LEDS) maxLen = NUM_LEDS;
  for(int s = 0; s < NUM_STRIPS; s++){
    for (int i = (NUM_LEDS-maxLen)/2; i <(NUM_LEDS+maxLen+1)/2; i++) {      // The louder the sound, the wider the soundbar.
      uint8_t index = inoise8(i*bandValues[0]+xdist, ydist+i*bandValues[0]);        // Get a value from the noise function. I'm using both x and y axis.
      g_LEDs[s][i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);  // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    }
  }
  xdist=xdist+beatsin8(5,0,10);
  ydist=ydist+beatsin8(4,0,10);
                                                                        
} // fillnoise8()