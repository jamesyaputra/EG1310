
XBeeResponse response   = XBeeResponse();
ZBRxResponse        rx  = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

uint8_t payload[14] = {0};
   
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

void flashLed(int pin, int times, int wait) 
{
  for (int i = 0; i < times; i++) 
  {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);
    if (i + 1 < times) delay(wait);
  }
}

void TX(short BMP_temp, long BMP_pres, int HMC_x, int HMC_y, int HMC_z, int HIH_value)
{
  payload[0] = BMP_temp >> 8 & 0xff;
  payload[1] = BMP_temp & 0xff;
  
  payload[2] = BMP_pres >> 24 & 0xff;
  payload[3] = BMP_pres >> 16 & 0xff;
  payload[4] = BMP_pres >> 8 & 0xff;
  payload[5] = BMP_pres & 0xff;
  
  payload[6] = HMC_x >> 8 & 0xff;
  payload[7] = HMC_x & 0xff;
  
  payload[8] = HMC_y >> 8 & 0xff;
  payload[9] = HMC_y & 0xff;
  
  payload[10] = HMC_z >> 8 & 0xff;
  payload[11] = HMC_z & 0xff;
  
  payload[12] = HIH_value >> 8 & 0xff;
  payload[13] = HIH_value & 0xff;
  
  xbee.send(zbTx);
  // flash TX indicator
  flashLed(dataLed, 1, 10);
  
  // after sending a tx request, we expect a status response
  // wait up to half a second for the status response
  if (xbee.readPacket(500))
  {
   // got a response!
   // should be a znet tx status              
   if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE)
   {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
      // get the delivery status, the fifth byte
      if (txStatus.getDeliveryStatus() == SUCCESS) 
      {
        // success.  time to celebrate
        // flashLed(statusLed, 5, 10); 
    flashLed(statusLed, 1, 10); 
      } 
      else
      {
        // the remote XBee did not receive our packet. is it powered on?
        flashLed(errorLed, 1, 50);
      }
    }      
  } 
  else 
  {
   // local XBee did not provide a timely TX Status Response -- should not happen
   flashLed(errorLed, 5, 50);
  }
}

bool RX(int *TMP_value, int *keypad)
{
  xbee.readPacket();
  if (xbee.getResponse().isAvailable())
  {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) 
    {
      xbee.getResponse().getZBRxResponse(rx);
      int value = rx.getData(0) << 8 | rx.getData(1);
      if (rx.getData(0) < 0x40)           //Second MSBit used to denote if data is keypad is temperature or keypad value
      {
        *keypad = value - 48;
        *TMP_value = 0;                   //Denote that recieved data is keypad value
      }
      else
      {
        *TMP_value = value & 0x2fff;      //remove marker on second MSBit
      }
      return 1;                           //return 1 when there is data recieved
    }
  }
  return 0;                               //return 0 when no data is recieved
}
