void radioInit(){
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69.init()) {Serial.println("RFM69 radio init failed");while (1);}
  Serial.println("RFM69 radio init OK!");

  if (!rf69.setFrequency(RF69_FREQ)) {Serial.println("setFrequency failed");}
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  uint8_t key[] = { 0x23, 0x70, 0x4A, 0x88, 0x5F, 0x60, 0x71, 0x0A, 0x10, 0xA0, 0x06, 0x9E, 0x5D, 0x17, 0x35, 0x34};
  rf69.setEncryptionKey(key);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
}

void radioReceive(){
  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf69.recv(buf, &len)) {
    if (!len) return;
    buf[len] = 0;
    timeoutCounter = 0;
    if(ghostLightType == 1){    //Small Light
      motorSpdVal = buf[0];
      ghostlightVal = buf[3];
    }
    if(ghostLightType == 2){    //Medium Light
      motorSpdVal = buf[1];
      ghostlightVal = buf[4];
    }
    if(ghostLightType == 3){    //Tall Light
      motorSpdVal = buf[2];
      ghostlightVal = buf[5];
    }
    if(debugReceive){
      Serial.print("Received [");
      Serial.print(len);
      Serial.print("]: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf69.lastRssi(), DEC);
      for (int i = 0; i < len; i++) {
        Serial.print("Byte ");
        Serial.print(i);
        Serial.print("  Data: ");
        Serial.println(buf[i]);
      }
    }
    if(ghostlightVal){lampTarget = lampbrightness;}
    else{lampTarget = 0;}
    ledState = !ledState;
  } else {
    if(debugReceive){Serial.println("Receive failed");}
  }
}
