 
#include <FastLED.h>
#include <EEPROM.h>

#define colA 13
#define colB 17
#define colC 16
#define commA 10
#define commB 11
#define commC 12
#define buttonMatrixMux 9
#define gatesMux 6
#define jacksIO 15
#define menuButtonMux 4
#define ledPin 3
#define swingPot 20
#define animationPot 21
#define regD 19
#define regL 18
#define regC 14
#define encoderA 2
#define encoderB 5


bool buttonMatrix[65];
bool buttonMatrixLast[65];
bool buttonMatrixToggleState[9][65];
int counter;
uint8_t gateState;
bool clockOut;
bool clockIn;
bool resetOut;
bool resetIn;
bool animationIn;
bool coderButton;
bool menuButton[9];
bool menuButtonLast[9];
int animation = 0;
uint8_t sequenceLenght = 63;

int brightness = 20;
#define FPS 30

int colPos;
int commPos;
bool clockInLast;
int masterClock;
uint8_t masterClockLast;
bool clockToggleTrigger;
bool anyButtonPressed;
int triggerLenght = 30;//ms
int gHue;
uint8_t encoderMenu;
uint8_t encoderMenuClearLedsLast;
uint8_t buttonsDisplayPage;
uint8_t buttonsDisplayPageControl = 8;
uint8_t channelZoom = 8;
bool saveButtonLast[4];
bool loadButtonLast[4];
uint8_t savedTo;
uint8_t loadedFrom;

int i;

long Pmill;
long PmillTrig;
long PLEDmill;

CRGB leds[72];

const uint8_t control[9] {66, 67, 68, 69, 70, 65, 71, 64};
const uint8_t matrix[9][9] { //x(horizontal) ,y(vertical)
  {0, 15, 16, 31, 32, 47, 48, 63},
  {1, 14, 17, 30, 33, 46, 49, 62},
  {2, 13, 18, 29, 34, 45, 50, 61},
  {3, 12, 19, 28, 35, 44, 51, 60},
  {4, 11, 20, 27, 36, 43, 52, 59},
  {5, 10, 21, 26, 37, 42, 53, 58},
  {6, 9, 22, 25, 38, 41, 54, 57},
  {7, 8, 23, 24, 39, 40, 55, 56},
};
const uint8_t buttonMatrixAdress[9][9] { //x(horizontal) ,y(vertical)
  {0, 1, 2, 3, 4, 5, 6, 7},
  {8, 9, 10, 11, 12, 13, 14, 15},
  {16, 17, 18, 19, 20, 21, 22, 23},
  {24, 25, 26, 27, 28, 29, 30, 31},
  {32, 33, 34, 35, 36, 37, 38, 39},
  {40, 41, 42, 43, 44, 45, 46, 47},
  {48, 49, 50, 51, 52, 53, 54, 55},
  {56, 57, 58, 59, 60, 61, 62, 63},
};
const uint32_t numbers1[13] {
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
const uint32_t numbers2[14] {
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
const uint32_t letters1[29] {
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

void fadeall() {
  for (int i = 0; i < 72; i++) {
    leds[i].nscale8(250);
  }
}

void setChars(uint32_t font[], uint8_t input, uint8_t xOrigin, uint8_t yOrigin, uint8_t hue, uint8_t saturation, uint8_t value) {
  for (uint8_t i = 0; i < font[0] * font[1]; i++) {
    if (bitRead(font[input + 2], i) == 1) leds[matrix[(i - ((i / font[1]) * font[1])) + xOrigin][(i / font[1]) + yOrigin]] = CHSV(hue, saturation, value);
    if (bitRead(font[input + 2], i) == 0) leds[matrix[(i - ((i / font[1]) * font[1])) + xOrigin][(i / font[1]) + yOrigin]] = CHSV(0, 0, 0);
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
      leds[matrix[X + x][Y + y]] = CHSV(hue, saturation, value);
    }
  }
}

bool readRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  bool out = 0;
  for (uint8_t i = 0; i < width * height; i++) {
    if ((buttonMatrix[buttonMatrixAdress[x + (i - ((i / width) * width))][y + i / width]]) == 1) {
      out = 1;
      break;
    }
  }
  return out;
}

void col(int pos) {
  digitalWrite(colA, bitRead(pos, 0));
  digitalWrite(colB, bitRead(pos, 1));
  digitalWrite(colC, bitRead(pos, 2));
}

void comm(int pos) {
  digitalWrite(commA, bitRead(pos, 0));
  digitalWrite(commB, bitRead(pos, 1));
  digitalWrite(commC, bitRead(pos, 2));
  switch (pos) {
    case 0:
      digitalWrite(jacksIO, clockOut);
      break;
    case 1:
      clockIn = digitalRead(jacksIO);
      break;
    case 2:
      digitalWrite(jacksIO, resetOut);
      break;
    case 3:
      resetIn = digitalRead(jacksIO);
      break;
    case 4:
      animationIn = digitalRead(jacksIO);
      break;
    case 5:
      coderButton = digitalRead(jacksIO);
      break;
  }
  menuButton[pos] = digitalRead(menuButtonMux);
}

void clockBar(int localClock) {
  int clockBarstep = localClock - ((localClock / 8) * 8); //deals with the value being bigger than 7
  if (channelZoom == 8) {
  switch (localClock / 8) {
    case 0:
      leds[matrix[clockBarstep][0]] = CRGB::Blue;
      leds[matrix[clockBarstep][1]] = CRGB::Red;
      leds[matrix[clockBarstep][2]] = CRGB::Red;
      leds[matrix[clockBarstep][3]] = CRGB::Red;
      leds[matrix[clockBarstep][4]] = CRGB::Red;
      leds[matrix[clockBarstep][5]] = CRGB::Red;
      leds[matrix[clockBarstep][6]] = CRGB::Red;
      leds[matrix[clockBarstep][7]] = CRGB::Red;
      break;
    case 1:
      leds[matrix[clockBarstep][0]] = CRGB::Blue;
      leds[matrix[clockBarstep][1]] = CRGB::Blue;
      leds[matrix[clockBarstep][2]] = CRGB::Red;
      leds[matrix[clockBarstep][3]] = CRGB::Red;
      leds[matrix[clockBarstep][4]] = CRGB::Red;
      leds[matrix[clockBarstep][5]] = CRGB::Red;
      leds[matrix[clockBarstep][6]] = CRGB::Red;
      leds[matrix[clockBarstep][7]] = CRGB::Red;
      break;
    case 2:
      leds[matrix[clockBarstep][0]] = CRGB::Blue;
      leds[matrix[clockBarstep][1]] = CRGB::Blue;
      leds[matrix[clockBarstep][2]] = CRGB::Blue;
      leds[matrix[clockBarstep][3]] = CRGB::Red;
      leds[matrix[clockBarstep][4]] = CRGB::Red;
      leds[matrix[clockBarstep][5]] = CRGB::Red;
      leds[matrix[clockBarstep][6]] = CRGB::Red;
      leds[matrix[clockBarstep][7]] = CRGB::Red;
      break;
    case 3:
      leds[matrix[clockBarstep][0]] = CRGB::Blue;
      leds[matrix[clockBarstep][1]] = CRGB::Blue;
      leds[matrix[clockBarstep][2]] = CRGB::Blue;
      leds[matrix[clockBarstep][3]] = CRGB::Blue;
      leds[matrix[clockBarstep][4]] = CRGB::Red;
      leds[matrix[clockBarstep][5]] = CRGB::Red;
      leds[matrix[clockBarstep][6]] = CRGB::Red;
      leds[matrix[clockBarstep][7]] = CRGB::Red;
      break;
    case 4:
      leds[matrix[clockBarstep][0]] = CRGB::Blue;
      leds[matrix[clockBarstep][1]] = CRGB::Blue;
      leds[matrix[clockBarstep][2]] = CRGB::Blue;
      leds[matrix[clockBarstep][3]] = CRGB::Blue;
      leds[matrix[clockBarstep][4]] = CRGB::Blue;
      leds[matrix[clockBarstep][5]] = CRGB::Red;
      leds[matrix[clockBarstep][6]] = CRGB::Red;
      leds[matrix[clockBarstep][7]] = CRGB::Red;
      break;
    case 5:
      leds[matrix[clockBarstep][0]] = CRGB::Blue;
      leds[matrix[clockBarstep][1]] = CRGB::Blue;
      leds[matrix[clockBarstep][2]] = CRGB::Blue;
      leds[matrix[clockBarstep][3]] = CRGB::Blue;
      leds[matrix[clockBarstep][4]] = CRGB::Blue;
      leds[matrix[clockBarstep][5]] = CRGB::Blue;
      leds[matrix[clockBarstep][6]] = CRGB::Red;
      leds[matrix[clockBarstep][7]] = CRGB::Red;
      break;
    case 6:
      leds[matrix[clockBarstep][0]] = CRGB::Blue;
      leds[matrix[clockBarstep][1]] = CRGB::Blue;
      leds[matrix[clockBarstep][2]] = CRGB::Blue;
      leds[matrix[clockBarstep][3]] = CRGB::Blue;
      leds[matrix[clockBarstep][4]] = CRGB::Blue;
      leds[matrix[clockBarstep][5]] = CRGB::Blue;
      leds[matrix[clockBarstep][6]] = CRGB::Blue;
      leds[matrix[clockBarstep][7]] = CRGB::Red;
      break;
    case 7:
      leds[matrix[clockBarstep][0]] = CRGB::Blue;
      leds[matrix[clockBarstep][1]] = CRGB::Blue;
      leds[matrix[clockBarstep][2]] = CRGB::Blue;
      leds[matrix[clockBarstep][3]] = CRGB::Blue;
      leds[matrix[clockBarstep][4]] = CRGB::Blue;
      leds[matrix[clockBarstep][5]] = CRGB::Blue;
      leds[matrix[clockBarstep][6]] = CRGB::Blue;
      leds[matrix[clockBarstep][7]] = CRGB::Blue;
      break;
  }
  }
  if (channelZoom != 8) {
    leds[matrix[localClock - ((localClock / 8) * 8)][localClock / 8]] = CRGB::Red;
  }
}

void buttonRead(uint8_t i, int reading) {
  buttonMatrix[i] = reading;
}

void write2gates(int linePos) {
  for (int z = 0; z < 8; z++) {
    bitWrite(gateState, z, buttonMatrixToggleState[z][linePos]);
  }
  /*if (masterClock != masterClockLast) {
    Serial.println(lineRead);
    Serial.println(masterClock);
    masterClockLast = masterClock;
    }*/
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

  if (Cmill - PmillTrig >= triggerLenght && writingState == 1) {
    triggerOut = 0;
    writingState = 0;
  }
  return triggerOut;
}

void regwrite(uint8_t output) {
  digitalWrite(regL, LOW);
  shiftOut(regD, regC, MSBFIRST, output);
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

void buttonConversion(uint8_t page) {
  if (channelZoom == 8) {
    for (int i = 0; i < 64; i++) {
      if (buttonMatrixToggleState[i - ((i / 8) * 8)][i / 8 + (page * 8)] == 1) leds[matrix[i / 8][i - ((i / 8) * 8)]] = CRGB::Green;
      if (buttonMatrixToggleState[i - ((i / 8) * 8)][i / 8 + (page * 8)] == 0) leds[matrix[i / 8] [i - ((i / 8) * 8)]] = CRGB::Black;
      if (buttonMatrix[buttonMatrixAdress[i - ((i / 8) * 8)][i / 8]] == 1 && buttonMatrixLast[buttonMatrixAdress[i - ((i / 8) * 8)][i / 8]] == 0) {
        buttonMatrixToggleState[i - ((i / 8) * 8)][i / 8 + (page * 8)] = !buttonMatrixToggleState[i - ((i / 8) * 8)][i / 8 + (page * 8)];// swoped array adresses
        buttonMatrixLast[buttonMatrixAdress[i - ((i / 8) * 8)][i / 8]] = 1;
        anyButtonPressed = 1;
      }
      if (buttonMatrix[buttonMatrixAdress[i - ((i / 8) * 8)][i / 8]] == 0 && buttonMatrixLast[buttonMatrixAdress[i - ((i / 8) * 8)][i / 8]] == 1) {
        buttonMatrixLast[buttonMatrixAdress[i - ((i / 8) * 8)][i / 8]] = 0;
      }
    }
  }
  if (channelZoom != 8) {
    for (int i = 0; i < 64; i++) {
      if (buttonMatrixToggleState[channelZoom][i] == 1) leds[matrix[i - ((i / 8) * 8)][i / 8]] = CRGB::Green;
      if (buttonMatrixToggleState[channelZoom][i] == 0) leds[matrix[i - ((i / 8) * 8)][i / 8]] = CRGB::Black;
      if (buttonMatrix[i] == 1 && buttonMatrixLast[i] == 0) {
        buttonMatrixToggleState[channelZoom][i] = !buttonMatrixToggleState[channelZoom][i];
        buttonMatrixLast[i] = 1;
        anyButtonPressed = 1;
      }
      if (buttonMatrix[i] == 0 && buttonMatrixLast[i] == 1) {
        buttonMatrixLast[i] = 0;
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
  leds[control[0]] = CHSV(map(sequenceLenght, 0, 63, 0, 215), 255, selectBrightness(encoderMenu, 1, 150, 255));
  leds[control[1]] = CHSV(map(buttonsDisplayPageControl, 0, 8, 0, 215), 255, selectBrightness(encoderMenu, 2, 150, 255));
  leds[control[2]] = CHSV(215, 255, selectBrightness(encoderMenu, 3, 150, 255));
  leds[control[3]] = CHSV(215, 255, selectBrightness(encoderMenu, 4, 150, 255));
  leds[control[4]] = CHSV(215, 255, selectBrightness(encoderMenu, 5, 150, 255));
  leds[control[5]] = CHSV(map(loadedFrom, 3, 0, 0, 215), 255, selectBrightness(encoderMenu, 6, 150, 255));
  leds[control[6]] = CHSV(map(savedTo, 3, 0, 0, 215), 255, selectBrightness(encoderMenu, 7, 150, 255));
  leds[control[7]] = CHSV(map(channelZoom, 0, 8, 0, 215), 255, selectBrightness(encoderMenu, 8, 150, 255));

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
      setNumber(numbers2, sequenceLenght + 1, 0, 1, 4, map(sequenceLenght, 0, 63, 0, 215), 255, 255);
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
          setChars(numbers2, buttonsDisplayPageControl + 1, 3, 1, map(buttonsDisplayPageControl, 0, 8, 0, 215), 255, 255);
          buttonsDisplayPage = buttonsDisplayPageControl;
          break;
        case 8:
          setChars(numbers2, 10, 3, 1, map(buttonsDisplayPageControl, 0, 8, 0, 215), 255, 255);
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
          setChars(numbers2, channelZoom + 1, 3, 1, map(channelZoom, 0, 8, 0, 215), 255, 255);
          break;
        case 8:
          setChars(numbers2, 11, 3, 1, map(channelZoom, 0, 8, 0, 215), 255, 255);
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
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  attachInterrupt(0, rotate, CHANGE);

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
  leds[64] = CRGB::Magenta; //215 hue value when HSV used
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
  clockInput();
  if (masterClock > sequenceLenght) masterClock = 0;
  col(counter - ((counter / 8) * 8));
  comm(counter / 8);
  buttonRead(counter, digitalRead(buttonMatrixMux));
  counter++;
  regwrite(gate2trigger(gateState));

  /*  uint32_t forSerial1;
    uint32_t forSerial2;
  for (uint8_t i = 0; i < 32; i++) {
     bitWrite(forSerial1, i, buttonMatrixToggleState[0][i]);
    }
  for (uint8_t i = 0; i < 32; i++) {
    bitWrite(forSerial2, i, buttonMatrixToggleState[0][i + 32]);
  }
  Serial.print(forSerial1, BIN);
  Serial.println(forSerial2, BIN);*/
  //clockGenerator(120);
  //buttonConversion(masterClock / 8);
  //clockBar(masterClock);
  write2gates(masterClock);
  controlButtons();
  controlLeds();
  /*for (uint8_t i = 0; i < 3 * 3; i++) {
    Serial.print(buttonMatrix[buttonMatrixAdress[3 + (i - ((i / 3) * 3))][i / 3 + 3]]);
  }*/
  //setRectangle(3, 3, 3, 3, 255, 0, 255);
  showLed(map(analogRead(animationPot), 0, 0B1111111111, 0, 255));
}
