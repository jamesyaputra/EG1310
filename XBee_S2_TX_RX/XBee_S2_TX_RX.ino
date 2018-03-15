#include <Time.h>
#include <TimeLib.h>

#include <Wire.h>
#include <XBee.h>


//HMC

#define HMC_address 0x1E //0011110b, I2C 7bit address of HMC5883

//

bool AT = 1;  //1 for AT, 0 for API

// create the XBee object

XBee xbee = XBee();  
XBeeAddress64 addr64 = XBeeAddress64(0x0013A200, 0x4098D965);   // address of the receiver XBee

int statusLed = 10; // green
int errorLed  = 11; // red
int dataLed   = 12; // yellow
int led       = 13;

void setup()
{
  Serial.begin(9600);
  
  //XBee
  Serial2.begin(9600);
  xbee.setSerial(Serial2);
  
  //BMP
  Wire.begin();
  bmp085Calibration();

  //HMC
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(HMC_address); //open communication with HMC5883
  Wire.write((byte)0x02); //select mode register
  Wire.write((byte)0x00); //continuous measurement mode
  Wire.endTransmission();
}

void loop()
{
  //Sensor variables
  static short BMP_temp;
  static long BMP_pres;
  static int HMC_x, HMC_y, HMC_z;
  static int HIH_value;
  static int TMP_RX, keypad_RX;

  //Boolean to denote if there is recived data
  static bool rx = 0;
  
  if (AT){          //recieve using AT
    rx = AT_RX(&TMP_RX, &keypad_RX);    //returns 1 when there is recieved data else 0
  }
  else{             //recieve using API
    rx = RX(&TMP_RX, &keypad_RX);       //returns 1 when there is recieved data else 0
  }
  if(rx)            //If there is recieved data
  {
    if(TMP_RX == 0) //If recieved value is not temperature reading
    {
      Serial.print("Keypad pressed: ");
      if (keypad_RX >= 0)
      {
        Serial.println(keypad_RX);
      }
      else if (keypad_RX == -6)
      {
        Serial.println("*");
      }
      else if (keypad_RX == -13)
      {
        Serial.println("#");
      }
    }
    else
    {
      Serial.println(TMP_RX*0.0625);
      
      //Only obtain and send sensor readings when required to conserve energy
      BMP(&BMP_temp, &BMP_pres);    //Obtain BMP reading
      HMC(&HMC_x, &HMC_y, &HMC_z);  //Obtain HMC reading
      HIH(&HIH_value);              //Obtain HIH reading
      
      if(AT){     //send using AT
        AT_TX(BMP_temp, BMP_pres, HMC_x, HMC_y, HMC_z, HIH_value);
      }
      else{       //send using API
        TX(BMP_temp, BMP_pres, HMC_x, HMC_y, HMC_z, HIH_value);
      }
    }
  }
  /*
  Serial.println(BMP_temp);
  Serial.println(BMP_pres);
  Serial.println(HMC_x);
  Serial.println(HMC_y);
  Serial.println(HMC_z);
  Serial.println(HIH_value);
  */
  delay(5);       //timekeeping is done on system 1, system 2 only replies with sensor data after recieving data from system 1
}
