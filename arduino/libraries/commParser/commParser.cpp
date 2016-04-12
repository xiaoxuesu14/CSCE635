#include <stdint.h>
#include <string.h>
#include "commParser.h"

commParser::commParser(emilyStatus*st){
  bad_checksums=0;
  bad_packets=0;
  status = st;
  // set the stream times to zero
  next_stream_time_millis[0] = 0;
  next_stream_time_millis[1] = 0;
  // sest number of messages to zero
  received_messages = 0;
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
  //increment the counter
  received_messages++;
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
    case MSG_CONTROL:/** Direct control of rudder/throttle */
      float rudd,thro;
      if (esp_unpack_control(msg,&rudd, &thro) > 0){
        //set status rudder, throttle, and mode
        status->control_rudder = rudd;
        status->control_throttle = thro;
        status->control_mode = CONTROL_MODE_DIRECT;
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

void commParser::misc_tasks(uint32_t millis){
  // check the time since last message and set the status appropriately
  if (millis-last_message_millis < TIMEOUT_WARNING_MILLIS)
    status->comm_status = COMM_STATUS_HEALTHY;
  // if we're in a non-healthy comm status mode, set the control mode to passive and wait to see if things improve
  if (millis-last_message_millis > TIMEOUT_WARNING_MILLIS & millis-last_message_millis < TIMEOUT_LOST_MILLIS){
    status->comm_status = COMM_STATUS_WARNING;
    status->control_mode = CONTROL_MODE_PASSIVE;
  }
  if (millis-last_message_millis > TIMEOUT_LOST_MILLIS){
    status->comm_status = COMM_STATUS_LOST;
    status->control_mode = CONTROL_MODE_PASSIVE;
  }
  // reset the send buffer position
  send_buffer_counter = 0;
  send_buffer_counter_helper = 0;
  //if( status->comm_status == COMM_STATUS_HEALTHY){
  // send periodic messages
  for(int k = 0;k<2;k++){
    if (next_stream_time_millis[k] <= millis){
      if (k == 0){ // GPS stream, send current GPS
        next_stream_time_millis[k] = millis + STREAM_PERIOD_GPS;
        if (status->gpsNow.init){
          // send GPS if initialized
          if (esp_pack_gps(&send_buffer[send_buffer_counter],status->gpsNow.lon,status->gpsNow.lat,status->gpsNow.t) > 0)
            send_buffer_counter+=MSG_GPS_LEN;
        }
      }
      if (k == 1){ // control-related message stream
        next_stream_time_millis[k] = millis + STREAM_PERIOD_CONTROL;
        if (status->control_mode == CONTROL_MODE_DIRECT){// in DIRECT mode, echo the rudder and throttle commands
          if (esp_pack_control(&send_buffer[send_buffer_counter],status->control_rudder,status->control_throttle) > 0)
            send_buffer_counter+=MSG_CONTROL_LEN;
        }
        else if(status->control_mode == CONTROL_MODE_INDIRECT){// in INDIRECT mode, send the current rudder and throttle settings
          continue;//TODO stream messages after we add this functionality
        }
      }
    }
  }
  //}
}

uint8_t commParser::bytes_to_send(){
  return send_buffer_counter-send_buffer_counter_helper;
}

uint8_t commParser::get_next_byte(){
  send_buffer_counter_helper++;
  return send_buffer[send_buffer_counter_helper-1];
}

uint32_t commParser::get_number_received_messages(){
  return received_messages;
}
