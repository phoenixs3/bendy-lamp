/*
Mind Mangler - Bending Stands Controller
PCA9685 Driven Version


Created: March 2023
Updated: 
Code: Nick Blower / nick@junctioninc.co.uk

Version:  1.0a
		  
Ports:
Port  Tpye  I/O  Desc
A0    D     -    -
A1    D     -    -
A2    D     Out  LED - Yellow
A3    D     Out  LED - Red
A4    D     Out  SDA
A5    D     Out  SCL
A6    -     -    -
A7    -     -    -
D0    D     In   Serial: Rx - DMX In - Not Used
D1    D     Out  Serial: Tx - DMX Out - Not Used
D2    D     Out  DMX Direction
D3    D-PWM In   DIP1  Test
D4    D     In   DIP2  addr bit 256
D5    D-PWM In   DIP3  addr bit 128
D6    D-PWM In   DIP4  addr bit 64
D7    D     In   DIP5  addr bit 32
D8    D     In   DIP6  addr bit 16
D9    D-PWM In   DIP7  addr bit 8
D10   D-PWM In   DIP8  addr bit 4
D11   D-PWM In   DIP9  addr bit 2
D12   D     In   DIP10 addr bit 1
D13   D     Out  SCK
*/

#define PIN_LED_YEL A2
#define PIN_LED_RED A3
#define PIN_SDA     A4
#define PIN_SCL     A5

#define RED     0
#define GREEN   1
#define BLUE    2
#define WHITE   3

#include <avr/wdt.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#include <Bounce2.h>
Bounce dbSpeedLow = Bounce();
Bounce dbSpeedMed = Bounce();
Bounce dbSpeedHigh = Bounce();
Bounce dbLampToggle = Bounce();
Bounce dbLocalBump = Bounce();
Bounce dbLimitSwitch = Bounce();



//1 channel
uint8_t  pwmCount = 2;

//DIP Vars
#define  DIP_NUM 10
//						   DIPpos    1   2   3   4   5   6   7   8   9   10
//						   arrayPos  0   1   2   3   4   5   6   7   8   9
uint8_t  pinDIPs [DIP_NUM] = {       12, 11, 10, 9,  8,  7,  6,  5,  4,  3};
#define  DEBOUNCE_TIME	5

//this is what the DIPs do when triggered
#define SPEED_LOW	0
#define SPEED_MED	1
#define	SPEED_HIGH	2
#define LAMP_ON		3
#define LOCAL_BUMP	4
#define LIMIT_SW	5
#define MED_SP_A	6
#define MED_SP_B	7
#define MED_SP_C	8
#define MED_SP_D	9

//pwm output
#define MOTOR		0
#define LAMP		1

#define SPEED_L_VAL	500
#define SPEED_H_VAL	2625
uint16_t speedMedValue = 0;

int16_t lampCurrent = 0;
uint16_t lampTarget  = 0;
uint8_t  lampOn    = 0;

uint8_t  motorRemote = 0;
	

void setup () {

	wdt_enable    (WDTO_250MS);							// enable the Watchdog Time with an ~250ms interval

	digitalWrite  (PIN_DMXDIR, LOW);
	pinMode       (PIN_DMXDIR, OUTPUT);

	//start PWM library and set i2c bus to max speed
	pwm.begin();
	pwm.setPWMFreq(120);								// this is the PWM frequency
// 	pwm.setPWMFreq(1600);								// this is the maximum PWM frequency [1kHz]
	uint8_t twbrbackup = TWBR;							// save old i2c bitrate
	TWBR = 12;											// upgrade to fast 400kHz i2c mode

	setAllOff();										// ensure all outputs are at 0. Chip stores if power isn't cycled

	for (uint8_t i = 0; i < DIP_NUM; i++) {
		pinMode		(pinDIPs[i], INPUT_PULLUP);
	}

	pinMode			(PIN_LED_YEL,  OUTPUT);
	digitalWrite	(PIN_LED_YEL,  LOW);

	pinMode			(PIN_LED_RED,    OUTPUT);
	digitalWrite	(PIN_LED_RED,    LOW);
	
	dbSpeedLow.attach(pinDIPs[0]);
	dbSpeedLow.interval(DEBOUNCE_TIME);

	dbSpeedMed.attach(pinDIPs[1]);
	dbSpeedMed.interval(DEBOUNCE_TIME);

	dbSpeedHigh.attach(pinDIPs[2]);
	dbSpeedHigh.interval(DEBOUNCE_TIME);

	dbLampToggle.attach(pinDIPs[3]);
	dbLampToggle.interval(DEBOUNCE_TIME);

	dbLocalBump.attach(pinDIPs[4]);
	dbLocalBump.interval(DEBOUNCE_TIME);

	dbLimitSwitch.attach(pinDIPs[5]);
	dbLimitSwitch.interval(DEBOUNCE_TIME);

	wdt_reset();

	flashErrLed(75);
	flashErrLed(75);
	flashErrLed(75);

}

void flashErrLed(byte inDelay) {
	wdt_reset();
	digitalWrite(PIN_LED_RED,HIGH);
	digitalWrite(PIN_LED_YEL,HIGH);
	delay(inDelay);
	wdt_reset();
	digitalWrite(PIN_LED_RED,LOW);
	digitalWrite(PIN_LED_YEL,LOW);
	delay(inDelay);
	wdt_reset();
}

void loop () {

	wdt_reset();								// reset the Watchdog Timer
	
	checkDIPs();								// read DIPs for input
	medSpeedSet();								// read DIPs to set medium speed
	
	motorRemote = 0;							// is motor under remote control?
	
	if (dbLimitSwitch.read() == 0) {			// the status indicators are handy but not essential
		digitalWrite(PIN_LED_YEL, 1);
		if (dbSpeedLow.read() == 0) {
			pwm.setPWM(MOTOR, 0, SPEED_L_VAL);
			motorRemote = 1;
			digitalWrite(PIN_LED_RED, 1);
		} else if (dbSpeedMed.read() == 0) {
			pwm.setPWM(MOTOR, 0, speedMedValue);
			motorRemote = 1;
			digitalWrite(PIN_LED_RED, 1);
		} else if (dbSpeedHigh.read() == 0) {
			pwm.setPWM(MOTOR, 0, SPEED_H_VAL);
			motorRemote = 1;
			digitalWrite(PIN_LED_RED, 1);
		} else {
			digitalWrite(PIN_LED_RED, 0);
			pwm.setPWM(MOTOR, 0, 0);
		}
	} else {
		digitalWrite(PIN_LED_YEL, 0);
		digitalWrite(PIN_LED_RED, 0);
		pwm.setPWM(MOTOR, 0, 0);
	}
	
	if (dbLampToggle.fell()) {					// simple non blocking routine to dim the lamp up and down
		if (lampOn) {
			lampTarget = 0;
			lampOn = 0;
		} else {
			lampTarget = 4095;
			lampOn = 1;
		}
	}
	fadeLamp();									// now poll for an update to lamp brightness.
	
	// local bumping ignores limit swtich
	if (motorRemote == 0) {						// allow local bumping if motor is not under remote control
		if (dbLocalBump.read() == 0) {
			pwm.setPWM(MOTOR, 0, SPEED_H_VAL);
			digitalWrite(PIN_LED_RED, HIGH);
		} else {
			pwm.setPWM(MOTOR, 0, 0);
			digitalWrite(PIN_LED_RED, LOW);
		}
	}

}


//check DIPs and set actions
void checkDIPs() {
	
	  dbSpeedLow.update();
	  dbSpeedMed.update();
	  dbSpeedHigh.update();
	  dbLampToggle.update();
	  dbLocalBump.update();
	  dbLimitSwitch.update();

}

void fadeLamp() {
	
	
	if (lampCurrent > lampTarget) {
		lampCurrent -= 20;
		if (lampCurrent < 0) lampCurrent = 0;
		pwm.setPWM(LAMP, 0, lampCurrent);
	} else if (lampCurrent < lampTarget) {
		lampCurrent += 5;
		if (lampCurrent > 4095) lampCurrent = 4095;
		pwm.setPWM(LAMP, 0, lampCurrent);
	}
	
}

void setAllOff() {
  wdt_reset();
  for (int i=0; i<pwmCount; i++) {
    pwm.setPWM(i, 0, 0);
  }
}


void medSpeedSet() {
	
	uint8_t j = 0;
	for (uint8_t i=9; i>5; i--) {
		j = (j << 1) | !(digitalRead(pinDIPs[i]));
	}
	
	speedMedValue = map(j, 0, 15, SPEED_L_VAL, SPEED_H_VAL);
	
}