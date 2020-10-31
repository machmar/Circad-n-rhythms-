//pcb rev 2.0
#include <FastLED.h> //https://github.com/FastLED/FastLED
#include <EEPROM.h> //already built in with arduino IDE
#include <avr/pgmspace.h> //already built in with arduino IDE

/*NOTES
   buttons are red top to bottom > left to right
*/

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

//nonreal values
#define keyPressDuration 300
#define longPressDuration 1000
#define triggerLenght 20
#define hardwareClockTimerFrquency (F_CPU * 60) //precalculate the left part of the claculation with crystal frequency
#define BPMTimeout 400
#define SYSfirmwareVersion "2.5.0 current firmware."

//EEPROM positions
#define EEPROMBPM 1000
#define EEPROMLastSavedTo 1001
#define EEPROMLastLoadedFrom 1002
#define EEPROMClockMultiplier 1003 //has 100 added to it so it allways is an integer
#define EEPROMClockSource 1004
#define EEPROMGateTrigger 1005
#define EEPROMUsedCheck 1023 //this position has to be eaqul to 17, if it is not, it will reset all adresses

uint8_t counter;
bool menuButton[9];
bool menuButtonLast[9];
bool buttonState[65];
bool buttonStatePrev[65];
uint64_t currentSequence[9];
uint64_t currentAccents[9];
uint64_t nextSequence[9];
uint64_t nextAccents[9];
uint8_t jackState;
uint8_t controlReg;
uint8_t brightnessAdj;
uint8_t temperatureAdj;
float temperatureSens;
uint16_t variable1Adj;
uint16_t variable2Adj;
uint8_t verticalBtnScanPrev;
uint8_t channelZoom;
uint8_t sequenceLenght = 63;
uint8_t masterClock;
bool gate2TriggerClockToggle;
bool gate2TriggerTimerRunning;
int16_t scrollStringPosition;
uint8_t helpInfoTextPrev;
uint8_t helpInfoText;
uint8_t helpInfoVariable;
uint8_t displayButtonMode = 0;
bool encoderSwitchPrev;
uint8_t encoderClosedMenu;
uint8_t gateTriggerChannel;
uint8_t buttonUIChannelZoomBeforeZero;
bool menuButtonShiftActive;
bool menuButtonShiftActivePrev;
bool clockRunning = 1;
bool clockSource = 0; //1 internal, 0 external
uint32_t blinkMainCounter;
bool blink20msState;
bool blink40msState;
bool blink80msState;
bool blink160msState;
uint8_t globalInternalBPM = 120;
uint8_t globalInternalSwing = 0;
int8_t clockMultiplier = 1;
bool DisableControlLeds;
bool clockMultipliButtonPrev;
bool clockDivideButtonPrev;
uint8_t lastSavedTo;
uint8_t lastLoadedFrom;
uint8_t nextSequenceReady = 255; //default state is 255 because 0 is a valid loading position
uint16_t clockInputSpeed;
bool clockResetJacksSourcePrev;
uint8_t modeSavingPrev;

uint32_t showLedPMill;
uint32_t gate2triggerPMill;
uint32_t scrollStringPMill;
uint32_t buttonPMill;
uint32_t menuButtonPMill;
uint32_t encoderSwitchPMill;
uint32_t testMeasuringPMill;
uint32_t blinkMainPMill;
uint32_t displayBPMPMill;
uint32_t clockMeasuringPMill;

CRGB leds[72];

static const uint8_t controlLeds[9] = {64, 65, 66, 67, 68, 69, 70 , 71};
static const uint8_t ledMatrix[10][9] = { //x(horizontal) ,y(vertical)
  {0, 15, 16, 31, 32, 47, 48, 63},
  {1, 14, 17, 30, 33, 46, 49, 62},
  {2, 13, 18, 29, 34, 45, 50, 61},
  {3, 12, 19, 28, 35, 44, 51, 60},
  {4, 11, 20, 27, 36, 43, 52, 59},
  {5, 10, 21, 26, 37, 42, 53, 58},
  {6, 9, 22, 25, 38, 41, 54, 57},
  {7, 8, 23, 24, 39, 40, 55, 56},
  {64, 65, 66, 67, 68, 69, 70, 71},
};
static const uint8_t buttonMatrix[9][9] = { //x(horizontal) ,y(vertical)
  {0  ,  1,  2,  3,  4,  5,  6,  7},
  {8  ,  9, 10, 11, 12, 13, 14, 15},
  {16 , 17, 18, 19, 20, 21, 22, 23},
  {24 , 25, 26, 27, 28, 29, 30, 31},
  {32 , 33, 34, 35, 36, 37, 38, 39},
  {40 , 41, 42, 43, 44, 45, 46, 47},
  {48 , 49, 50, 51, 52, 53, 54, 55},
  {56 , 57, 58, 59, 60, 61, 62, 63},
};
/*----------FONT-7x8-version-1----------
   ---       ---   0   --- 00 00 ---   0 0 ---   0   ---  0    ---       ---   0   ---     0 --- 0     ---       ---       ---
   ---       ---   0   --- 0  0  ---   0 0 ---  000  --- 0 0   ---   00  ---   0   ---     0 --- 0     ---   0   ---   0   ---
   ---       ---   0   ---       ---  0 0  --- 0     ---  0    ---  0    ---   0   ---    0  ---  0    --- 00000 ---   0   ---
   ---       ---   0   ---       --- 00000 --- 0     ---    00 ---  0    ---       ---    0  ---  0    ---   0   ---   0   ---
   ---       ---   0   ---       ---  0 0  ---  000  --- 000   --- 0 0 0 ---       ---    0  ---  0    ---  0 0  ---0000000---
   ---       ---       ---       --- 00000 --- 0  0  ---    0  --- 0  0  ---       ---    0  ---  0    ---  0 0  ---   0   ---
   ---       ---       ---       ---  0 0  --- 000   ---   0 0 ---  00 0 ---       ---     0 --- 0     ---       ---   0   ---
   ---       ---   0   ---       --- 0 0   ---   0   ---    0  ---       ---       ---     0 --- 0     ---       ---   0   ---

   ---       ---       ---       ---    0  ---  000  ---  00   ---  000  ---  000  ---    00 ---  0000 ---   000 --- 00000 ---
   ---       ---       ---       ---    0  --- 0   0 ---   0   --- 0   0 --- 0   0 ---   0 0 ---  0    ---  0    --- 0   0 ---
   ---       ---       ---       ---   0   --- 0   0 ---   0   ---     0 ---     0 ---   0 0 ---  0    --- 0     ---     0 ---
   ---       ---       ---       ---   0   --- 0   0 ---   0   ---    0  ---   00  ---  0  0 ---  000  --- 0000  ---    0  ---
   ---  00   ---0000000---       ---  0    --- 0   0 ---   0   ---   0   ---     0 --- 0   0 ---     0 --- 0   0 ---    0  ---
   ---  0    ---       ---       ---  0    --- 0   0 ---   0   ---  0    ---     0 --- 000000---     0 --- 0   0 ---    0  ---
   --- 00    ---       ---  00   --- 0     --- 0   0 ---   0   --- 0   0 --- 0   0 ---     0 --- 0   0 --- 0   0 ---   0   ---
   --- 0     ---       ---  00   --- 0     ---  000  --- 00000 --- 00000 ---  000  ---    000---  000  ---  000  ---   0   ---

   ---  000  ---  000  ---       ---       ---       ---       ---       ---       ---  000  ---  00   ---00000  ---  0000 ---
   --- 0   0 --- 0   0 ---       ---       ---    00 ---       --- 00    ---  00   --- 0   0 ---   0   --- 0   0 --- 0   0 ---
   --- 0   0 --- 0   0 ---  00   ---  00   ---   0   ---       ---   0   --- 0  0  --- 0  00 ---  0 0  --- 0   0 --- 0     ---
   ---  000  --- 0   0 ---  00   ---  00   --- 00    ---0000000---    00 ---    0  --- 0 0 0 ---  0 0  --- 0000  --- 0     ---
   --- 0   0 ---  0000 ---       ---       ---0      ---       ---      0---   0   --- 0 0 0 --- 0   0 --- 0   0 --- 0     ---
   --- 0   0 ---     0 ---       ---  00   --- 00    ---0000000---    00 ---  0    --- 0  00 --- 00000 --- 0   0 --- 0     ---
   --- 0   0 ---    0  ---  00   --- 00    ---   0   ---       ---   0   ---       --- 0     --- 0   0 --- 0   0 --- 0   0 ---
   ---  000  --- 000   ---  00   --- 0     ---    00 ---       --- 00    ---  0    ---  000  ---000 000---00000  ---  000  ---

   ---0000   ---000000 ---000000 --- 0000  ---000 000--- 00000 ---  0000 ---000 000--- 000   ---000 000---000 000---  000  ---
   --- 0  0  --- 0   0 --- 0   0 ---0    0 --- 0   0 ---   0   ---    0  --- 0   0 ---  0    --- 00 00 --- 00  0 --- 0   0 ---
   --- 0   0 --- 0 0   --- 0 0   ---0      --- 0   0 ---   0   ---    0  --- 0  0  ---  0    --- 00 00 --- 00  0 --- 0   0 ---
   --- 0   0 --- 000   --- 000   ---0      --- 00000 ---   0   ---    0  --- 0 0   ---  0    --- 0 0 0 --- 0 0 0 --- 0   0 ---
   --- 0   0 --- 0 0   --- 0 0   ---0  000 --- 0   0 ---   0   ---    0  --- 000   ---  0    --- 0 0 0 --- 0 0 0 --- 0   0 ---
   --- 0   0 --- 0     --- 0     ---0   0  --- 0   0 ---   0   --- 0  0  --- 0  0  ---  0    --- 0   0 --- 0 0 0 --- 0   0 ---
   --- 0  0  --- 0   0 --- 0     ---0   0  --- 0   0 ---   0   --- 0  0  --- 0   0 ---  0  0 --- 0   0 --- 0  00 --- 0   0 ---
   ---0000   ---000000 ---000    --- 000   ---000 000--- 00000 ---  00   ---000 000--- 00000 ---000 000---000 000---  000  ---

   --- 0000  ---  000  ---00000  ---  00 0 ---0000000---000 000---000 000---000 000---00   00---000 000--- 00000 ---   000 ---
   ---  0  0 --- 0   0 --- 0   0 --- 0  00 ---0  0  0--- 0   0 --- 0   0 --- 0   0 --- 0   0 --- 0   0 --- 0   0 ---   0   ---
   ---  0  0 --- 0   0 --- 0   0 --- 0     ---   0   --- 0   0 --- 0   0 --- 0   0 ---  0 0  ---  0 0  ---    0  ---   0   ---
   ---  000  --- 0   0 --- 0000  ---  000  ---   0   --- 0   0 ---  0 0  --- 0 0 0 ---   0   ---  0 0  ---   0   ---   0   ---
   ---  0    --- 0   0 --- 0  0  ---     0 ---   0   --- 0   0 ---  0 0  --- 0 0 0 ---   0   ---   0   ---   0   ---   0   ---
   ---  0    --- 0   0 --- 0  0  ---     0 ---   0   --- 0   0 ---  0 0  --- 0 0 0 ---  0 0  ---   0   ---  0    ---   0   ---
   ---  0    ---  000  --- 0   0 --- 00  0 ---   0   --- 0   0 ---   0   --- 0 0 0 --- 0   0 ---   0   --- 0   0 ---   0   ---
   --- 000   ---   000 ---000   0--- 0 00  ---  000  ---  000  ---   0   ---  0 0  ---00   00---  000  --- 00000 ---   000 ---

   --- 0     --- 000   ---   0   ---       --- 0     ---       ---       ---       ---       ---       ---       ---       ---
   --- 0     ---   0   ---   0   ---       ---  0    ---       ---00     ---       ---     00---       ---   000 ---  00 00---
   ---  0    ---   0   ---  0 0  ---       ---       --- 000   --- 0     ---  0000 ---     0 ---  000  ---  0    --- 0  00 ---
   ---  0    ---   0   --- 0   0 ---       ---       ---0   0  --- 0 00  --- 0   0 ---  00 0 --- 0   0 --- 00000 --- 0   0 ---
   ---   0   ---   0   ---       ---       ---       --- 0000  --- 00  0 --- 0     --- 0  00 --- 00000 ---  0    --- 0   0 ---
   ---   0   ---   0   ---       ---       ---       ---0   0  --- 0   0 --- 0     --- 0   0 --- 0     ---  0    ---  0000 ---
   ---    0  ---   0   ---       ---       ---       ---0   0  --- 0   0 --- 0   0 --- 0   0 --- 0     ---  0    ---     0 ---
   ---    0  --- 000   ---       ---0000000---       --- 00000 ---00000  ---  000  ---  00000---  0000 --- 00000 ---  000  ---

   ---       ---       ---       ---       ---       ---       ---       ---       --- 00 00 ---  00 00---       ---       ---
   ---00     ---   0   ---   0   ---00     ---  00   ---       ---       ---       ---  00  0--- 0  00 ---       ---       ---
   --- 0     ---       ---       --- 0 000 ---   0   ---000 0  ---00 00  ---  000  ---  0   0--- 0   0 ---  00 00---  0000 ---
   --- 0 00  --- 000   --- 0000  --- 0  0  ---   0   --- 0 0 0 --- 00  0 --- 0   0 ---  0   0--- 0   0 ---   00  --- 0   0 ---
   --- 00  0 ---   0   ---    0  --- 000   ---   0   --- 0 0 0 --- 0   0 --- 0   0 ---  0   0--- 0   0 ---   0   ---  000  ---
   --- 0   0 ---   0   ---    0  --- 0 0   ---   0   --- 0 0 0 --- 0   0 --- 0   0 ---  0000 ---  0000 ---   0   ---     0 ---
   --- 0   0 ---   0   ---    0  --- 0  0  ---   0   --- 0 0 0 --- 0   0 --- 0   0 ---  0    ---     0 ---   0   --- 0   0 ---
   ---000 000--- 00000 --- 000   ---00 000 --- 00000 ---0000000---000 000---  000  --- 000   ---    000---- 0000 --- 0000  ---

   ---       ---       ---       ---       ---       ---       ---       ---     0 ---   0   --- 0     ---       ---
   --- 0     ---       ---       ---       ---       ---       ---       ---    0  ---   0   ---  0    ---       ---
   ---00000  ---00  00 ---000 000---000 000---00  00 ---000 000--- 00000 ---    0  ---   0   ---  0    ---       ---
   --- 0     --- 0   0 --- 0   0 --- 0   0 --- 0  0  --- 0   0 --- 0  0  ---    0  ---   0   ---  0    ---  0  0 ---
   --- 0     --- 0   0 --- 0   0 --- 0 0 0 ---  00   ---  0 0  ---   0   ---   0   ---   0   ---   0   --- 0 00  ---
   --- 0     --- 0   0 ---  0 0  --- 0 0 0 ---  00   ---   00  ---  0    ---    0  ---   0   ---  0    ---       ---
   --- 0   0 --- 0  00 ---  0 0  --- 0 0 0 --- 0  0  ---   0   --- 0   0 ---    0  ---   0   ---  0    ---       ---
   ---  000  ---  00 00---   0   ---  0 0  ---00  00 --- 0000  --- 00000 ---     0 ---   0   --- 0     ---       ---
*/
static const uint8_t FontArial7x8v1[] PROGMEM = {
  0x0F, 0xDE, // size
  0x07, // width
  0x08, // height
  0x20, // first char
  0x60, // char count

  // char widths
  0x03, 0x01, 0x05, 0x05, 0x04, 0x05, 0x05, 0x01, 0x02, 0x02,
  0x05, 0x05, 0x03, 0x05, 0x02, 0x04, 0x05, 0x05, 0x05, 0x05,
  0x06, 0x05, 0x05, 0x05, 0x05, 0x05, 0x02, 0x03, 0x06, 0x05,
  0x06, 0x04, 0x05, 0x07, 0x06, 0x05, 0x06, 0x06, 0x06, 0x07,
  0x07, 0x05, 0x05, 0x07, 0x05, 0x07, 0x07, 0x05, 0x05, 0x05,
  0x07, 0x05, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x05, 0x03,
  0x04, 0x03, 0x05, 0x07, 0x02, 0x06, 0x06, 0x05, 0x06, 0x05,
  0x05, 0x06, 0x07, 0x05, 0x05, 0x06, 0x05, 0x07, 0x07, 0x05,
  0x06, 0x06, 0x05, 0x05, 0x06, 0x07, 0x07, 0x07, 0x06, 0x07,
  0x05, 0x03, 0x01, 0x03, 0x05, 0x07,

  // font data
  0x00, 0x00, 0x00, // 32
  0x9F, // 33
  0x03, 0x01, 0x00, 0x03, 0x01, // 34
  0xA8, 0x7C, 0xAB, 0x7C, 0x2B, // 35
  0x6C, 0x52, 0xD3, 0x32, // 36
  0x12, 0x15, 0x52, 0xA8, 0x48, // 37
  0x30, 0x4C, 0x52, 0x22, 0x50, // 38
  0x07, // 39
  0x3C, 0xC3, // 40
  0xC3, 0x3C, // 41
  0x04, 0x34, 0x0E, 0x34, 0x04, // 42
  0x10, 0x10, 0x7C, 0x10, 0x10, // 43
  0xC0, 0x70, 0x10, // 44
  0x10, 0x10, 0x10, 0x10, 0x10, // 45
  0xC0, 0xC0, // 46
  0xC0, 0x30, 0x0C, 0x03, // 47
  0x7E, 0x81, 0x81, 0x81, 0x7E, // 48
  0x80, 0x81, 0xFF, 0x80, 0x80, // 49
  0xC2, 0xA1, 0x91, 0x89, 0xC6, // 50
  0x42, 0x81, 0x89, 0x89, 0x76, // 51
  0x30, 0x28, 0x26, 0xA1, 0xFF, 0xA0, // 52
  0x40, 0x8F, 0x89, 0x89, 0x71, // 53
  0x7C, 0x8A, 0x89, 0x89, 0x71, // 54
  0x03, 0x01, 0xC1, 0x39, 0x07, // 55
  0x76, 0x89, 0x89, 0x89, 0x76, // 56
  0x8E, 0x91, 0x91, 0x51, 0x3E, // 57
  0xCC, 0xCC, // 58
  0xC0, 0x6C, 0x2C, // 59
  0x10, 0x28, 0x28, 0x44, 0x82, 0x82, // 60
  0x28, 0x28, 0x28, 0x28, 0x28, // 61
  0x82, 0x82, 0x44, 0x28, 0x28, 0x10, // 62
  0x04, 0xA2, 0x12, 0x0C, // 63
  0x7E, 0x81, 0x99, 0xA5, 0x3E, // 64
  0x80, 0xF0, 0xAD, 0x23, 0xAC, 0xF0, 0x80, // 65
  0x81, 0xFF, 0x89, 0x89, 0x76, 0x00, // 66
  0x7E, 0x81, 0x81, 0x81, 0x43, // 67
  0x81, 0xFF, 0x81, 0x81, 0x7E, 0x00, // 68
  0x81, 0xFF, 0x89, 0x9D, 0x81, 0xC3, // 69
  0x81, 0xFF, 0x91, 0x39, 0x01, 0x03, // 70
  0x7E, 0x81, 0x81, 0x81, 0x91, 0x72, 0x10, // 71
  0x81, 0xFF, 0x89, 0x08, 0x89, 0xFF, 0x81, // 72
  0x81, 0x81, 0xFF, 0x81, 0x81, // 73
  0x60, 0x81, 0x81, 0x7F, 0x01, // 74
  0x81, 0xFF, 0x91, 0x18, 0xA5, 0xC3, 0x81, // 75
  0x81, 0xFF, 0x81, 0x80, 0xC0, // 76
  0x81, 0xFF, 0x87, 0x18, 0x87, 0xFF, 0x81, // 77
  0x81, 0xFF, 0x87, 0x38, 0xC1, 0xFF, 0x81, // 78
  0x7E, 0x81, 0x81, 0x81, 0x7E, // 79
  0x81, 0xFF, 0x89, 0x09, 0x06, // 80
  0x3E, 0x41, 0xC1, 0xC1, 0xBE, // 81
  0x81, 0xFF, 0x89, 0x09, 0x39, 0x46, 0x80, // 82
  0xC6, 0x49, 0x89, 0x8A, 0x73, // 83
  0x03, 0x01, 0x81, 0xFF, 0x81, 0x01, 0x03, // 84
  0x01, 0x7F, 0x81, 0x80, 0x81, 0x7F, 0x01, // 85
  0x01, 0x07, 0x39, 0xC0, 0x39, 0x07, 0x01, // 86
  0x01, 0x7F, 0x81, 0x78, 0x81, 0x7F, 0x01, // 87
  0x81, 0xC3, 0x24, 0x18, 0x24, 0xC3, 0x81, // 88
  0x01, 0x03, 0x8D, 0xF0, 0x8D, 0x03, 0x01, // 89
  0xC3, 0xA1, 0x99, 0x85, 0xC3, // 90
  0xFF, 0x81, 0x81, // 91
  0x03, 0x0C, 0x30, 0xC0, // 92
  0x81, 0x81, 0xFF, // 93
  0x08, 0x04, 0x03, 0x04, 0x08, // 94
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, // 95
  0x01, 0x02, // 96
  0x68, 0x94, 0x94, 0x94, 0xF8, 0x80, // 97
  0x82, 0xFE, 0x90, 0x88, 0x88, 0x70, // 98
  0x78, 0x84, 0x84, 0x84, 0x4C, // 99
  0x70, 0x88, 0x88, 0x90, 0xFE, 0x82, // 100
  0x78, 0x94, 0x94, 0x94, 0x98, // 101
  0x88, 0xFC, 0x8A, 0x8A, 0x8A, // 102
  0x1C, 0xA2, 0xA2, 0xA4, 0x7E, 0x02, // 103
  0x82, 0xFE, 0x90, 0x08, 0x88, 0xF0, 0x80, // 104
  0x88, 0x88, 0xFA, 0x80, 0x80, // 105
  0x88, 0x88, 0x8A, 0x78, 0x00, // 106
  0x82, 0xFE, 0x10, 0xB4, 0xCC, 0x84, // 107
  0x80, 0x82, 0xFE, 0x80, 0x80, // 108
  0x84, 0xFC, 0x84, 0xF8, 0x84, 0xF8, 0x80, // 109
  0x84, 0xFC, 0x88, 0x04, 0x84, 0xF8, 0x80, // 110
  0x78, 0x84, 0x84, 0x84, 0x78, // 111
  0x82, 0xFE, 0xA4, 0x22, 0x22, 0x1C, // 112
  0x1C, 0x22, 0x22, 0xA4, 0xFE, 0x82, // 113
  0x84, 0xFC, 0x88, 0x84, 0x04, // 114
  0xC8, 0x94, 0x94, 0x94, 0x6C, // 115
  0x04, 0x7E, 0x84, 0x84, 0x84, 0x40, // 116
  0x04, 0x7C, 0x80, 0x80, 0x44, 0xFC, 0x80, // 117
  0x04, 0x1C, 0x64, 0x80, 0x64, 0x1C, 0x04, // 118
  0x04, 0x7C, 0x84, 0x70, 0x84, 0x7C, 0x04, // 119
  0x84, 0xCC, 0x30, 0x30, 0xCC, 0x84, // 120
  0x04, 0x8C, 0x94, 0xE0, 0xB4, 0x0C, 0x04, // 121
  0xCC, 0xA4, 0x94, 0x8C, 0xC4, // 122
  0x10, 0x6E, 0x81, // 123
  0xFF, // 124
  0x81, 0x6E, 0x10, // 125
  0x10, 0x08, 0x10, 0x10, 0x08, // 126
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // 127

};
static const uint8_t FontGeneric3x6v1[] PROGMEM = {
  0x06, 0x90, // size
  0x03, // width
  0x06, // height
  0x20, // first char
  0x60, // char count

  // char widths
  0x02, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x02, 0x02,
  0x03, 0x03, 0x01, 0x03, 0x01, 0x03, 0x03, 0x02, 0x03, 0x03,
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03,
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x02,
  0x03, 0x02, 0x03, 0x03, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03,
  0x03, 0x03, 0x03, 0x01, 0x03, 0x03, 0x01, 0x03, 0x03, 0x03,
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
  0x03, 0x03, 0x01, 0x03, 0x04, 0x03,

  // font data
  0x00, 0x00, // 32
  0xBC, // 33
  0x0C, 0x00, 0x0C, // 34
  0xFC, 0x84, 0xFC, // 35
  0x10, 0xAC, 0x40, // 36
  0x28, 0x20, 0x90, // 37
  0xFC, 0x84, 0xFC, // 38
  0x0C, // 39
  0x84, 0x78, // 40
  0x78, 0x84, // 41
  0x14, 0x08, 0x14, // 42
  0x20, 0x70, 0x20, // 43
  0xC0, // 44
  0x20, 0x20, 0x20, // 45
  0x80, // 46
  0xC0, 0x30, 0x0C, // 47
  0x7C, 0x84, 0xF8, // 48
  0x04, 0xFC, // 49
  0xC8, 0xA4, 0x98, // 50
  0x44, 0x94, 0x6C, // 51
  0x38, 0x20, 0xFC, // 52
  0x5C, 0x94, 0x64, // 53
  0x78, 0x94, 0x60, // 54
  0xC4, 0x34, 0x0C, // 55
  0x68, 0x94, 0x68, // 56
  0x18, 0xA4, 0x78, // 57
  0x48, // 58
  0xC8, // 59
  0x20, 0x50, 0x88, // 60
  0x50, 0x50, 0x50, // 61
  0x88, 0x50, 0x20, // 62
  0x08, 0xA4, 0x18, // 63
  0xFC, 0x84, 0xFC, // 64
  0xFC, 0x24, 0xF8, // 65
  0xFC, 0x94, 0x68, // 66
  0x78, 0x84, 0x84, // 67
  0xFC, 0x84, 0x78, // 68
  0xFC, 0x94, 0x84, // 69
  0xFC, 0x24, 0x04, // 70
  0x78, 0x84, 0xE4, // 71
  0xFC, 0x10, 0xFC, // 72
  0x84, 0xFC, 0x84, // 73
  0x40, 0x80, 0xFC, // 74
  0xFC, 0x10, 0xEC, // 75
  0xFC, 0x80, 0x80, // 76
  0xFC, 0x08, 0xFC, // 77
  0xFC, 0x04, 0xF8, // 78
  0xF8, 0x84, 0x7C, // 79
  0xFC, 0x24, 0x18, // 80
  0x7C, 0xC4, 0xF8, // 81
  0xFC, 0x24, 0xD8, // 82
  0x88, 0x94, 0x64, // 83
  0x04, 0xFC, 0x04, // 84
  0xFC, 0x80, 0x7C, // 85
  0x7C, 0x80, 0x7C, // 86
  0xFC, 0x40, 0xFC, // 87
  0xCC, 0x30, 0xCC, // 88
  0x1C, 0xE0, 0x1C, // 89
  0xC4, 0xB4, 0x8C, // 90
  0xFC, 0x84, // 91
  0x0C, 0x30, 0xC0, // 92
  0x84, 0xFC, // 93
  0x08, 0x04, 0x08, // 94
  0x80, 0x80, 0x80, // 95
  0x0C, // 96
  0x40, 0xA8, 0xF0, // 97
  0xFC, 0x90, 0x60, // 98
  0x70, 0x88, 0x88, // 99
  0x60, 0x90, 0xFC, // 100
  0x70, 0xA8, 0xB0, // 101
  0x10, 0xF8, 0x14, // 102
  0x90, 0xA8, 0x70, // 103
  0xFC, 0x10, 0xE0, // 104
  0xF4, // 105
  0x40, 0x80, 0xF4, // 106
  0xFC, 0x20, 0xD8, // 107
  0xFC, // 108
  0xF8, 0x10, 0xF8, // 109
  0xF8, 0x08, 0xF0, // 110
  0xF0, 0x88, 0x78, // 111
  0xF8, 0x48, 0x30, // 112
  0x30, 0x48, 0xF8, // 113
  0xF8, 0x10, 0x08, // 114
  0x90, 0xA8, 0x48, // 115
  0x10, 0x7C, 0x90, // 116
  0xF8, 0x80, 0x78, // 117
  0x38, 0xC0, 0x38, // 118
  0xF8, 0x40, 0xF8, // 119
  0xD8, 0x20, 0xD8, // 120
  0x98, 0xA0, 0x78, // 121
  0xC8, 0xA8, 0x98, // 122
  0x10, 0x78, 0x84, // 123
  0xFC, // 124
  0x84, 0x78, 0x10, // 125
  0x20, 0x10, 0x20, 0x10, // 126
  0x00, 0x00, 0x00 // 127
};
static const uint8_t FontNumbers3x6v1[] PROGMEM = {
  0x04, 0x41, // size
  0x03, // width
  0x06, // height
  0x30, // first char
  0x0A, // char count

  // char widths
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,

  // font data
  0xFC, 0x84, 0xFC, // 48
  0x88, 0xFC, 0x80, // 49
  0xE4, 0xA4, 0xBC, // 50
  0x94, 0x94, 0xFC, // 51
  0x3C, 0x20, 0xF0, // 52
  0x9C, 0x94, 0xF4, // 53
  0xFC, 0xA4, 0xE4, // 54
  0x04, 0x04, 0xFC, // 55
  0xFC, 0x94, 0xFC, // 56
  0x1C, 0x14, 0xFC, // 57
};
static const uint32_t imageGenericGuy1[] PROGMEM = {
  8, //width
  8, //height
  0x000000, 0x000000, 0x754c24, 0x754c24, 0x754c24, 0x754c24, 0x000000, 0x000000,
  0x000000, 0x754c24, 0xffc794, 0xffc794, 0xffc794, 0xffc794, 0x000000, 0x000000,
  0x000000, 0x754c24, 0xffc794, 0x2023ff, 0xffc794, 0x2023ff, 0x000000, 0x000000,
  0x000000, 0x754c24, 0xffc794, 0xffc794, 0xffc794, 0xffc794, 0x000000, 0x000000,
  0x000000, 0xf7941d, 0xffc794, 0xffc794, 0xffc794, 0xf7941d, 0x000000, 0x000000,
  0xffc794, 0x000000, 0xf7941d, 0xf7941d, 0xf7941d, 0x000000, 0xffc794, 0x000000,
  0x000000, 0x000000, 0x252525, 0x252525, 0x252525, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x363636, 0x000000, 0x363636, 0x000000, 0x000000, 0x000000
};
static const uint32_t imageGenericGuy2[] PROGMEM = {
  8, //width
  8, //height
  0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x754c24, 0x754c24, 0x754c24, 0x754c24, 0x000000, 0x000000,
  0x000000, 0x754c24, 0xffc794, 0xffc794, 0xffc794, 0xffc794, 0x000000, 0x000000,
  0x000000, 0x754c24, 0xffc794, 0x373aff, 0xffc794, 0x373aff, 0x000000, 0x000000,
  0x000000, 0x754c24, 0xffc794, 0xffc794, 0xffc794, 0xffc794, 0x000000, 0x000000,
  0xffc794, 0xf7941d, 0xffc794, 0xffc794, 0xffc794, 0xf7941d, 0xffc794, 0x000000,
  0x000000, 0x000000, 0xf7941d, 0xf7941d, 0xf7941d, 0x000000, 0x000000, 0x000000,
  0x000000, 0x000000, 0x363636, 0x000000, 0x363636, 0x000000, 0x000000, 0x000000
};
static const uint32_t imageEuclidOffset[] PROGMEM = {
  8, //width
  4, //height
  0xff0000, 0xff0000, 0xff0000, 0x000000, 0xff0000, 0xff0000, 0xff0000, 0x000000,
  0xff0000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0x000000, 0x000000,
  0xff0000, 0x000000, 0xff0000, 0x000000, 0xff0000, 0xff0000, 0x000000, 0x000000,
  0xff0000, 0xff0000, 0xff0000, 0x000000, 0xff0000, 0x000000, 0x000000, 0x000000
};
static const uint32_t imageEuclidHits[] PROGMEM = {
  8, //width
  4, //height
  0xff00ff, 0x000000, 0xff00ff, 0x000000, 0xff00ff, 0xff00ff, 0xff00ff, 0xff00ff,
  0xff00ff, 0x000000, 0xff00ff, 0x000000, 0xff00ff, 0x000000, 0xff00ff, 0x000000,
  0xff00ff, 0xff00ff, 0xff00ff, 0x000000, 0xff00ff, 0x000000, 0xff00ff, 0x000000,
  0xff00ff, 0x000000, 0xff00ff, 0x000000, 0xff00ff, 0x000000, 0xff00ff, 0x000000
};
static const uint32_t imageCross5x5[] PROGMEM = {
  5, //width
  5, //height
  0xff0000, 0x00002b, 0x00002b, 0x00002b, 0xff0000,
  0x00002b, 0xff0000, 0x00002b, 0xff0000, 0x00002b,
  0x00002b, 0x00002b, 0xff0000, 0x00002b, 0x00002b,
  0x00002b, 0xff0000, 0x00002b, 0xff0000, 0x00002b,
  0xff0000, 0x00002b, 0x00002b, 0x00002b, 0xff0000
};
static const uint32_t imageChannelSelectionPage[] PROGMEM = {
  7, //width
  8, //height
  0xff95f7, 0xff95f7, 0xff95f7, 0x000000, 0x86c7ff, 0x000000, 0x86c7ff,
  0xff95f7, 0x000000, 0x000000, 0x000000, 0x86c7ff, 0x000000, 0x86c7ff,
  0xff95f7, 0x000000, 0x000000, 0x000000, 0x86c7ff, 0x86c7ff, 0x86c7ff,
  0xff95f7, 0xff95f7, 0xff95f7, 0x000000, 0x86c7ff, 0x000000, 0x86c7ff,
  0x000000, 0x86ff98, 0x000000, 0x000000, 0xffde89, 0xffde89, 0x000000,
  0x86ff98, 0x000000, 0x86ff98, 0x000000, 0xffde89, 0x000000, 0xffde89,
  0x86ff98, 0x86ff98, 0x86ff98, 0x000000, 0xffde89, 0x000000, 0xffde89,
  0x86ff98, 0x000000, 0x86ff98, 0x000000, 0xffde89, 0x000000, 0xffde89
};
static const uint32_t imageSymbolMultiply[] PROGMEM = {
  3, //width
  3, //height
  0x37ff55, 0x000000, 0x37ff55, 
  0x000000, 0x37ff55, 0x000000, 
  0x37ff55, 0x000000, 0x37ff55
};
static const uint32_t imageSymbolDivide[] PROGMEM = {
  3, //width
  4, //height
  0x000000, 0x000000, 0x37ff55, 
  0x000000, 0x37ff55, 0x000000, 
  0x000000, 0x37ff55, 0x000000, 
  0x37ff55, 0x000000, 0x000000
};
static const uint32_t imageGateTrigger[] PROGMEM = {
  6, //width
  6, //height
  0x00c800, 0x00c800, 0x00c800, 0x0000c8, 0x0000c8, 0x000000, 
  0x000000, 0x00c800, 0x0000c8, 0x000000, 0x000000, 0x0000c8, 
  0x000000, 0x00c800, 0x0000c8, 0x000000, 0x000000, 0x000000, 
  0x000000, 0x00c800, 0x0000c8, 0x000000, 0x0000c8, 0x0000c8, 
  0x000000, 0x00c800, 0x0000c8, 0x000000, 0x000000, 0x0000c8, 
  0x000000, 0x00c800, 0x000000, 0x0000c8, 0x0000c8, 0x000000, 
};
static const uint32_t imageSavePreset[] PROGMEM = {
  8, //width
  5, //height
  0x000000, 0x964600, 0x964600, 0x000000, 0x000000, 0x460096, 0x000000, 0x000000, 
  0x964600, 0x000000, 0x000000, 0x000000, 0x000000, 0x460096, 0x000000, 0x000000, 
  0x000000, 0x964600, 0x000000, 0x000000, 0x000000, 0x460096, 0x000000, 0x000000, 
  0x000000, 0x000000, 0x964600, 0x000000, 0x000000, 0x460096, 0x000000, 0x000000, 
  0x964600, 0x964600, 0x000000, 0x000000, 0x000000, 0x460096, 0x460096, 0x460096
};


//redefining bit functions to work with 64bit variables - BEGIN
#undef bitRead
#undef bitWrite
template <typename T> void bitWrite (T & value, byte bit, byte val) {
  if (val) value |= (T(1) << bit);
  else value &= ~ (T(1) << bit);
}
template <typename T> bool bitRead (T value, byte bit) {
  return  (value & ((T)1 << bit)) != 0;
}
//redefining bit functions to work with 64bit variables - END

void fadeall() {
  for (int i = 0; i < 72; i++) {
    leds[i].nscale8(250);
  }
}

void clearDisplay() {
  for (uint8_t i = 0; i < 64; i++) leds[i] = 0;
}

void clearDisplayFull() {
  for (uint8_t i = 0; i < 72; i++) leds[i] = 0;
}

void setCharsGLCDCompataible(uint8_t font[], char input, int16_t xOrigin, int16_t yOrigin, bool controlLeds, uint32_t rgb) {
  DisableControlLeds = controlLeds; //disable control LEDs when it goes over them
  uint8_t CharWidthSkip = input - pgm_read_byte(font + 4); //this is all to skip the unwanted bytes, if you use "letter" you are giving it an array (I think)
  uint32_t FontDataSkip = 0;
  uint8_t fontInfo[7] = {0, 0, 0, 0, 0, 0};
  for (uint8_t i = 0; i < 6; i++) {
    fontInfo[i] = pgm_read_byte(font + i);
  }

  for (uint8_t i = 0; i < CharWidthSkip; i++) {
    FontDataSkip += pgm_read_byte(font + 6 + i);
  }
  uint16_t TablePosition = 6 + fontInfo[5] + FontDataSkip; //this is for skipping all thew stuff AND the unwanted bytes

  uint8_t width = pgm_read_byte(font + (input - fontInfo[4] + 6));
  uint8_t height = fontInfo[3];
  for (uint8_t w = 0; w < width; w++) {
    for (uint8_t h = 0; h < height; h++) {
      if (xOrigin + w < 8 + controlLeds && xOrigin + w >= 0 && yOrigin + h < 8 && yOrigin + h >= 0) { //ignore overlaps
        uint8_t byteToRead = pgm_read_byte(font + w + TablePosition);
        leds[ledMatrix[xOrigin + w][yOrigin + h]] = rgb * bitRead(byteToRead, h + (8 - height));
      }
    }
  }
}

uint8_t charWidth(uint8_t font[], uint8_t input) {
  uint8_t fontFirstChar = pgm_read_byte(font + 4);
  return pgm_read_byte(font + (input - fontFirstChar + 6));
}

int16_t moveChars(uint8_t font[], char chars[], int8_t spacing, uint8_t toNumber) {
  int16_t out = 0;
  for (uint8_t i = 0; i < toNumber; i++) {
    out += charWidth(font, chars[i]) + spacing;
  }
  return out;
}

bool scrollString(uint8_t font[], char input[], uint8_t spacing, int8_t xOrigin, int8_t yOrigin, uint8_t scrollSpeed, uint32_t rgb) {
  uint32_t CMill = millis();
  uint8_t StringLenght = strlen(input);
  int16_t scrollWidth = 0;
  for (uint8_t i = 0; i < StringLenght; i++) {
    scrollWidth += charWidth(font, input[i]);
    scrollWidth += spacing;
  }
  scrollWidth -= spacing;
  scrollWidth += 9;
  if (CMill - scrollStringPMill >= scrollSpeed * 10) {
    scrollStringPMill = CMill;
    if (scrollStringPosition >= (scrollWidth * (-1)) - 9) {
      scrollStringPosition--;
      return 0;
    }
    else {
      scrollStringPosition = 0;
      return 1;
    }
  }
  int16_t positionAdjusted = scrollStringPosition + 9;
  clearDisplay();
  for (uint8_t i = 0; i < StringLenght; i++) {
    int8_t xPosition = moveChars(font, input, spacing,  i) + positionAdjusted + xOrigin;
    setCharsGLCDCompataible(font, input[i], xPosition, yOrigin, 0, rgb); //I tried optimizing the offscreen characters but it doesnt seem to work
  }
}

void setNumber(uint16_t input) {
  clearDisplayFull();

  char buf[4];
  sprintf(buf, "%03d", input);

  setCharsGLCDCompataible(FontNumbers3x6v1, buf[2], 6, 1, 1, 0x0000ff);
  setCharsGLCDCompataible(FontNumbers3x6v1, buf[1], 3, 1, 1, 0x00ff00);
  setCharsGLCDCompataible(FontNumbers3x6v1, buf[0], 0, 1, 1, 0xff0000);
}

void setRectangle(int8_t xOrigin, int8_t yOrigin, uint8_t width, uint8_t height, bool controlLeds, uint32_t rgb) {
  DisableControlLeds = controlLeds; //disable control LEDs when it goes over them
  for (uint8_t h = 0; h < height; h++) {
    for (uint8_t w = 0; w < width; w++) {
      if (xOrigin + w < 8 + controlLeds && xOrigin + w >= 0 && yOrigin + h < 8 && yOrigin + h >= 0) {
        leds[ledMatrix[xOrigin + w][yOrigin + h]] = rgb;
      }
    }
  }
}

void setImage(uint32_t image[], int8_t xOrigin, int8_t yOrigin, bool controlLeds) {
  DisableControlLeds = controlLeds; //disable control LEDs when it goes over them
  uint8_t width = pgm_read_dword(image);
  uint8_t height = pgm_read_dword(image + 1);
  for (uint8_t h = 0; h < height; h++) {
    for (uint8_t w = 0; w < width; w++) {
      if (xOrigin + w < 8 + controlLeds && xOrigin + w >= 0 && yOrigin + h < 8 && yOrigin + h >= 0) {
        leds[ledMatrix[xOrigin + w][yOrigin + h]] = pgm_read_dword(image + (w + (h * width) + 2));
      }
    }
  }
}

bool readRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  bool out = 0;
  for (uint8_t i = 0; i < width * height; i++) {
    if ((buttonState[buttonMatrix[x + (i - ((i / width) * width))][y + i / width]])) {
      out = 1;
      break;
    }
  }
  return out;
}

void showLed(int brightness, int fps) {
  uint32_t Cmill = millis();
  if (Cmill - showLedPMill >= 1000 / fps) {
    showLedPMill = Cmill;
    ledUI(displayButtonMode);
    helpInfo();
    if (!DisableControlLeds) controlLEDs();
    DisableControlLeds = 0;
    LEDS.setBrightness(brightness);
    FastLED.show();
  }
}

void comm(uint8_t pos) {
  digitalWrite(commA, bitRead(pos, 0));
  digitalWrite(commB, bitRead(pos, 1));
  digitalWrite(commC, bitRead(pos, 2));
  switch (pos) {
    case 0:
      temperatureSens = map(analogRead(anaMux), 0, 1023, -500, 1500) / 10.0;
      break;
    case 1:
      brightnessAdj = map(analogRead(anaMux), 0, 1023, 0, 50);
      break;
    case 2:
      temperatureAdj = map(analogRead(anaMux), 0, 1023, 30, 70);
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

void clockBar() {
  uint8_t clockBarstep = masterClock - ((masterClock / 8) * 8); //deals with the value being bigger than 7
  if (channelZoom == 0) {
    for (uint8_t i = 0; i < 8; i++) {
      leds[ledMatrix[clockBarstep][i]] /= 3;
      if (i <= masterClock / 8) leds[ledMatrix[clockBarstep][i]] += 0x0000b0;
      else leds[ledMatrix[clockBarstep][i]] += 0xff0000;
    }
  }
  else {
    leds[ledMatrix[masterClock - ((masterClock / 8) * 8)][masterClock / 8]] /= 3;
    leds[ledMatrix[masterClock - ((masterClock / 8) * 8)][masterClock / 8]] += 0xff0000;
  }
}

void buttonRead(uint8_t pos) {
  buttonState[buttonMatrix[map(pos - ((pos / 8) * 8), 0, 7, 7, 0)][pos / 8]] = digitalRead(buttonColOut);
}

void gate2Trigger() {
  uint32_t CMill = millis();

  if (gate2TriggerClockToggle) {
    for (uint8_t i = 0; i < 8; i++) bitWrite(jackState, i, bitRead(currentSequence[i], masterClock));
    gate2triggerPMill = CMill;
    gate2TriggerClockToggle = 0;
    gate2TriggerTimerRunning = 1;
    bitWrite(controlReg, clockOut, 1);
    shiftRegRefresh();
  }

  if (CMill - gate2triggerPMill >= triggerLenght && gate2TriggerTimerRunning) {
    for (uint8_t i = 0; i < 8; i++) {
      if (!bitRead(gateTriggerChannel, i)) bitWrite(jackState, i, 0);
    }
    gate2TriggerTimerRunning = 0;
    bitWrite(controlReg, clockOut, 0);
    shiftRegRefresh();
  }
}

void col(uint8_t pos) {
  if (pos != verticalBtnScanPrev) {
    verticalBtnScanPrev = pos;
    if (!pos) {
      digitalWrite(buttonRowReset, 1);
      digitalWrite(buttonRowReset, 0);
    }
    digitalWrite(buttonRowPulse, 1);
    digitalWrite(buttonRowPulse, 0);
  }
}

void loadPattern(uint8_t pos) {
  uint64_t buf;
  for (uint8_t i = 0; i < 8; i++) {
    nextSequence[i] = 0;
    for (uint8_t y = 0; y < 8; y++) {
      buf = EEPROM.read((i * 8) + (pos * 129) + y);
      nextSequence[i] |= buf << y * 8;
      buf = EEPROM.read((i * 8) + (pos * 129) + y + 64);
      nextAccents[i] |= buf << y * 8;
      sequenceLenght = EEPROM.read((pos * 129) + 128);
    }
  }
  nextSequenceReady = pos;
}

void moveNextSequenceToCurrentSequence() {
  for (uint8_t i = 0; i < 8; i++) {
    currentSequence[i] = nextSequence[i];
    currentAccents[i] = nextAccents[i];
  }
  nextSequenceReady = 255;
}

void savePattern(uint8_t pos) {
  uint8_t buf;
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t y = 0; y < 8; y++) {
      buf = currentSequence[i] >> y * 8;
      EEPROM.write((i * 8) + (pos * 129) + y, buf);
      buf = currentAccents[i] >> y * 8;
      EEPROM.write((i * 8) + (pos * 129) + y + 64, buf);
      EEPROM.write((pos * 129) + 128, sequenceLenght);
    }
  }
}

void shiftRegRefresh() {
  digitalWrite(regL, 0);
  shiftOut(regD, regC, MSBFIRST, jackState);
  shiftOut(regD, regC, MSBFIRST, controlReg);
  digitalWrite(regL, 1);
}

void dacConfig(bool gain, bool buf, bool vdd, uint8_t ldac) {
  /*DESCRIPTIONS
     GAIN - output max voltage = 2x referece
     BUF - output has buffered reference
     VDD - output uses VDD as voltage reference instead of the dedicated reference input
     LDAC - 00 - LDAC held LOW, output updates with end of dac data transfer
            01 - LDAC held HIGH, output wont react to any dac data change and it will stay latched, default state
            10 - LDAC pulsed from HIGH to LOW, output will be update once and then get latched again, ignoring all dac data
  */
  uint16_t data = 0b1000000000000000;
  bitWrite(data, 5, gain);
  bitWrite(data, 4, gain);
  bitWrite(data, 3, buf);
  bitWrite(data, 2, buf);
  bitWrite(data, 1, vdd);
  bitWrite(data, 0, vdd);
  bitWrite(controlReg, dacCS, 0);
  shiftRegRefresh();
  shiftOut(dacD, regC, MSBFIRST, data >> 8);
  shiftOut(dacD, regC, MSBFIRST, data);
  bitWrite(controlReg, dacCS, 1);
  shiftRegRefresh();
  data = 0b1010000000000000;
  bitWrite(data, 1, bitRead(ldac, 1));
  bitWrite(data, 0, bitRead(ldac, 0));
  bitWrite(controlReg, dacCS, 0);
  shiftRegRefresh();
  shiftOut(dacD, regC, MSBFIRST, data >> 8);
  shiftOut(dacD, regC, MSBFIRST, data);
  bitWrite(controlReg, dacCS, 1);
  shiftRegRefresh();
}

void dacSet(uint8_t channel, uint16_t value) {
  uint16_t data = 0b0000000000000000;
  for (uint8_t i = 0; i < 3; i++) bitWrite(data, i + 12, bitRead(channel, i));
  for (uint8_t i = 0; i < 12; i++) bitWrite(data, i, bitRead(value, i));
  bitWrite(controlReg, dacCS, 0);
  shiftRegRefresh();
  shiftOut(dacD, regC, MSBFIRST, data >> 8);
  shiftOut(dacD, regC, MSBFIRST, data);
  bitWrite(controlReg, dacCS, 1);
  shiftRegRefresh();
}

void dacPowerOff() {
  bitWrite(controlReg, dacCS, 0);
  shiftRegRefresh();
  shiftOut(dacD, regC, MSBFIRST, 0b11110000);
  shiftOut(dacD, regC, MSBFIRST, 0b00000000);
  bitWrite(controlReg, dacCS, 1);
  shiftRegRefresh();
  bitWrite(controlReg, dacCS, 0);
  shiftRegRefresh();
  shiftOut(dacD, regC, MSBFIRST, 0b11111000);
  shiftOut(dacD, regC, MSBFIRST, 0b00000000);
  bitWrite(controlReg, dacCS, 1);
  shiftRegRefresh();
  bitWrite(controlReg, dacCS, 0);
  shiftRegRefresh();
  shiftOut(dacD, regC, MSBFIRST, 0b10100000);
  shiftOut(dacD, regC, MSBFIRST, 0b11111111);
  bitWrite(controlReg, dacCS, 1);
  shiftRegRefresh();
}

void dacPowerOn() {
  bitWrite(controlReg, dacCS, 0);
  shiftRegRefresh();
  shiftOut(dacD, regC, MSBFIRST, 0b11110000);
  shiftOut(dacD, regC, MSBFIRST, 0b00000000);
  bitWrite(controlReg, dacCS, 1);
  shiftRegRefresh();
  bitWrite(controlReg, dacCS, 0);
  shiftRegRefresh();
  shiftOut(dacD, regC, MSBFIRST, 0b11111000);
  shiftOut(dacD, regC, MSBFIRST, 0b00000000);
  bitWrite(controlReg, dacCS, 1);
  shiftRegRefresh();
  bitWrite(controlReg, dacCS, 0);
  shiftRegRefresh();
  shiftOut(dacD, regC, MSBFIRST, 0b10100000);
  shiftOut(dacD, regC, MSBFIRST, 0b00000000);
  bitWrite(controlReg, dacCS, 1);
  shiftRegRefresh();
}

void buttonUI(uint8_t mode) {
  uint32_t CMill = millis();
  uint8_t page = masterClock / 8;
  bool encoderSwitch = digitalRead(encoderS);
  switch (mode) {
    case 0: {//sequencing
      switch (channelZoom) {
        case 0:// 8 steps of 8 channels
          for (uint8_t i = 0; i < 64; i++) {
            if (buttonState[i] != buttonStatePrev[i]) {
              buttonStatePrev[i] = buttonState[i];
              if (buttonState[i]) buttonPMill = CMill;
              if (!buttonState[i] && CMill - buttonPMill < longPressDuration) {
                bitWrite(currentSequence[i - ((i / 8) * 8)], (i / 8) + (page * 8),
                         !bitRead(currentSequence[i - ((i / 8) * 8)], (i / 8) + (page * 8)));
              }
            }
            if (buttonState[i] && CMill - buttonPMill >= longPressDuration) {
              helpInfoText = 1;
            }
          }
          break;
        default: //64 steps of one channel
          for (uint8_t i = 0; i < 64; i++) {
            if (buttonState[i] != buttonStatePrev[i]) {
              buttonStatePrev[i] = buttonState[i];
              if (buttonState[i]) buttonPMill = CMill;
              if (!buttonState[i] && CMill - buttonPMill < longPressDuration) {
                bitWrite(currentSequence[channelZoom - 1], buttonMatrix[i - ((i / 8) * 8)][i / 8],
                         !bitRead(currentSequence[channelZoom - 1], buttonMatrix[i - ((i / 8) * 8)][i / 8]));
              }
            }
            if (buttonState[i] && CMill - buttonPMill >= longPressDuration) {
              helpInfoText = 1;
            }
          }
          break;
      }
    }  break;
    case 1: {//sequence lenght
      for (uint8_t i = 0; i < 64; i++) {
        if (buttonState[i] != buttonStatePrev[i]) {
          buttonStatePrev[i] = buttonState[i];
          if (buttonState[i]) {
            buttonPMill = CMill;
            helpInfoVariable = buttonMatrix[i - ((i / 8) * 8)][i / 8] + 1;
          }
          if (!buttonState[i] && CMill - buttonPMill < longPressDuration) {
            sequenceLenght = buttonMatrix[i - ((i / 8) * 8)][i / 8];
            displayButtonMode = 0;
          }
        }
        if (buttonState[i] && CMill - buttonPMill >= longPressDuration) {
          helpInfoText = 3;
          //helpInfoVariable = i; //mooved up a bit so it has the value of the pressed button and not the constatly changing i
        }
      }
    }  break;
    case 2: {//channel zoom
      for (uint8_t i = 0; i < 8; i++) {
        if (buttonState[i] != buttonStatePrev[i]) {
          buttonStatePrev[i] = buttonState[i];
          if (buttonState[i]) {
            buttonPMill = CMill;
            helpInfoVariable = i + 1;
          }
          if (!buttonState[i] && CMill - buttonPMill < longPressDuration) {
            channelZoom = i + 1;
            displayButtonMode = 0;
          }
        }
        if (buttonState[i] && CMill - buttonPMill >= longPressDuration) {
          helpInfoText = 5;
        }
      }
    }  break;
    case 4: {//clock multiplier
      bool virtualButtonState = readRectangle(0, 0, 3, 4);
      if (virtualButtonState != clockMultipliButtonPrev) {
        clockMultipliButtonPrev = virtualButtonState;
        if (virtualButtonState) buttonPMill = CMill;
        if (clockMultiplier <= 0 && !virtualButtonState && CMill - buttonPMill < longPressDuration) clockMultiplier *= (-1);
      }
      if (virtualButtonState && CMill - buttonPMill >= longPressDuration) {
          helpInfoText = 13;
        }
      virtualButtonState = readRectangle(4, 0, 3, 4);
      if (virtualButtonState != clockDivideButtonPrev) {
        clockDivideButtonPrev = virtualButtonState;
        if (virtualButtonState) buttonPMill = CMill;
        if (clockMultiplier >= 0 && !virtualButtonState && CMill - buttonPMill < longPressDuration) clockMultiplier *= (-1);
      }
      if (virtualButtonState && CMill - buttonPMill >= longPressDuration) {
          helpInfoText = 14;
        }
      int8_t negativeMultiplier;
      if (clockMultiplier >= 0) negativeMultiplier = 1;
      else negativeMultiplier = (-1);
      for (uint8_t i = 0; i < 8; i++) {
        if (buttonState[buttonMatrix[i][7]] != buttonStatePrev[buttonMatrix[i][7]]) {
          buttonStatePrev[buttonMatrix[i][7]] = buttonState[buttonMatrix[i][7]];
          if (buttonState[buttonMatrix[i][7]]) {
            buttonPMill = CMill;
            helpInfoVariable = i;
          }
          if (!buttonState[buttonMatrix[i][7]] && CMill - buttonPMill < longPressDuration) {
            clockMultiplier = (i + 1) * negativeMultiplier;
            displayButtonMode = 0;
          }
        }
        if (buttonState[buttonMatrix[i][7]] && CMill - buttonPMill >= longPressDuration) {
          if (clockMultiplier >= 0) helpInfoText = 15;
          else helpInfoText = 16;
        }
      }
    }break;
    case 5: {//gate trigger selection
      for (uint8_t i = 0; i < 8; i++) {
        if (buttonState[buttonMatrix[0][i]] != buttonStatePrev[buttonMatrix[0][i]]) {
          buttonStatePrev[buttonMatrix[0][i]] = buttonState[buttonMatrix[0][i]];
          if (buttonState[buttonMatrix[0][i]]) {
            buttonPMill = CMill;
            helpInfoVariable = i + 1;
          }
          if (!buttonState[buttonMatrix[0][i]] && CMill - buttonPMill < longPressDuration) {
            bitWrite(gateTriggerChannel, i, 0);
          }
        }
        if (buttonState[buttonMatrix[0][i]] && CMill - buttonPMill >= longPressDuration) helpInfoText = 17;
        
        if (buttonState[buttonMatrix[7][i]] != buttonStatePrev[buttonMatrix[7][i]]) {
          buttonStatePrev[buttonMatrix[7][i]] = buttonState[buttonMatrix[7][i]];
          if (buttonState[buttonMatrix[7][i]]) {
            buttonPMill = CMill;
            helpInfoVariable = i + 1;
          }
          if (!buttonState[buttonMatrix[7][i]] && CMill - buttonPMill < longPressDuration) {
            bitWrite(gateTriggerChannel, i, 1);
          }
        }
        if (buttonState[buttonMatrix[7][i]] && CMill - buttonPMill >= longPressDuration) helpInfoText = 18;
      }
    } break;
    case 6: {//pattern saving
      for (uint8_t w = 0; w < 2; w++) {
        for (uint8_t h = 0; h < 3; h++) {
          if (buttonState[buttonMatrix[w * 2][h + 5]] != buttonStatePrev[buttonMatrix[w * 2][h + 5]]) { //save buttons
            buttonStatePrev[buttonMatrix[w * 2][h + 5]] = buttonState[buttonMatrix[w * 2][h + 5]];
            if (buttonState[buttonMatrix[w * 2][h + 5]]) {
              buttonPMill = CMill;
              helpInfoVariable = w * 2 + (h * 4) + 1;
            }
            if (!buttonState[buttonMatrix[w * 2][h + 5]] && CMill - buttonPMill < longPressDuration) {
              savePattern(w * 2 + (h * 4));
              lastSavedTo = w * 2 + (h * 4);
              displayButtonMode = 0;
            }
          }
          if (buttonState[buttonMatrix[w * 2][h + 5]] && CMill - buttonPMill >= longPressDuration) helpInfoText = 21;
          
          if (buttonState[buttonMatrix[w * 2 + 4][h + 5]] != buttonStatePrev[buttonMatrix[w * 2 + 4][h + 5]]) { //load buttons
            buttonStatePrev[buttonMatrix[w * 2 + 4][h + 5]] = buttonState[buttonMatrix[w * 2 + 4][h + 5]];
            if (buttonState[buttonMatrix[w * 2 + 4][h + 5]]) {
              buttonPMill = CMill;
              helpInfoVariable = w * 2 + (h * 4) + 1;
            }
            if (!buttonState[buttonMatrix[w * 2 + 4][h + 5]] && CMill - buttonPMill < longPressDuration) {
              if (clockRunning) {
                if (nextSequenceReady != (w * 2 + (h * 4))) loadPattern(w * 2 + (h * 4));
                else {
                  moveNextSequenceToCurrentSequence();
                  displayButtonMode = 0;
                }
              }
              else {
                loadPattern(w * 2 + (h * 4));
                moveNextSequenceToCurrentSequence();
                displayButtonMode = 0;
              }
              lastLoadedFrom = w * 2 + (h * 4);
            }
          }
          if (buttonState[buttonMatrix[w * 2 + 4][h + 5]] && CMill - buttonPMill >= longPressDuration) helpInfoText = 22;
        }
      }
    } break;
  }
  if (mode == 0) { //encoder back button
    if (encoderSwitch != encoderSwitchPrev) {
      encoderSwitchPrev = encoderSwitch;
      if (!encoderSwitch) encoderSwitchPMill = CMill;
      if (encoderSwitch && CMill - encoderSwitchPMill < longPressDuration) displayButtonMode = encoderClosedMenu;
    }
    if (!encoderSwitch && CMill - encoderSwitchPMill >= longPressDuration) helpInfoText = 8;
  }
  else {
    if (encoderSwitch != encoderSwitchPrev) {
      encoderSwitchPrev = encoderSwitch;
      if (!encoderSwitch) encoderSwitchPMill = CMill;
      if (encoderSwitch && CMill - encoderSwitchPMill < longPressDuration) {
        encoderClosedMenu = displayButtonMode;
        displayButtonMode = 0;
      }
    }
    if (!encoderSwitch && CMill - encoderSwitchPMill >= longPressDuration) helpInfoText = 6;
  }
}

void ledUI(uint8_t mode) {
  uint32_t CMill = millis();
  uint8_t page = masterClock / 8;
  if (helpInfoText == 0) { //so it doesnt fight with helpInfo
    switch (mode) {
      case 0: {//sequencing
        switch (channelZoom) {
          case 0:// 8 steps of 8 channels
            for (uint8_t w = 0; w < 8; w++) {
              for (uint8_t h = 0; h < 8; h++) {
                leds[ledMatrix[w][h]] = 0x00ff00 * bitRead(currentSequence[h], w + (page * 8));
              }
            }
            break;
          default:
            for (uint8_t w = 0; w < 8; w++) {
              for (uint8_t h = 0; h < 8; h++) {
                leds[ledMatrix[w][h]] = 0x00ff00 * bitRead(currentSequence[channelZoom - 1], w + (h * 8));
              }
            }
            break;
        }
        clockBar();
      } break;
      case 1: {//sequence lenght
        clearDisplay();
        for (uint8_t c = 0; c < 8; c++) {
          for (uint8_t w = 0; w < 8; w++) {
            for (uint8_t h = 0; h < 8; h++) {
              leds[ledMatrix[w][h]] += 0x001500 * bitRead(currentSequence[c], w + (h * 8));
            }
          }
        }
        leds[ledMatrix[masterClock - ((masterClock / 8) * 8)][masterClock / 8]] += 0xb00000;
        leds[ledMatrix[sequenceLenght - ((sequenceLenght / 8) * 8)][sequenceLenght / 8]] += 0xff5100;
      } break;
      case 2: {//channel zoom
        clearDisplay();
        setImage(imageChannelSelectionPage, 1, 0, 0);
        for (uint8_t i = 0; i < 8; i++) {
          leds[ledMatrix[0][i]] = CHSV(i * 31, 255, selectBrightness(channelZoom - 1, i, 120, 255));
        }
      } break;
      case 3: {//adjusting clock BPM
        clearDisplay();
        setNumber(globalInternalBPM);
        if (CMill - displayBPMPMill >= BPMTimeout) displayButtonMode = 0;
      } break;
      case 4: {//adjusting clock multiplier
        clearDisplay();
        setImage(imageSymbolMultiply, 0, 1, 0);
        setImage(imageSymbolDivide, 4, 0, 0);
        uint8_t adjustedClockMultiplier;
        if (clockMultiplier >= 0) {
          setRectangle(0, 4, 4, 1, 0, 0xffffff);
          setRectangle(3, 0, 1, 4, 0, 0xffffff);
          adjustedClockMultiplier = clockMultiplier;
        }
        else {
          setRectangle(4, 4, 4, 1, 0, 0xffffff);
          setRectangle(7, 0, 1, 4, 0, 0xffffff);
          adjustedClockMultiplier = clockMultiplier * (-1);
        }
        for (int8_t i = 7; i > -1; i--) {
          leds[ledMatrix[i][7]] = CHSV(map(i, 0, 7, 7, 0) * 35, 255, selectBrightness(adjustedClockMultiplier - 1, i, 120, 255));
        }
      } break;
      case 5: {//gate trigger selection
        #define inactiveTrigger 0x000f00
        #define activeTrigger 0x00ff00
        #define inactiveGate 0x00000f
        #define activeGate 0x0000ff
        clearDisplay();
        setImage(imageGateTrigger, 1, 1, 0);
        for (uint8_t i = 0; i < 8; i++) {
          if (bitRead(gateTriggerChannel, i)) {
            leds[ledMatrix[0][i]] = inactiveTrigger;
            leds[ledMatrix[7][i]] = activeGate;
          }
          else {
            leds[ledMatrix[0][i]] = activeTrigger;
            leds[ledMatrix[7][i]] = inactiveGate;
          }
        }
      } break;
      case 6: {//pattern saving
        clearDisplay();
        setImage(imageSavePreset, 0, 0, 0);
        for (uint8_t w = 0; w < 2; w++) {
          for (uint8_t h = 0; h < 3; h++) {
            if (lastSavedTo == w * 2 + (h * 4)) leds[ledMatrix[w * 2][h + 5]] = 0xff0000;
            else leds[ledMatrix[w * 2][h + 5]] = 0x300000;
            if (lastLoadedFrom == w * 2 + (h * 4)) {
              if (nextSequenceReady == w * 2 + (h * 4)) leds[ledMatrix[w * 2 + 4][h + 5]] = 0x0000ff * blink40ms();
              else leds[ledMatrix[w * 2 + 4][h + 5]] = 0x0000ff;
            }
            else leds[ledMatrix[w * 2 + 4][h + 5]] = 0x000030;
          }
        }
      } break;
      default: {
        helpInfoText = 250; //requests non-existing help - there for displaying "out of bounds error"
      } break;
    }
  }
}

void clockInterrupt() { //add time measuring to determin the bpm and multiply the clock
  clockInputSpeed = millis() - clockMeasuringPMill; //for clock multiplication
  clockMeasuringPMill = millis();
  if (clockRunning && !clockSource) {
    masterClock++;
    if (masterClock > sequenceLenght) {
      masterClock = 0;
      if (nextSequenceReady != 255) moveNextSequenceToCurrentSequence();
    }
    gate2TriggerClockToggle = 1;
    gate2Trigger();
  }
}

void clockGeneration(bool enable, uint32_t bpm, int8_t math) { //prepares stuff for the clock source
  if (math > 0) bpm *= math; //bpm multipling
  else if (math < 0) bpm /= (math * (-1)); //bpm dividing

  if (bpm > 1500) bpm = 1500;
  if (bpm < 30) bpm = 30;

  uint16_t compareNumber = hardwareClockTimerFrquency / (bpm * (long)256); //the (long) is there to signify it is not an int
  //some code to handle swing
  OCR1A = compareNumber;
  bitWrite(TIMSK1, OCIE1A, enable); //enable timer1 compare interrupt
}

ISR(TIMER1_COMPA_vect) { //hardware timer for internal clock
  if (clockSource) {
    masterClock++;
    if (masterClock > sequenceLenght) {
      masterClock = 0;
      if (nextSequenceReady != 255) moveNextSequenceToCurrentSequence();
    }
    gate2TriggerClockToggle = 1;
    gate2Trigger();
  }
}

void clockResetJacks() {
  if (clockSource != clockResetJacksSourcePrev) {
    clockResetJacksSourcePrev = clockSource;
    bitWrite(controlReg, clockResetSwitch, clockSource);
    shiftRegRefresh();
  }
  
  if (clockSource) {
    bitWrite(controlReg, resetOut, !clockRunning);
    shiftRegRefresh();
  }

  if (digitalRead(resetIn)) masterClock = 63;
}

bool blinkMain() {
  uint32_t CMill = millis();
  if (CMill - blinkMainPMill >= 10) {
    blinkMainPMill = CMill;
    blinkMainCounter++;
    if ((blinkMainCounter & 0b0001) == 0b0001) {
      blink20msState = !blink20msState;
      if ((blinkMainCounter & 0b0011) == 0b0011) {
        blink40msState = !blink40msState;
        if ((blinkMainCounter & 0b0111) == 0b0111) {
          blink80msState = !blink80msState;
          if ((blinkMainCounter & 0b1111) == 0b1111) {
            blink160msState = !blink160msState;
          }
        }
      }
    }
  }
}

bool blink20ms() {
  blinkMain();
  return blink20msState;
}

bool blink40ms() {
  blinkMain();
  return blink40msState;
}

bool blink80ms() {
  blinkMain();
  return blink80msState;
}

bool blink160ms() {
  blinkMain();
  return blink160msState;
}

void controlLEDs() {
  /*COLORS
     inactive, dark default - 0x330030
     active, bright default - 0xff00f2
     shift, bright orange - 0xff5900
     shift inactive, dark red pink - 0x3d000f
     shift active, bright red pink - 0xff0040
     play, yellow green - 0xccff00
  */
#define inactiveRegular 0x330030
#define activeRegular 0xff00f2
#define shift 0xff5900
#define inactiveShift 0x3d000f
#define activeShift 0xff0040
#define play 0xccff00
  for (uint8_t i = 0; i < 8; i++) leds[controlLeds[i]] = inactiveRegular; //set all leds to the base color before aditing
  if (menuButtonShiftActive != menuButtonShiftActivePrev) {
    menuButtonShiftActivePrev = menuButtonShiftActive;
    if (menuButtonShiftActive) for (uint8_t i = 0; i < 8; i++) leds[controlLeds[i]] = inactiveShift;
    else for (uint8_t i = 0; i < 8; i++) leds[controlLeds[i]] = inactiveRegular;
  }
  switch (menuButtonShiftActive) {
    case 0:
      for (uint8_t i = 0; i < 8; i++) leds[controlLeds[i]] = inactiveRegular; //reset all leds to default color
      
      if (displayButtonMode == 1) leds[controlLeds[0]] = activeRegular; //editing sequence lenght
      else leds[controlLeds[0]] = inactiveRegular;
      
      if (clockRunning) leds[controlLeds[5]] = play * blink20ms(); //play / pause button
      else leds[controlLeds[5]] = play * blink80ms();

      if (displayButtonMode == 0 || displayButtonMode == 2) { //channel zoom indication
        if (channelZoom == 0) {
          leds[controlLeds[1]] = inactiveRegular;
          leds[controlLeds[2]] = activeRegular;
        }
        else {
          leds[controlLeds[1]] = CHSV((channelZoom - 1) * 31, 255, 255);
          leds[controlLeds[2]] = inactiveRegular;
        }
      }
      else {
         if (channelZoom == 0) {
          leds[controlLeds[1]] = inactiveRegular;
          leds[controlLeds[2]] = inactiveRegular;
        }
        else {
          leds[controlLeds[1]] = CHSV((channelZoom - 1) * 31, 255, 150);
          leds[controlLeds[2]] = inactiveRegular;
        }
      }
      
      break;
    case 1:
      for (uint8_t i = 0; i < 8; i++) leds[controlLeds[i]] = inactiveShift; //reset all leds to default color

      if (clockSource && displayButtonMode == 0) leds[controlLeds[0]] = activeShift; //internal / external clock source
      else leds[controlLeds[0]] = inactiveShift;

      uint8_t adjustedClockMultiplier; //clock multiplier
      if (clockMultiplier < 0) adjustedClockMultiplier = clockMultiplier * (-1);
      else adjustedClockMultiplier = clockMultiplier;
      leds[controlLeds[2]] = CHSV(map(adjustedClockMultiplier, 1, 8, 244, 0), 255, selectBrightness(displayButtonMode, 4, 150, 255));

      leds[controlLeds[4]] = (displayButtonMode == 5) ? activeShift : inactiveShift; //gate trigger selection

      leds[controlLeds[7]] = (displayButtonMode == 6) ? activeShift : inactiveShift; //pattern saving menu
      break;
  }
  if (menuButtonShiftActive) leds[controlLeds[6]] = shift;
  else leds[controlLeds[6]] = inactiveRegular;
}

void modeLoading() {
  globalInternalBPM = EEPROM.read(EEPROMBPM);
  lastSavedTo = EEPROM.read(EEPROMLastSavedTo);
  lastLoadedFrom = EEPROM.read(EEPROMLastLoadedFrom);
  clockMultiplier = EEPROM.read(EEPROMClockMultiplier) - 100;
  clockSource = EEPROM.read(EEPROMClockSource);
  gateTriggerChannel = EEPROM.read(EEPROMGateTrigger);
}

void modeSaving() {
  if (displayButtonMode != modeSavingPrev) {
    if (modeSavingPrev != 0) {
      switch (modeSavingPrev) {
        case 3:
          EEPROM.write(EEPROMBPM, globalInternalBPM);
          break;
        case 6:
          EEPROM.write(EEPROMLastSavedTo, lastSavedTo);
          EEPROM.write(EEPROMLastLoadedFrom, lastLoadedFrom);
          break;
        case 4:
          EEPROM.write(EEPROMClockMultiplier, clockMultiplier + 100);
          break;
        case 5:
          EEPROM.write(EEPROMGateTrigger, gateTriggerChannel);
          break;
      }
    }
    modeSavingPrev = displayButtonMode;
  }
}

void EEPROMInit() {
  for (uint16_t i = 0; i < 1000; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.write(EEPROMBPM, 120);
  EEPROM.write(EEPROMLastSavedTo, 0);
  EEPROM.write(EEPROMLastLoadedFrom, 0);
  EEPROM.write(EEPROMClockMultiplier, 101);
  EEPROM.write(EEPROMClockSource, 0);
  EEPROM.write(EEPROMGateTrigger, 0);
  EEPROM.write(EEPROMUsedCheck, 17);
}

void controlButtons() {
  uint32_t CMill = millis();
  if (menuButton[6] != menuButtonLast[6]) {//shift
    menuButtonLast[6] = menuButton[6];
    if (menuButton[6]) menuButtonPMill = CMill;
    if (!menuButton[6] && CMill - menuButtonPMill < longPressDuration) {
      menuButtonShiftActive = !menuButtonShiftActive;
      displayButtonMode = 0;
    }
  }
  if (menuButton[6] && CMill - menuButtonPMill >= longPressDuration) {
    helpInfoText = 9;
  }

  switch (menuButtonShiftActive) {
    case 0:
      if (menuButton[0] != menuButtonLast[0]) {//sequence lenght
        menuButtonLast[0] = menuButton[0];
        if (menuButton[0]) menuButtonPMill = CMill;
        if (!menuButton[0] && CMill - menuButtonPMill < longPressDuration) {
          if (displayButtonMode != 1) displayButtonMode = 1;
          else displayButtonMode = 0;
        }
      }
      if (menuButton[0] && CMill - menuButtonPMill >= longPressDuration) {
        helpInfoText = 2;
      }

      if (menuButton[1] != menuButtonLast[1]) {//1 channel view
        menuButtonLast[1] = menuButton[1];
        if (menuButton[1]) menuButtonPMill = CMill;
        if (!menuButton[1] && CMill - menuButtonPMill < longPressDuration) {
          if (!channelZoom) channelZoom = buttonUIChannelZoomBeforeZero;
          displayButtonMode = (displayButtonMode != 2) ? 2 : 0;
        }
      }
      if (menuButton[1] && CMill - menuButtonPMill >= longPressDuration) {
        helpInfoText = 4;
      }

      if (menuButton[2] != menuButtonLast[2]) {//all channel view
        menuButtonLast[2] = menuButton[2];
        if (menuButton[2]) menuButtonPMill = CMill;
        if (!menuButton[2] && CMill - menuButtonPMill < longPressDuration) {
          displayButtonMode = 0;
          if (channelZoom != 0) buttonUIChannelZoomBeforeZero = channelZoom;
          channelZoom = 0;
        }
      }
      if (menuButton[2] && CMill - menuButtonPMill >= longPressDuration) {
        helpInfoText = 7;
      }

      if (menuButton[3] != menuButtonLast[3]) {
        menuButtonLast[3] = menuButton[3];
      }

      if (menuButton[4] != menuButtonLast[4]) {
        menuButtonLast[4] = menuButton[4];
      }

      if (menuButton[5] != menuButtonLast[5]) {//start / stop clock
        menuButtonLast[5] = menuButton[5];
        if (menuButton[5]) menuButtonPMill = CMill;
        if (!menuButton[5] && CMill - menuButtonPMill < longPressDuration) {
          clockRunning = !clockRunning;
          if (clockRunning) masterClock = 63; //set the clock to the last step so the clock interrupt starts the sequence from the first step
        }
      }
      if (menuButton[5] && CMill - menuButtonPMill >= longPressDuration) {
        helpInfoText = 10;
        helpInfoVariable = clockRunning;
      }

      if (menuButton[7] != menuButtonLast[7]) {
        menuButtonLast[7] = menuButton[7];
      }
      break;
    case 1:
      if (menuButton[0] != menuButtonLast[0]) {//clock source selection
        menuButtonLast[0] = menuButton[0];
        if (menuButton[0]) menuButtonPMill = CMill;
        if (!menuButton[0] && CMill - menuButtonPMill < longPressDuration) {
          clockSource = !clockSource;
          EEPROM.write(EEPROMClockSource, clockSource);
          displayButtonMode = 0;
        }
      }
      if (menuButton[0] && CMill - menuButtonPMill >= longPressDuration) {
        helpInfoText = 11;
        helpInfoVariable = clockSource;
      }

      if (menuButton[2] != menuButtonLast[2]) {//clock division / multiplication
        menuButtonLast[2] = menuButton[2];
        if (menuButton[2]) menuButtonPMill = CMill;
        if (!menuButton[2] && CMill - menuButtonPMill < longPressDuration) {
          displayButtonMode = (displayButtonMode != 4) ? 4 : 0;
        }
      }
      if (menuButton[2] && CMill - menuButtonPMill >= longPressDuration) {
        helpInfoText = 12;
        helpInfoVariable = clockSource;
      }

      if (menuButton[4] != menuButtonLast[4]) {//gate trigger selection
        menuButtonLast[4] = menuButton[4];
        if (menuButton[4]) menuButtonPMill = CMill;
        if (!menuButton[4] && CMill - menuButtonPMill < longPressDuration) {
          displayButtonMode = (displayButtonMode != 5) ? 5 : 0;
        }
      }
      if (menuButton[4] && CMill - menuButtonPMill >= longPressDuration) {
        helpInfoText = 19;
        helpInfoVariable = clockSource;
      }
      
      if (menuButton[7] != menuButtonLast[7]) {//save load pattern
        menuButtonLast[7] = menuButton[7];
        if (menuButton[7]) menuButtonPMill = CMill;
        if (!menuButton[7] && CMill - menuButtonPMill < longPressDuration) {
          displayButtonMode = (displayButtonMode != 6) ? 6 : 0;
        }
      }
      if (menuButton[7] && CMill - menuButtonPMill >= longPressDuration) {
        helpInfoText = 20;
        helpInfoVariable = clockSource;
      }
      break;
  }
}

uint8_t selectBrightness(uint8_t value, uint8_t light, uint8_t dimValue, uint8_t lightValue) {
  if (value != light) return dimValue;
  else return lightValue;
}

void helpInfo() {
  uint32_t CMill = millis();
  #define helpTextColor 0xbff9d00

  if (helpInfoText != helpInfoTextPrev) {
    helpInfoTextPrev = helpInfoText;
    if (helpInfoText != 0) {
      scrollStringPosition = 0;
      scrollStringPMill = CMill;
    }
    else clearDisplay();
  }

  switch (helpInfoText) {
    case 0:
      break;
    case 1: { //toggle sequence step
      scrollString(FontArial7x8v1, "Toggle this step.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 2: { //sequence lenght
      scrollString(FontArial7x8v1, "Set sequence lenght.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 3: { //sequence lenght
      char buf[37];
      sprintf(buf, "Set sequence lenght to %d.", helpInfoVariable);
      scrollString(FontArial7x8v1, buf, 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 4: { //zoom one channel
      scrollString(FontArial7x8v1, "See all steps of a channel.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 5: { //zoom select one channel
      char buf[22];
      sprintf(buf, "Select channel %d.", helpInfoVariable);
      scrollString(FontArial7x8v1, buf, 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 6: { //accept and close
      scrollString(FontArial7x8v1, "Accept and close.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 7: { //zoom all channels
      scrollString(FontArial7x8v1, "See all channels.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 8: { //encoder reopen menu
      scrollString(FontArial7x8v1, "Reopen last closed menu.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 9: { //shift
      scrollString(FontArial7x8v1, "Enable alternative button functions.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 10: { //clock start stop
      if (helpInfoVariable) scrollString(FontArial7x8v1, "Stop clock.", 1, 0, 0, 10, helpTextColor);
      else scrollString(FontArial7x8v1, "Start clock.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 11: { //select internal external clock
      if (helpInfoVariable) scrollString(FontArial7x8v1, "Use external clock.", 1, 0, 0, 10, helpTextColor);
      else scrollString(FontArial7x8v1, "Use internal clock.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 12: { //clock multiplier
      scrollString(FontArial7x8v1, "Multiply or divide clock speed.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 13: { //multiply clock
      scrollString(FontArial7x8v1, "Multiply clock speed.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 14: { //divide clock
      scrollString(FontArial7x8v1, "Divide clock speed.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 15: { //multiply clock by
      char buf[22];
      sprintf(buf, "Multiply clock by %d.", helpInfoVariable);
      scrollString(FontArial7x8v1, buf, 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 16: { //divide clock by
      char buf[22];
      sprintf(buf, "Divide clock by %d.", helpInfoVariable);
      scrollString(FontArial7x8v1, buf, 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 17: { //set channel to trig
      char buf[31];
      sprintf(buf, "Set channel %d to TRIGGER mode.", helpInfoVariable);
      scrollString(FontArial7x8v1, buf, 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 18: { //set channel to gate
      char buf[28];
      sprintf(buf, "Set channel %d to GATE mode.", helpInfoVariable);
      scrollString(FontArial7x8v1, buf, 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 19: { //trig gate selection menu
      scrollString(FontArial7x8v1, "Choose TRIGGER or GATE for each output.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 20: { //pattern saving menu menu
      scrollString(FontArial7x8v1, "Save / Load a pattern.", 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 21: { //save to position
      char buf[20];
      sprintf(buf, "Save pattern to %d.", helpInfoVariable);
      scrollString(FontArial7x8v1, buf, 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    case 22: { //load from position
      char buf[22];
      sprintf(buf, "Load pattern from %d.", helpInfoVariable);
      scrollString(FontArial7x8v1, buf, 1, 0, 0, 10, helpTextColor);
      helpInfoText = 0;
    } break;
    default: { //out of bounds error
      char buf[37];
      sprintf(buf, "OOB help message ERROR! Dev code %d. Please contact Marek Mach.", helpInfoText);
      scrollString(FontArial7x8v1, buf, 1, 0, 0, 15, 0xff0000);
      helpInfoText = 0;
    } break;
  }
}

void rotate() {
  uint32_t CMill = millis();
  bool encoderAState = digitalRead(encoderA);
  bool encoderBState = digitalRead(encoderB);
  switch (displayButtonMode) {
    case 0: //default
    case 3: { //editing clock bpm
      if (encoderAState == 0 && encoderBState == 1) {
        if (globalInternalBPM < 250) globalInternalBPM++;
      }
      else if (encoderAState == 0 && encoderBState == 0) {
        if (globalInternalBPM > 30) globalInternalBPM--;
      }
      displayBPMPMill = CMill;
      displayButtonMode = 3;
    } break;
    case 1: { //sequence lenght
      if (encoderAState == 0 && encoderBState == 1) {
        if (sequenceLenght < 63) sequenceLenght++;
      }
      else if (encoderAState == 0 && encoderBState == 0) {
        if (sequenceLenght > 0) sequenceLenght--;
      }
    } break;
    case 2: { //channel zoom
      if (encoderAState == 0 && encoderBState == 1) {
        if (channelZoom < 8) channelZoom++;
      }
      else if (encoderAState == 0 && encoderBState == 0) {
        if (channelZoom > 1) channelZoom--;
      }
    } break;
    case 4: { //clock multiplier
      if (encoderAState == 0 && encoderBState == 1) {
        if (clockMultiplier < 8) clockMultiplier++;
        if (clockMultiplier == 0) clockMultiplier = 1;
      }
      else if (encoderAState == 0 && encoderBState == 0) {
        if (clockMultiplier > -8) clockMultiplier--;
        if (clockMultiplier == 0) clockMultiplier = -1;
      }
    } break;
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
  pinMode(encoderS, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderA), rotate, CHANGE);
  attachInterrupt(digitalPinToInterrupt(clockInPin), clockInterrupt, RISING);
  digitalWrite(buttonRowReset, 1);
  digitalWrite(buttonRowReset, 0);
  dacPowerOn();
  dacConfig(0, 1, 0, 0b00);

  if (EEPROM.read(EEPROMUsedCheck) != 17) EEPROMInit(); //initializes the whole EEPROM if it doesnt find 17 in the last byte, SLOW!!
  modeLoading();
  loadPattern(lastSavedTo);
  moveNextSequenceToCurrentSequence();

  //clockInterruptSetup - BEGIN
  TCCR1A = 0; //reset timer1 control register A
  TCCR1B = 0; //reset timer1 control register B

  bitWrite(TCCR1B, CS12, 1); //set the prescaler to divide by 256
  bitWrite(TCCR1B, CS11, 0);
  bitWrite(TCCR1B, CS10, 0);
  bitWrite(TCCR1B, WGM12, 1); //set the timer to CTC (clear timer on compare) mode
  TCNT1 = 0; //counter starting value
  OCR1A = (60 * F_CPU) / (120 * 256); //compare interrupt, 120 is the BPM
  bitWrite(TIMSK1, OCIE1A, clockRunning); //enable timer1 compare interrupt
  sei(); //enable global interrupts
  //clockInterruptSetup - END

  Serial.begin(1000000);

  //led_test_start---------------------
  LEDS.setBrightness(80);
  uint8_t hue = 0;
  for (int i = 0; i < 72; i++) {
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
  LEDS.setBrightness(255);
  for (uint8_t i = 0; i < 8; i++) {
    leds[controlLeds[i]] = 0xff00f2;
    FastLED.show();
    delay(20);
  }
  //led_test_end----------------------
  for (uint8_t i = 0; i < 8; i++) leds[controlLeds[i]] = 0x330030; //set all control leds to dark

  while (!digitalRead(encoderS)) {
    scrollString(FontArial7x8v1, SYSfirmwareVersion, 1, 0, 0, 10, 0x00ff00);
    FastLED.show();
  }
}

void loop() {
  testMeasuringPMill = micros();
  if (counter > 63) counter = 0; //counter reset
  comm(counter - ((counter / 8) * 8));
  col(counter / 8);
  clockGeneration(clockRunning, globalInternalBPM, clockMultiplier);
  buttonRead(counter);
  controlButtons();
  counter++;
  gate2Trigger();
  clockResetJacks();
  modeSaving();
  buttonUI(displayButtonMode);
  showLed(brightnessAdj, 30);
  Serial.println(micros() - testMeasuringPMill); //code speed debug
}
