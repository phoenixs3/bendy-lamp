void radioInit(){
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  if (!rf69.init()) {Serial.println("Radio init failed - Halt!"); while (1);}
  Serial.println("Radio init OK!");

  if (!rf69.setFrequency(RF69_FREQ)) {Serial.println("setFrequency failed");}
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  uint8_t key[] = { 0x23, 0x70, 0x4A, 0x88, 0x5F, 0x60, 0x71, 0x0A, 0x10, 0xA0, 0x06, 0x9E, 0x5D, 0x17, 0x35, 0x34};
  rf69.setEncryptionKey(key);
  
  Serial.print("RFM69 radio @");
  Serial.print((int)RF69_FREQ);
  Serial.println("MHz");
}

void sendData(){
  uint8_t radiopacket[6];  
  radiopacket[0] = smallGhostSpdVal;
  radiopacket[1] = medGhostSpdVal;
  radiopacket[2] = tallGhostSpdVal;
  radiopacket[3] = smallghostlightval;
  radiopacket[4] = mediumghostlightval;
  radiopacket[5] = tallghostlightval;
  if(debugSend){
    Serial.println("Sending Data");  
    for (int i = 0; i <= 5; i++) {
      Serial.print("Byte ");
      Serial.print(i);
      Serial.print("  Data: ");
      Serial.println(radiopacket[i]);
    }
  }
  rf69.send(radiopacket, 6);
  rf69.waitPacketSent();
  ledState = !ledState;
}
