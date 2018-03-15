void AT_TX(short BMP_temp, long BMP_pres, int HMC_x, int HMC_y, int HMC_z, int HIH_value){
  Serial2.write(BMP_temp >> 8 & 0xff);
  Serial2.write(BMP_temp & 0xff);
  
  Serial2.write(BMP_pres >> 24 & 0xff);
  Serial2.write(BMP_pres >> 16 & 0xff);
  Serial2.write(BMP_pres >> 8 & 0xff);
  Serial2.write(BMP_pres & 0xff);
  
  Serial2.write(HMC_x >> 8 & 0xff);
  Serial2.write(HMC_x & 0xff);
  
  Serial2.write(HMC_y >> 8 & 0xff);
  Serial2.write(HMC_y & 0xff);
  
  Serial2.write(HMC_z >> 8 & 0xff);
  Serial2.write(HMC_z & 0xff);
  
  Serial2.write(HIH_value >> 8 & 0xff);
  Serial2.write(HIH_value & 0xff);
}
bool AT_RX(int *TMP_value, int *keypad){
  if (Serial2.available() >= 2){
    int value = Serial2.read() << 8 | Serial2.read();
    if (value < 0x4000)               //Second MSBit used to denote if data is keypad is temperature or keypad value
      {
        *keypad = value - 48;
        *TMP_value = 0;               //Denote that recieved data is keypad value
      }
    else
      {
        *TMP_value = value & 0x2fff;  //remove marker on second MSBit
      }
    return 1;                         //return 1 when there is data recieved
  }
  return 0;                           //return 0 when no data is recieved
}

