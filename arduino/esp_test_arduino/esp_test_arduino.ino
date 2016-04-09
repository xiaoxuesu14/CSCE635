#include "emilyStatus.h"
#include "commParser.h"
#include <Arduino.h>

/** period in milliseconds at which to check the serial port */
#define SERIAL_PERIOD_MICROS 10000
/** serial port name to use: Serial is USB, Serial1 is the RX and TX pins */
#define COMM_SERIAL Serial1

/** global status object */
emilyStatus stat;
/** communications parser object */
commParser comm(&stat);

uint32_t millis_now = 0;
uint32_t serial_millis_next = 0;
uint8_t serialByte;

void setup() {
  // put your setup code here, to run once:
  COMM_SERIAL.begin(9600);
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
    if (COMM_SERIAL.peek() > -1){
      while(COMM_SERIAL.available()){ // not sure if we should run as fast as available or not
        // read while available
        serialByte = COMM_SERIAL.read();
        //COMM_SERIAL.print(serialByte);
        //COMM_SERIAL.print(",");
        // parse the message
        comm.newBytes(&serialByte,1,millis_now);
      }
      /*
      print human-readable status
      */
      
      COMM_SERIAL.print("bad_packets:");
      COMM_SERIAL.print(comm.get_bad_packets());
      COMM_SERIAL.print(",bad_chksums:");
      COMM_SERIAL.print(comm.get_bad_checksums());
      COMM_SERIAL.print(",bytes_to_send:");
      COMM_SERIAL.print(comm.bytes_to_send());
      COMM_SERIAL.print(",recvd_msgs:");
      COMM_SERIAL.print(comm.get_number_received_messages());
      COMM_SERIAL.print("\n");
      
    }
  }
  // call periodic functions
  comm.misc_tasks(millis_now);

  // send any bytes in the transmit buffer
  while(comm.bytes_to_send() > 0){
    COMM_SERIAL.print( comm.get_next_byte() );
  }
}
