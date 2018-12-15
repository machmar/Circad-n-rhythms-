#include <Rotary.h>
#include <FastLED.h>

int brightness = 20;
Rotary encoder = Rotary(5, 2);




const int pinIO = 9;
const int pinA1 = 10;
const int pinB1 = 11;
const int pinC1 = 12;

const int pinA2 = 13;
const int pinB2 = A3;
const int pinC2 = A2;

int a0 , a1 , a2, a3, a4, a5, a6, a7;
int b0 , b1 , b2, b3, b4, b5, b6, b7;
int c0 , c1 , c2, c3, c4, c5, c6, c7;
int d0 , d1 , d2, d3, d4, d5, d6, d7;
int e0 , e1 , e2, e3, e4, e5, e6, e7;
int f0 , f1 , f2, f3, f4, f5, f6, f7;
int g0 , g1 , g2, g3, g4, g5, g6, g7;
int h0 , h1 , h2, h3, h4, h5, h6, h7;

bool a0l , a1l , a2l, a3l, a4l, a5l, a6l, a7l = 0;
bool b0l , b1l , b2l, b3l, b4l, b5l, b6l, b7l = 0;
bool c0l , c1l , c2l, c3l, c4l, c5l, c6l, c7l = 0;
bool d0l , d1l , d2l, d3l, d4l, d5l, d6l, d7l = 0;
bool e0l , e1l , e2l, e3l, e4l, e5l, e6l, e7l = 0;
bool f0l , f1l , f2l, f3l, f4l, f5l, f6l, f7l = 0;
bool g0l , g1l , g2l, g3l, g4l, g5l, g6l, g7l = 0;
bool h0l , h1l , h2l, h3l, h4l, h5l, h6l, h7l = 0;

bool a0s , a1s , a2s, a3s, a4s, a5s, a6s, a7s = 0;
bool b0s , b1s , b2s, b3s, b4s, b5s, b6s, b7s = 0;
bool c0s , c1s , c2s, c3s, c4s, c5s, c6s, c7s = 0;
bool d0s , d1s , d2s, d3s, d4s, d5s, d6s, d7s = 0;
bool e0s , e1s , e2s, e3s, e4s, e5s, e6s, e7s = 0;
bool f0s , f1s , f2s, f3s, f4s, f5s, f6s, f7s = 0;
bool g0s , g1s , g2s, g3s, g4s, g5s, g6s, g7s = 0;
bool h0s , h1s , h2s, h3s, h4s, h5s, h6s, h7s = 0;



unsigned long previousMillis0 = 0;
unsigned long previousMillis1 = 0;
int swingVar = 0;
int swing = 50;
int clockspeed;
int clock_all;
int clock_state;
int startingBpm = 120;
int bpm;

int clockBar8x8step = 0;
bool clockBar8x8stepPrevious = 1;



CRGB leds[72];
void fadeall() {
  for (int i = 0; i < 72; i++) {
    leds[i].nscale8(250);
  }
}

void setup() {
  pinMode(pinA1, OUTPUT);
  pinMode(pinB1, OUTPUT);
  pinMode(pinC1, OUTPUT);
  pinMode(pinIO, INPUT);

  pinMode(pinA2, OUTPUT);
  pinMode(pinB2, OUTPUT);
  pinMode(pinC2, OUTPUT);

  attachInterrupt (2, encoderChange, CHANGE);
  
bpm = startingBpm;
  FastLED.addLeds<NEOPIXEL, 3>(leds, 72);
  //led_test_start---------------------
  LEDS.setBrightness(20);
  static uint8_t hue = 0;
  for (int i; i < 72; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }
  for (int i = (72) - 1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }
  for (int i = 0; i < 144; i++) {
    fadeall();
    FastLED.show();
    //delay(20);
  }
  leds[64] = CRGB::Magenta;
  LEDS.setBrightness(28);
  FastLED.show();
  delay(10);
  leds[71] = CRGB::Magenta;
  LEDS.setBrightness(56);
  FastLED.show();
  delay(10);
  leds[65] = CRGB::Magenta;
  LEDS.setBrightness(84);
  FastLED.show();
  delay(10);
  leds[70] = CRGB::Magenta;
  LEDS.setBrightness(112);
  FastLED.show();
  delay(10);
  leds[69] = CRGB::Magenta;
  LEDS.setBrightness(140);
  FastLED.show();
  delay(10);
  leds[68] = CRGB::Magenta;
  LEDS.setBrightness(169);
  FastLED.show();
  delay(10);
  leds[67] = CRGB::Magenta;
  LEDS.setBrightness(197);
  FastLED.show();
  delay(10);
  leds[66] = CRGB::Magenta;
  LEDS.setBrightness(225);
  FastLED.show();
  delay(50);
  LEDS.setBrightness(brightness);
  //led_test_end----------------------
  Serial.begin(2000000);
}

void bpmTick() {
  long interval;
  unsigned long currentMillis = millis();
  clockspeed = 15000 / bpm;


  if (swingVar == 0) interval = clockspeed / 100 * swing;
  if (swingVar == 1) interval = clockspeed / 100 * (100 - swing);
  if (currentMillis - previousMillis1 >= clockspeed) {
    previousMillis1 = currentMillis;
    if (clock_all == 0) {
      clock_all = 1;
    } else {
      clock_all = 0;
      if (swingVar == 0 ) swingVar = 1;
      else swingVar = 0;
    }
  }
}

void clockBar8x8() {
  if (clock_all == 1 && clockBar8x8stepPrevious == 0) {
    clockBar8x8step++;
    clockBar8x8stepPrevious = 1;
  }
  if (clock_all == 0 && clockBar8x8stepPrevious == 1) {
    clockBar8x8stepPrevious = 0;
  }

  if (clockBar8x8step == 0) {
    leds[0] = CRGB::Red;
    leds[15] = CRGB::Red;
    leds[16] = CRGB::Red;
    leds[31] = CRGB::Red;
    leds[32] = CRGB::Red;
    leds[47] = CRGB::Red;
    leds[48] = CRGB::Red;
    leds[63] = CRGB::Red;
    FastLED.show();
  }
  if (clockBar8x8step == 1) {
    leds[1] = CRGB::Red;
    leds[14] = CRGB::Red;
    leds[17] = CRGB::Red;
    leds[30] = CRGB::Red;
    leds[33] = CRGB::Red;
    leds[46] = CRGB::Red;
    leds[49] = CRGB::Red;
    leds[62] = CRGB::Red;
    FastLED.show();
  }
  if (clockBar8x8step == 2) {
    leds[2] = CRGB::Red;
    leds[13] = CRGB::Red;
    leds[18] = CRGB::Red;
    leds[29] = CRGB::Red;
    leds[34] = CRGB::Red;
    leds[45] = CRGB::Red;
    leds[50] = CRGB::Red;
    leds[61] = CRGB::Red;
    FastLED.show();
  }
  if (clockBar8x8step == 3) {
    leds[3] = CRGB::Red;
    leds[12] = CRGB::Red;
    leds[19] = CRGB::Red;
    leds[28] = CRGB::Red;
    leds[35] = CRGB::Red;
    leds[44] = CRGB::Red;
    leds[51] = CRGB::Red;
    leds[60] = CRGB::Red;
    FastLED.show();
  }
  if (clockBar8x8step == 4) {
    leds[4] = CRGB::Red;
    leds[11] = CRGB::Red;
    leds[20] = CRGB::Red;
    leds[27] = CRGB::Red;
    leds[36] = CRGB::Red;
    leds[43] = CRGB::Red;
    leds[52] = CRGB::Red;
    leds[59] = CRGB::Red;
    FastLED.show();
  }
  if (clockBar8x8step == 5) {
    leds[5] = CRGB::Red;
    leds[10] = CRGB::Red;
    leds[21] = CRGB::Red;
    leds[26] = CRGB::Red;
    leds[37] = CRGB::Red;
    leds[42] = CRGB::Red;
    leds[53] = CRGB::Red;
    leds[58] = CRGB::Red;
    FastLED.show();
  }
  if (clockBar8x8step == 6) {
    leds[6] = CRGB::Red;
    leds[9] = CRGB::Red;
    leds[22] = CRGB::Red;
    leds[25] = CRGB::Red;
    leds[38] = CRGB::Red;
    leds[41] = CRGB::Red;
    leds[54] = CRGB::Red;
    leds[57] = CRGB::Red;
    FastLED.show();
  }
  if (clockBar8x8step == 7) {
    leds[7] = CRGB::Red;
    leds[8] = CRGB::Red;
    leds[23] = CRGB::Red;
    leds[24] = CRGB::Red;
    leds[39] = CRGB::Red;
    leds[40] = CRGB::Red;
    leds[55] = CRGB::Red;
    leds[56] = CRGB::Red;
    FastLED.show();
  }

  if (clockBar8x8step == 8) clockBar8x8step = 0;

}

void encoderChange() {
  unsigned char result = encoder.process();
  if (result == DIR_CW) {
    bpm++;
    Serial.print(bpm);
    Serial.print(",");
    Serial.println(clockspeed);
  } else if (result == DIR_CCW) {
    bpm--;
    Serial.print(bpm);
    Serial.print(",");
    Serial.println(clockspeed);
  }
    
}

void buttonsToggle() {
  if (a0 == 0 && a0l == 0) {
    a0s = !a0s;
    a0l = 1;
  }
  if (a0 == 1 && a0l == 1) {
    a0l = 0;
  }

  if (a1 == 0 && a1l == 0) {
    a1s = !a1s;
    a1l = 1;
  }
  if (a1 == 1 && a1l == 1) {
    a1l = 0;
  }
  if (a2 == 0 && a2l == 0) {
    a2s = !a2s;
    a2l = 1;
  }
  if (a2 == 1 && a2l == 1) {
    a2l = 0;
  }
  if (a3 == 0 && a3l == 0) {
    a3s = !a3s;
    a3l = 1;
  }
  if (a3 == 1 && a3l == 1) {
    a3l = 0;
  }
  if (a4 == 0 && a4l == 0) {
    a4s = !a4s;
    a4l = 1;
  }
  if (a4 == 1 && a4l == 1) {
    a4l = 0;
  }
  if (a5 == 0 && a5l == 0) {
    a5s = !a5s;
    a5l = 1;
  }
  if (a5 == 1 && a5l == 1) {
    a5l = 0;
  }
  if (a6 == 0 && a6l == 0) {
    a6s = !a6s;
    a6l = 1;
  }
  if (a6 == 1 && a6l == 1) {
    a6l = 0;
  }
  if (a7 == 0 && a7l == 0) {
    a7s = !a7s;
    a7l = 1;
  }
  if (a7 == 1 && a7l == 1) {
    a7l = 0;
  }


  if (b0 == 0 && b0l == 0) {
    b0s = !b0s;
    b0l = 1;
  }
  if (b0 == 1 && b0l == 1) {
    b0l = 0;
  }

  if (b1 == 0 && b1l == 0) {
    b1s = !b1s;
    b1l = 1;
  }
  if (b1 == 1 && b1l == 1) {
    b1l = 0;
  }
  if (b2 == 0 && b2l == 0) {
    b2s = !b2s;
    b2l = 1;
  }
  if (b2 == 1 && b2l == 1) {
    b2l = 0;
  }
  if (b3 == 0 && b3l == 0) {
    b3s = !b3s;
    b3l = 1;
  }
  if (b3 == 1 && b3l == 1) {
    b3l = 0;
  }
  if (b4 == 0 && b4l == 0) {
    b4s = !b4s;
    b4l = 1;
  }
  if (b4 == 1 && b4l == 1) {
    b4l = 0;
  }
  if (b5 == 0 && b5l == 0) {
    b5s = !b5s;
    b5l = 1;
  }
  if (b5 == 1 && b5l == 1) {
    b5l = 0;
  }
  if (b6 == 0 && b6l == 0) {
    b6s = !b6s;
    b6l = 1;
  }
  if (b6 == 1 && b6l == 1) {
    b6l = 0;
  }
  if (b7 == 0 && b7l == 0) {
    b7s = !b7s;
    b7l = 1;
  }
  if (b7 == 1 && b7l == 1) {
    b7l = 0;
  }


  if (c0 == 0 && c0l == 0) {
    c0s = !c0s;
    c0l = 1;
  }
  if (c0 == 1 && c0l == 1) {
    c0l = 0;
  }

  if (c1 == 0 && c1l == 0) {
    c1s = !c1s;
    c1l = 1;
  }
  if (c1 == 1 && c1l == 1) {
    c1l = 0;
  }
  if (c2 == 0 && c2l == 0) {
    c2s = !c2s;
    c2l = 1;
  }
  if (c2 == 1 && c2l == 1) {
    c2l = 0;
  }
  if (c3 == 0 && c3l == 0) {
    c3s = !c3s;
    c3l = 1;
  }
  if (c3 == 1 && c3l == 1) {
    c3l = 0;
  }
  if (c4 == 0 && c4l == 0) {
    c4s = !c4s;
    c4l = 1;
  }
  if (c4 == 1 && c4l == 1) {
    c4l = 0;
  }
  if (c5 == 0 && c5l == 0) {
    c5s = !c5s;
    c5l = 1;
  }
  if (c5 == 1 && c5l == 1) {
    c5l = 0;
  }
  if (c6 == 0 && c6l == 0) {
    c6s = !c6s;
    c6l = 1;
  }
  if (c6 == 1 && c6l == 1) {
    c6l = 0;
  }
  if (c7 == 0 && c7l == 0) {
    c7s = !c7s;
    c7l = 1;
  }
  if (c7 == 1 && c7l == 1) {
    c7l = 0;
  }


  if (d0 == 0 && d0l == 0) {
    d0s = !d0s;
    d0l = 1;
  }
  if (d0 == 1 && d0l == 1) {
    d0l = 0;
  }

  if (d1 == 0 && d1l == 0) {
    d1s = !d1s;
    d1l = 1;
  }
  if (d1 == 1 && d1l == 1) {
    d1l = 0;
  }
  if (d2 == 0 && d2l == 0) {
    d2s = !d2s;
    d2l = 1;
  }
  if (d2 == 1 && d2l == 1) {
    d2l = 0;
  }
  if (d3 == 0 && d3l == 0) {
    d3s = !d3s;
    d3l = 1;
  }
  if (d3 == 1 && d3l == 1) {
    d3l = 0;
  }
  if (d4 == 0 && d4l == 0) {
    d4s = !d4s;
    d4l = 1;
  }
  if (d4 == 1 && d4l == 1) {
    d4l = 0;
  }
  if (d5 == 0 && d5l == 0) {
    d5s = !d5s;
    d5l = 1;
  }
  if (d5 == 1 && d5l == 1) {
    d5l = 0;
  }
  if (d6 == 0 && d6l == 0) {
    d6s = !d6s;
    d6l = 1;
  }
  if (d6 == 1 && d6l == 1) {
    d6l = 0;
  }
  if (d7 == 0 && d7l == 0) {
    d7s = !d7s;
    d7l = 1;
  }
  if (d7 == 1 && d7l == 1) {
    d7l = 0;
  }


  if (e0 == 0 && e0l == 0) {
    e0s = !e0s;
    e0l = 1;
  }
  if (e0 == 1 && e0l == 1) {
    e0l = 0;
  }

  if (e1 == 0 && e1l == 0) {
    e1s = !e1s;
    e1l = 1;
  }
  if (e1 == 1 && e1l == 1) {
    e1l = 0;
  }
  if (e2 == 0 && e2l == 0) {
    e2s = !e2s;
    e2l = 1;
  }
  if (e2 == 1 && e2l == 1) {
    e2l = 0;
  }
  if (e3 == 0 && e3l == 0) {
    e3s = !e3s;
    e3l = 1;
  }
  if (e3 == 1 && e3l == 1) {
    e3l = 0;
  }
  if (e4 == 0 && e4l == 0) {
    e4s = !e4s;
    e4l = 1;
  }
  if (e4 == 1 && e4l == 1) {
    e4l = 0;
  }
  if (e5 == 0 && e5l == 0) {
    e5s = !e5s;
    e5l = 1;
  }
  if (e5 == 1 && e5l == 1) {
    e5l = 0;
  }
  if (e6 == 0 && e6l == 0) {
    e6s = !e6s;
    e6l = 1;
  }
  if (e6 == 1 && e6l == 1) {
    e6l = 0;
  }
  if (e7 == 0 && e7l == 0) {
    e7s = !e7s;
    e7l = 1;
  }
  if (e7 == 1 && e7l == 1) {
    e7l = 0;
  }


  if (f0 == 0 && f0l == 0) {
    f0s = !f0s;
    f0l = 1;
  }
  if (f0 == 1 && f0l == 1) {
    f0l = 0;
  }

  if (f1 == 0 && f1l == 0) {
    f1s = !f1s;
    f1l = 1;
  }
  if (f1 == 1 && f1l == 1) {
    f1l = 0;
  }
  if (f2 == 0 && f2l == 0) {
    f2s = !f2s;
    f2l = 1;
  }
  if (f2 == 1 && f2l == 1) {
    f2l = 0;
  }
  if (f3 == 0 && f3l == 0) {
    f3s = !f3s;
    f3l = 1;
  }
  if (f3 == 1 && f3l == 1) {
    f3l = 0;
  }
  if (f4 == 0 && f4l == 0) {
    f4s = !f4s;
    f4l = 1;
  }
  if (f4 == 1 && f4l == 1) {
    f4l = 0;
  }
  if (f5 == 0 && f5l == 0) {
    f5s = !f5s;
    f5l = 1;
  }
  if (f5 == 1 && f5l == 1) {
    f5l = 0;
  }
  if (f6 == 0 && f6l == 0) {
    f6s = !f6s;
    f6l = 1;
  }
  if (f6 == 1 && f6l == 1) {
    f6l = 0;
  }
  if (f7 == 0 && f7l == 0) {
    f7s = !f7s;
    f7l = 1;
  }
  if (f7 == 1 && f7l == 1) {
    f7l = 0;
  }


  if (g0 == 0 && g0l == 0) {
    g0s = !g0s;
    g0l = 1;
  }
  if (g0 == 1 && g0l == 1) {
    g0l = 0;
  }

  if (g1 == 0 && g1l == 0) {
    g1s = !g1s;
    g1l = 1;
  }
  if (g1 == 1 && g1l == 1) {
    g1l = 0;
  }
  if (g2 == 0 && g2l == 0) {
    g2s = !g2s;
    g2l = 1;
  }
  if (g2 == 1 && g2l == 1) {
    g2l = 0;
  }
  if (g3 == 0 && g3l == 0) {
    g3s = !g3s;
    g3l = 1;
  }
  if (g3 == 1 && g3l == 1) {
    g3l = 0;
  }
  if (g4 == 0 && g4l == 0) {
    g4s = !g4s;
    g4l = 1;
  }
  if (g4 == 1 && g4l == 1) {
    g4l = 0;
  }
  if (g5 == 0 && g5l == 0) {
    g5s = !g5s;
    g5l = 1;
  }
  if (g5 == 1 && g5l == 1) {
    g5l = 0;
  }
  if (g6 == 0 && g6l == 0) {
    g6s = !g6s;
    g6l = 1;
  }
  if (g6 == 1 && g6l == 1) {
    g6l = 0;
  }
  if (g7 == 0 && g7l == 0) {
    g7s = !g7s;
    g7l = 1;
  }
  if (g7 == 1 && g7l == 1) {
    g7l = 0;
  }


  if (h0 == 0 && h0l == 0) {
    h0s = !h0s;
    h0l = 1;
  }
  if (h0 == 1 && h0l == 1) {
    h0l = 0;
  }

  if (h1 == 0 && h1l == 0) {
    h1s = !h1s;
    h1l = 1;
  }
  if (h1 == 1 && h1l == 1) {
    h1l = 0;
  }
  if (h2 == 0 && h2l == 0) {
    h2s = !h2s;
    h2l = 1;
  }
  if (h2 == 1 && h2l == 1) {
    h2l = 0;
  }
  if (h3 == 0 && h3l == 0) {
    h3s = !h3s;
    h3l = 1;
  }
  if (h3 == 1 && h3l == 1) {
    h3l = 0;
  }
  if (h4 == 0 && h4l == 0) {
    h4s = !h4s;
    h4l = 1;
  }
  if (h4 == 1 && h4l == 1) {
    h4l = 0;
  }
  if (h5 == 0 && h5l == 0) {
    h5s = !h5s;
    h5l = 1;
  }
  if (h5 == 1 && h5l == 1) {
    h5l = 0;
  }
  if (h6 == 0 && h6l == 0) {
    h6s = !h6s;
    h6l = 1;
  }
  if (h6 == 1 && h6l == 1) {
    h6l = 0;
  }
  if (h7 == 0 && h7l == 0) {
    h7s = !h7s;
    h7l = 1;
  }
  if (h7 == 1 && h7l == 1) {
    h7l = 0;
  }

}

void buttonsLedResponse() {
  if (a0s == 1) {
    leds[0] = CRGB::Yellow;
  }
  if (a0s == 0) {
    leds[0] = CRGB::Black;
  }

  if (a1s == 1) {
    leds[15] = CRGB::Yellow;
  }
  if (a1s == 0) {
    leds[15] = CRGB::Black;
  }

  if (a2s == 1) {
    leds[16] = CRGB::Yellow;
  }
  if (a2s == 0) {
    leds[16] = CRGB::Black;
  }

  if (a3s == 1) {
    leds[31] = CRGB::Yellow;
  }
  if (a3s == 0) {
    leds[31] = CRGB::Black;
  }

  if (a4s == 1) {
    leds[32] = CRGB::Yellow;
  }
  if (a4s == 0) {
    leds[32] = CRGB::Black;
  }

  if (a5s == 1) {
    leds[47] = CRGB::Yellow;
  }
  if (a5s == 0) {
    leds[47] = CRGB::Black;
  }
  if (a6s == 1) {
    leds[48] = CRGB::Yellow;
  }
  if (a6s == 0) {
    leds[48] = CRGB::Black;
  }
  if (a7s == 1) {
    leds[63] = CRGB::Yellow;
  }
  if (a7s == 0) {
    leds[63] = CRGB::Black;
  }
  if (b0s == 1) {
    leds[1] = CRGB::Yellow;
  }
  if (b0s == 0) {
    leds[1] = CRGB::Black;
  }
  if (b1s == 1) {
    leds[14] = CRGB::Yellow;
  }
  if (b1s == 0) {
    leds[14] = CRGB::Black;
  }
  if (b2s == 1) {
    leds[17] = CRGB::Yellow;
  }
  if (b2s == 0) {
    leds[17] = CRGB::Black;
  }
  if (b3s == 1) {
    leds[30] = CRGB::Yellow;
  }
  if (b3s == 0) {
    leds[30] = CRGB::Black;
  }
  if (b4s == 1) {
    leds[33] = CRGB::Yellow;
  }
  if (b4s == 0) {
    leds[33] = CRGB::Black;
  }
  if (b5s == 1) {
    leds[46] = CRGB::Yellow;
  }
  if (b5s == 0) {
    leds[46] = CRGB::Black;
  }
  if (b6s == 1) {
    leds[49] = CRGB::Yellow;
  }
  if (b6s == 0) {
    leds[49] = CRGB::Black;
  }
  if (b7s == 1) {
    leds[62] = CRGB::Yellow;
  }
  if (b7s == 0) {
    leds[62] = CRGB::Black;
  }
  if (c0s == 1) {
    leds[2] = CRGB::Yellow;
  }
  if (c0s == 0) {
    leds[2] = CRGB::Black;
  }
  if (c1s == 1) {
    leds[13] = CRGB::Yellow;
  }
  if (c1s == 0) {
    leds[13] = CRGB::Black;
  }
  if (c2s == 1) {
    leds[18] = CRGB::Yellow;
  }
  if (c2s == 0) {
    leds[18] = CRGB::Black;
  }
  if (c3s == 1) {
    leds[29] = CRGB::Yellow;
  }
  if (c3s == 0) {
    leds[29] = CRGB::Black;
  }
  if (c4s == 1) {
    leds[34] = CRGB::Yellow;
  }
  if (c4s == 0) {
    leds[34] = CRGB::Black;
  }
  if (c5s == 1) {
    leds[45] = CRGB::Yellow;
  }
  if (c5s == 0) {
    leds[45] = CRGB::Black;
  }
  if (c6s == 1) {
    leds[50] = CRGB::Yellow;
  }
  if (c6s == 0) {
    leds[50] = CRGB::Black;
  }
  if (c7s == 1) {
    leds[61] = CRGB::Yellow;
  }
  if (c7s == 0) {
    leds[61] = CRGB::Black;
  }
  if (d0s == 1) {
    leds[3] = CRGB::Yellow;
  }
  if (d0s == 0) {
    leds[3] = CRGB::Black;
  }
  if (d1s == 1) {
    leds[12] = CRGB::Yellow;
  }
  if (d1s == 0) {
    leds[12] = CRGB::Black;
  }
  if (d2s == 1) {
    leds[19] = CRGB::Yellow;
  }
  if (d2s == 0) {
    leds[19] = CRGB::Black;
  }
  if (d3s == 1) {
    leds[28] = CRGB::Yellow;
  }
  if (d3s == 0) {
    leds[28] = CRGB::Black;
  }
  if (d4s == 1) {
    leds[35] = CRGB::Yellow;
  }
  if (d4s == 0) {
    leds[35] = CRGB::Black;
  }
  if (d5s == 1) {
    leds[44] = CRGB::Yellow;
  }
  if (d5s == 0) {
    leds[44] = CRGB::Black;
  }
  if (d6s == 1) {
    leds[51] = CRGB::Yellow;
  }
  if (d6s == 0) {
    leds[51] = CRGB::Black;
  }
  if (d7s == 1) {
    leds[60] = CRGB::Yellow;
  }
  if (d7s == 0) {
    leds[60] = CRGB::Black;
  }
  if (e0s == 1) {
    leds[4] = CRGB::Yellow;
  }
  if (e0s == 0) {
    leds[4] = CRGB::Black;
  }
  if (e1s == 1) {
    leds[11] = CRGB::Yellow;
  }
  if (e1s == 0) {
    leds[11] = CRGB::Black;
  }
  if (e2s == 1) {
    leds[20] = CRGB::Yellow;
  }
  if (e2s == 0) {
    leds[20] = CRGB::Black;
  }
  if (e3s == 1) {
    leds[27] = CRGB::Yellow;
  }
  if (e3s == 0) {
    leds[27] = CRGB::Black;
  }
  if (e4s == 1) {
    leds[36] = CRGB::Yellow;
  }
  if (e4s == 0) {
    leds[36] = CRGB::Black;
  }
  if (e5s == 1) {
    leds[43] = CRGB::Yellow;
  }
  if (e5s == 0) {
    leds[43] = CRGB::Black;
  }
  if (e6s == 1) {
    leds[52] = CRGB::Yellow;
  }
  if (e6s == 0) {
    leds[52] = CRGB::Black;
  }
  if (e7s == 1) {
    leds[59] = CRGB::Yellow;
  }
  if (e7s == 0) {
    leds[59] = CRGB::Black;
  }
  if (f0s == 1) {
    leds[5] = CRGB::Yellow;
  }
  if (f0s == 0) {
    leds[5] = CRGB::Black;
  }
  if (f1s == 1) {
    leds[10] = CRGB::Yellow;
  }
  if (f1s == 0) {
    leds[10] = CRGB::Black;
  }
  if (f2s == 1) {
    leds[21] = CRGB::Yellow;
  }
  if (f2s == 0) {
    leds[21] = CRGB::Black;
  }
  if (f3s == 1) {
    leds[26] = CRGB::Yellow;
  }
  if (f3s == 0) {
    leds[26] = CRGB::Black;
  }
  if (f4s == 1) {
    leds[37] = CRGB::Yellow;
  }
  if (f4s == 0) {
    leds[37] = CRGB::Black;
  }
  if (f5s == 1) {
    leds[42] = CRGB::Yellow;
  }
  if (f5s == 0) {
    leds[42] = CRGB::Black;
  }
  if (f6s == 1) {
    leds[53] = CRGB::Yellow;
  }
  if (f6s == 0) {
    leds[53] = CRGB::Black;
  }
  if (f7s == 1) {
    leds[58] = CRGB::Yellow;
  }
  if (f7s == 0) {
    leds[58] = CRGB::Black;
  }

  if (g0s == 1) {
    leds[6] = CRGB::Yellow;
  }
  if (g0s == 0) {
    leds[6] = CRGB::Black;
  }

  if (g1s == 1) {
    leds[9] = CRGB::Yellow;
  }
  if (g1s == 0) {
    leds[9] = CRGB::Black;
  }

  if (g2s == 1) {
    leds[22] = CRGB::Yellow;
  }
  if (g2s == 0) {
    leds[22] = CRGB::Black;
  }

  if (g3s == 1) {
    leds[25] = CRGB::Yellow;
  }
  if (g3s == 0) {
    leds[25] = CRGB::Black;
  }

  if (g4s == 1) {
    leds[38] = CRGB::Yellow;
  }
  if (g4s == 0) {
    leds[38] = CRGB::Black;
  }

  if (g5s == 1) {
    leds[41] = CRGB::Yellow;
  }
  if (g5s == 0) {
    leds[41] = CRGB::Black;
  }

  if (g6s == 1) {
    leds[54] = CRGB::Yellow;
  }
  if (g6s == 0) {
    leds[54] = CRGB::Black;
  }

  if (g7s == 1) {
    leds[57] = CRGB::Yellow;
  }
  if (g7s == 0) {
    leds[57] = CRGB::Black;
  }

  if (h0s == 1) {
    leds[7] = CRGB::Yellow;
  }
  if (h0s == 0) {
    leds[7] = CRGB::Black;
  }

  if (h1s == 1) {
    leds[8] = CRGB::Yellow;
  }
  if (h1s == 0) {
    leds[8] = CRGB::Black;
  }

  if (h2s == 1) {
    leds[23] = CRGB::Yellow;
  }
  if (h2s == 0) {
    leds[23] = CRGB::Black;
  }

  if (h3s == 1) {
    leds[24] = CRGB::Yellow;
  }
  if (h3s == 0) {
    leds[24] = CRGB::Black;
  }

  if (h4s == 1) {
    leds[39] = CRGB::Yellow;
  }
  if (h4s == 0) {
    leds[39] = CRGB::Black;
  }

  if (h5s == 1) {
    leds[40] = CRGB::Yellow;
  }
  if (h5s == 0) {
    leds[40] = CRGB::Black;
  }

  if (h6s == 1) {
    leds[55] = CRGB::Yellow;
  }
  if (h6s == 0) {
    leds[55] = CRGB::Black;
  }

  if (h7s == 1) {
    leds[56] = CRGB::Yellow;
  }
  if (h7s == 0) {
    leds[56] = CRGB::Black;
  }
  FastLED.show();
}

void checkState() {

  //set Multiplex to binary 0
  digitalWrite(pinA2, 0);
  digitalWrite(pinB2, 0);
  digitalWrite(pinC2, 0);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  a0 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  a1 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  a2 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  a3 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  a4 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  a5 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  a6 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  a7 = ! digitalRead(pinIO);

  //set Multiplex to binary 1
  digitalWrite(pinA2, 1);
  digitalWrite(pinB2, 0);
  digitalWrite(pinC2, 0);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  b0 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  b1 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  b2 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  b3 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  b4 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  b5 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  b6 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  b7 = ! digitalRead(pinIO);

  //set Multiplex to binary 2
  digitalWrite(pinA2, 0);
  digitalWrite(pinB2, 1);
  digitalWrite(pinC2, 0);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  c0 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  c1 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  c2 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  c3 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  c4 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  c5 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  c6 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  c7 = ! digitalRead(pinIO);

  //set Multiplex to binary 3
  digitalWrite(pinA2, 1);
  digitalWrite(pinB2, 1);
  digitalWrite(pinC2, 0);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  d0 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  d1 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  d2 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  d3 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  d4 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  d5 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  d6 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  d7 = ! digitalRead(pinIO);

  //set Multiplex to binary 4
  digitalWrite(pinA2, 0);
  digitalWrite(pinB2, 0);
  digitalWrite(pinC2, 1);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  e0 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  e1 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  e2 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  e3 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  e4 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  e5 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  e6 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  e7 = ! digitalRead(pinIO);

  //set Multiplex to binary 5
  digitalWrite(pinA2, 1);
  digitalWrite(pinB2, 0);
  digitalWrite(pinC2, 1);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  f0 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  f1 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  f2 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  f3 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  f4 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  f5 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  f6 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  f7 = ! digitalRead(pinIO);

  //set Multiplex to binary 6
  digitalWrite(pinA2, 0);
  digitalWrite(pinB2, 1);
  digitalWrite(pinC2, 1);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  g0 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  g1 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  g2 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  g3 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  g4 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  g5 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  g6 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  g7 = ! digitalRead(pinIO);

  //set Multiplex to binary 7
  digitalWrite(pinA2, 1);
  digitalWrite(pinB2, 1);
  digitalWrite(pinC2, 1);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  h0 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  h1 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  h2 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  h3 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  h4 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  h5 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  h6 = ! digitalRead(pinIO);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  h7 = ! digitalRead(pinIO);
}

void serialPrintAll() {
  Serial.print(a0);
  Serial.print(a1);
  Serial.print(a2);
  Serial.print(a3);
  Serial.print(a4);
  Serial.print(a5);
  Serial.print(a6);
  Serial.println(a7);

  Serial.print(b0);
  Serial.print(b1);
  Serial.print(b2);
  Serial.print(b3);
  Serial.print(b4);
  Serial.print(b5);
  Serial.print(b6);
  Serial.println(b7);

  Serial.print(c0);
  Serial.print(c1);
  Serial.print(c2);
  Serial.print(c3);
  Serial.print(c4);
  Serial.print(c5);
  Serial.print(c6);
  Serial.println(c7);

  Serial.print(d0);
  Serial.print(d1);
  Serial.print(d2);
  Serial.print(d3);
  Serial.print(d4);
  Serial.print(d5);
  Serial.print(d6);
  Serial.println(d7);

  Serial.print(e0);
  Serial.print(e1);
  Serial.print(e2);
  Serial.print(e3);
  Serial.print(e4);
  Serial.print(e5);
  Serial.print(e6);
  Serial.println(e7);

  Serial.print(f0);
  Serial.print(f1);
  Serial.print(f2);
  Serial.print(f3);
  Serial.print(f4);
  Serial.print(f5);
  Serial.print(f6);
  Serial.println(f7);

  Serial.print(g0);
  Serial.print(g1);
  Serial.print(g2);
  Serial.print(g3);
  Serial.print(g4);
  Serial.print(g5);
  Serial.print(g6);
  Serial.println(g7);

  Serial.print(h0);
  Serial.print(h1);
  Serial.print(h2);
  Serial.print(h3);
  Serial.print(h4);
  Serial.print(h5);
  Serial.print(h6);
  Serial.println(h7);
  Serial.println();
  Serial.println();
}

void loop() {
  encoderChange();
  checkState();
  encoderChange();
  buttonsToggle();
  encoderChange();
  buttonsLedResponse();
  encoderChange();
  bpmTick();
  encoderChange();
  clockBar8x8();
  encoderChange();
  //serialPrintAll();
}
