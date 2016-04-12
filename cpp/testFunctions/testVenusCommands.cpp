#include <cstdio>
#include <stdint.h>
#include <string.h>
#include "emilyGPS.h"

int main(){
  char buffer[256];
  emilyStatus stat;
  emilyGPS GPS(&stat);
  // pack a test rate message
  int len = GPS.send_command_configure_position_rate(buffer,uint8_t(1));
  printf("Rate message: should be\nA0 A1 00 03 0E 01 00 0F 0D 0A\n");
  for(int k = 0;k<len;k++){
    printf("%02x,",uint8_t(buffer[k]));
  }
  printf("\n");
  // pack a serial message
  len = GPS.send_command_configure_serial_port(buffer,0);
  printf("Rate message: should be\nA0 A1 00 04 05 00 00 00 05 0D 0A \n");
  for(int k = 0;k<len;k++){
    printf("%02x,",uint8_t(buffer[k]));
  }
  printf("\n");
  return 0;
}
