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
  uint8_t radiopacket[9];  
  radiopacket[0] = smallGhostSpdVal;
  radiopacket[1] = medGhostSpdVal;
  radiopacket[2] = tallGhostSpdVal;
  radiopacket[3] = smallghostlightval;
  radiopacket[4] = mediumghostlightval;
  radiopacket[5] = tallghostlightval;
  radiopacket[6] = smallghostmotorval;
  radiopacket[7] = mediumghostmotorval;
  radiopacket[8] = tallghostmotorval;
  if(debugSend){
    Serial.println("Sending Data");  
    for (int i = 0; i <= 8; i++) {
      Serial.print("Byte ");
      Serial.print(i);
      Serial.print("  Data: ");
      Serial.println(radiopacket[i]);
    }
  }
  rf69.send(radiopacket, 9);
  rf69.waitPacketSent();
  ledState = !ledState;
}
