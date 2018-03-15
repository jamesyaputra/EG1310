//TX RX functions for API mode

//TX
uint8_t payload[] = {0,0};
ZBTxRequest packet = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

//RX
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

bool RX (int* temp, long* pressure, int* x, int* y, int* z, int* humidity) {
  xbee.readPacket();  
  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(rx);
      //Serial.println("RECEIVED SUCCESSFULLY");
      *temp = rx.getData(0) << 8 | rx.getData(1);
      *pressure = (long)rx.getData(2) << 24 | (long)rx.getData(3) << 16 | (long)rx.getData(4) << 8 | (long)rx.getData(5);
      *x = rx.getData(6) << 8 | rx.getData(7);
      *y = rx.getData(8) << 8 | rx.getData(9);
      *z = rx.getData(10) << 8 | rx.getData(11);
      *humidity = rx.getData(12) << 8 | rx.getData(13);
      return 1;
    }
  }
  return 0;
}

void TX (bool x, int value) {
  if (!x) { //sends value for key press when x is 0, sends temperature otherwise
    //payload for key press
    payload[0] = value >> 8 & 0xff;
    payload[1] = value & 0xff;
  } else {
    //payload for temperature reading
    payload[0] = (value >> 8 & 0xff) | 0x40;
    payload[1] = value & 0xff;
  }
  xbee.send(packet);
    
  if (xbee.readPacket(500) && xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);
  } else Serial.println("XBee did not provide a timely TX status response");
}

