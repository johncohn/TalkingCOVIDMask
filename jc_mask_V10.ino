//**********************************************************************************
//   talking face mask project -  copyright John Cohn Aug 2020 johncohnvt@gmail.com
//   based on earlier code used for 6ton Army  truck Booga https://www.youtube.com/watch?v=CejMA55O8L0
//   by John Cohn. Max Cohn and Colin Bramstedt 2013
//
//**********************************************************************************
// This #include statement was automatically added by the Particle IDE.
#define PHOTON //  only tested this on the Particle photon… Comment this out if you wanna try on a regular Arduino.… 
// No guarantees on the results

#ifdef PHOTON
#include <SparkIntervalTimer.h>
#include <neomatrix.h>
#else

#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#endif
#include <math.h>
//**********************************************************************************

//**********************************************************************************
int anaInPin = 0; // analog in pin (note this is anlaog 0, not digital
//**********************************************************************************
// NOTE:  you  will likely need to tune these to your microphone !
//**********************************************************************************
int smin = 550;    // normalize input signal min .. you will need to tune these
int smax = 600;    // normalize input signal max . you will need to tune these
int bsize = 3;    // averaging buffer size.. higher is smoother

int width = 8;  // array width
int height = 16; // array height

unsigned int sample;

volatile bool  onLine = true;  // is phonton conne ted to wifi ?

volatile int aCount = 0;

const int aWindow = 20;
int aBuffer[aWindow];

//**********************************************************************************
//**********************************************************************************

// ============================================================
//  this sets the photon up to buy in to Wi-Fi if it's available to run even if it's not.
// I find that very helpful for updating
// ============================================================

#ifdef PHOTON
SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);
#endif

static const unsigned char countValue = 6;
int in = 0;

int inputVal = 0;  // variables used for averaging of input
int mouthAvg = 0;
int inputTotal = 0;
int inputArray[16];
int i = 0;
int countMouth = 0;

#ifdef PHOTON
IntervalTimer myTimer;    // 3 for the Core
#endif
boolean talking = false;

int lowV = 9999;
int highV = 0;

// Number of RGB LEDs in strand:
int nLEDs = 112;
long randNumber;

int tempR = 0;
int tempG = 0;
int tempB = 0;

byte R_mouthImg[][16] = {                 // Mouth animation frames
  // 0 lipmouth0.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth1.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00001000,
    0B00010000,
    0B00010000,
    0B00001000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00010000,
    0B00011000,
    0B00011000,
    0B00010000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth2.bmp
  {
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth3.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B01111100,
    0B11101110,
    0B11101110,
    0B01111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth4.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00001000,
    0B00111100,
    0B01110110,
    0B11100110,
    0B11100110,
    0B01110110,
    0B00111100,
    0B00001000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth5.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00001000,
    0B00110100,
    0B01100110,
    0B11000110,
    0B11000110,
    0B01100110,
    0B00110100,
    0B00001000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth6.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00001000,
    0B00110110,
    0B01100111,
    0B11000011,
    0B11000011,
    0B01100111,
    0B00110110,
    0B00001000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth7.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00111100,
    0B01100110,
    0B11000011,
    0B11000011,
    0B11000011,
    0B11000011,
    0B11000011,
    0B11000011,
    0B01100110,
    0B00111100,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth8.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00111100,
    0B01111110,
    0B11111111,
    0B11111111,
    0B11100111,
    0B11000011,
    0B11100111,
    0B11111111,
    0B11111111,
    0B01111110,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth9.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00111100,
    0B01111110,
    0B11111111,
    0B11111111,
    0B11100111,
    0B10000001,
    0B11000011,
    0B11100111,
    0B11111111,
    0B11111111,
    0B01111110,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth10.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00111100,
    0B01111110,
    0B11111111,
    0B11111111,
    0B11100111,
    0B10000001,
    0B11000011,
    0B11100111,
    0B11111111,
    0B11111111,
    0B01111110,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth11.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00111100,
    0B01111110,
    0B11111111,
    0B11111111,
    0B11100111,
    0B11000011,
    0B10000001,
    0B11000011,
    0B11100111,
    0B11111111,
    0B11111111,
    0B01111110,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth12.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00111110,
    0B11111111,
    0B11111111,
    0B11100111,
    0B11000011,
    0B10000001,
    0B10000001,
    0B11000011,
    0B11100111,
    0B11111111,
    0B11111111,
    0B01111110,
    0B00000000,
    0B00000000
  }
  ,
  // 0 lipmouth13.bmp
  {
    0B00000000,
    0B00111100,
    0B11111111,
    0B11111111,
    0B11111111,
    0B11100111,
    0B11000011,
    0B10000001,
    0B10000001,
    0B10000001,
    0B11000011,
    0B11100111,
    0B11111111,
    0B11111111,
    0B01111110,
    0B00000000
  }
  ,
  // 0 lipmouth14.bmp
  {
    0B00000000,
    0B01111110 ,
    0B11111111,
    0B11111111,
    0B11111111,
    0B11000011,
    0B10000001,
    0B10000001,
    0B10000001,
    0B10000001,
    0B10000001,
    0B11000011,
    0B11111111,
    0B11111111,
    0B01111110,
    0B00000000
  }
  ,
  // 0 lipmouth15.bmp

};

byte G_mouthImg[][16] = {
  // 1 lipmouth0.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth1.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth2.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth3.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth4.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth5.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B01111110,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth6.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B01111110,
    0B01111110,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth7.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00111100,
    0B01111110,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth8.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth9.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B00000000,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth10.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B00000000,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth11.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B01000010,
    0B00000000,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth12.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B01000010,
    0B00000000,
    0B00000000,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth13.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B01000010,
    0B00000000,
    0B00000000,
    0B00000000,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth14.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00111100,
    0B01111110,
    0B01000010,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B01000010,
    0B01111110,
    0B00111100,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 1 lipmouth15.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00111100,
    0B01111110,
    0B01000010,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B01000010,
    0B01111110,
    0B00111100,
    0B00000000,
    0B00000000,
    0B00000000
  }
};

byte B_mouthImg[][16] = {
  // 2 lipmouth0.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth1.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth2.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth3.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth4.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth5.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B01111110,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth6.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B01111110,
    0B01111110,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth7.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B01111110,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth8.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth9.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B00000000,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth10.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B00000000,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth11.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B01000010,
    0B00000000,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth12.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B01000010,
    0B00000000,
    0B00000000,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth13.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00011000,
    0B00111100,
    0B01100110,
    0B01000010,
    0B00000000,
    0B00000000,
    0B00000000,
    0B01000010,
    0B01100110,
    0B00111100,
    0B00011000,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth14.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00111100,
    0B01111110,
    0B01000010,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B01000010,
    0B01111110,
    0B00111100,
    0B00000000,
    0B00000000,
    0B00000000
  }
  ,
  // 2 lipmouth15.bmp
  {
    0B00000000 ,
    0B00000000,
    0B00111100,
    0B01111110,
    0B01000010,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B00000000,
    0B01000010,
    0B01111110,
    0B00111100,
    0B00000000,
    0B00000000,
    0B00000000
  }
};


// init strips, assumes DI is pin 11, CI is pin 13
#ifdef PHOTON
#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812B
#else
#define PIXEL_PIN 2
#endif 




// ============================================================
// Some basic neopixel managment routines
// ============================================================
#ifdef PHOTON
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 10, PIXEL_PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            PIXEL_TYPE);
#else
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 10, PIXEL_PIN,
                            NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
                            NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_RGB + NEO_KHZ800);
#endif                          
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};


void setXYPixelColor(uint16_t x, uint16_t y, uint32_t c) {  // maps the leds into an array
  //3,7 =, 49
  //4,7 == 64
  if (x & 1) // Odd line, wired top-to-bottom
    matrix.setPixelColor(height * x + (y + 1) - 1, c);
  else {// Even line, wired bottom-to-top
    matrix.setPixelColor(height * x + (height - y) - 1, c);
  }
}


void stripOff()  // clear  all leds in strip
{
  for (int j = 0 ; j < width; j++) {
    for (int i = 0 ; i < height; i++) {
      setXYPixelColor(j, i, matrix.Color(0, 0, 0));
    }
  }
}

void seeMouth(int j) {  // drive mouth frames to lights
  matrix.clear();
  for (int i = height ; i > 0; i--) {

    for (int k = 0; k < 8; k++) {
      int r = 0;
      int g = 0;
      int b = 0;
      if (R_mouthImg[j][i] & (1 << k)) {
        //  this is a mouthy pink… I tried many other colors. Your mileage will vary..
        // just hardcode the RGB levels here if you want to single color
        r = 255;
        g = 156;
        b = 156;
      }
      if ((i > 2) && (i < 13)) { //  had to experiment a little to center the mouth
        matrix.drawPixel(i - 4, k, matrix.Color(r, g, b));
      }
    }
  }
  matrix.show();
}

// this is an overly complicated floating point scale…
// I found it very helpful to try to get more lifelike movement of the lips.
// Otherwise they tend to just stay all the way open we're all the way shut.


float fscale( float originalMin, float originalMax, float newBegin, float
              newEnd, float inputValue, float curve) {

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;

  // condition curve parameter
  // limit range

  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }
  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0) {
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
  }
  else     // invert the ranges
  {
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }
  return rangedValue;
}


int avgMax(int tMax) {
  // const int aWindow = 5;
  // int aBuffer[aWindow];
  int avg = 0;
  aCount++;
  if (aCount == aWindow) aCount = 0;
  aBuffer[aCount] = tMax;
  for (int q = 0; q < aWindow; q++) {
    avg += aBuffer[q];
  }
  avg = max(round(avg / aWindow), 15);
  return avg;
}

void setup()
{
  digitalWrite(anaInPin , LOW);
  pinMode(anaInPin, INPUT);

  Serial.begin(115200);

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(30);
  matrix.setTextColor(matrix.Color(80, 255, 0));

  for (int i = 0; i < 16; i++) {
    seeMouth(i);
  }
}
#ifdef PHOTON
//  disconnect from Wi-Fi

void noCon(void) {
  Particle.disconnect();
  onLine = false;

}

//  attempt to connect to Wi-Fi

void doCon(void) {
  myTimer.begin(noCon, 10000, hmSec);
  Particle.connect();
  myTimer.end();
}
#endif

void loop()
{
  int x    = matrix.width();
  //int pass = 0;

  const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  int mapV = 0;

  //  check for available Wi-Fi connection… Connect if available
#ifdef PHOTON
  if ((onLine && !Particle.connected()) == true) {

    doCon();
  }
#endif

  //  initialize matrix

  matrix.fillScreen(0);
  matrix.setCursor(x, 0);

  //   sample audio from microphone into a moving average buffer

  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  //  find ranges for overall audio volume

  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  int sVolts = volts * 100;
  int aMax = avgMax(sVolts);
  //  adjust the scale of audio input to maximally use all of the lip frames
  //  this is key… Otherwise the mouth is usually always open or always closed

  int lVolts = (int) fscale( 0, aMax, 0,  15, sVolts, -2);

#ifdef DEBUG
  Serial.print(sVolts);
  Serial.print(" ");
  Serial.print(lVolts);
  Serial.print(" ");
#endif

  // map  adjusted range to maximize spread across all lip frames

  mapV = map(constrain(lVolts, 0, 15), 0, 15, 1, 7);

#ifdef DEBUG
  Serial.print(mapV);
  for (int p = 0; p < mapV; p++) Serial.print("*");
  Serial.println();
#endif

  // update the display
  seeMouth(mapV);
  delay(80);

}
