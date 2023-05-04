#include <SPI.h>
#include <RH_RF69.h>

#define debug 1

/*
Mind Mangler - Bending Stands Pickle
RFM69HCW Version

Created: May 2023
Updated: 
Code: Nick Blower / nick@junctioninc.co.uk

Version:  1.0a
Based on: DMX_Dimmer

Ports:
Port  Tpye  I/O  Desc
A0    D     I    Small Ghost Speed
A1    D     I    Medium Ghost Speed
A2    D     I    Tall Ghost Speed
A3    D     I    Run Small Ghost Motor - MAX SPEED
A4    D     I    Run Medium Ghost Motor - MAX SPEED
A5	  D		  I    Run Tall Ghost Motor - MAX SPEED
D2    D		  IO   SDA
D3    D		  IO   SCL
D4    D     Out  RADIO - RESET
D5    D     I    Run Tall Ghost Light
D6    D     -    -
D7    D     Out  RADIO - IRQ
D8    D     Out  RADIO - CS
D9    D     I    Run Small Ghost Motor
D10   D     I    Run Medium Ghost Motor
D11   D     I    Run Tall Ghost Motor
D12   D     I    Run Small Ghost Light
D13   D     I    Run Medium Ghost Light
*/
#define RF69_FREQ	434.0

#define RFM69_CS	8
#define RFM69_INT	7
#define RFM69_RST	4
#define LED			13

RH_RF69 rf69(RFM69_CS, RFM69_INT);

#define  HB_INTERVAL	50
uint32_t hbMillisLast = 0;
bool ledState;

uint8_t  ghostHBs = 0;
uint8_t  ghostHBm = 0;
uint8_t  ghostHBt = 0;

uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

void setup() {
	
	Serial.begin(115200);
  if(debug){while(!Serial)delay(1);}
	
	pinMode		(LED, OUTPUT);
	pinMode		(RFM69_RST, OUTPUT);
  pinMode   (LED, OUTPUT);
	digitalWrite(RFM69_RST, LOW);

	Serial.println("Mind Mangler Pickle TX");
	Serial.println();

  radioInit();
}

void loop() {
	if (millis() - hbMillisLast > HB_INTERVAL) {
		hbMillisLast = millis();
		//sendHeartBeat(RX_SHORT_ADDR);
    sendData();
    ledState = !ledState;
    digitalWrite(LED, ledState);
    //Serial.println("TX");
	}
}
