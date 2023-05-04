void radioInit(){
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  if (!rf69.init()) {Serial.println("Radio init failed - Halt!"); while (1);}
  Serial.println("Radio init OK!");

  if (!rf69.setFrequency(RF69_FREQ)) {Serial.println("setFrequency failed");}

  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the server
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  
  Serial.print("RFM69 radio @");
  Serial.print((int)RF69_FREQ);
  Serial.println("MHz");
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void sendData(){
  char radiopacket[20] = "Test Data";
  //itoa(packetnum++, radiopacket, 10);
  Serial.print("Sending "); Serial.println(radiopacket);

  rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
  rf69.waitPacketSent();
}
