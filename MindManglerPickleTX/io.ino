void ioInit(){
  pinMode       (LED, OUTPUT);
  pinMode       (RFM69_RST, OUTPUT);
  pinMode       (smallGhostSpd, INPUT);
  pinMode       (medGhostSpd, INPUT);
  pinMode       (tallGhostSpd, INPUT);
  pinMode       (smallGhostSpdMax, INPUT_PULLUP);
  pinMode       (medGhostSpdMax, INPUT_PULLUP);
  pinMode       (tallGhostSpdMax, INPUT_PULLUP);
  pinMode       (smallghostlight, INPUT_PULLUP);
  pinMode       (mediumghostlight, INPUT_PULLUP);
  pinMode       (tallghostlight, INPUT_PULLUP);
  pinMode       (smallghostmotor, INPUT_PULLUP);
  pinMode       (mediumghostmotor, INPUT_PULLUP);
  pinMode       (tallghostmotor, INPUT_PULLUP);
  digitalWrite  (RFM69_RST, LOW);
}

void readInputs(){

  smallghostlightval = !digitalRead(smallghostlight);
  mediumghostlightval = !digitalRead(mediumghostlight);
  tallghostlightval = !digitalRead(tallghostlight);
  smallghostmotorval = !digitalRead(smallghostmotor);
  mediumghostmotorval = !digitalRead(mediumghostmotor);
  tallghostmotorval = !digitalRead(tallghostmotor);
  
  //Feather 32u4 has 10 bit ADC, so max value of 1024. Map from 10 to allow deadzone
  int16_t adc1 = map(analogRead(smallGhostSpd), 10, 1024, 30, 255);
  int16_t adc2 = map(analogRead(medGhostSpd), 10, 1024, 30, 255);
  int16_t adc3 = map(analogRead(tallGhostSpd), 10, 1024, 30, 255);

  if(adc1 < 31){adc1 = 0;}
  if(adc2 < 31){adc2 = 0;}
  if(adc3 < 31){adc3 = 0;}
  
  if(smallghostmotorval){smallGhostSpdVal = constrain(adc1, 0, 255);}
  if(mediumghostmotorval){medGhostSpdVal = constrain(adc2, 0, 255);}
  if(tallghostmotorval){tallGhostSpdVal = constrain(adc3, 0, 255);}

  //If analog pins can be treated as digital
  if(!digitalRead(smallGhostSpdMax)){smallGhostSpdVal = 255;}
  else if (!smallghostmotorval){smallGhostSpdVal = 0;}
  if(!digitalRead(medGhostSpdMax)){medGhostSpdVal = 255;}
  else if (!mediumghostmotorval){medGhostSpdVal = 0;}
  if(!digitalRead(tallGhostSpdMax)){tallGhostSpdVal = 255;}
  else if (!tallghostmotorval){tallGhostSpdVal = 0;}

  //If analog pins cant be treated as digital
  //uint8_t smallGhostSpdMaxVal = map(analogRead(smallGhostSpdMax), 0, 1024, 0, 255);
  //uint8_t medGhostSpdMaxVal = map(analogRead(medGhostSpdMax), 0, 1024, 0, 255);
  //uint8_t tallGhostSpdMaxVal = map(analogRead(tallGhostSpdMax), 0, 1024, 0, 255);
  //if(smallGhostSpdMaxVal > 100){smallGhostSpdVal = 254;}
  //if(medGhostSpdMaxVal > 100){medGhostSpdVal = 254;}
  //if(tallGhostSpdMaxVal > 100){tallGhostSpdVal = 254;}
}
