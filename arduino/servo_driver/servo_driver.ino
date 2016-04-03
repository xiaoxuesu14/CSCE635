/** @file */
/** @brief Basic test of servo driving with EMILY. Connect GND from Arduino, ESC output, and servo together. Connect ESC voltage to servo voltage line. Connect Arduino pin 6 to servo signal line. Send a '1' character over serial to drive the servo between 0 and 255 (max) - send a '0' to stop.

#define RUDDEROUT 6 // 3,5,6,9,10,11 are the Uno PWM pins out
#define SERVO_PERIOD_MICROS 100000 // 10 Hz servo write
#define SERIAL_PERIOD_MICROS 10000 // 100 Hz serial check
#define SERIAL_BAUD_RATE 9600 // baud rate

uint32_t servo_millis_last = 0;
uint32_t serial_millis_last = 0;
uint32_t serial_millis_next = 0;
uint32_t millis_now = 0;
uint32_t serial_silent_ticks = 0;// how many times did we check serial and get nothing?

int serialByte = 0;//bytes read from serial
uint8_t rudderVal = 0;

bool DRIVING_ON = false;// set to zero to STOP servo from driving

/**
  * Initialize the serial port for writing and set up variables
*/
void setup() {
  // enable Serial
  Serial.begin(SERIAL_BAUD_RATE);
  // enable rudder output
  pinMode(RUDDEROUT,OUTPUT);
  servo_millis_last = millis();
  serial_millis_last = millis();
  serial_millis_next = serial_millis_last + (SERIAL_PERIOD_MICROS/1000);
}

/**
  * Run the communications, sensor, and servo command loops
 */
void loop() {
  // read clock
  millis_now = millis();
  // see if it's time to check serial
  if( millis_now >= serial_millis_next ){
    serial_millis_last = millis_now;
    serial_millis_next += (SERIAL_PERIOD_MICROS/1000);
    // check if serial is available
    if (Serial.peek() > -1){
      serial_silent_ticks = 0;
      while(Serial.available()){ // not sure if we should run as fast as available or not
        // read while available
        serialByte = Serial.read();
        // parse the control mode
        serialParse(&serialByte);
        // print
        Serial.print(0.001*millis_now,DEC);
        Serial.print("\t");
        Serial.print(serialByte,DEC);
        Serial.print("\t");
        Serial.print(DRIVING_ON);
        Serial.print("\t");
        Serial.print(rudderVal);
        Serial.print("\n");
      }
    }
    else{
      serial_silent_ticks++;
    }
  }
  // see if it's time to write to servo
  if( (millis_now - servo_millis_last) >= (SERVO_PERIOD_MICROS/1000) ){ //
    servo_millis_last = millis_now;
    // write to servo
    if (!DRIVING_ON){
      analogWrite(RUDDEROUT,0);//0 to 255 output
    }
    else{
      rudderVal++;
      analogWrite(RUDDEROUT,rudderVal);
    }
  }
}

/**
  * Simple parser function for handling serial bytes
*/
void serialParse(int*serialBuffer){
  if (serialBuffer[0] == 48){// '0'
    // STOP driving
    DRIVING_ON = false;
  }
  if (serialBuffer[0] == 49){// '0'
    // START driving
    DRIVING_ON = true;
  }
}
