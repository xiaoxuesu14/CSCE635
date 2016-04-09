#include "emilyStatus.h"
#include "commParser.h"
#include <Arduino.h>

#define SERIAL_PERIOD_MICROS 10000

/** global status object */
emilyStatus stat;
/** communications parser object */
commParser comm(&stat);

uint32_t millis_now = 0;
uint32_t serial_millis_next = 0;
uint32_t serial_silent_ticks = 0;// how many times did we check serial and get nothing?
uint8_t serialByte;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  serial_millis_next = millis() + (SERIAL_PERIOD_MICROS/1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // read clock
  millis_now = millis();
  // see if it's time to check serial
  if( millis_now >= serial_millis_next ){
    serial_millis_next += (SERIAL_PERIOD_MICROS/1000);
    // check if serial is available
    if (Serial.peek() > -1){
      serial_silent_ticks = 0;
      while(Serial.available()){ // not sure if we should run as fast as available or not
        // read while available
        serialByte = Serial.read();
        //Serial.print(serialByte);
        //Serial.print(",");
        // parse the message
        comm.newBytes(&serialByte,1,millis_now);
      }
      /*
      for(int k = 0;k<20;k++){
        Serial.print(k);
        Serial.print("  ");
        Serial.print(comm.msg[k]);
        Serial.print("\n");
      }*/
      
      Serial.print("t=");
      Serial.print(stat.gpsCmd.t);
      Serial.print(",x=");
      Serial.print(stat.gpsCmd.x);
      Serial.print(",y=");
      Serial.print(stat.gpsCmd.y);
      Serial.print(",bad_packets");
      Serial.print(comm.get_bad_packets());
      Serial.print(",bad_chksums");
      Serial.print(comm.get_bad_checksums());
      Serial.print("\n");
      
    }
    else{
      serial_silent_ticks++;
    }
  }
  
}
