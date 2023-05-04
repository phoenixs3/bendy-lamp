#include <SPI.h>
#include <RH_RF69.h>

/// User Changeable Params /////
#define RF69_FREQ 434.0
#define ghostLightType          1   //1 = Small, 2 = Medium, 3 = Tall
#define debug                   1   //Toggle view program state in serial monitor
#define debugReceive            0   //Toggle view receive bytes in serial monitor
#define failsafereceiveerrors   5   //In multiples of 50 milliseconds how many is allowed before timout (5*50ms = 250ms)
#define bumpswitchmotorspeed    100 //Duty cycle from 0 to 255
#define lampfadespeed           5   //Amount of ms between duty cycle change of lamp, 255 * value = full fade time
#define lampbrightness          255 //Duty cycle from 0-255 to determine lamp on brightness
////////////////////////////////

//////// Pin Definitions ///////
#define RFM69_CS          8
#define RFM69_INT         7
#define RFM69_RST         4
#define MOTORENDSTOPINPUT 9
#define BUMPSWITCHINPUT   12
#define MOTORPWMOUT       10
#define GHOSTLIGHT        11
#define LED               13
////////////////////////////////

//////// Program defines ///////
#define  TASK1_INTERVAL  50
////////////////////////////////

////// Program Variables ///////
RH_RF69 rf69(RFM69_CS, RFM69_INT);
uint32_t task1MillisLast, task2MillisLast = 0;
bool ledState = true;
bool failsafe, ghostlightVal, bumpswitchVal, endstopswitchVal;
uint8_t timeoutCounter, motorSpdVal;
uint8_t lampTarget, lampCurrent;
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
  if (millis() - task2MillisLast > lampfadespeed) {
    task2MillisLast = millis();
    controlLamp();
  }
  if (rf69.available()) {radioReceive();}
  digitalWrite(LED, ledState);
  readInputs();
  controlMotor();
}

void printDebug(){
  Serial.println("Bendy Mic Stand Receiver Status");
  Serial.println();

  Serial.print(" -Failsafe: ");
  if(failsafe){Serial.println("True");}
  if(!failsafe){Serial.println("False");}

  Serial.print(" -RX ghostlightVal: ");
  Serial.println(ghostlightVal);

  Serial.print(" -RX motorSpdVal: ");
  Serial.println(motorSpdVal);

  Serial.print(" -bumpswitch Input: ");
  Serial.println(bumpswitchVal);

  Serial.print(" -endstopswitch Input: ");
  Serial.println(endstopswitchVal);

  Serial.print(" -lampCurrent: ");
  Serial.println(lampCurrent);

  Serial.print(" -lampTarget: ");
  Serial.println(lampTarget);

  Serial.println();
  Serial.println("-------------------------------");
}
