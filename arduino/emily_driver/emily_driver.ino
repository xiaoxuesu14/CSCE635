#include <Arduino.h>
#include <SoftwareSerial.h>
#include "emilyStatus.h"
#include "commParser.h"
#include "emilyGPS.h"
#include "emilyControl.h"

/** serial object for reading GPS */
SoftwareSerial gpsSerial(8, 9); // RX, TX (TX not used)
#define GPS_BAUD_RATE 9600
/** period in milliseconds at which to check the serial port */
#define SERIAL_PERIOD_MICROS 10000
/** serial port name to use: Serial is USB, Serial1 is the RX and TX pins */
#define COMM_SERIAL Serial1
/** rudder signal pin output */
#define RUDDER_PIN 5
/** throttle signal pin output */
#define THROTTLE_PIN 6

/** global status object */
emilyStatus stat;
/** communications parser object */
commParser comm(&stat);
/** GPS object */
emilyGPS GPS(&stat);
/** Control object */
emilyControl control(&stat);

uint32_t millis_now = 0;
uint32_t serial_millis_next = 0;
uint8_t serialByte;
/** Rudder signal variable */
uint8_t pwm_rudder;
/** Throttle signal variable */
uint8_t pwm_throttle;

void setup() {
  // put your setup code here, to run once:
  COMM_SERIAL.begin(9600);
  gpsSerial.begin(GPS_BAUD_RATE);
  serial_millis_next = millis() + (SERIAL_PERIOD_MICROS/1000);
  // initialize servo pins out
  pinMode(RUDDER_PIN,OUTPUT);
  pinMode(THROTTLE_PIN,OUTPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
  // read clock
  millis_now = millis();
  
  // see if it's time to READ XBee serial
  if( millis_now >= serial_millis_next ){
    serial_millis_next += (SERIAL_PERIOD_MICROS/1000);
    // check if serial is available
    if (COMM_SERIAL.peek() > -1){
      while(COMM_SERIAL.available()){ // not sure if we should run as fast as available or not
        // read while available
        serialByte = COMM_SERIAL.read();
        // parse the message
        comm.newBytes(&serialByte,1,millis_now);
      }
    }
  }
  /** See if new GPS available */
  if (gpsSerial.available())
  {
    char ch = gpsSerial.read();
    GPS.parseBytes(ch);
  }
  // call periodic functions
  comm.misc_tasks(millis_now);
  GPS.misc_tasks();
  control.misc_tasks(millis_now);

  // read the control values and write them
  if(control.new_control() > 0){
    // read from control
    control.get_pwm(&pwm_rudder,&pwm_throttle);
    // write out rudder
    analogWrite(RUDDER_PIN,pwm_rudder);
    // write out throttle
    analogWrite(THROTTLE_PIN,pwm_throttle);
  }

  // send any bytes in the transmit buffer
  while(comm.bytes_to_send() > 0){
    COMM_SERIAL.write( comm.get_next_byte() );
  }
}
