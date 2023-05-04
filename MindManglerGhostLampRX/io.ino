void ioInit(){
  pinMode(LED, OUTPUT);
  pinMode(RFM69_RST, OUTPUT);
  pinMode(MOTORPWMOUT, OUTPUT);
  pinMode(GHOSTLIGHT, OUTPUT);
  pinMode(MOTORENDSTOPINPUT, INPUT_PULLUP);
  pinMode(BUMPSWITCHINPUT, INPUT_PULLUP);
  digitalWrite(RFM69_RST, LOW);
}

void readInputs(){
  bumpswitchVal = !digitalRead(BUMPSWITCHINPUT);
  endstopswitchVal = !digitalRead(MOTORENDSTOPINPUT);
}

void controlMotor(){
  if(bumpswitchVal){
    analogWrite(MOTORPWMOUT, bumpswitchmotorspeed);
  } 
  else if (endstopswitchVal && !failsafe){        //If we havent reached endstop and comms ok with controller
    analogWrite(MOTORPWMOUT, motorSpdVal);
  }
  else {
    analogWrite(MOTORPWMOUT, 0);
  }
}

void controlLamp(){
  if(lampCurrent < lampTarget){lampCurrent++;}
  else if(lampCurrent > lampTarget){lampCurrent--;}
  analogWrite(GHOSTLIGHT, lampCurrent);
}
