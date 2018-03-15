//TX RX functions for AT mode
void AT_TX(bool x, int value) {
  if (!x) {
    //payload for key press
    Serial2.write(value >> 8 & 0xff);
    Serial2.write(value & 0xff);
  }
  
  else {
    //payload for temperature reading
    Serial2.write((value >> 8 & 0xff) | 0x40);
    Serial2.write(value & 0xff);
  }
}

int AT_RX(int* temp, long* pressure, int* x, int* y, int* z, int* humidity) {
  if (Serial2.available() >= 14) {
    *temp = Serial2.read() << 8 | Serial2.read();
    *pressure = (long)Serial2.read() << 24 | (long)Serial2.read() << 16 | (long)Serial2.read() << 8 | (long)Serial2.read();
    *x = Serial2.read() << 8 | Serial2.read();
    *y = Serial2.read() << 8 | Serial2.read();
    *z = Serial2.read() << 8 | Serial2.read();
    *humidity = Serial2.read() << 8 | Serial2.read();
    return 1;
  }
  return 0;
}

