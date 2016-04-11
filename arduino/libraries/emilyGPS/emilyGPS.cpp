#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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
    parseSentence();
  }
}

void emilyGPS::parseSentence(){
  char field[sentenceSize];
  getField(field, 0);
  if (strcmp(field, "$GPRMC") == 0)
  {
    // read time
    getField(field,1);
    //int32_t timei = convertTime(field); // time in seconds
    int32_t timei = convertTimef(field); // time in seconds
    //int32_t timei = 0;
    // read lat
    getField(field, 3);  // number in 0.01 * degrees
    int32_t lat = convertGPS(field);
    getField(field, 4); // N/S
    if (strcmp(field,"S")==0){
      lat = -lat;
    }
    // read lon
    getField(field, 5);  // number in 0.01 * degrees
    int32_t lon = convertGPS(field);
    getField(field, 6);  // E/W
    if (strcmp(field,"E")==0){
      lon = -lon;
    }
    // set the status object
    status->gpsNow.set(lat,lon,float(timei));
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

void emilyGPS::misc_tasks(){
  return;
}

int32_t convertGPS(char* buffer){
  double deg = (atof(buffer)*0.01);
  double rem = deg - (int32_t(deg));
  int32_t ret = int32_t(deg)*1e7;
  ret += int32_t(1e7*(5.0*rem)/3.0);//convert 0.01*minutes to degrees -> multiply by 100, then divide by 60 minutes/deg
  return ret;
}

float convertTimef(char*buffer){
  float time = atof(buffer);
  int32_t hr = int32_t(1.0e-4*time);
  time = time - (1e4*hr);
  int32_t min = int32_t(1.0e-2*time);
  float sec = time-(1e2*min);
  return (sec + min*60 + hr*3600);
}


int32_t convertTime(char*buffer){
  int32_t time = atol(buffer);
  int32_t hr = int32_t(1.0e-4*time);
  time = time - (1e4*hr);
  int32_t min = int32_t(1.0e-2*time);
  int32_t sec = time-(1e2*min);
  return (sec + min*60 + hr*3600);
}
