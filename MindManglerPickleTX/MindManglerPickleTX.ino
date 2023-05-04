#include <SPI.h>
#include <RH_RF69.h>

/// User Changeable Params /////
#define debug 1
#define debugSend 0
////////////////////////////////

/*
Mind Mangler - Bending Stands Pickle
RFM69HCW Version

Created: May 2023
Updated: 
Code: Nick Blower / nick@junctioninc.co.uk / Seb S / seb@sghinnovations.com

Version:  1.0b
Based on: DMX_Dimmer

Ports:
Port  Tpye  I/O  Desc
A0    A     I    Small Ghost Speed
A1    A     I    Medium Ghost Speed
A2    A     I    Tall Ghost Speed
A3    D     I    Run Small Ghost Motor - MAX SPEED
A4    D     I    Run Medium Ghost Motor - MAX SPEED
A5	  D		  I    Run Tall Ghost Motor - MAX SPEED
D2    D		  IO   SDA
D3    D		  IO   SCL
D4    D     Out  RADIO - RESET
D5    D     I    Run Tall Ghost Light
D6    D     I    Run Medium Ghost Light
D7    D     Out  RADIO - IRQ
D8    D     Out  RADIO - CS
D9    D     I    Run Small Ghost Motor
D10   D     I    Run Medium Ghost Motor
D11   D     I    Run Tall Ghost Motor
D12   D     I    Run Small Ghost Light
D13   D     O    LED
*/

//////// Program defines ///////
#define RF69_FREQ	434.0
#define  TASK1_INTERVAL  50
#define  TASK2_INTERVAL  200
////////////////////////////////

//////// Pin Definitions ///////
#define RFM69_CS	8
#define RFM69_INT	7
#define RFM69_RST	4
#define LED			13
#define smallGhostSpd A0
#define medGhostSpd A1
#define tallGhostSpd A2
#define smallGhostSpdMax A3
#define medGhostSpdMax A4
#define tallGhostSpdMax A5
#define smallghostlight 12
#define mediumghostlight 6
#define tallghostlight 5
#define smallghostmotor 9
#define mediumghostmotor 10
#define tallghostmotor 11
////////////////////////////////

////// Program Variables ///////
RH_RF69 rf69(RFM69_CS, RFM69_INT);
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
uint32_t task1MillisLast, task2MillisLast = 0;
bool ledState;
uint8_t  smallGhostSpdVal, medGhostSpdVal, tallGhostSpdVal;
bool smallghostlightval, mediumghostlightval, tallghostlightval, smallghostmotorval, mediumghostmotorval, tallghostmotorval;
////////////////////////////////

void setup() {
	Serial.begin(115200);
  if(debug){while(!Serial)delay(1);}
	Serial.println("Mind Mangler Pickle TX");
	Serial.println();
  ioInit();
  radioInit();
}

void loop() {
	if (millis() - task1MillisLast > TASK1_INTERVAL) {
		task1MillisLast = millis();
    readInputs();
    sendData();
    digitalWrite(LED, ledState);
	}

  if (millis() - task2MillisLast > TASK2_INTERVAL) {
   task2MillisLast = millis();
   if(debug){printDebug();}
  }
}

void printDebug(){
  Serial.println("Bendy Mic Stand Pickle Status");
  Serial.println();
  Serial.print(" -smallGhostSpdVal: ");
  Serial.println(smallGhostSpdVal);
  Serial.print(" -medGhostSpdVal: ");
  Serial.println(medGhostSpdVal);
  Serial.print(" -tallGhostSpdVal: ");
  Serial.println(tallGhostSpdVal);
  Serial.print(" -smallghostlightval: ");
  Serial.println(smallghostlightval);
  Serial.print(" -mediumghostlightval: ");
  Serial.println(mediumghostlightval);
  Serial.print(" -tallghostlightval: ");
  Serial.println(tallghostlightval);
  Serial.print(" -smallghostmotorval: ");
  Serial.println(smallghostmotorval);
  Serial.print(" -mediumghostmotorval: ");
  Serial.println(mediumghostmotorval);
  Serial.print(" -tallghostmotorval: ");
  Serial.println(tallghostmotorval);
  Serial.println();
  Serial.println("-------------------------------");
}
