
#include <cstdio>
#include <stdint.h>
#include <string.h>
#include "ESP.h"

int main(){
  uint8_t byteArray[100];
  uint8_t msg[100];

  if (esp_pack_set_pid(byteArray,1,1.0,-0.8,1.25) > 0){
    // run through parse_byte
    for(int k = 0;k<MSG_SET_PID_LEN;k++){
      uint8_t out = esp_parse_byte(byteArray[k],msg);
      printf("%d,%d,%d\n",k,out,byteArray[k]);
      if (out > 0){
        printf("message: %s\n",msg);
      }
    }
  }

  return 0;
}
