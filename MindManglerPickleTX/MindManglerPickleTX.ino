#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

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

// Addresses:
//  TX Ghost  1
//  RX Short  2
//  RX Medium 3
//  RX Tall   4
//
//  TX Mic    11
//  RX Mic    12
#define RF69_FREQ	434.0
#define TX_GHOST_ADDR	1
#define RX_SHORT_ADDR	2
#define RX_MEDIUM_ADDR	3
#define RX_TALL_ADDR	4
#define TX_MIC_ADDR		11
#define RX_MIC_ADDR		12

#define RFM69_CS	8
#define RFM69_INT	7
#define RFM69_RST	4
#define LED			13

RH_RF69 rf69(RFM69_CS, RFM69_INT);
RHReliableDatagram rfMan(rf69, TX_GHOST_ADDR);

#define RF_HB_ID	1
#define RF_CMD_ID	2

#define  HB_INTERVAL	500
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
	Serial.print("Address: ");
	Serial.println(TX_GHOST_ADDR);

	//Manual reset
	digitalWrite(RFM69_RST, HIGH);
	delay(10);
	digitalWrite(RFM69_RST, LOW);
	delay(10);
	
	if (!rfMan.init()) {Serial.println("Radio init failed - Halt!"); while (1);}
	Serial.println("Radio init OK!");

	if (!rf69.setFrequency(RF69_FREQ)) {Serial.println("setFrequency failed");}

	rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

	// The encryption key has to be the same as the one in the server
	uint8_t key[] = {	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	rf69.setEncryptionKey(key);
  
	Serial.print("RFM69 radio @");
	Serial.print((int)RF69_FREQ);
	Serial.println("MHz");
}

void loop() {
	if (millis() - hbMillisLast > HB_INTERVAL) {
		hbMillisLast = millis();
		//sendHeartBeat(RX_SHORT_ADDR);
    sendHelloWorld();
    ledState = !ledState;
    digitalWrite(LED, ledState);
    Serial.println("TX");
	}
}

void sendHeartBeat(uint8_t nodeAddr) {
		rfMan.setHeaderId(RF_HB_ID);
		rfMan.setRetries(1);
		rfMan.setTimeout(50);
		
		char packet[6] = "pulse";
		
		if (rfMan.sendtoWait((uint8_t *)packet, strlen(packet), nodeAddr)) {
			uint8_t len = sizeof(buf);
			uint8_t from;
			uint8_t to;
			uint8_t id;
			if (rfMan.recvfromAckTimeout(buf, &len, 250, &from, &to, &id)) {
				buf[len] = 0; // zero out remaining string
			}
		}	
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void sendHelloWorld(){
  char radiopacket[20] = "Hello World #";
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);

  // Send a message!
  rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
  rf69.waitPacketSent();
}
