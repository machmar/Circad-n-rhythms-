#include <FastLED.h>

#define colA 13
#define colB 17
#define colC 16
#define commA 10
#define commB 11
#define commC 12
#define buttonMatrixMux 9
#define gatesMux 6
#define jacksIO 15
#define encoderA 2
#define encoderB 5
#define menuButtonMux 4
#define ledPin 3
#define swingPot 20
#define animationPot 21
#define regD 19
#define regL 18
#define regC 14

bool buttonMatrix[65];
bool buttonMatrixLast[65];
bool buttonMatrixToggleState1[65];
int counter;
uint8_t gateState;
bool clockOut;
bool clockIn;
bool resetOut;
bool resetIn;
bool animationIn;
bool coderButton;
bool menuButton[9];
int animation = 0;

int brightness = 20;
#define FPS 60

int colPos;
int commPos;
bool clockInLast;
int masterClock;
int masterClockLast;
bool clockToggleTrigger;
bool anyButtonPressed;
int triggerLenght = 30;//ms
int gHue;

int i;

long Pmill;
long PmillTrig;
long PLEDmill;

CRGB leds[72];

void fadeall() {
  for (int i = 0; i < 72; i++) {
    leds[i].nscale8(250);
  }
}

void col(int pos) {
  switch (pos) {
    case 0:
      digitalWrite(colA, 0);
      digitalWrite(colB, 0);
      digitalWrite(colC, 0);
      break;
    case 1:
      digitalWrite(colA, 1);
      digitalWrite(colB, 0);
      digitalWrite(colC, 0);
      break;
    case 2:
      digitalWrite(colA, 0);
      digitalWrite(colB, 1);
      digitalWrite(colC, 0);
      break;
    case 3:
      digitalWrite(colA, 1);
      digitalWrite(colB, 1);
      digitalWrite(colC, 0);
      break;
    case 4:
      digitalWrite(colA, 0);
      digitalWrite(colB, 0);
      digitalWrite(colC, 1);
      break;
    case 5:
      digitalWrite(colA, 1);
      digitalWrite(colB, 0);
      digitalWrite(colC, 1);
      break;
    case 6:
      digitalWrite(colA, 0);
      digitalWrite(colB, 1);
      digitalWrite(colC, 1);
      break;
    case 7:
      digitalWrite(colA, 1);
      digitalWrite(colB, 1);
      digitalWrite(colC, 1);
      break;
    default:
      break;
  }
  //Serial.println(pos);
}

void comm(int pos) {
  switch (pos) {
    case 0:
      digitalWrite(commA, 0);
      digitalWrite(commB, 0);
      digitalWrite(commC, 0);
      digitalWrite(jacksIO, clockOut);
      break;
    case 1:
      digitalWrite(commA, 1);
      digitalWrite(commB, 0);
      digitalWrite(commC, 0);
      clockIn = digitalRead(jacksIO);
      break;
    case 2:
      digitalWrite(commA, 0);
      digitalWrite(commB, 1);
      digitalWrite(commC, 0);
      digitalWrite(jacksIO, resetOut);
      break;
    case 3:
      digitalWrite(commA, 1);
      digitalWrite(commB, 1);
      digitalWrite(commC, 0);
      resetIn = digitalRead(jacksIO);
      break;
    case 4:
      digitalWrite(commA, 0);
      digitalWrite(commB, 0);
      digitalWrite(commC, 1);
      animationIn = digitalRead(jacksIO);
      break;
    case 5:
      digitalWrite(commA, 1);
      digitalWrite(commB, 0);
      digitalWrite(commC, 1);
      coderButton = digitalRead(jacksIO);
      break;
    case 6:
      digitalWrite(commA, 0);
      digitalWrite(commB, 1);
      digitalWrite(commC, 1);
      break;
    case 7:
      digitalWrite(commA, 1);
      digitalWrite(commB, 1);
      digitalWrite(commC, 1);
      break;
    default:
      break;
  }
  menuButton[pos] = digitalRead(menuButtonMux);
}

void clockBar8x8(int localClock) {
  if (animation == 0) {
  int clockBar8x8step = localClock - ((localClock / 8) * 8);
    leds[0 + clockBar8x8step] = CRGB::Red;
    leds[15 - clockBar8x8step] = CRGB::Red;
    leds[16 + clockBar8x8step] = CRGB::Red;
    leds[31 - clockBar8x8step] = CRGB::Red;
    leds[32 + clockBar8x8step] = CRGB::Red;
    leds[47 - clockBar8x8step] = CRGB::Red;
    leds[48 + clockBar8x8step] = CRGB::Red;
    leds[63 - clockBar8x8step] = CRGB::Red;
  }
}

void clockGenerator(int bpm) {
  int timing = 60000 / bpm;
  long Cmill = millis();
  if (Cmill - Pmill >= timing){
    masterClock++;
    clockToggleTrigger = 1;
    Pmill = Cmill;
  }
}

void buttonRead(int pos, int reading) {
  switch (pos) {
    case 0:
      buttonMatrix[0] = reading;
      break;
    case 1:
      buttonMatrix[15] = reading;
      break;
    case 2:
      buttonMatrix[16] = reading;
      break;
    case 3:
      buttonMatrix[31] = reading;
      break;
    case 4:
      buttonMatrix[32] = reading;
      break;
    case 5:
      buttonMatrix[47] = reading;
      break;
    case 6:
      buttonMatrix[48] = reading;
      break;
    case 7:
      buttonMatrix[63] = reading;
      break;
    case 8:
      buttonMatrix[1] = reading;
      break;
    case 9:
      buttonMatrix[14] = reading;
      break;
    case 10:
      buttonMatrix[17] = reading;
      break;
    case 11:
      buttonMatrix[30] = reading;
      break;
    case 12:
      buttonMatrix[33] = reading;
      break;
    case 13:
      buttonMatrix[46] = reading;
      break;
    case 14:
      buttonMatrix[49] = reading;
      break;
    case 15:
      buttonMatrix[62] = reading;
      break;
    case 16:
      buttonMatrix[2] = reading;
      break;
    case 17:
      buttonMatrix[13] = reading;
      break;
    case 18:
      buttonMatrix[18] = reading;
      break;
    case 19:
      buttonMatrix[29] = reading;
      break;
    case 20:
      buttonMatrix[34] = reading;
      break;
    case 21:
      buttonMatrix[45] = reading;
      break;
    case 22:
      buttonMatrix[50] = reading;
      break;
    case 23:
      buttonMatrix[61] = reading;
      break;
    case 24:
      buttonMatrix[3] = reading;
      break;
    case 25:
      buttonMatrix[12] = reading;
      break;
    case 26:
      buttonMatrix[19] = reading;
      break;
    case 27:
      buttonMatrix[28] = reading;
      break;
    case 28:
      buttonMatrix[35] = reading;
      break;
    case 29:
      buttonMatrix[44] = reading;
      break;
    case 30:
      buttonMatrix[51] = reading;
      break;
    case 31:
      buttonMatrix[60] = reading;
      break;
    case 32:
      buttonMatrix[4] = reading;
      break;
    case 33:
      buttonMatrix[11] = reading;
      break;
    case 34:
      buttonMatrix[20] = reading;
      break;
    case 35:
      buttonMatrix[27] = reading;
      break;
    case 36:
      buttonMatrix[36] = reading;
      break;
    case 37:
      buttonMatrix[43] = reading;
      break;
    case 38:
      buttonMatrix[52] = reading;
      break;
    case 39:
      buttonMatrix[59] = reading;
      break;
    case 40:
      buttonMatrix[5] = reading;
      break;
    case 41:
      buttonMatrix[10] = reading;
      break;
    case 42:
      buttonMatrix[21] = reading;
      break;
    case 43:
      buttonMatrix[26] = reading;
      break;
    case 44:
      buttonMatrix[37] = reading;
      break;
    case 45:
      buttonMatrix[42] = reading;
      break;
    case 46:
      buttonMatrix[53] = reading;
      break;
    case 47:
      buttonMatrix[58] = reading;
      break;
    case 48:
      buttonMatrix[6] = reading;
      break;
    case 49:
      buttonMatrix[9] = reading;
      break;
    case 50:
      buttonMatrix[22] = reading;
      break;
    case 51:
      buttonMatrix[25] = reading;
      break;
    case 52:
      buttonMatrix[38] = reading;
      break;
    case 53:
      buttonMatrix[41] = reading;
      break;
    case 54:
      buttonMatrix[54] = reading;
      break;
    case 55:
      buttonMatrix[57] = reading;
      break;
    case 56:
      buttonMatrix[7] = reading;
      break;
    case 57:
      buttonMatrix[8] = reading;
      break;
    case 58:
      buttonMatrix[23] = reading;
      break;
    case 59:
      buttonMatrix[24] = reading;
      break;
    case 60:
      buttonMatrix[39] = reading;
      break;
    case 61:
      buttonMatrix[40] = reading;
      break;
    case 62:
      buttonMatrix[55] = reading;
      break;
    case 63:
      buttonMatrix[56] = reading;
      break;
  }
}

void write2gates(int linePos){
  int lineRead = linePos - ((linePos/ 8) * 8);
  bitWrite(gateState, 0, buttonMatrixToggleState1[0 + lineRead]);
  bitWrite(gateState, 1, buttonMatrixToggleState1[15 - lineRead]);
  bitWrite(gateState, 2, buttonMatrixToggleState1[16 + lineRead]);
  bitWrite(gateState, 3, buttonMatrixToggleState1[31 - lineRead]);
  bitWrite(gateState, 4, buttonMatrixToggleState1[32 + lineRead]);
  bitWrite(gateState, 5, buttonMatrixToggleState1[47 - lineRead]);
  bitWrite(gateState, 6, buttonMatrixToggleState1[48 + lineRead]);
  bitWrite(gateState, 7, buttonMatrixToggleState1[63 - lineRead]);
}

int gate2trigger(uint8_t gate) {
  long Cmill = millis();
  bool writingState;
  uint8_t triggerOut;
  
  if (clockToggleTrigger == 1) {
      bitWrite(triggerOut, 0, bitRead(gate, 0));
      bitWrite(triggerOut, 1, bitRead(gate, 1));
      bitWrite(triggerOut, 2, bitRead(gate, 2));
      bitWrite(triggerOut, 3, bitRead(gate, 3));
      bitWrite(triggerOut, 4, bitRead(gate, 4));
      bitWrite(triggerOut, 5, bitRead(gate, 5));
      bitWrite(triggerOut, 6, bitRead(gate, 6));
      bitWrite(triggerOut, 7, bitRead(gate, 7));
    PmillTrig = Cmill;
    clockToggleTrigger = 0;
    writingState = 1;
  }
  
  if (Cmill - PmillTrig >= triggerLenght && writingState == 1){
    bitWrite(triggerOut, 0, 0);
    bitWrite(triggerOut, 1, 0);
    bitWrite(triggerOut, 2, 0);
    bitWrite(triggerOut, 3, 0);
    bitWrite(triggerOut, 4, 0);
    bitWrite(triggerOut, 5, 0);
    bitWrite(triggerOut, 6, 0);
    bitWrite(triggerOut, 7, 0);
    writingState = 0;
  }
  return triggerOut;
}

void regwrite(uint8_t output) {
  digitalWrite(regL, LOW);
  for (int i = 7; i >= 0; i--) {
    digitalWrite(regC, LOW);
    digitalWrite(regD, bitRead(output, i));
    digitalWrite(regC, HIGH);
  }
  digitalWrite(regL, HIGH);
}

void showLed(int brightness) {
  long Cmill = millis();
  if (Cmill - PLEDmill >= 1000 / FPS) {
    FastLED.show();
    LEDS.setBrightness(brightness);
    PLEDmill = Cmill;
  }
}

void buttonConversion(){
  for (int i; i < 64; i++) {
      if (buttonMatrixToggleState1[i] == 1 && animation == 0) leds[i] = CRGB::Yellow;
      if (buttonMatrixToggleState1[i] == 0 && animation == 0) leds[i] = CRGB::Black;
      if (buttonMatrix[i] == 0 && buttonMatrixLast[i] == 1) {
        buttonMatrixToggleState1[i] = !buttonMatrixToggleState1[i];
        buttonMatrixLast[i] = 0;
        anyButtonPressed = 1;
      }
      if (buttonMatrix[i] == 1 && buttonMatrixLast[i] == 0) {
        buttonMatrixLast[i] = 1;
      }
    }
}

void confetti() {
  EVERY_N_MILLISECONDS( 10 ) {
  fadeToBlackBy( leds, 64, 10);
  int pos = random16(64);
  leds[pos] += CHSV( gHue + random8(32), 200, 255);
  }
  EVERY_N_MILLISECONDS( 50 ) { gHue++; }
}

void clockInput() {
  if (clockIn == 1 && clockInLast == 0) {
      masterClock++;
      clockToggleTrigger = 1;
      clockInLast = 1;
    }
    if (clockIn == 0 && clockInLast == 1) {
      clockInLast = 0;
    }
}

void setup() {
  pinMode(colA, OUTPUT);
  pinMode(colB, OUTPUT);
  pinMode(colC, OUTPUT);
  pinMode(commA, OUTPUT);
  pinMode(commB, OUTPUT);
  pinMode(commC, OUTPUT);
  pinMode(regD, OUTPUT);
  pinMode(regL, OUTPUT);
  pinMode(regC, OUTPUT);
  pinMode(buttonMatrixMux, INPUT);
  pinMode(gatesMux, OUTPUT);

  FastLED.addLeds<NEOPIXEL, 3>(leds, 72);

   //led_test_start---------------------
  LEDS.setBrightness(brightness + 20);
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
  FastLED.show();
  //led_test_end----------------------

  Serial.begin(2000000);
}

void loop() {
  if (counter > 63) counter = 0;
  comm(counter - ((counter / 8) * 8));
  col(counter/8);
  buttonRead(counter, digitalRead(buttonMatrixMux));
  counter++;

  regwrite(gate2trigger(gateState));
  //clockGenerator(120);
  clockInput();
  buttonConversion();
  clockBar8x8(masterClock);
  write2gates(masterClock);

  if (animation == 1) {
    confetti();
  }

  if(masterClock > 7) masterClock = 0;  
  showLed(map(analogRead(animationPot), 0, 1023, 0, 255));

  Serial.print(menuButton[0]);
  Serial.print(menuButton[1]);
  Serial.print(menuButton[2]);
  Serial.print(menuButton[3]);
  Serial.print(menuButton[4]);
  Serial.print(menuButton[5]);
  Serial.print(menuButton[6]);
  Serial.println(menuButton[7]);
}
