// ---------------------------------------------------------------------------------------
// -- INFO
// ---------------------------------------------------------------------------------------
// USB Type: Serial + Midi + Audio

// ---------------------------------------------------------------------------------------
// -- inlcudes
// ---------------------------------------------------------------------------------------
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// ---------------------------------------------------------------------------------------
// GUItool: begin automatically generated code
AudioOutputI2S i2s1;
AudioSynthWaveformSine oTone;          //xy=284,370
AudioInputUSB oUsbIn;           //xy=680,372
AudioOutputUSB oUsbOut;           //xy=680,372
AudioConnection patchCordLeft(oTone, 0, oUsbOut, 0);
AudioConnection patchCordRight(oTone, 0, oUsbOut, 1);
AudioConnection patchCordLeftI2S(oTone, 0, i2s1, 0);
AudioConnection patchCordRightI2S(oTone, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;     //xy=314,290

// GUItool: end automatically generated code
// ---------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------
// -- PINS
// ---------------------------------------------------------------------------------------
#define PINLED 13
#define PINKEY 24
#define PINVOLUP 4 // TODO
#define PINVOLDN 2
#define PINFREQUP 3
#define PINFREQDN 5
#define PINLENUP 6
#define PINLENDN 11
#define PINDIT 7
#define PINDAH 9

// ---------------------------------------------------------------------------------------
// -- Globale Variables
// ---------------------------------------------------------------------------------------
#define VOLSTEP 0.1
#define FREQSTEP 50
#define LENSTEP 5

int g_iDitLen = 100;

// Tone values
#define PHASE 0
int g_iFreq = 800;
float g_fLvl = 0.5;

// ---------------------------------------------------------------------------------------
// set LED on/off
// ---------------------------------------------------------------------------------------
void setLedOn()
{
  digitalWrite(PINLED, HIGH);
}

void setLedOff()
{
  digitalWrite(PINLED, LOW);
}

// ---------------------------------------------------------------------------------------
// set tone on/off
// ---------------------------------------------------------------------------------------
void setToneOff()
{
  setLedOff();
  oTone.frequency(g_iFreq);
  oTone.amplitude(0.0);
}

void setToneOn()
{
  setLedOn();
  oTone.frequency(g_iFreq);
  oTone.amplitude(g_fLvl);
}

// ---------------------------------------------------------------------------------------
// Paddle specific functions
// ---------------------------------------------------------------------------------------
void sendDit()
{
  setToneOn();
  delay(g_iDitLen);
  setToneOff();
}

void sendDah()
{
  setToneOn();
  delay(3 * g_iDitLen);
  setToneOff();
}

// ---------------------------------------------------------------------------------------
// -- Set Vol, Freq, Len
// ---------------------------------------------------------------------------------------
void setValues()
{
  bool bChange = false;
  
  // -- Volume
  if ((!digitalRead(PINVOLUP)) && (g_fLvl < 1.0)) {
    g_fLvl += VOLSTEP;
    bChange = true;
  }
  if ((!digitalRead(PINVOLDN)) && (g_fLvl > 0.0)) {
    g_fLvl -= VOLSTEP;
    bChange = true;
  }

  // -- Frequency
  if ((!digitalRead(PINFREQUP)) && (g_iFreq < 800)) {
    g_iFreq += FREQSTEP;
    bChange = true;
  }
  if ((!digitalRead(PINFREQDN)) && (g_iFreq > 300)) {
    g_iFreq -= FREQSTEP;
    bChange = true;
  }

  // -- Dit Length
  if ((!digitalRead(PINLENUP)) && (g_iDitLen < 300)) {
    g_iDitLen += LENSTEP;
    bChange = true;
  }
  if ((!digitalRead(PINLENDN)) && (g_iDitLen > 5)) {
    g_iDitLen -= LENSTEP;
    bChange = true;
  }

  if (bChange) {
    delay(100);
  }
}

// ---------------------------------------------------------------------------------------
// -- Setup/Init
// ---------------------------------------------------------------------------------------
void setup()
{
  delay(250);
  AudioMemory(12);
  delay(250);
  
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.volume(0.5);
  
  setToneOff();

  pinMode(PINLED, OUTPUT);
  digitalWrite(PINLED, LOW);

  pinMode(PINKEY, INPUT_PULLUP);
  digitalWrite(PINKEY, HIGH);

  pinMode(PINVOLUP, INPUT_PULLUP);
  digitalWrite(PINVOLUP, HIGH);

  pinMode(PINVOLDN, INPUT_PULLUP);
  digitalWrite(PINVOLDN, HIGH);

  pinMode(PINFREQUP, INPUT_PULLUP);
  digitalWrite(PINFREQUP, HIGH);

  pinMode(PINFREQDN, INPUT_PULLUP);
  digitalWrite(PINFREQDN, HIGH);

  pinMode(PINLENUP, INPUT_PULLUP);
  digitalWrite(PINLENUP, HIGH);

  pinMode(PINLENDN, INPUT_PULLUP);
  digitalWrite(PINLENDN, HIGH);

  pinMode(PINDIT, INPUT_PULLUP);
  digitalWrite(PINDIT, HIGH);

  pinMode(PINDAH, INPUT_PULLUP);
  digitalWrite(PINDAH, HIGH);

}

// ---------------------------------------------------------------------------------------
// -- Loops
// ---------------------------------------------------------------------------------------
void loopPaddle()
{
  if (!digitalRead(PINDIT)) {
    sendDit();
  }
  if (!digitalRead(PINDAH)) {
    sendDah();
  }
  delay(g_iDitLen);
}

void loopStraight()
{
  if ((!digitalRead(PINDIT)) || (!digitalRead(PINDAH))) {
    setToneOn();
  }
  else {
    setToneOff();
  }
}

// ---------------------------------------------------------------------------------------
// -- Main Loop
// ---------------------------------------------------------------------------------------
void loop()
{
  
  if (!digitalRead(PINKEY)) {
    loopStraight();
  }
  else {
    loopPaddle();
  }

  setValues();
}
