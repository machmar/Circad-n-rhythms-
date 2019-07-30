#include <Rotary.h> //https://github.com/brianlow/Rotary
#include <FastLED.h>

int brightness = 20;
Rotary encoder = Rotary(5, 2);


const int gatesOut = 6;

const int buttons8x8 = 9;
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

bool a0t , a1t , a2t , a3t , a4t , a5t , a6t , a7t , a8t , a9t , a10t , a11t , a12t , a13t , a14t , a15t , a16t , a17t , a18t , a19t , a20t , a21t , a22t , a23t , a24t , a25t , a26t , a27t , a28t , a29t , a30t , a31t , a32t , a33t , a34t , a35t , a36t , a37t , a38t , a39t , a40t , a41t , a42t , a43t , a44t , a45t , a46t , a47t , a48t , a49t , a50t , a51t , a52t , a53t , a54t , a55t , a56t , a57t , a58t , a59t , a60t , a61t , a62t , a63t = 0;
bool b0t , b1t , b2t , b3t , b4t , b5t , b6t , b7t , b8t , b9t , b10t , b11t , b12t , b13t , b14t , b15t , b16t , b17t , b18t , b19t , b20t , b21t , b22t , b23t , b24t , b25t , b26t , b27t , b28t , b29t , b30t , b31t , b32t , b33t , b34t , b35t , b36t , b37t , b38t , b39t , b40t , b41t , b42t , b43t , b44t , b45t , b46t , b47t , b48t , b49t , b50t , b51t , b52t , b53t , b54t , b55t , b56t , b57t , b58t , b59t , b60t , b61t , b62t , b63t = 0;
bool c0t , c1t , c2t , c3t , c4t , c5t , c6t , c7t , c8t , c9t , c10t , c11t , c12t , c13t , c14t , c15t , c16t , c17t , c18t , c19t , c20t , c21t , c22t , c23t , c24t , c25t , c26t , c27t , c28t , c29t , c30t , c31t , c32t , c33t , c34t , c35t , c36t , c37t , c38t , c39t , c40t , c41t , c42t , c43t , c44t , c45t , c46t , c47t , c48t , c49t , c50t , c51t , c52t , c53t , c54t , c55t , c56t , c57t , c58t , c59t , c60t , c61t , c62t , c63t = 0;
bool d0t , d1t , d2t , d3t , d4t , d5t , d6t , d7t , d8t , d9t , d10t , d11t , d12t , d13t , d14t , d15t , d16t , d17t , d18t , d19t , d20t , d21t , d22t , d23t , d24t , d25t , d26t , d27t , d28t , d29t , d30t , d31t , d32t , d33t , d34t , d35t , d36t , d37t , d38t , d39t , d40t , d41t , d42t , d43t , d44t , d45t , d46t , d47t , d48t , d49t , d50t , d51t , d52t , d53t , d54t , d55t , d56t , d57t , d58t , d59t , d60t , d61t , d62t , d63t = 0;
bool e0t , e1t , e2t , e3t , e4t , e5t , e6t , e7t , e8t , e9t , e10t , e11t , e12t , e13t , e14t , e15t , e16t , e17t , e18t , e19t , e20t , e21t , e22t , e23t , e24t , e25t , e26t , e27t , e28t , e29t , e30t , e31t , e32t , e33t , e34t , e35t , e36t , e37t , e38t , e39t , e40t , e41t , e42t , e43t , e44t , e45t , e46t , e47t , e48t , e49t , e50t , e51t , e52t , e53t , e54t , e55t , e56t , e57t , e58t , e59t , e60t , e61t , e62t , e63t = 0;
bool f0t , f1t , f2t , f3t , f4t , f5t , f6t , f7t , f8t , f9t , f10t , f11t , f12t , f13t , f14t , f15t , f16t , f17t , f18t , f19t , f20t , f21t , f22t , f23t , f24t , f25t , f26t , f27t , f28t , f29t , f30t , f31t , f32t , f33t , f34t , f35t , f36t , f37t , f38t , f39t , f40t , f41t , f42t , f43t , f44t , f45t , f46t , f47t , f48t , f49t , f50t , f51t , f52t , f53t , f54t , f55t , f56t , f57t , f58t , f59t , f60t , f61t , f62t , f63t = 0;
bool g0t , g1t , g2t , g3t , g4t , g5t , g6t , g7t , g8t , g9t , g10t , g11t , g12t , g13t , g14t , g15t , g16t , g17t , g18t , g19t , g20t , g21t , g22t , g23t , g24t , g25t , g26t , g27t , g28t , g29t , g30t , g31t , g32t , g33t , g34t , g35t , g36t , g37t , g38t , g39t , g40t , g41t , g42t , g43t , g44t , g45t , g46t , g47t , g48t , g49t , g50t , g51t , g52t , g53t , g54t , g55t , g56t , g57t , g58t , g59t , g60t , g61t , g62t , g63t = 0;
bool h0t , h1t , h2t , h3t , h4t , h5t , h6t , h7t , h8t , h9t , h10t , h11t , h12t , h13t , h14t , h15t , h16t , h17t , h18t , h19t , h20t , h21t , h22t , h23t , h24t , h25t , h26t , h27t , h28t , h29t , h30t , h31t , h32t , h33t , h34t , h35t , h36t , h37t , h38t , h39t , h40t , h41t , h42t , h43t , h44t , h45t , h46t , h47t , h48t , h49t , h50t , h51t , h52t , h53t , h54t , h55t , h56t , h57t , h58t , h59t , h60t , h61t , h62t , h63t = 0;

int gate1 ,  gate2 , gate3 , gate4 , gate5 , gate6 , gate7 , gate8 = 0;





unsigned long previousMillis0 = 0;
unsigned long previousMillis1 = 0;
int swingVar = 0;
int swing = 50;
int clockspeed;
int clock_all;
int clock_state;
int startingBpm = 20;
int bpm;

int clockBar8x8step = 0;
bool clockBar8x8stepPrevious = 1;
int mainStep = 0;
int mainStepPrevious = 1;


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
  pinMode(buttons8x8, INPUT);
  pinMode(gatesOut, OUTPUT);

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

void mainStepControl() {
  if (clock_all == 1 && mainStepPrevious == 0) {
    mainStep++;
    mainStepPrevious = 1;
  }
  if (clock_all == 0 && mainStepPrevious == 1) {
    mainStepPrevious = 0;
  }
  if (mainStep ==  16) mainStep = 0;
}

void clockBar8x8() {
  if (clock_all == 1 && clockBar8x8stepPrevious == 0) {
    clockBar8x8step++;
    clockBar8x8stepPrevious = 1;
  }
  if (clock_all == 0 && clockBar8x8stepPrevious == 1) {
    clockBar8x8stepPrevious = 0;
  }

  if (mainStep >= 0 && mainStep <= 7) {
    leds[0 + clockBar8x8step] = CRGB::Blue;
    leds[15 - clockBar8x8step] = CRGB::Red;
    leds[16 + clockBar8x8step] = CRGB::Red;
    leds[31 - clockBar8x8step] = CRGB::Red;
    leds[32 + clockBar8x8step] = CRGB::Red;
    leds[47 - clockBar8x8step] = CRGB::Red;
    leds[48 + clockBar8x8step] = CRGB::Red;
    leds[63 - clockBar8x8step] = CRGB::Red;
    FastLED.show();
  }

  if (mainStep >= 8 && mainStep <= 15) {
    leds[0 + clockBar8x8step] = CRGB::Blue;
    leds[15 - clockBar8x8step] = CRGB::Blue;
    leds[16 + clockBar8x8step] = CRGB::Red;
    leds[31 - clockBar8x8step] = CRGB::Red;
    leds[32 + clockBar8x8step] = CRGB::Red;
    leds[47 - clockBar8x8step] = CRGB::Red;
    leds[48 + clockBar8x8step] = CRGB::Red;
    leds[63 - clockBar8x8step] = CRGB::Red;
    FastLED.show();
  }

  if (mainStep >= 16 && mainStep <= 23) {
    leds[0 + clockBar8x8step] = CRGB::Blue;
    leds[15 - clockBar8x8step] = CRGB::Blue;
    leds[16 + clockBar8x8step] = CRGB::Blue;
    leds[31 - clockBar8x8step] = CRGB::Red;
    leds[32 + clockBar8x8step] = CRGB::Red;
    leds[47 - clockBar8x8step] = CRGB::Red;
    leds[48 + clockBar8x8step] = CRGB::Red;
    leds[63 - clockBar8x8step] = CRGB::Red;
    FastLED.show();
  }

  if (mainStep >= 24 && mainStep <= 31) {
    leds[0 + clockBar8x8step] = CRGB::Blue;
    leds[15 - clockBar8x8step] = CRGB::Blue;
    leds[16 + clockBar8x8step] = CRGB::Blue;
    leds[31 - clockBar8x8step] = CRGB::Blue;
    leds[32 + clockBar8x8step] = CRGB::Red;
    leds[47 - clockBar8x8step] = CRGB::Red;
    leds[48 + clockBar8x8step] = CRGB::Red;
    leds[63 - clockBar8x8step] = CRGB::Red;
    FastLED.show();
  }

  if (mainStep >= 32 && mainStep <= 39) {
    leds[0 + clockBar8x8step] = CRGB::Blue;
    leds[15 - clockBar8x8step] = CRGB::Blue;
    leds[16 + clockBar8x8step] = CRGB::Blue;
    leds[31 - clockBar8x8step] = CRGB::Blue;
    leds[32 + clockBar8x8step] = CRGB::Blue;
    leds[47 - clockBar8x8step] = CRGB::Red;
    leds[48 + clockBar8x8step] = CRGB::Red;
    leds[63 - clockBar8x8step] = CRGB::Red;
    FastLED.show();
  }

  if (mainStep >= 40 && mainStep <= 47) {
    leds[0 + clockBar8x8step] = CRGB::Blue;
    leds[15 - clockBar8x8step] = CRGB::Blue;
    leds[16 + clockBar8x8step] = CRGB::Blue;
    leds[31 - clockBar8x8step] = CRGB::Blue;
    leds[32 + clockBar8x8step] = CRGB::Blue;
    leds[47 - clockBar8x8step] = CRGB::Blue;
    leds[48 + clockBar8x8step] = CRGB::Red;
    leds[63 - clockBar8x8step] = CRGB::Red;
    FastLED.show();
  }

  if (mainStep >= 48 && mainStep <= 55) {
    leds[0 + clockBar8x8step] = CRGB::Blue;
    leds[15 - clockBar8x8step] = CRGB::Blue;
    leds[16 + clockBar8x8step] = CRGB::Blue;
    leds[31 - clockBar8x8step] = CRGB::Blue;
    leds[32 + clockBar8x8step] = CRGB::Blue;
    leds[47 - clockBar8x8step] = CRGB::Blue;
    leds[48 + clockBar8x8step] = CRGB::Blue;
    leds[63 - clockBar8x8step] = CRGB::Red;
    FastLED.show();
  }

  if (mainStep >= 56 && mainStep <= 63) {
    leds[0 + clockBar8x8step] = CRGB::Blue;
    leds[15 - clockBar8x8step] = CRGB::Blue;
    leds[16 + clockBar8x8step] = CRGB::Blue;
    leds[31 - clockBar8x8step] = CRGB::Blue;
    leds[32 + clockBar8x8step] = CRGB::Blue;
    leds[47 - clockBar8x8step] = CRGB::Blue;
    leds[48 + clockBar8x8step] = CRGB::Blue;
    leds[63 - clockBar8x8step] = CRGB::Blue;
    FastLED.show();
  }




  if (clockBar8x8step == 8) clockBar8x8step = 0;

}

void encoderChange() {
  unsigned char result = encoder.process();
  if (result == DIR_CW) {
    bpm++;
  } else if (result == DIR_CCW) {
    bpm--;
  }

}

void buttonsToggle() {
  if (mainStep >= 0 && mainStep <= 7) {
    if (a0 == 0 && a0l == 0) {
      a0t = !a0t;
      a0l = 1;
    }
    if (a0 == 1 && a0l == 1) {
      a0l = 0;
    }

    if (a1 == 0 && a1l == 0) {
      a1t = !a1t;
      a1l = 1;
    }
    if (a1 == 1 && a1l == 1) {
      a1l = 0;
    }
    if (a2 == 0 && a2l == 0) {
      a2t = !a2t;
      a2l = 1;
    }
    if (a2 == 1 && a2l == 1) {
      a2l = 0;
    }
    if (a3 == 0 && a3l == 0) {
      a3t = !a3t;
      a3l = 1;
    }
    if (a3 == 1 && a3l == 1) {
      a3l = 0;
    }
    if (a4 == 0 && a4l == 0) {
      a4t = !a4t;
      a4l = 1;
    }
    if (a4 == 1 && a4l == 1) {
      a4l = 0;
    }
    if (a5 == 0 && a5l == 0) {
      a5t = !a5t;
      a5l = 1;
    }
    if (a5 == 1 && a5l == 1) {
      a5l = 0;
    }
    if (a6 == 0 && a6l == 0) {
      a6t = !a6t;
      a6l = 1;
    }
    if (a6 == 1 && a6l == 1) {
      a6l = 0;
    }
    if (a7 == 0 && a7l == 0) {
      a7t = !a7t;
      a7l = 1;
    }
    if (a7 == 1 && a7l == 1) {
      a7l = 0;
    }


    if (b0 == 0 && b0l == 0) {
      b0t = !b0t;
      b0l = 1;
    }
    if (b0 == 1 && b0l == 1) {
      b0l = 0;
    }

    if (b1 == 0 && b1l == 0) {
      b1t = !b1t;
      b1l = 1;
    }
    if (b1 == 1 && b1l == 1) {
      b1l = 0;
    }
    if (b2 == 0 && b2l == 0) {
      b2t = !b2t;
      b2l = 1;
    }
    if (b2 == 1 && b2l == 1) {
      b2l = 0;
    }
    if (b3 == 0 && b3l == 0) {
      b3t = !b3t;
      b3l = 1;
    }
    if (b3 == 1 && b3l == 1) {
      b3l = 0;
    }
    if (b4 == 0 && b4l == 0) {
      b4t = !b4t;
      b4l = 1;
    }
    if (b4 == 1 && b4l == 1) {
      b4l = 0;
    }
    if (b5 == 0 && b5l == 0) {
      b5t = !b5t;
      b5l = 1;
    }
    if (b5 == 1 && b5l == 1) {
      b5l = 0;
    }
    if (b6 == 0 && b6l == 0) {
      b6t = !b6t;
      b6l = 1;
    }
    if (b6 == 1 && b6l == 1) {
      b6l = 0;
    }
    if (b7 == 0 && b7l == 0) {
      b7t = !b7t;
      b7l = 1;
    }
    if (b7 == 1 && b7l == 1) {
      b7l = 0;
    }


    if (c0 == 0 && c0l == 0) {
      c0t = !c0t;
      c0l = 1;
    }
    if (c0 == 1 && c0l == 1) {
      c0l = 0;
    }

    if (c1 == 0 && c1l == 0) {
      c1t = !c1t;
      c1l = 1;
    }
    if (c1 == 1 && c1l == 1) {
      c1l = 0;
    }
    if (c2 == 0 && c2l == 0) {
      c2t = !c2t;
      c2l = 1;
    }
    if (c2 == 1 && c2l == 1) {
      c2l = 0;
    }
    if (c3 == 0 && c3l == 0) {
      c3t = !c3t;
      c3l = 1;
    }
    if (c3 == 1 && c3l == 1) {
      c3l = 0;
    }
    if (c4 == 0 && c4l == 0) {
      c4t = !c4t;
      c4l = 1;
    }
    if (c4 == 1 && c4l == 1) {
      c4l = 0;
    }
    if (c5 == 0 && c5l == 0) {
      c5t = !c5t;
      c5l = 1;
    }
    if (c5 == 1 && c5l == 1) {
      c5l = 0;
    }
    if (c6 == 0 && c6l == 0) {
      c6t = !c6t;
      c6l = 1;
    }
    if (c6 == 1 && c6l == 1) {
      c6l = 0;
    }
    if (c7 == 0 && c7l == 0) {
      c7t = !c7t;
      c7l = 1;
    }
    if (c7 == 1 && c7l == 1) {
      c7l = 0;
    }


    if (d0 == 0 && d0l == 0) {
      d0t = !d0t;
      d0l = 1;
    }
    if (d0 == 1 && d0l == 1) {
      d0l = 0;
    }

    if (d1 == 0 && d1l == 0) {
      d1t = !d1t;
      d1l = 1;
    }
    if (d1 == 1 && d1l == 1) {
      d1l = 0;
    }
    if (d2 == 0 && d2l == 0) {
      d2t = !d2t;
      d2l = 1;
    }
    if (d2 == 1 && d2l == 1) {
      d2l = 0;
    }
    if (d3 == 0 && d3l == 0) {
      d3t = !d3t;
      d3l = 1;
    }
    if (d3 == 1 && d3l == 1) {
      d3l = 0;
    }
    if (d4 == 0 && d4l == 0) {
      d4t = !d4t;
      d4l = 1;
    }
    if (d4 == 1 && d4l == 1) {
      d4l = 0;
    }
    if (d5 == 0 && d5l == 0) {
      d5t = !d5t;
      d5l = 1;
    }
    if (d5 == 1 && d5l == 1) {
      d5l = 0;
    }
    if (d6 == 0 && d6l == 0) {
      d6t = !d6t;
      d6l = 1;
    }
    if (d6 == 1 && d6l == 1) {
      d6l = 0;
    }
    if (d7 == 0 && d7l == 0) {
      d7t = !d7t;
      d7l = 1;
    }
    if (d7 == 1 && d7l == 1) {
      d7l = 0;
    }


    if (e0 == 0 && e0l == 0) {
      e0t = !e0t;
      e0l = 1;
    }
    if (e0 == 1 && e0l == 1) {
      e0l = 0;
    }

    if (e1 == 0 && e1l == 0) {
      e1t = !e1t;
      e1l = 1;
    }
    if (e1 == 1 && e1l == 1) {
      e1l = 0;
    }
    if (e2 == 0 && e2l == 0) {
      e2t = !e2t;
      e2l = 1;
    }
    if (e2 == 1 && e2l == 1) {
      e2l = 0;
    }
    if (e3 == 0 && e3l == 0) {
      e3t = !e3t;
      e3l = 1;
    }
    if (e3 == 1 && e3l == 1) {
      e3l = 0;
    }
    if (e4 == 0 && e4l == 0) {
      e4t = !e4t;
      e4l = 1;
    }
    if (e4 == 1 && e4l == 1) {
      e4l = 0;
    }
    if (e5 == 0 && e5l == 0) {
      e5t = !e5t;
      e5l = 1;
    }
    if (e5 == 1 && e5l == 1) {
      e5l = 0;
    }
    if (e6 == 0 && e6l == 0) {
      e6t = !e6t;
      e6l = 1;
    }
    if (e6 == 1 && e6l == 1) {
      e6l = 0;
    }
    if (e7 == 0 && e7l == 0) {
      e7t = !e7t;
      e7l = 1;
    }
    if (e7 == 1 && e7l == 1) {
      e7l = 0;
    }

    if (f0 == 0 && f0l == 0) {
      f0t = !f0t;
      f0l = 1;
    }
    if (f0 == 1 && f0l == 1) {
      f0l = 0;
    }

    if (f1 == 0 && f1l == 0) {
      f1t = !f1t;
      f1l = 1;
    }
    if (f1 == 1 && f1l == 1) {
      f1l = 0;
    }
    if (f2 == 0 && f2l == 0) {
      f2t = !f2t;
      f2l = 1;
    }
    if (f2 == 1 && f2l == 1) {
      f2l = 0;
    }
    if (f3 == 0 && f3l == 0) {
      f3t = !f3t;
      f3l = 1;
    }
    if (f3 == 1 && f3l == 1) {
      f3l = 0;
    }
    if (f4 == 0 && f4l == 0) {
      f4t = !f4t;
      f4l = 1;
    }
    if (f4 == 1 && f4l == 1) {
      f4l = 0;
    }
    if (f5 == 0 && f5l == 0) {
      f5t = !f5t;
      f5l = 1;
    }
    if (f5 == 1 && f5l == 1) {
      f5l = 0;
    }
    if (f6 == 0 && f6l == 0) {
      f6t = !f6t;
      f6l = 1;
    }
    if (f6 == 1 && f6l == 1) {
      f6l = 0;
    }
    if (f7 == 0 && f7l == 0) {
      f7t = !f7t;
      f7l = 1;
    }
    if (f7 == 1 && f7l == 1) {
      f7l = 0;
    }


    if (g0 == 0 && g0l == 0) {
      g0t = !g0t;
      g0l = 1;
    }
    if (g0 == 1 && g0l == 1) {
      g0l = 0;
    }

    if (g1 == 0 && g1l == 0) {
      g1t = !g1t;
      g1l = 1;
    }
    if (g1 == 1 && g1l == 1) {
      g1l = 0;
    }
    if (g2 == 0 && g2l == 0) {
      g2t = !g2t;
      g2l = 1;
    }
    if (g2 == 1 && g2l == 1) {
      g2l = 0;
    }
    if (g3 == 0 && g3l == 0) {
      g3t = !g3t;
      g3l = 1;
    }
    if (g3 == 1 && g3l == 1) {
      g3l = 0;
    }
    if (g4 == 0 && g4l == 0) {
      g4t = !g4t;
      g4l = 1;
    }
    if (g4 == 1 && g4l == 1) {
      g4l = 0;
    }
    if (g5 == 0 && g5l == 0) {
      g5t = !g5t;
      g5l = 1;
    }
    if (g5 == 1 && g5l == 1) {
      g5l = 0;
    }
    if (g6 == 0 && g6l == 0) {
      g6t = !g6t;
      g6l = 1;
    }
    if (g6 == 1 && g6l == 1) {
      g6l = 0;
    }
    if (g7 == 0 && g7l == 0) {
      g7t = !g7t;
      g7l = 1;
    }
    if (g7 == 1 && g7l == 1) {
      g7l = 0;
    }


    if (h0 == 0 && h0l == 0) {
      h0t = !h0t;
      h0l = 1;
    }
    if (h0 == 1 && h0l == 1) {
      h0l = 0;
    }

    if (h1 == 0 && h1l == 0) {
      h1t = !h1t;
      h1l = 1;
    }
    if (h1 == 1 && h1l == 1) {
      h1l = 0;
    }
    if (h2 == 0 && h2l == 0) {
      h2t = !h2t;
      h2l = 1;
    }
    if (h2 == 1 && h2l == 1) {
      h2l = 0;
    }
    if (h3 == 0 && h3l == 0) {
      h3t = !h3t;
      h3l = 1;
    }
    if (h3 == 1 && h3l == 1) {
      h3l = 0;
    }
    if (h4 == 0 && h4l == 0) {
      h4t = !h4t;
      h4l = 1;
    }
    if (h4 == 1 && h4l == 1) {
      h4l = 0;
    }
    if (h5 == 0 && h5l == 0) {
      h5t = !h5t;
      h5l = 1;
    }
    if (h5 == 1 && h5l == 1) {
      h5l = 0;
    }
    if (h6 == 0 && h6l == 0) {
      h6t = !h6t;
      h6l = 1;
    }
    if (h6 == 1 && h6l == 1) {
      h6l = 0;
    }
    if (h7 == 0 && h7l == 0) {
      h7t = !h7t;
      h7l = 1;
    }
    if (h7 == 1 && h7l == 1) {
      h7l = 0;
    }
  }
  if (mainStep >= 8 && mainStep <= 15) {
    if (a0 == 0 && a0l == 0) {
      a9t = !a9t;
      a0l = 1;
    }
    if (a0 == 1 && a0l == 1) {
      a0l = 0;
    }

    if (a1 == 0 && a1l == 0) {
      a10t = !a10t;
      a1l = 1;
    }
    if (a1 == 1 && a1l == 1) {
      a1l = 0;
    }
    if (a2 == 0 && a2l == 0) {
      a11t = !a11t;
      a2l = 1;
    }
    if (a2 == 1 && a2l == 1) {
      a2l = 0;
    }
    if (a3 == 0 && a3l == 0) {
      a12t = !a12t;
      a3l = 1;
    }
    if (a3 == 1 && a3l == 1) {
      a3l = 0;
    }
    if (a4 == 0 && a4l == 0) {
      a13t = !a13t;
      a4l = 1;
    }
    if (a4 == 1 && a4l == 1) {
      a4l = 0;
    }
    if (a5 == 0 && a5l == 0) {
      a14t = !a14t;
      a5l = 1;
    }
    if (a5 == 1 && a5l == 1) {
      a5l = 0;
    }
    if (a6 == 0 && a6l == 0) {
      a15t = !a15t;
      a6l = 1;
    }
    if (a6 == 1 && a6l == 1) {
      a6l = 0;
    }
    if (a7 == 0 && a7l == 0) {
      a16t = !a16t;
      a7l = 1;
    }
    if (a7 == 1 && a7l == 1) {
      a7l = 0;
    }


    if (b0 == 0 && b0l == 0) {
      b9t = !b9t;
      b0l = 1;
    }
    if (b0 == 1 && b0l == 1) {
      b0l = 0;
    }

    if (b1 == 0 && b1l == 0) {
      b10t = !b10t;
      b1l = 1;
    }
    if (b1 == 1 && b1l == 1) {
      b1l = 0;
    }
    if (b2 == 0 && b2l == 0) {
      b11t = !b11t;
      b2l = 1;
    }
    if (b2 == 1 && b2l == 1) {
      b2l = 0;
    }
    if (b3 == 0 && b3l == 0) {
      b12t = !b12t;
      b3l = 1;
    }
    if (b3 == 1 && b3l == 1) {
      b3l = 0;
    }
    if (b4 == 0 && b4l == 0) {
      b13t = !b13t;
      b4l = 1;
    }
    if (b4 == 1 && b4l == 1) {
      b4l = 0;
    }
    if (b5 == 0 && b5l == 0) {
      b14t = !b14t;
      b5l = 1;
    }
    if (b5 == 1 && b5l == 1) {
      b5l = 0;
    }
    if (b6 == 0 && b6l == 0) {
      b15t = !b15t;
      b6l = 1;
    }
    if (b6 == 1 && b6l == 1) {
      b6l = 0;
    }
    if (b7 == 0 && b7l == 0) {
      b16t = !b16t;
      b7l = 1;
    }
    if (b7 == 1 && b7l == 1) {
      b7l = 0;
    }


    if (c0 == 0 && c0l == 0) {
      c9t = !c9t;
      c0l = 1;
    }
    if (c0 == 1 && c0l == 1) {
      c0l = 0;
    }

    if (c1 == 0 && c1l == 0) {
      c10t = !c10t;
      c1l = 1;
    }
    if (c1 == 1 && c1l == 1) {
      c1l = 0;
    }
    if (c2 == 0 && c2l == 0) {
      c11t = !c11t;
      c2l = 1;
    }
    if (c2 == 1 && c2l == 1) {
      c2l = 0;
    }
    if (c3 == 0 && c3l == 0) {
      c12t = !c12t;
      c3l = 1;
    }
    if (c3 == 1 && c3l == 1) {
      c3l = 0;
    }
    if (c4 == 0 && c4l == 0) {
      c13t = !c13t;
      c4l = 1;
    }
    if (c4 == 1 && c4l == 1) {
      c4l = 0;
    }
    if (c5 == 0 && c5l == 0) {
      c14t = !c14t;
      c5l = 1;
    }
    if (c5 == 1 && c5l == 1) {
      c5l = 0;
    }
    if (c6 == 0 && c6l == 0) {
      c15t = !c15t;
      c6l = 1;
    }
    if (c6 == 1 && c6l == 1) {
      c6l = 0;
    }
    if (c7 == 0 && c7l == 0) {
      c16t = !c16t;
      c7l = 1;
    }
    if (c7 == 1 && c7l == 1) {
      c7l = 0;
    }


    if (d0 == 0 && d0l == 0) {
      d9t = !d9t;
      d0l = 1;
    }
    if (d0 == 1 && d0l == 1) {
      d0l = 0;
    }

    if (d1 == 0 && d1l == 0) {
      d10t = !d10t;
      d1l = 1;
    }
    if (d1 == 1 && d1l == 1) {
      d1l = 0;
    }
    if (d2 == 0 && d2l == 0) {
      d11t = !d11t;
      d2l = 1;
    }
    if (d2 == 1 && d2l == 1) {
      d2l = 0;
    }
    if (d3 == 0 && d3l == 0) {
      d12t = !d12t;
      d3l = 1;
    }
    if (d3 == 1 && d3l == 1) {
      d3l = 0;
    }
    if (d4 == 0 && d4l == 0) {
      d13t = !d13t;
      d4l = 1;
    }
    if (d4 == 1 && d4l == 1) {
      d4l = 0;
    }
    if (d5 == 0 && d5l == 0) {
      d14t = !d14t;
      d5l = 1;
    }
    if (d5 == 1 && d5l == 1) {
      d5l = 0;
    }
    if (d6 == 0 && d6l == 0) {
      d15t = !d15t;
      d6l = 1;
    }
    if (d6 == 1 && d6l == 1) {
      d6l = 0;
    }
    if (d7 == 0 && d7l == 0) {
      d16t = !d16t;
      d7l = 1;
    }
    if (d7 == 1 && d7l == 1) {
      d7l = 0;
    }


    if (e0 == 0 && e0l == 0) {
      e9t = !e9t;
      e0l = 1;
    }
    if (e0 == 1 && e0l == 1) {
      e0l = 0;
    }

    if (e1 == 0 && e1l == 0) {
      e10t = !e10t;
      e1l = 1;
    }
    if (e1 == 1 && e1l == 1) {
      e1l = 0;
    }
    if (e2 == 0 && e2l == 0) {
      e11t = !e11t;
      e2l = 1;
    }
    if (e2 == 1 && e2l == 1) {
      e2l = 0;
    }
    if (e3 == 0 && e3l == 0) {
      e12t = !e12t;
      e3l = 1;
    }
    if (e3 == 1 && e3l == 1) {
      e3l = 0;
    }
    if (e4 == 0 && e4l == 0) {
      e13t = !e13t;
      e4l = 1;
    }
    if (e4 == 1 && e4l == 1) {
      e4l = 0;
    }
    if (e5 == 0 && e5l == 0) {
      e14t = !e14t;
      e5l = 1;
    }
    if (e5 == 1 && e5l == 1) {
      e5l = 0;
    }
    if (e6 == 0 && e6l == 0) {
      e15t = !e15t;
      e6l = 1;
    }
    if (e6 == 1 && e6l == 1) {
      e6l = 0;
    }
    if (e7 == 0 && e7l == 0) {
      e16t = !e16t;
      e7l = 1;
    }
    if (e7 == 1 && e7l == 1) {
      e7l = 0;
    }

    if (f0 == 0 && f0l == 0) {
      f9t = !f9t;
      f0l = 1;
    }
    if (f0 == 1 && f0l == 1) {
      f0l = 0;
    }

    if (f1 == 0 && f1l == 0) {
      f10t = !f10t;
      f1l = 1;
    }
    if (f1 == 1 && f1l == 1) {
      f1l = 0;
    }
    if (f2 == 0 && f2l == 0) {
      f11t = !f11t;
      f2l = 1;
    }
    if (f2 == 1 && f2l == 1) {
      f2l = 0;
    }
    if (f3 == 0 && f3l == 0) {
      f12t = !f12t;
      f3l = 1;
    }
    if (f3 == 1 && f3l == 1) {
      f3l = 0;
    }
    if (f4 == 0 && f4l == 0) {
      f13t = !f13t;
      f4l = 1;
    }
    if (f4 == 1 && f4l == 1) {
      f4l = 0;
    }
    if (f5 == 0 && f5l == 0) {
      f14t = !f14t;
      f5l = 1;
    }
    if (f5 == 1 && f5l == 1) {
      f5l = 0;
    }
    if (f6 == 0 && f6l == 0) {
      f15t = !f15t;
      f6l = 1;
    }
    if (f6 == 1 && f6l == 1) {
      f6l = 0;
    }
    if (f7 == 0 && f7l == 0) {
      f16t = !f16t;
      f7l = 1;
    }
    if (f7 == 1 && f7l == 1) {
      f7l = 0;
    }


    if (g0 == 0 && g0l == 0) {
      g9t = !g9t;
      g0l = 1;
    }
    if (g0 == 1 && g0l == 1) {
      g0l = 0;
    }

    if (g1 == 0 && g1l == 0) {
      g10t = !g10t;
      g1l = 1;
    }
    if (g1 == 1 && g1l == 1) {
      g1l = 0;
    }
    if (g2 == 0 && g2l == 0) {
      g11t = !g11t;
      g2l = 1;
    }
    if (g2 == 1 && g2l == 1) {
      g2l = 0;
    }
    if (g3 == 0 && g3l == 0) {
      g12t = !g12t;
      g3l = 1;
    }
    if (g3 == 1 && g3l == 1) {
      g3l = 0;
    }
    if (g4 == 0 && g4l == 0) {
      g13t = !g13t;
      g4l = 1;
    }
    if (g4 == 1 && g4l == 1) {
      g4l = 0;
    }
    if (g5 == 0 && g5l == 0) {
      g14t = !g14t;
      g5l = 1;
    }
    if (g5 == 1 && g5l == 1) {
      g5l = 0;
    }
    if (g6 == 0 && g6l == 0) {
      g15t = !g15t;
      g6l = 1;
    }
    if (g6 == 1 && g6l == 1) {
      g6l = 0;
    }
    if (g7 == 0 && g7l == 0) {
      g16t = !g16t;
      g7l = 1;
    }
    if (g7 == 1 && g7l == 1) {
      g7l = 0;
    }


    if (h0 == 0 && h0l == 0) {
      h9t = !h9t;
      h0l = 1;
    }
    if (h0 == 1 && h0l == 1) {
      h0l = 0;
    }

    if (h1 == 0 && h1l == 0) {
      h10t = !h10t;
      h1l = 1;
    }
    if (h1 == 1 && h1l == 1) {
      h1l = 0;
    }
    if (h2 == 0 && h2l == 0) {
      h11t = !h11t;
      h2l = 1;
    }
    if (h2 == 1 && h2l == 1) {
      h2l = 0;
    }
    if (h3 == 0 && h3l == 0) {
      h12t = !h12t;
      h3l = 1;
    }
    if (h3 == 1 && h3l == 1) {
      h3l = 0;
    }
    if (h4 == 0 && h4l == 0) {
      h13t = !h13t;
      h4l = 1;
    }
    if (h4 == 1 && h4l == 1) {
      h4l = 0;
    }
    if (h5 == 0 && h5l == 0) {
      h14t = !h5t;
      h5l = 1;
    }
    if (h5 == 1 && h5l == 1) {
      h5l = 0;
    }
    if (h6 == 0 && h6l == 0) {
      h15t = !h15t;
      h6l = 1;
    }
    if (h6 == 1 && h6l == 1) {
      h6l = 0;
    }
    if (h7 == 0 && h7l == 0) {
      h16t = !h16t;
      h7l = 1;
    }
    if (h7 == 1 && h7l == 1) {
      h7l = 0;
    }
  }
}

void ledResponse() {
  if (mainStep >= 0 && mainStep <= 7) {
    if (a0t == 1) {
      leds[0] = CRGB::Yellow;
    }
    if (a0t == 0) {
      leds[0] = CRGB::Black;
    }

    if (a1t == 1) {
      leds[15] = CRGB::Yellow;
    }
    if (a1t == 0) {
      leds[15] = CRGB::Black;
    }

    if (a2t == 1) {
      leds[16] = CRGB::Yellow;
    }
    if (a2t == 0) {
      leds[16] = CRGB::Black;
    }

    if (a3t == 1) {
      leds[31] = CRGB::Yellow;
    }
    if (a3t == 0) {
      leds[31] = CRGB::Black;
    }

    if (a4t == 1) {
      leds[32] = CRGB::Yellow;
    }
    if (a4t == 0) {
      leds[32] = CRGB::Black;
    }

    if (a5t == 1) {
      leds[47] = CRGB::Yellow;
    }
    if (a5t == 0) {
      leds[47] = CRGB::Black;
    }
    if (a6t == 1) {
      leds[48] = CRGB::Yellow;
    }
    if (a6t == 0) {
      leds[48] = CRGB::Black;
    }
    if (a7t == 1) {
      leds[63] = CRGB::Yellow;
    }
    if (a7t == 0) {
      leds[63] = CRGB::Black;
    }
    if (b0t == 1) {
      leds[1] = CRGB::Yellow;
    }
    if (b0t == 0) {
      leds[1] = CRGB::Black;
    }
    if (b1t == 1) {
      leds[14] = CRGB::Yellow;
    }
    if (b1t == 0) {
      leds[14] = CRGB::Black;
    }
    if (b2t == 1) {
      leds[17] = CRGB::Yellow;
    }
    if (b2t == 0) {
      leds[17] = CRGB::Black;
    }
    if (b3t == 1) {
      leds[30] = CRGB::Yellow;
    }
    if (b3t == 0) {
      leds[30] = CRGB::Black;
    }
    if (b4t == 1) {
      leds[33] = CRGB::Yellow;
    }
    if (b4t == 0) {
      leds[33] = CRGB::Black;
    }
    if (b5t == 1) {
      leds[46] = CRGB::Yellow;
    }
    if (b5t == 0) {
      leds[46] = CRGB::Black;
    }
    if (b6t == 1) {
      leds[49] = CRGB::Yellow;
    }
    if (b6t == 0) {
      leds[49] = CRGB::Black;
    }
    if (b7t == 1) {
      leds[62] = CRGB::Yellow;
    }
    if (b7t == 0) {
      leds[62] = CRGB::Black;
    }
    if (c0t == 1) {
      leds[2] = CRGB::Yellow;
    }
    if (c0t == 0) {
      leds[2] = CRGB::Black;
    }
    if (c1t == 1) {
      leds[13] = CRGB::Yellow;
    }
    if (c1t == 0) {
      leds[13] = CRGB::Black;
    }
    if (c2t == 1) {
      leds[18] = CRGB::Yellow;
    }
    if (c2t == 0) {
      leds[18] = CRGB::Black;
    }
    if (c3t == 1) {
      leds[29] = CRGB::Yellow;
    }
    if (c3t == 0) {
      leds[29] = CRGB::Black;
    }
    if (c4t == 1) {
      leds[34] = CRGB::Yellow;
    }
    if (c4t == 0) {
      leds[34] = CRGB::Black;
    }
    if (c5t == 1) {
      leds[45] = CRGB::Yellow;
    }
    if (c5t == 0) {
      leds[45] = CRGB::Black;
    }
    if (c6t == 1) {
      leds[50] = CRGB::Yellow;
    }
    if (c6t == 0) {
      leds[50] = CRGB::Black;
    }
    if (c7t == 1) {
      leds[61] = CRGB::Yellow;
    }
    if (c7t == 0) {
      leds[61] = CRGB::Black;
    }
    if (d0t == 1) {
      leds[3] = CRGB::Yellow;
    }
    if (d0t == 0) {
      leds[3] = CRGB::Black;
    }
    if (d1t == 1) {
      leds[12] = CRGB::Yellow;
    }
    if (d1t == 0) {
      leds[12] = CRGB::Black;
    }
    if (d2t == 1) {
      leds[19] = CRGB::Yellow;
    }
    if (d2t == 0) {
      leds[19] = CRGB::Black;
    }
    if (d3t == 1) {
      leds[28] = CRGB::Yellow;
    }
    if (d3t == 0) {
      leds[28] = CRGB::Black;
    }
    if (d4t == 1) {
      leds[35] = CRGB::Yellow;
    }
    if (d4t == 0) {
      leds[35] = CRGB::Black;
    }
    if (d5t == 1) {
      leds[44] = CRGB::Yellow;
    }
    if (d5t == 0) {
      leds[44] = CRGB::Black;
    }
    if (d6t == 1) {
      leds[51] = CRGB::Yellow;
    }
    if (d6t == 0) {
      leds[51] = CRGB::Black;
    }
    if (d7t == 1) {
      leds[60] = CRGB::Yellow;
    }
    if (d7t == 0) {
      leds[60] = CRGB::Black;
    }
    if (e0t == 1) {
      leds[4] = CRGB::Yellow;
    }
    if (e0t == 0) {
      leds[4] = CRGB::Black;
    }
    if (e1t == 1) {
      leds[11] = CRGB::Yellow;
    }
    if (e1t == 0) {
      leds[11] = CRGB::Black;
    }
    if (e2t == 1) {
      leds[20] = CRGB::Yellow;
    }
    if (e2t == 0) {
      leds[20] = CRGB::Black;
    }
    if (e3t == 1) {
      leds[27] = CRGB::Yellow;
    }
    if (e3t == 0) {
      leds[27] = CRGB::Black;
    }
    if (e4t == 1) {
      leds[36] = CRGB::Yellow;
    }
    if (e4t == 0) {
      leds[36] = CRGB::Black;
    }
    if (e5t == 1) {
      leds[43] = CRGB::Yellow;
    }
    if (e5t == 0) {
      leds[43] = CRGB::Black;
    }
    if (e6t == 1) {
      leds[52] = CRGB::Yellow;
    }
    if (e6t == 0) {
      leds[52] = CRGB::Black;
    }
    if (e7t == 1) {
      leds[59] = CRGB::Yellow;
    }
    if (e7t == 0) {
      leds[59] = CRGB::Black;
    }
    if (f0t == 1) {
      leds[5] = CRGB::Yellow;
    }
    if (f0t == 0) {
      leds[5] = CRGB::Black;
    }
    if (f1t == 1) {
      leds[10] = CRGB::Yellow;
    }
    if (f1t == 0) {
      leds[10] = CRGB::Black;
    }
    if (f2t == 1) {
      leds[21] = CRGB::Yellow;
    }
    if (f2t == 0) {
      leds[21] = CRGB::Black;
    }
    if (f3t == 1) {
      leds[26] = CRGB::Yellow;
    }
    if (f3t == 0) {
      leds[26] = CRGB::Black;
    }
    if (f4t == 1) {
      leds[37] = CRGB::Yellow;
    }
    if (f4t == 0) {
      leds[37] = CRGB::Black;
    }
    if (f5t == 1) {
      leds[42] = CRGB::Yellow;
    }
    if (f5t == 0) {
      leds[42] = CRGB::Black;
    }
    if (f6t == 1) {
      leds[53] = CRGB::Yellow;
    }
    if (f6t == 0) {
      leds[53] = CRGB::Black;
    }
    if (f7t == 1) {
      leds[58] = CRGB::Yellow;
    }
    if (f7t == 0) {
      leds[58] = CRGB::Black;
    }
    if (g0t == 1) {
      leds[6] = CRGB::Yellow;
    }
    if (g0t == 0) {
      leds[6] = CRGB::Black;
    }

    if (g1t == 1) {
      leds[9] = CRGB::Yellow;
    }
    if (g1t == 0) {
      leds[9] = CRGB::Black;
    }

    if (g2t == 1) {
      leds[22] = CRGB::Yellow;
    }
    if (g2t == 0) {
      leds[22] = CRGB::Black;
    }

    if (g3t == 1) {
      leds[25] = CRGB::Yellow;
    }
    if (g3t == 0) {
      leds[25] = CRGB::Black;
    }

    if (g4t == 1) {
      leds[38] = CRGB::Yellow;
    }
    if (g4t == 0) {
      leds[38] = CRGB::Black;
    }

    if (g5t == 1) {
      leds[41] = CRGB::Yellow;
    }
    if (g5t == 0) {
      leds[41] = CRGB::Black;
    }

    if (g6t == 1) {
      leds[54] = CRGB::Yellow;
    }
    if (g6t == 0) {
      leds[54] = CRGB::Black;
    }

    if (g7t == 1) {
      leds[57] = CRGB::Yellow;
    }
    if (g7t == 0) {
      leds[57] = CRGB::Black;
    }

    if (h0t == 1) {
      leds[7] = CRGB::Yellow;
    }
    if (h0t == 0) {
      leds[7] = CRGB::Black;
    }

    if (h1t == 1) {
      leds[8] = CRGB::Yellow;
    }
    if (h1t == 0) {
      leds[8] = CRGB::Black;
    }

    if (h2t == 1) {
      leds[23] = CRGB::Yellow;
    }
    if (h2t == 0) {
      leds[23] = CRGB::Black;
    }

    if (h3t == 1) {
      leds[24] = CRGB::Yellow;
    }
    if (h3t == 0) {
      leds[24] = CRGB::Black;
    }

    if (h4t == 1) {
      leds[39] = CRGB::Yellow;
    }
    if (h4t == 0) {
      leds[39] = CRGB::Black;
    }

    if (h5t == 1) {
      leds[40] = CRGB::Yellow;
    }
    if (h5t == 0) {
      leds[40] = CRGB::Black;
    }

    if (h6t == 1) {
      leds[55] = CRGB::Yellow;
    }
    if (h6t == 0) {
      leds[55] = CRGB::Black;
    }

    if (h7t == 1) {
      leds[56] = CRGB::Yellow;
    }
    if (h7t == 0) {
      leds[56] = CRGB::Black;
    }
    FastLED.show();
  }
  if (mainStep >= 8 && mainStep <= 15) {
    if (a9t == 1) {
      leds[0] = CRGB::Yellow;
    }
    if (a9t == 0) {
      leds[0] = CRGB::Black;
    }
    if (a10t == 1) {
      leds[15] = CRGB::Yellow;
    }
    if (a10t == 0) {
      leds[15] = CRGB::Black;
    }
    if (a11t == 1) {
      leds[16] = CRGB::Yellow;
    }
    if (a11t == 0) {
      leds[16] = CRGB::Black;
    }
    if (a12t == 1) {
      leds[31] = CRGB::Yellow;
    }
    if (a12t == 0) {
      leds[31] = CRGB::Black;
    }
    if (a13t == 1) {
      leds[32] = CRGB::Yellow;
    }
    if (a13t == 0) {
      leds[32] = CRGB::Black;
    }
    if (a14t == 1) {
      leds[47] = CRGB::Yellow;
    }
    if (a14t == 0) {
      leds[47] = CRGB::Black;
    }
    if (a15t == 1) {
      leds[48] = CRGB::Yellow;
    }
    if (a15t == 0) {
      leds[48] = CRGB::Black;
    }
    if (a16t == 1) {
      leds[63] = CRGB::Yellow;
    }
    if (a16t == 0) {
      leds[63] = CRGB::Black;
    }
    if (b9t == 1) {
      leds[1] = CRGB::Yellow;
    }
    if (b9t == 0) {
      leds[1] = CRGB::Black;
    }
    if (b10t == 1) {
      leds[14] = CRGB::Yellow;
    }
    if (b10t == 0) {
      leds[14] = CRGB::Black;
    }
    if (b11t == 1) {
      leds[17] = CRGB::Yellow;
    }
    if (b11t == 0) {
      leds[17] = CRGB::Black;
    }
    if (b12t == 1) {
      leds[30] = CRGB::Yellow;
    }
    if (b12t == 0) {
      leds[30] = CRGB::Black;
    }
    if (b13t == 1) {
      leds[33] = CRGB::Yellow;
    }
    if (b13t == 0) {
      leds[33] = CRGB::Black;
    }
    if (b14t == 1) {
      leds[46] = CRGB::Yellow;
    }
    if (b14t == 0) {
      leds[46] = CRGB::Black;
    }
    if (b15t == 1) {
      leds[49] = CRGB::Yellow;
    }
    if (b15t == 0) {
      leds[49] = CRGB::Black;
    }
    if (b16t == 1) {
      leds[62] = CRGB::Yellow;
    }
    if (b16t == 0) {
      leds[62] = CRGB::Black;
    }
    if (c9t == 1) {
      leds[2] = CRGB::Yellow;
    }
    if (c9t == 0) {
      leds[2] = CRGB::Black;
    }
    if (c10t == 1) {
      leds[13] = CRGB::Yellow;
    }
    if (c10t == 0) {
      leds[13] = CRGB::Black;
    }
    if (c11t == 1) {
      leds[18] = CRGB::Yellow;
    }
    if (c11t == 0) {
      leds[18] = CRGB::Black;
    }
    if (c12t == 1) {
      leds[29] = CRGB::Yellow;
    }
    if (c12t == 0) {
      leds[29] = CRGB::Black;
    }
    if (c13t == 1) {
      leds[34] = CRGB::Yellow;
    }
    if (c13t == 0) {
      leds[34] = CRGB::Black;
    }
    if (c14t == 1) {
      leds[45] = CRGB::Yellow;
    }
    if (c14t == 0) {
      leds[45] = CRGB::Black;
    }
    if (c15t == 1) {
      leds[50] = CRGB::Yellow;
    }
    if (c15t == 0) {
      leds[50] = CRGB::Black;
    }
    if (c16t == 1) {
      leds[61] = CRGB::Yellow;
    }
    if (c16t == 0) {
      leds[61] = CRGB::Black;
    }
    if (d9t == 1) {
      leds[3] = CRGB::Yellow;
    }
    if (d9t == 0) {
      leds[3] = CRGB::Black;
    }
    if (d10t == 1) {
      leds[12] = CRGB::Yellow;
    }
    if (d10t == 0) {
      leds[12] = CRGB::Black;
    }
    if (d11t == 1) {
      leds[19] = CRGB::Yellow;
    }
    if (d11t == 0) {
      leds[19] = CRGB::Black;
    }
    if (d12t == 1) {
      leds[28] = CRGB::Yellow;
    }
    if (d12t == 0) {
      leds[28] = CRGB::Black;
    }
    if (d13t == 1) {
      leds[35] = CRGB::Yellow;
    }
    if (d13t == 0) {
      leds[35] = CRGB::Black;
    }
    if (d14t == 1) {
      leds[44] = CRGB::Yellow;
    }
    if (d14t == 0) {
      leds[44] = CRGB::Black;
    }
    if (d15t == 1) {
      leds[51] = CRGB::Yellow;
    }
    if (d15t == 0) {
      leds[51] = CRGB::Black;
    }
    if (d16t == 1) {
      leds[60] = CRGB::Yellow;
    }
    if (d16t == 0) {
      leds[60] = CRGB::Black;
    }
    if (e9t == 1) {
      leds[4] = CRGB::Yellow;
    }
    if (e9t == 0) {
      leds[4] = CRGB::Black;
    }
    if (e10t == 1) {
      leds[11] = CRGB::Yellow;
    }
    if (e10t == 0) {
      leds[11] = CRGB::Black;
    }
    if (e11t == 1) {
      leds[20] = CRGB::Yellow;
    }
    if (e11t == 0) {
      leds[20] = CRGB::Black;
    }
    if (e12t == 1) {
      leds[27] = CRGB::Yellow;
    }
    if (e12t == 0) {
      leds[27] = CRGB::Black;
    }
    if (e13t == 1) {
      leds[36] = CRGB::Yellow;
    }
    if (e13t == 0) {
      leds[36] = CRGB::Black;
    }
    if (e14t == 1) {
      leds[43] = CRGB::Yellow;
    }
    if (e14t == 0) {
      leds[43] = CRGB::Black;
    }
    if (e15t == 1) {
      leds[52] = CRGB::Yellow;
    }
    if (e15t == 0) {
      leds[52] = CRGB::Black;
    }
    if (e16t == 1) {
      leds[59] = CRGB::Yellow;
    }
    if (e16t == 0) {
      leds[59] = CRGB::Black;
    }
    if (f9t == 1) {
      leds[5] = CRGB::Yellow;
    }
    if (f9t == 0) {
      leds[5] = CRGB::Black;
    }
    if (f10t == 1) {
      leds[10] = CRGB::Yellow;
    }
    if (f10t == 0) {
      leds[10] = CRGB::Black;
    }
    if (f11t == 1) {
      leds[21] = CRGB::Yellow;
    }
    if (f11t == 0) {
      leds[21] = CRGB::Black;
    }
    if (f12t == 1) {
      leds[26] = CRGB::Yellow;
    }
    if (f12t == 0) {
      leds[26] = CRGB::Black;
    }
    if (f13t == 1) {
      leds[37] = CRGB::Yellow;
    }
    if (f13t == 0) {
      leds[37] = CRGB::Black;
    }
    if (f14t == 1) {
      leds[42] = CRGB::Yellow;
    }
    if (f14t == 0) {
      leds[42] = CRGB::Black;
    }
    if (f15t == 1) {
      leds[53] = CRGB::Yellow;
    }
    if (f15t == 0) {
      leds[53] = CRGB::Black;
    }
    if (f16t == 1) {
      leds[58] = CRGB::Yellow;
    }
    if (f16t == 0) {
      leds[58] = CRGB::Black;
    }
    if (g9t == 1) {
      leds[6] = CRGB::Yellow;
    }
    if (g9t == 0) {
      leds[6] = CRGB::Black;
    }
    if (g10t == 1) {
      leds[9] = CRGB::Yellow;
    }
    if (g10t == 0) {
      leds[9] = CRGB::Black;
    }
    if (g11t == 1) {
      leds[22] = CRGB::Yellow;
    }
    if (g11t == 0) {
      leds[22] = CRGB::Black;
    }
    if (g12t == 1) {
      leds[25] = CRGB::Yellow;
    }
    if (g12t == 0) {
      leds[25] = CRGB::Black;
    }
    if (g13t == 1) {
      leds[38] = CRGB::Yellow;
    }
    if (g13t == 0) {
      leds[38] = CRGB::Black;
    }
    if (g14t == 1) {
      leds[41] = CRGB::Yellow;
    }
    if (g14t == 0) {
      leds[41] = CRGB::Black;
    }
    if (g15t == 1) {
      leds[54] = CRGB::Yellow;
    }
    if (g15t == 0) {
      leds[54] = CRGB::Black;
    }
    if (g16t == 1) {
      leds[57] = CRGB::Yellow;
    }
    if (g16t == 0) {
      leds[57] = CRGB::Black;
    }
    if (h9t == 1) {
      leds[7] = CRGB::Yellow;
    }
    if (h9t == 0) {
      leds[7] = CRGB::Black;
    }
    if (h10t == 1) {
      leds[8] = CRGB::Yellow;
    }
    if (h1t == 0) {
      leds[8] = CRGB::Black;
    }
    if (h11t == 1) {
      leds[23] = CRGB::Yellow;
    }
    if (h11t == 0) {
      leds[23] = CRGB::Black;
    }
    if (h12t == 1) {
      leds[24] = CRGB::Yellow;
    }
    if (h12t == 0) {
      leds[24] = CRGB::Black;
    }
    if (h13t == 1) {
      leds[39] = CRGB::Yellow;
    }
    if (h13t == 0) {
      leds[39] = CRGB::Black;
    }
    if (h14t == 1) {
      leds[40] = CRGB::Yellow;
    }
    if (h14t == 0) {
      leds[40] = CRGB::Black;
    }
    if (h15t == 1) {
      leds[55] = CRGB::Yellow;
    }
    if (h15t == 0) {
      leds[55] = CRGB::Black;
    }
    if (h16t == 1) {
      leds[56] = CRGB::Yellow;
    }
    if (h16t == 0) {
      leds[56] = CRGB::Black;
    }
    FastLED.show();
  }
}

void checkState() {

  //set Multiplex to binary 0
  digitalWrite(pinA2, 0);
  digitalWrite(pinB2, 0);
  digitalWrite(pinC2, 0);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  a0 = ! digitalRead(buttons8x8);
  digitalWrite(gatesOut, gate1);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  a1 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate2);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  a2 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate3);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  a3 = ! digitalRead(buttons8x8);  
  digitalWrite(gatesOut, gate4);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  a4 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate5);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  a5 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate6);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  a6 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate7);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  a7 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate8);

  //set Multiplex to binary 1
  digitalWrite(pinA2, 1);
  digitalWrite(pinB2, 0);
  digitalWrite(pinC2, 0);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  b0 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate1);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  b1 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate2);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  b2 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate3);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  b3 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate4);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  b4 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate5);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  b5 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate6);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  b6 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate7);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  b7 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate8);

  //set Multiplex to binary 2
  digitalWrite(pinA2, 0);
  digitalWrite(pinB2, 1);
  digitalWrite(pinC2, 0);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  c0 = ! digitalRead(buttons8x8);
  digitalWrite(gatesOut, gate1);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  c1 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate2);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  c2 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate3);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  c3 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate4);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  c4 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate5);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  c5 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate6);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  c6 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate7);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  c7 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate8);

  //set Multiplex to binary 3
  digitalWrite(pinA2, 1);
  digitalWrite(pinB2, 1);
  digitalWrite(pinC2, 0);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  d0 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate1);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  d1 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate2);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  d2 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate3);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  d3 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate4);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  d4 = ! digitalRead(buttons8x8);
  digitalWrite(gatesOut, gate5);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  d5 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate6);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  d6 = ! digitalRead(buttons8x8);
  digitalWrite(gatesOut, gate7);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  d7 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate8);

  //set Multiplex to binary 4
  digitalWrite(pinA2, 0);
  digitalWrite(pinB2, 0);
  digitalWrite(pinC2, 1);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  e0 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate1);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  e1 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate2);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  e2 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate3);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  e3 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate4);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  e4 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate5);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  e5 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate6);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  e6 = ! digitalRead(buttons8x8);
  digitalWrite(gatesOut, gate7);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  e7 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate8);

  //set Multiplex to binary 5
  digitalWrite(pinA2, 1);
  digitalWrite(pinB2, 0);
  digitalWrite(pinC2, 1);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  f0 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate1);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  f1 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate2);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  f2 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate3);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  f3 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate4);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  f4 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate5);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  f5 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate6);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  f6 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate7);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  f7 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate8);

  //set Multiplex to binary 6
  digitalWrite(pinA2, 0);
  digitalWrite(pinB2, 1);
  digitalWrite(pinC2, 1);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  g0 = ! digitalRead(buttons8x8);
  digitalWrite(gatesOut, gate1);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  g1 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate2);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  g2 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate3);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  g3 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate4);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  g4 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate5);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  g5 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate6);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  g6 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate7);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  g7 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate8);

  //set Multiplex to binary 7
  digitalWrite(pinA2, 1);
  digitalWrite(pinB2, 1);
  digitalWrite(pinC2, 1);

  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  h0 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate1);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 0);
  h1 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate2);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  h2 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate3);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 0);
  h3 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate4);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  h4 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate5);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 0);
  digitalWrite(pinC1, 1);
  h5 = ! digitalRead(buttons8x8);
  digitalWrite(gatesOut, gate6);
  digitalWrite(pinA1, 0);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  h6 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate7);
  digitalWrite(pinA1, 1);
  digitalWrite(pinB1, 1);
  digitalWrite(pinC1, 1);
  h7 = ! digitalRead(buttons8x8); 
  digitalWrite(gatesOut, gate8);
}

void gates()  {
  gate1 = 0;
   gate2 = 0;
    gate3 = 0;
     gate4 = 1;
      gate5 = 0;
       gate6 = 0;
        gate7 = 0;
         gate8 = 0;
}

void loop() {
  encoderChange();
  checkState();
  encoderChange();
  buttonsToggle();
  encoderChange();
  gates();
  encoderChange();
  bpmTick();
  encoderChange();
  clockBar8x8();
  encoderChange();
  mainStepControl();
  encoderChange();
  ledResponse();
  encoderChange();
  Serial.println(gatesOut);
}
