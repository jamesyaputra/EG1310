void TMP(int *value) {
  byte byteOne, byteTwo;
  Wire.beginTransmission(TMP_ADDRESS);
  Wire.write((byte)0x00);
  Wire.endTransmission();

  Wire.requestFrom(TMP_ADDRESS, 2);
  if (Wire.available() >= 2) {
    byteOne = (Wire.read());
    byteTwo = (Wire.read());
  }
  
  *value = byteOne << 4 | byteTwo >> 4;
}

