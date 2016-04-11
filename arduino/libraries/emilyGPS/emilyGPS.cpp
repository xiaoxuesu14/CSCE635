#include <stdint.h>
#include <cstdlib>
#include <cstring>
#include "emilyGPS.h"

emilyGPS::emilyGPS(emilyStatus*st){
  status = st;
  parseCounter = 0;
}

void emilyGPS::parseBytes(char ch){
  if (ch != '\n' && parseCounter < sentenceSize){
    sentence[parseCounter] = ch;
    parseCounter++;
  }
  else
  {
    sentence[parseCounter] = '\0';
    parseCounter = 0;
    // run some parser function
  }
}

void emilyGPS::parseSentence(){
  char field[20];
  getField(field, 0);
  if (strcmp(field, "$GPRMC") == 0)
  {
    long lon, lat;
    getField(field, 3);  // number in 0.01 * degrees
    lon = atof(field)*100000;// scale
    getField(field, 4); // N/S
    if (strcmp(field,"S")==0){
      lon = -lon;
    }
    getField(field, 5);  // number in 0.01 * degrees
    lat = atof(field)*100000;// scale
    getField(field, 6);  // E/W
    if (strcmp(field,"E")==0){
      lat = -lat;
    }
    // set the status object
    status->gpsNow.set(lat,lon,0.0);
  }
}

void emilyGPS::getField(char* buffer, int index)
{
  int sentencePos = 0;
  int fieldPos = 0;
  int commaCount = 0;
  while (sentencePos < sentenceSize)
  {
    if (sentence[sentencePos] == ',')
    {
      commaCount ++;
      sentencePos ++;
    }
    if (commaCount == index)
    {
      buffer[fieldPos] = sentence[sentencePos];
      fieldPos ++;
    }
    sentencePos ++;
  }
  buffer[fieldPos] = '\0';
}

int32_t convertGPS(char* buffer){
  double deg = (atof(buffer)*0.01);
  double rem = deg - (int32_t(deg));
  int32_t ret = int32_t(deg)*1e7;
  ret += int32_t(1e7*(5.0*rem)/3.0);//convert 0.01*minutes to degrees -> multiply by 100, then divide by 60 minutes/deg
  return ret;

}
