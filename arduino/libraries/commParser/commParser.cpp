#include <stdint.h>
#include <string.h>
#include "commParser.h"

commParser::commParser(emilyStatus*st){
  bad_checksums=0;
  bad_packets=0;
  status = st;
}

void commParser::newBytes(uint8_t*bytes,int len,uint32_t millis){
  for(int k = 0;k<len;k++){
    int8_t out = esp_parse_byte(bytes[k],msg);
    if (out > 0){
      //we got a new message: handle it
      handleMsg();
      // save the time
      last_message_millis = millis;
    }
    if (out == -2){//increment the bad checksum counter
      bad_checksums++;
    }
  }
}

void commParser::handleMsg(){
  //switch based on the message ID
  switch (msg[2]){
    case MSG_GPS:
      float t;
      int32_t lon,lat;
      if (esp_unpack_gps(msg,&lon, &lat, &t) > 0){
        //set status
        status->gpsCmd.set(lat,lon,t);
      }
      else//increment the bad packet counter
        bad_packets++;
      break;
    case MSG_CONTROL:
      float rudd,thro;
      if (esp_unpack_control(msg,&rudd, &thro) > 0){
        //set status
        status->control_rudder = rudd;
        status->control_throttle = thro;
      }
      else//increment the bad packet counter
        bad_packets++;
      break;
    case MSG_COMMAND:
      break;
    case MSG_SET_PID:
      break;
  }
}

uint32_t commParser::get_bad_packets(){
  return bad_packets;
}

uint32_t commParser::get_bad_checksums(){
  return bad_checksums;
}
