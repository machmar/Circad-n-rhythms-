// for pcb rev 2.0
#include <FastLED.h> //https://github.com/FastLED/FastLED
#include <EEPROM.h> //allready built in with arduino IDE

//arduino pin
#define encoderA 2
#define encoderB 11
#define encoderS 12
#define clockInPin 3
#define commA 4
#define commB 5
#define commC 6
#define buttonColOut 7
#define buttonRowPulse 8
#define ledPin 9
#define buttonMenuOut 10
#define resetIn 13
#define regL 14
#define regC 15
#define dacD 16
#define regD 17
#define buttonRowReset 18
#define anaMux 21
//shiftreg
#define dacCS 0
#define clockOut 1
#define resetOut 2
#define clockResetSwitch 3
#define dacLdac 5

uint8_t counter;
bool menuButton[9];
bool menuButtonLast[9];
bool buttonState[65];
bool buttonStateLast[65];
bool buttonMatrixToggleState[9][65];
uint8_t jackState;
uint8_t gateState;
uint8_t controlReg;
float temperatureSens;
uint16_t brightnessAdj;
uint16_t temperatureAdj;
uint16_t variable1Adj;
uint16_t variable2Adj;
uint8_t verticalBtnScanPrev;
uint8_t encoderMenu;
uint8_t encoderMenuClearLedsLast;
uint8_t buttonsDisplayPage;
uint8_t buttonsDisplayPageControl = 8;
uint8_t channelZoom = 8;
bool anyButtonPressed;
uint8_t sequenceLenght = 63;
int gHue;
bool clockInLast;
uint8_t masterClock;
bool clockToggleTrigger;
bool saveButtonLast[4];
bool loadButtonLast[4];
uint8_t savedTo;
uint8_t loadedFrom;
uint16_t triggerLenght = 70;

uint32_t PLEDmill;
uint32_t PmillTrig;
long CmillGlobal;

CRGB leds[72];

const uint8_t buttonMenu[9] {64, 65, 66, 67, 68, 69, 70 , 71};
const uint8_t ledMatrix[9][9] { //x(horizontal) ,y(vertical)
  {0, 15, 16, 31, 32, 47, 48, 63},
  {1, 14, 17, 30, 33, 46, 49, 62},
  {2, 13, 18, 29, 34, 45, 50, 61},
  {3, 12, 19, 28, 35, 44, 51, 60},
  {4, 11, 20, 27, 36, 43, 52, 59},
  {5, 10, 21, 26, 37, 42, 53, 58},
  {6, 9, 22, 25, 38, 41, 54, 57},
  {7, 8, 23, 24, 39, 40, 55, 56},
};
const uint8_t buttonMatrix[9][9] { //x(horizontal) ,y(vertical)
  {0 ,  8, 16, 24, 32, 40, 48, 56},
  {1 ,  9, 17, 25, 33, 41, 49, 57},
  {2 , 10, 18, 26, 34, 42, 50, 58},
  {3 , 11, 19, 27, 35, 43, 51, 59},
  {4 , 12, 20, 28, 36, 44, 52, 60},
  {5 , 13, 21, 29, 37, 45, 53, 61},
  {6 , 14, 22, 30, 38, 46, 54, 62},
  {7 , 15, 23, 31, 39, 47, 55, 63},
};
const uint64_t numbers1[] {
  6,//height
  3,//width
  0B010101101101101010,//0
  0B111010010010011010,//1
  0B111001010100101010,//2
  0B010101100010100010,//3
  0B100100100111101101,//4
  0B010101100011001111,//5
  0B010101101011001110,//6
  0B001001010100100111,//7
  0B010101101010101010,//8
  0B010101100110101010,//9
};
/*----------FONT-6x3-version-1----------
   --- 0 ---0 0--- 0 --- 0 ---   ---   ---  0--- 0 --- 0 --- 0 --- 0 ---0 0---000--- 00---000--- 0 --- 0 ---
   --- 0 ---  0---0  ---  0---   ---   ---  0---0 0---00 ---0 0---  0---0 0---0  ---0  ---  0---0 0---0 0---
   --- 0 --- 0 ---0  ---  0---   ---   --- 0 ---0 0--- 0 ---  0--- 0 ---000---00 ---00 ---  0--- 0 --- 00---
   --- 0 --- 0 ---0  ---  0---000---   --- 0 ---0 0--- 0 --- 0 ---  0---  0---  0---0 0--- 0 ---0 0---  0---
   ---   ---0  ---0  ---  0---   --- 00---0  ---0 0--- 0 ---0  ---0 0---  0---0 0---0 0---0  ---0 0---0 0---
   --- 0 ---0 0--- 0 --- 0 ---   --- 00---0  --- 0 ---000---000--- 0 ---  0--- 0 --- 0 ---0  --- 0 --- 0 ---
*/
const uint64_t numbers2[] {
  6,//height
  3,//width
  0B010101101101101010,//0
  0B111010010010011010,//1
  0B111001010100101010,//2
  0B011100100011100011,//3
  0B100100100111101101,//4
  0B011100100011001111,//5
  0B010101101011001110,//6
  0B001010010100100111,//7
  0B010101101010101010,//8
  0B011100110101101010,//9
  0B101101111101101010,//A
  0B101101101101111001,//N
};
/*----------FONT-6x3-version-2----------
   --- 0 ---0 0--- 00---00 ---   ---   ---  0--- 0 --- 0 --- 0 ---00 ---0 0---000--- 00---000--- 0 --- 0 --- 0 ---0  ---
   --- 0 ---  0---0  ---  0---   ---   ---  0---0 0---00 ---0 0---  0---0 0---0  ---0  ---  0---0 0---0 0---0 0---000---
   --- 0 --- 0 ---0  ---  0---   ---   --- 0 ---0 0--- 0 ---  0---00 ---000---00 ---00 ---  0--- 0 ---0 0---0 0---0 0---
   --- 0 --- 0 ---0  ---  0---000---   --- 0 ---0 0--- 0 --- 0 ---  0---  0---  0---0 0--- 0 ---0 0--- 00---000---0 0---
   ---   ---0  ---0  ---  0---   --- 00---0  ---0 0--- 0 ---0  ---  0---  0---  0---0 0--- 0 ---0 0---  0---0 0---0 0---
   --- 0 ---0 0--- 00---00 ---   --- 00---0  --- 0 ---000---000---00 ---  0---00 --- 0 ---0  --- 0 ---00 ---0 0---0 0---
*/
const uint64_t letters1[] {
  4,//height
  3,//width
  0B101111101010,//A
  0B111101111011,//B
  0B111001001111,//C
  0B011101101011,//D
  0B111001011111,//E
  0B001001011111,//F
  0B011101001111,//G
  0B101111101101,//H
  0B111010010111,//I
  0B010101100100,//J
  0B101101011101,//K
  0B111001001001,//L
  0B101101111111,//M
  0B101101101011,//N
  0B111101101111,//O
  0B001111101011,//P
  0B101011101011,//R
  0B111100111011,//S
  0B010010010111,//T
  0B110101101101,//U
  0B010101101101,//V
  0B111111101101,//W
  0B101101010101,//X
  0B010010010101,//Y
  0B111001100111,//Z
};
/*----------FONT-4x3-version-1----------
   --- 0 ---00 ---000---00 ---000---000---000---0 0---000---  0---0 0---0  ---000---00 ---000---00 ---00 ---00 ---000---0 0---0 0---0 0---0 0---0 0---000---
   ---0 0---000---0  ---0 0---00 ---00 ---0  ---0 0--- 0 ---  0---00 ---0  ---000---0 0---0 0---0 0---0 0---000--- 0 ---0 0---0 0---0 0--- 0 --- 0 ---  0---
   ---000---0 0---0  ---0 0---0  ---0  ---0 0---000--- 0 ---0 0---0 0---0  ---0 0---0 0---0 0---000---00 ---  0--- 0 ---0 0---0 0---000---0 0--- 0 ---0  ---
   ---0 0---000---000---00 ---000---0  ---00 ---0 0---000--- 0 ---0 0---000---0 0---0 0---000---0  ---0 0---000--- 0 --- 00--- 0 ---000---0 0--- 0 ---000---
*/
const uint64_t numbers3[] {
  8,//height
  8,//width
  0b0111111011111111111001111110011111100111111001111111111101111110,//0
  0b0011100000111000001110000011100000111000001111100011110000111000,//1
  0b1111111111111111000001110001110001110000111001111111111101111110,//2
  0b0111111111111111111000000111111101111111111000001111111101111111,//3
  0b0111000001110000011100001111111111111111011101110111011101110111,//4
  0b0111111011111111111000001111111101111111000001111111111111111111,//5
  0b0111111011111111111001111111111101111111000001111111111101111110,//6
  0b0001110000011100000111000011100001110000111000001111111111111111,//7
  0b0111111011111111111001110111111001111110111001111111111101111110,//8
  0b0111111011111111111000001111111011111111111001111111111101111110,//9
  0b1110011111100111111111111111111111100111111001111111111101111110,//A
  0b1110011111110111111111111111111111111111111011111110111111100111,//N
};
/*----------FONT-8x8-version-1----------
 * --- 000000 ---   000  --- 000000 ---0000000 ---000 000 ---00000000--- 000000 ---00000000--- 000000 --- 000000 --- 000000 ---000  000
 * ---00000000---  0000  ---00000000---00000000---000 000 ---00000000---00000000---00000000---00000000---00000000---00000000---0000 000
 * ---000  000--- 00000  ---000  000---     000---000 000 ---000     ---000     ---     000---000  000---000  000---000  000---0000 000
 * ---000  000---   000  ---    000 ---0000000 ---00000000---0000000 ---0000000 ---    000 --- 000000 ---00000000---000  000---00000000
 * ---000  000---   000  ---  000   ---0000000 ---00000000---00000000---00000000---   000  --- 000000 --- 0000000---00000000---00000000
 * ---000  000---   000  ---000     ---     000---    000 ---     000---000  000---  000   ---000  000---     000---00000000---00000000
 * ---00000000---   000  ---00000000---00000000---    000 ---00000000---00000000---  000   ---00000000---00000000---000  000---000 0000
 * --- 000000 ---   000  ---00000000---0000000 ---    000 --- 000000 --- 000000 ---  000   --- 000000 --- 000000 ---000  000---000  000
 */

void fadeall() {
  for (int i = 0; i < 72; i++) {
    leds[i].nscale8(250);
  }
}

void setChars(uint64_t font[], uint8_t input, uint8_t xOrigin, uint8_t yOrigin, uint8_t hue, uint8_t saturation, uint8_t value) {
  uint8_t width = font[1];
  for (uint8_t i = 0; i < font[0] * width; i++) {
    if (bitRead(font[input + 2], i) == 1) leds[ledMatrix[(i - ((i / width) * width)) + xOrigin][(i / width) + yOrigin]] = CHSV(hue, saturation, value);
    if (bitRead(font[input + 2], i) == 0) leds[ledMatrix[(i - ((i / width) * width)) + xOrigin][(i / width) + yOrigin]] = CHSV(0, 0, 0);
  }
}

void setNumber(uint32_t font, uint8_t input, uint8_t xOrigin, uint8_t yOrigin, uint8_t spacing, uint8_t hue, uint8_t saturation, uint8_t value) {
  if (/*input > 10 && */input < 99 ) {
    setChars(font, input / 10, xOrigin, yOrigin, hue, saturation, value);
    setChars(font, input - (input / 10) * 10, xOrigin + spacing, yOrigin, hue, saturation, value);
  }
}

void setRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t hue, uint8_t saturation, uint8_t value) {
  for (uint8_t Y = 0; Y < height; Y++) {
    for (uint8_t X = 0; X < width; X++) {
      leds[ledMatrix[X + x][Y + y]] = CHSV(hue, saturation, value);
    }
  }
}

bool readRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  bool out = 0;
  for (uint8_t i = 0; i < width * height; i++) {
    if ((buttonState[buttonMatrix[x + (i - ((i / width) * width))][y + i / width]]) == 1) {
      out = 1;
      break;
    }
  }
  return out;
}

void showLed(int brightness, int fps) {
  uint32_t Cmill = millis();
  if (Cmill - PLEDmill >= 1000 / fps) {
    FastLED.show();
    LEDS.setBrightness(brightness);
    PLEDmill = Cmill;
  }
}

void comm(uint8_t pos) {
  digitalWrite(commA, bitRead(pos, 0));
  digitalWrite(commB, bitRead(pos, 1));
  digitalWrite(commC, bitRead(pos, 2));
  switch (pos) {
    case 0:
      temperatureSens = map(analogRead(anaMux), 0, 1023, -50, 150);
      break;
    case 1:
      brightnessAdj = map(analogRead(anaMux), 0, 1023, 0, 50);
      break;
    case 2:
      temperatureAdj = analogRead(anaMux);
      break;
    case 3:
      variable1Adj = analogRead(anaMux);
      break;
    case 4:
      variable2Adj = analogRead(anaMux);
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
  }
  menuButton[map(pos, 0, 7, 7, 0)] = !digitalRead(buttonMenuOut);
}

void clockBar(int localClock) {
  int clockBarstep = localClock - ((localClock / 8) * 8); //deals with the value being bigger than 7
  if (channelZoom == 8) {
    switch (localClock / 8) {
      case 0:
        leds[ledMatrix[clockBarstep][0]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][1]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][2]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][3]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][4]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][5]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][6]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][7]] = CRGB::Red;
        break;
      case 1:
        leds[ledMatrix[clockBarstep][0]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][1]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][2]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][3]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][4]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][5]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][6]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][7]] = CRGB::Red;
        break;
      case 2:
        leds[ledMatrix[clockBarstep][0]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][1]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][2]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][3]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][4]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][5]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][6]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][7]] = CRGB::Red;
        break;
      case 3:
        leds[ledMatrix[clockBarstep][0]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][1]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][2]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][3]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][4]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][5]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][6]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][7]] = CRGB::Red;
        break;
      case 4:
        leds[ledMatrix[clockBarstep][0]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][1]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][2]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][3]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][4]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][5]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][6]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][7]] = CRGB::Red;
        break;
      case 5:
        leds[ledMatrix[clockBarstep][0]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][1]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][2]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][3]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][4]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][5]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][6]] = CRGB::Red;
        leds[ledMatrix[clockBarstep][7]] = CRGB::Red;
        break;
      case 6:
        leds[ledMatrix[clockBarstep][0]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][1]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][2]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][3]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][4]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][5]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][6]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][7]] = CRGB::Red;
        break;
      case 7:
        leds[ledMatrix[clockBarstep][0]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][1]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][2]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][3]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][4]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][5]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][6]] = CRGB::Blue;
        leds[ledMatrix[clockBarstep][7]] = CRGB::Blue;
        break;
    }
  }
  if (channelZoom != 8) {
    leds[ledMatrix[localClock - ((localClock / 8) * 8)][localClock / 8]] = CRGB::Red;
  }
}

void buttonRead(uint8_t pos) {
  buttonState[buttonMatrix[pos / 8][map(pos - ((pos / 8) * 8), 0, 7, 7, 0)]] = digitalRead(buttonColOut);
}

void write2gates(int linePos) {
  for (int z = 0; z < 8; z++) {
    bitWrite(gateState, z, buttonMatrixToggleState[z][linePos]);
  }
}

uint8_t gate2trigger(uint8_t gate) {
  long Cmill = millis();
  bool writingState;
  uint8_t triggerOut;

  if (clockToggleTrigger == 1) {
    triggerOut = gate;
    PmillTrig = Cmill;
    clockToggleTrigger = 0;
    writingState = 1;
  }

  if (Cmill - PmillTrig >= triggerLenght && writingState == 1) {
    triggerOut = 0;
    writingState = 0;
  }
  return triggerOut;
}

void gate2triggerWithWrite(uint8_t gate) {
  long Cmill = millis();
  bool writingState;

  if (clockToggleTrigger == 1) {
    jackState = gate;
    PmillTrig = Cmill;
    clockToggleTrigger = 0;
    writingState = 1;
    shiftRegRefresh();
  }

  if (Cmill - PmillTrig >= triggerLenght && writingState == 1) {
    jackState = 0;
    writingState = 0;
    shiftRegRefresh();
  }
}

/*int gate2trigger(uint8_t gate) {
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

  if (Cmill - PmillTrig >= triggerLenght && writingState == 1) {
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
}*/

void col(uint8_t pos) {
  if (pos != verticalBtnScanPrev) {
    verticalBtnScanPrev = pos;
    if (pos == 0) {
      digitalWrite(18, 1);
      digitalWrite(18, 0);
    }
    digitalWrite(buttonRowPulse, 1);
    digitalWrite(buttonRowPulse, 0);
  }
}

void shiftRegRefresh() {
  digitalWrite(regL, 0);
  shiftOut(regD, regC, MSBFIRST, jackState);
  shiftOut(regD, regC, MSBFIRST, controlReg);
  digitalWrite(regL, 1);
}

void buttonConversion(uint8_t page) {
  if (channelZoom == 8) {
    for (int i = 0; i < 64; i++) {
      if (buttonMatrixToggleState[i - ((i / 8) * 8)][i / 8 + (page * 8)] == 1) leds[ledMatrix[i / 8][i - ((i / 8) * 8)]] = CRGB::Green;
      if (buttonMatrixToggleState[i - ((i / 8) * 8)][i / 8 + (page * 8)] == 0) leds[ledMatrix[i / 8] [i - ((i / 8) * 8)]] = CRGB::Black;
      if (buttonState[buttonMatrix[i - ((i / 8) * 8)][i / 8]] == 1 && buttonStateLast[buttonMatrix[i - ((i / 8) * 8)][i / 8]] == 0) {
        buttonMatrixToggleState[i - ((i / 8) * 8)][i / 8 + (page * 8)] = !buttonMatrixToggleState[i - ((i / 8) * 8)][i / 8 + (page * 8)];// swoped array adresses
        buttonStateLast[buttonMatrix[i - ((i / 8) * 8)][i / 8]] = 1;
        anyButtonPressed = 1;
      }
      if (buttonState[buttonMatrix[i - ((i / 8) * 8)][i / 8]] == 0 && buttonStateLast[buttonMatrix[i - ((i / 8) * 8)][i / 8]] == 1) {
        buttonStateLast[buttonMatrix[i - ((i / 8) * 8)][i / 8]] = 0;
      }
    }
  }
  if (channelZoom != 8) {
    for (int i = 0; i < 64; i++) {
      if (buttonMatrixToggleState[channelZoom][i] == 1) leds[ledMatrix[i - ((i / 8) * 8)][i / 8]] = CRGB::Green;
      if (buttonMatrixToggleState[channelZoom][i] == 0) leds[ledMatrix[i - ((i / 8) * 8)][i / 8]] = CRGB::Black;
      if (buttonState[buttonMatrix[i / 8][i - ((i / 8) * 8)]] == 1 && buttonStateLast[buttonMatrix[i / 8][i - ((i / 8) * 8)]] == 0) {
        buttonMatrixToggleState[channelZoom][i] = !buttonMatrixToggleState[channelZoom][i];
        buttonStateLast[buttonMatrix[i / 8][i - ((i / 8) * 8)]] = 1;
        anyButtonPressed = 1;
      }
      if (buttonState[buttonMatrix[i / 8][i - ((i / 8) * 8)]] == 0 && buttonStateLast[buttonMatrix[i / 8][i - ((i / 8) * 8)]] == 1) {
        buttonStateLast[buttonMatrix[i / 8][i - ((i / 8) * 8)]] = 0;
      }
    }
  }
}

void save(uint8_t channel, uint8_t preset) {
  uint8_t memPos = (channel * 8) + (preset * 64);
  uint8_t array2var;
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t y = 0; y < 8; y++) {
      bitWrite(array2var, y, buttonMatrixToggleState[channel][y + (i * 8)]);
    }
    EEPROM.write(memPos + i, array2var);
  }
  EEPROM.write(256 + (preset * 3), channelZoom);
  EEPROM.write(257 + (preset * 3), buttonsDisplayPageControl);
  EEPROM.write(258 + (preset * 3), sequenceLenght);
}

void load(uint8_t channel, uint8_t preset) {
  uint8_t memPos = (channel * 8) + (preset * 64);
  uint8_t var2array;
  for (uint8_t i = 0; i < 8; i++) {
    var2array = EEPROM.read(memPos + i);
    for (uint8_t y = 0; y < 8; y++) {
      buttonMatrixToggleState[channel][y + (i * 8)] = bitRead(var2array, y);
    }
  }
  channelZoom = EEPROM.read(256 + (preset * 3));
  buttonsDisplayPageControl = EEPROM.read(257 + (preset * 3));
  sequenceLenght = EEPROM.read(258 + (preset * 3));
}

void confetti() {
  EVERY_N_MILLISECONDS( 10 ) {
    fadeToBlackBy( leds, 64, 10);
    int pos = random16(64);
    leds[pos] += CHSV( gHue + random8(32), 200, 255);
  }
  EVERY_N_MILLISECONDS( 50 ) {
    gHue++;
  }
}

void clockInterrupt() {
  masterClock++;
  write2gates(masterClock);
  clockToggleTrigger = 1;
  gate2triggerWithWrite(gateState);
}

void controlButtons() {
  if (menuButton[0] == 1 && menuButton[0] != menuButtonLast[0]) {
    if (encoderMenu != 1) encoderMenu = 1;
    else encoderMenu = 0;
    menuButtonLast[0] = menuButton[0];
  }
  if (menuButton[1] == 1 && menuButton[1] != menuButtonLast[1]) {
    if (encoderMenu != 2) encoderMenu = 2;
    else encoderMenu = 0;
    menuButtonLast[1] = menuButton[1];
  }
  if (menuButton[5] == 1 && menuButton[5] != menuButtonLast[5]) {
    if (encoderMenu != 6) encoderMenu = 6;
    else encoderMenu = 0;
    menuButtonLast[5] = menuButton[5];
  }
  if (menuButton[6] == 1 && menuButton[6] != menuButtonLast[6]) {
    if (encoderMenu != 7) encoderMenu = 7;
    else encoderMenu = 0;
    menuButtonLast[6] = menuButton[6];
  }
  if (menuButton[7] == 1 && menuButton[7] != menuButtonLast[7]) {
    if (encoderMenu != 8) encoderMenu = 8;
    else encoderMenu = 0;
    menuButtonLast[7] = menuButton[7];
  }
  for (uint8_t i = 0; i < 8; i++) {
    if (menuButton[i] == 0 && menuButton[i] != menuButtonLast[i])menuButtonLast[i] = menuButton[i];
  }
  switch (encoderMenu) {
    case 6:
      if (readRectangle(0, 0, 4, 3) != loadButtonLast[0] && readRectangle(0, 0, 4, 3) == 0) { //A preset
        for (uint8_t i = 0; i < 8; i++) {
          load(i, 0);
        }
        encoderMenu = 0;
        loadedFrom = 0;
        loadButtonLast[0] = readRectangle(0, 0, 4, 3);
      }
      if (readRectangle(0, 4, 4, 3) != loadButtonLast[2] && readRectangle(0, 4, 4, 3) == 0) { //B preset
        for (uint8_t i = 0; i < 8; i++) {
          load(i, 1);
        }
        encoderMenu = 0;
        loadedFrom = 1;
        loadButtonLast[2] = readRectangle(0, 4, 4, 3);
      }
      if (readRectangle(4, 0, 4, 3) != loadButtonLast[1] && readRectangle(4, 0, 4, 3) == 0) { //C preset
        for (uint8_t i = 0; i < 8; i++) {
          load(i, 2);
        }
        encoderMenu = 0;
        loadedFrom = 2;
        loadButtonLast[1] = readRectangle(4, 0, 4, 3);
      }
      if (readRectangle(4, 4, 4, 3) != loadButtonLast[3] && readRectangle(4, 4, 4, 3) == 0) { //D preset
        for (uint8_t i = 0; i < 8; i++) {
          load(i, 3);
        }
        encoderMenu = 0;
        loadedFrom = 3;
        loadButtonLast[3] = readRectangle(4, 4, 4, 3);
      }
      if (readRectangle(0, 0, 4, 3) != loadButtonLast[0] && readRectangle(0, 0, 4, 3) == 1) loadButtonLast[0] = readRectangle(0, 0, 4, 3);
      if (readRectangle(4, 0, 4, 3) != loadButtonLast[1] && readRectangle(4, 0, 4, 3) == 1) loadButtonLast[1] = readRectangle(4, 0, 4, 3);
      if (readRectangle(0, 4, 4, 3) != loadButtonLast[2] && readRectangle(0, 4, 4, 3) == 1) loadButtonLast[2] = readRectangle(0, 4, 4, 3);
      if (readRectangle(4, 4, 4, 3) != loadButtonLast[3] && readRectangle(4, 4, 4, 3) == 1) loadButtonLast[3] = readRectangle(4, 4, 4, 3);
      break;
    case 7:
      if (readRectangle(0, 0, 4, 3) != saveButtonLast[0] && readRectangle(0, 0, 4, 3) == 0) { //A preset
        for (uint8_t i = 0; i < 8; i++) {
          save(i, 0);
        }
        encoderMenu = 0;
        savedTo = 0;
        saveButtonLast[0] = readRectangle(0, 0, 4, 3);
      }
      if (readRectangle(0, 4, 4, 3) != saveButtonLast[2] && readRectangle(0, 4, 4, 3) == 0) { //B preset
        for (uint8_t i = 0; i < 8; i++) {
          save(i, 1);
        }
        load(0, 0);
        encoderMenu = 0;
        savedTo = 1;
        saveButtonLast[2] = readRectangle(0, 4, 4, 3);
      }
      if (readRectangle(4, 0, 4, 3) != saveButtonLast[1] && readRectangle(4, 0, 4, 3) == 0) { //C preset
        for (uint8_t i = 0; i < 8; i++) {
          save(i, 2);
        }
        encoderMenu = 0;
        savedTo = 2;
        saveButtonLast[1] = readRectangle(4, 0, 4, 3);
      }
      if (readRectangle(4, 4, 4, 3) != saveButtonLast[3] && readRectangle(4, 4, 4, 3) == 0) { //D preset
        for (uint8_t i = 0; i < 8; i++) {
          save(i, 3);
        }
        encoderMenu = 0;
        savedTo = 3;
        saveButtonLast[3] = readRectangle(4, 4, 4, 3);
      }
      if (readRectangle(0, 0, 4, 3) != saveButtonLast[0] && readRectangle(0, 0, 4, 3) == 1) saveButtonLast[0] = readRectangle(0, 0, 4, 3);
      if (readRectangle(4, 0, 4, 3) != saveButtonLast[1] && readRectangle(4, 0, 4, 3) == 1) saveButtonLast[1] = readRectangle(4, 0, 4, 3);
      if (readRectangle(0, 4, 4, 3) != saveButtonLast[2] && readRectangle(0, 4, 4, 3) == 1) saveButtonLast[2] = readRectangle(0, 4, 4, 3);
      if (readRectangle(4, 4, 4, 3) != saveButtonLast[3] && readRectangle(4, 4, 4, 3) == 1) saveButtonLast[3] = readRectangle(4, 4, 4, 3);
      break;
  }
}

uint8_t selectBrightness(uint8_t value, uint8_t light, uint8_t dimValue, uint8_t lightValue) {
  uint8_t out;
  if (value != light) out = dimValue;
  if (value == light) out = lightValue;
  return out;
}

void controlLeds() {
  leds[buttonMenu[0]] = CHSV(map(sequenceLenght, 0, 63, 0, 215), 255, selectBrightness(encoderMenu, 1, 150, 255));
  leds[buttonMenu[1]] = CHSV(map(buttonsDisplayPageControl, 0, 8, 0, 215), 255, selectBrightness(encoderMenu, 2, 150, 255));
  leds[buttonMenu[2]] = CHSV(215, 255, selectBrightness(encoderMenu, 3, 150, 255));
  leds[buttonMenu[3]] = CHSV(215, 255, selectBrightness(encoderMenu, 4, 150, 255));
  leds[buttonMenu[4]] = CHSV(215, 255, selectBrightness(encoderMenu, 5, 150, 255));
  leds[buttonMenu[5]] = CHSV(map(loadedFrom, 3, 0, 0, 215), 255, selectBrightness(encoderMenu, 6, 150, 255));
  leds[buttonMenu[6]] = CHSV(map(savedTo, 3, 0, 0, 215), 255, selectBrightness(encoderMenu, 7, 150, 255));
  leds[buttonMenu[7]] = CHSV(map(channelZoom, 0, 8, 0, 215), 255, selectBrightness(encoderMenu, 8, 150, 255));

  if (encoderMenu != encoderMenuClearLedsLast) {
    for (uint8_t i = 0; i < 64; i++) {
      leds[i] = CRGB::Black;
    }
    encoderMenuClearLedsLast = encoderMenu;
  }
  switch (encoderMenu) {
    case 0:
      buttonConversion(buttonsDisplayPage);
      if (masterClock / 8 == buttonsDisplayPage && channelZoom == 8) clockBar(masterClock);
      if (channelZoom != 8) clockBar(masterClock);
      if (buttonsDisplayPageControl == 8) buttonsDisplayPage = masterClock / 8; //moved from void controlLeds
      break;
    case 1:
      setNumber(numbers2, sequenceLenght + 1, 1, 1, 4, map(sequenceLenght, 0, 63, 0, 215), 255, 255);
      break;
    case 2:
      switch (buttonsDisplayPageControl) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          setChars(numbers3, buttonsDisplayPageControl + 1, 0, 0, map(buttonsDisplayPageControl, 0, 8, 0, 215), 255, 255);
          buttonsDisplayPage = buttonsDisplayPageControl;
          break;
        case 8:
          setChars(numbers3, 10, 0, 0, map(buttonsDisplayPageControl, 0, 8, 0, 215), 255, 255);
          break;
      }
      break;
    case 7:
    case 6:
      setChars(letters1, 0, 0, 0, map(0, 3, 0, 0, 215), 255, 255);
      setChars(letters1, 1, 4, 0, map(1, 3, 0, 0, 215), 255, 255);
      setChars(letters1, 2, 0, 4, map(2, 3, 0, 0, 215), 255, 255);
      setChars(letters1, 3, 4, 4, map(3, 3, 0, 0, 215), 255, 255);
      break;
    case 8:
      switch (channelZoom) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          setChars(numbers3, channelZoom + 1, 0, 0, map(channelZoom, 0, 8, 0, 215), 255, 255);
          break;
        case 8:
          setChars(numbers3, 11, 0, 0, map(channelZoom, 0, 8, 0, 215), 255, 255);
          break;
      }
      break;
  }
}

void rotate() {
  bool encoderAState = digitalRead(encoderA);
  bool encoderBState = digitalRead(encoderB);
  switch (encoderMenu) {
    case 1:
      if (encoderAState == 0 && encoderBState == 1) {
        if (sequenceLenght < 63) sequenceLenght++;
      }
      else if (encoderAState == 0 && encoderBState == 0) {
        if (sequenceLenght > 0) sequenceLenght--;
      }
      break;
    case 2:
      if (encoderAState == 0 && encoderBState == 1) {
        if (buttonsDisplayPageControl < 8) buttonsDisplayPageControl++;
      }
      else if (encoderAState == 0 && encoderBState == 0) {
        if (buttonsDisplayPageControl > 0) buttonsDisplayPageControl--;
      }
      break;
    case 8:
      if (encoderAState == 0 && encoderBState == 1) {
        if (channelZoom < 8) channelZoom++;
      }
      else if (encoderAState == 0 && encoderBState == 0) {
        if (channelZoom > 0) channelZoom--;
      }
      break;
  }
}

void setup() {
  FastLED.addLeds<NEOPIXEL, ledPin>(leds, 72);
  pinMode(buttonMenuOut, INPUT);
  pinMode(buttonColOut, INPUT);
  pinMode(buttonRowPulse, OUTPUT);
  pinMode(commA, OUTPUT);
  pinMode(commB, OUTPUT);
  pinMode(commC, OUTPUT);
  pinMode(regL, OUTPUT);
  pinMode(regC, OUTPUT);
  pinMode(regD, OUTPUT);
  pinMode(dacD, OUTPUT);
  pinMode(clockInPin, INPUT);
  pinMode(buttonRowReset, OUTPUT);
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderA), rotate, CHANGE);
  attachInterrupt(digitalPinToInterrupt(clockInPin), clockInterrupt, RISING);
  digitalWrite(buttonRowReset, 1);
  digitalWrite(buttonRowReset, 0);

  //led_test_start---------------------
  LEDS.setBrightness(80);
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
  leds[buttonMenu[7]] = CRGB::Magenta; //215 hue value when HSV used
  LEDS.setBrightness(28);
  FastLED.show();
  delay(10);
  leds[buttonMenu[6]] = CRGB::Magenta;
  LEDS.setBrightness(56);
  FastLED.show();
  delay(10);
  leds[buttonMenu[5]] = CRGB::Magenta;
  LEDS.setBrightness(84);
  FastLED.show();
  delay(10);
  leds[buttonMenu[4]] = CRGB::Magenta;
  LEDS.setBrightness(112);
  FastLED.show();
  delay(10);
  leds[buttonMenu[3]] = CRGB::Magenta;
  LEDS.setBrightness(140);
  FastLED.show();
  delay(10);
  leds[buttonMenu[2]] = CRGB::Magenta;
  LEDS.setBrightness(169);
  FastLED.show();
  delay(10);
  leds[buttonMenu[1]] = CRGB::Magenta;
  LEDS.setBrightness(197);
  FastLED.show();
  delay(10);
  leds[buttonMenu[0]] = CRGB::Magenta;
  LEDS.setBrightness(225);
  FastLED.show();
  delay(50);
  LEDS.setBrightness(brightnessAdj);
  FastLED.show();
  //led_test_end----------------------
}

void loop() {
  CmillGlobal = millis();
  if (counter > 63) counter = 0;
  if (masterClock > sequenceLenght) masterClock = 0;
  comm(counter - ((counter / 8) * 8));
  col(counter / 8);
  buttonRead(counter);
  counter++;
  controlButtons();
  controlLeds();
  gate2triggerWithWrite(gateState);
  shiftRegRefresh();
  showLed(brightnessAdj, 30);
}
