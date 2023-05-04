#include <SPI.h>
#include <RH_RF69.h>

/// User Changeable Params /////
#define RF69_FREQ 434.0
#define debug 1
#define debugReceive 0
#define failsafereceiveerrors   5   //In multiples of 50 milliseconds how many is allowed before timout (5*50ms = 250ms)
////////////////////////////////

//////// Pin Definitions ///////
#define RFM69_CS    8
#define RFM69_INT   7
#define RFM69_RST   4
#define LED        13
////////////////////////////////

//////// Program defines ///////
#define  TASK1_INTERVAL  50
////////////////////////////////

////// Program Variables ///////
RH_RF69 rf69(RFM69_CS, RFM69_INT);
uint32_t task1MillisLast = 0;
bool ledState = true;
bool failsafe;
uint8_t timeoutCounter;
////////////////////////////////

void setup() {
  Serial.begin(115200);
  if(debug){while(!Serial)delay(1);}
  Serial.println("Bendy Mic Stand Receiver");
  ioInit();
  radioInit();
  digitalWrite(LED, ledState);
  Serial.println("Initialisation complete!");
}

void loop() {
  if (millis() - task1MillisLast > TASK1_INTERVAL) {
    task1MillisLast = millis();
    timeoutCounter++;
    if(timeoutCounter >= failsafereceiveerrors){failsafe = true;}
    else{failsafe = false;}
    if(debug){printDebug();}
  }
  if (rf69.available()) {radioReceive();}
  digitalWrite(LED, ledState);
}

void printDebug(){
  Serial.println("Bendy Mic Stand Receiver Status");
  Serial.println();

  Serial.print(" -Failsafe: ");
  if(failsafe){Serial.println("True");}
  if(!failsafe){Serial.println("False");}

  Serial.println();
  Serial.println("-------------------------------");
}
