#include <Time.h>
#include <TimeLib.h>
#include <math.h>

#include <LCDi2cR.h>
#include <XBee.h>
#include <Wire.h>
#define TMP_ADDRESS 0x48

const int interval = 1000;
bool AT = true; //set to true if XBee is configured in AT mode, false otherwise

//LCD
LCDi2cR lcd = LCDi2cR(4,20,0x63,0);

XBee xbee = XBee();
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4098DA0A);

void setup() {
  lcd.init();
  Wire.begin();
  Serial2.begin(9600);
  xbee.setSerial(Serial2);
  Serial.begin(9600);
}

unsigned long t, t_key;

void loop() {
  //RX
  int tempReceived;
  long pressure;
  int x, y, z;
  int humidity;

  //TX
  int tempSent;
  int keyInput;

  int rx;
  
  keyInput = lcd.keypad();
  if (keyInput != 32) { //32 is the default value received from the keypad when no keys are pressed
    if (!AT) TX(0, keyInput);
    else AT_TX(0, keyInput);
    
    TMP(&tempSent);
    printLCD_TX(tempSent, keyInput);
    delay(200);
    t_key = millis();
  }
  
  else if (millis() - t > interval) {
    TMP(&tempSent);
    if (AT) {
      AT_TX(1, tempSent);
    }
    else TX(1, tempSent);
    t = millis();
  }

  if (AT) rx = AT_RX(&tempReceived, &pressure, &x, &y, &z, &humidity);
  else rx = RX(&tempReceived, &pressure, &x, &y, &z, &humidity);
  if (rx) {
    if (millis() - t_key > 1000)
    {
      printLCD_RX(tempReceived, pressure, x, y, z, humidity);
    }
  }
  delay(5);
}
