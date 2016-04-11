#include <SoftwareSerial.h>
#include "emilyStatus.h"
#include "emilyGPS.h"

SoftwareSerial gpsSerial(8, 9); // RX, TX (TX not used)
#define READ_RATE_MILLIS 1000

emilyStatus stat;
emilyGPS GPS(&stat);

uint32_t millis_next = 0,
millis_now = 0;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initialized emilyGPS_test\n");
  gpsSerial.begin(9600);
}

void loop()
{
  millis_now = millis();
  static int i = 0;
  if (gpsSerial.available())
  {
    char ch = gpsSerial.read();
    GPS.parseBytes(ch);
    Serial.print(ch);
  }
  if (millis_now >= millis_next){
    millis_next += READ_RATE_MILLIS;
    if (stat.gpsNow.init){
      Serial.print("*********************\n");
      Serial.print("Lat: ");
      Serial.print(stat.gpsNow.lat);
  
      Serial.print(" Long: ");
      Serial.print(stat.gpsNow.lon);
      Serial.print("\n");
    }
  }
}
