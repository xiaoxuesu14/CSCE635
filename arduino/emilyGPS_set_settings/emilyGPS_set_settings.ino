/** @file
 * Send commands to the GPS to set the sample rate and baud rate
 */
#include <SoftwareSerial.h>
#include "emilyStatus.h"
#include "emilyGPS.h"

SoftwareSerial gpsSerial(8, 9); // RX, TX (TX not used)
#define READ_RATE_MILLIS 500
#define PRINT_RAW true
#define GPS_BAUD_RATE 9600

emilyStatus stat;
emilyGPS GPS(&stat);

uint32_t millis_next = 0,
millis_now = 0;
int len = 0;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initialized emilyGPS_test\n");
  gpsSerial.begin(GPS_BAUD_RATE);
  Serial.print("Commanding GPS rate in 5 seconds\n");
  delay(5000);
  uint8_t buffer[256];
  len = GPS.send_command_configure_position_rate(buffer,2);
  buffer[len] = 0;
  //gpsSerial.write(buffer,len);
  // write one byte at a time
  for(int j = 0;j<len;j++){
    gpsSerial.write(buffer[j]);
    delay(0.05);
  }
  Serial.print("Sent command: wait for 1 second\n");
  delay(1000);
  while (gpsSerial.available()){
    uint8_t ch = gpsSerial.read();
    if ( ch == 0xA0 )
      Serial.print("**************************************\n");
    Serial.print(ch,HEX);
    Serial.print(",");
  }
  Serial.print("\n");
  /*
  Serial.print("Sending the baud rate command\n");
  GPS.send_command_configure_serial_port(buffer,3);
  gpsSerial.write(buffer);
  Serial.print("Wait 1 second\n");
  delay(1000);
  Serial.print("Restarting the GPS\n");
  GPS.send_command_restart_cold(buffer);
  gpsSerial.write(buffer);*/
}

void loop()
{
  millis_now = millis();
  static int i = 0;
  if (gpsSerial.available())
  {
    char ch = gpsSerial.read();
    GPS.parseBytes(ch);
    if(PRINT_RAW)
      Serial.print(ch);
  }
  if (millis_now >= millis_next){
    millis_next += READ_RATE_MILLIS;
    if (stat.gpsNow.init){
      Serial.print("*********************\n");
      Serial.print("Time: ");
      Serial.print(stat.gpsNow.t);
      Serial.print("Lat: ");
      Serial.print(stat.gpsNow.lat);
      Serial.print(" Long: ");
      Serial.print(stat.gpsNow.lon);
      Serial.print(" X: ");
      Serial.print(stat.gpsNow.x);
      Serial.print(" Y: ");
      Serial.print(stat.gpsNow.y);
      Serial.print("\n");
    }
  }
}
