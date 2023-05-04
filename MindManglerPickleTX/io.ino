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


bool buttonState1, buttonState2, buttonState3;

void readInputs(){

  bool smallghostlightreading = !digitalRead(smallghostlight);
  bool mediumghostlightreading = !digitalRead(mediumghostlight);
  bool tallghostlightreading = !digitalRead(tallghostlight);
  
  if (smallghostlightreading != lastsmallghostlightval) {lastDebounceTime1 = millis();}
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if(smallghostlightreading != buttonState1){
      buttonState1 = smallghostlightreading;
      if(buttonState1 == true){smallghostlightval = !smallghostlightval;}
    }
  }
  lastsmallghostlightval = smallghostlightreading;

  if (mediumghostlightreading != lastmediumghostlightval) {lastDebounceTime2 = millis();}
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if(mediumghostlightreading != buttonState2){
      buttonState2 = mediumghostlightreading;
      if(buttonState2 == true){mediumghostlightval = !mediumghostlightval;}
    }
  }
  lastmediumghostlightval = mediumghostlightreading;

  if (tallghostlightreading != lasttallghostlightval) {lastDebounceTime3 = millis();}
  if ((millis() - lastDebounceTime3) > debounceDelay) {
    if(tallghostlightreading != buttonState3){
      buttonState3 = tallghostlightreading;
      if(buttonState3 == true){tallghostlightval = !tallghostlightval;}
    }
  }
  lasttallghostlightval = tallghostlightreading;
  
  
  smallghostmotorval = !digitalRead(smallghostmotor);
  mediumghostmotorval = !digitalRead(mediumghostmotor);
  tallghostmotorval = !digitalRead(tallghostmotor);
  
  //Feather 32u4 has 10 bit ADC, so max value of 1024. Map from 10 to allow deadzone
  int16_t adc1 = map(analogRead(smallGhostSpd), 0, 1024, 30, 255);
  int16_t adc2 = map(analogRead(medGhostSpd), 0, 1024, 30, 255);
  int16_t adc3 = map(analogRead(tallGhostSpd), 0, 1024, 30, 255);

  if(adc1 < 30){adc1 = 30;}
  if(adc2 < 30){adc2 = 30;}
  if(adc3 < 30){adc3 = 30;}
  
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
}
