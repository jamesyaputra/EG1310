void printLCD_RX (int temp, long pressure, int x, int y, int z, int humidity) {
  lcd.clear();
  lcd.print("TMP");
  lcd.setCursor(0,5);
  lcd.print("PRESSURE");
  lcd.setCursor(0,14);
  lcd.print("HMD");
  lcd.setCursor(1,0);
  lcd.print(temp / 10.0, 1);
  lcd.setCursor(1,5);
  lcd.print(pressure);
  lcd.setCursor(1,14);
  lcd.print(humidity/1024.0*100.0,1);
  lcd.print("%");
  lcd.setCursor(2,0);
  lcd.print("X");
  lcd.setCursor(2,6);
  lcd.print("Y");
  lcd.setCursor(2,12);
  lcd.print("Z ");
  lcd.print(atan((double)x/(-y))/PI*180.0 + 180.0, 1);
  lcd.print('\'');
  lcd.setCursor(3,0);
  if(x>=0) lcd.print(" ");
  lcd.print(x);
  lcd.setCursor(3,6);
  if(y>=0) lcd.print(" ");
  lcd.print(y);
  lcd.setCursor(3,12);
  if(z>=0) lcd.print(" ");
  lcd.print(z);
}

void printLCD_TX (int temp, int key) {
  lcd.clear();
  lcd.print("Temp: ");
  lcd.print(temp * 0.0625, 2);
  lcd.print(" ");
  lcd.print("Key: ");
  if (key >= 48)
  {
    lcd.print(key - 48);
  }
  else if (key == 42)
  {
    lcd.print("*");
  }
  else if (key == 35)
  {
    lcd.print("#");
  }
}

