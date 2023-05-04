#include <SPI.h>
#include <RH_RF69.h>

#define RF69_FREQ 434.0
#define debug 1

#define RFM69_CS    8
#define RFM69_INT   7
#define RFM69_RST   4
#define LED        13

RH_RF69 rf69(RFM69_CS, RFM69_INT);

bool ledState = true;

void setup() {
  Serial.begin(115200);
  if(debug){while(!Serial)delay(1);}

  pinMode(LED, OUTPUT);
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather RFM69 RX Test!");
  Serial.println();

  radioInit();
  
  digitalWrite(LED, ledState);
}

void loop() {
  if (rf69.available()) {radioReceive();}
  digitalWrite(LED, ledState);
}
