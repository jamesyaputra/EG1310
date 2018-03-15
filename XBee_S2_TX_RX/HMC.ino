void HMC(int *x, int *y, int *z)
{
  //Tell the HMC5883 where to begin reading data
  Wire.beginTransmission(HMC_address);
  Wire.write((byte)0x03); //select register 3, X MSB register
  Wire.endTransmission();
   
 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(HMC_address, 6);
  if(6<=Wire.available()){
    *x = Wire.read() << 8; //X msb
    *x |= Wire.read();     //X lsb
    *z = Wire.read() << 8; //Z msb
    *z |= Wire.read();     //Z lsb
    *y = Wire.read() << 8; //Y msb
    *y |= Wire.read();     //Y lsb
  }
  else{   //return 0 value when data is unavailable or component is unplugged or malfuntioning
    *x=0;
    *y=0;
    *z=0;
  }
}
